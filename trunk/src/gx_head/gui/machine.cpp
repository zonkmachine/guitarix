/*
 * Copyright (C) 2013 Andreas Degert
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "guitarix.h"
#include <sys/mman.h>
#include "jsonrpc_methods.h"
#include <netinet/in.h>
#include <netinet/tcp.h>

void lock_rt_memory() {
    extern char __rt_text__start[], __rt_text__end[];
    extern char __rt_data__start[], __rt_data__end[];
    struct {
	char *start;
	int len;
    } regions[] = {
	{ __rt_text__start, __rt_text__end - __rt_text__start },
	{ __rt_data__start, __rt_data__end - __rt_data__start },
    };
    for (unsigned int i = 0; i < sizeof(regions)/sizeof(regions[0]); i++) {
	if (mlock(regions[i].start, regions[i].len) != 0) {
	    throw gx_system::GxFatalError(
		boost::format(_("failed to lock memory: %1%")) % strerror(errno));
	}
    }
}


namespace gx_engine {

GxMachineBase::GxMachineBase()
    : rack_unit_order_changed() {
}

GxMachineBase::~GxMachineBase() {
}

sigc::signal<void, bool>& GxMachineBase::signal_rack_unit_order_changed() {
    return rack_unit_order_changed;
}


/****************************************************************
 ** GxMachine
 */

GxMachine::GxMachine(gx_system::CmdlineOptions& options_):
    GxMachineBase(),
    options(options_),
    engine(options.get_plugin_dir(), gx_engine::get_group_table(), options),
    jack(engine),
    settings(options, jack, engine.stereo_convolver, gx_engine::midi_std_ctr,
	     engine.controller_map, engine),
    tuner_switcher(settings, engine),
    sock(0),
    pmap(engine.get_param()) {
    engine.set_jack(&jack);

    /*
    ** setup parameters
    */

    static value_pair starter[] = {
	{ "other", "other" },
	{ "qjackctl", "qjackctl" },
	{ "autostart", "autostart" },
	{0}
    };
    pmap.reg_non_midi_enum_par(
	"ui.jack_starter_idx", "", starter, static_cast<int*>(0), false, 1);
    pmap.reg_non_midi_par("ui.ask_for_jack_starter", (bool*)0, false, true);
    pmap.reg_string("ui.jack_starter", "", 0, "");

    // rack tuner
    gx_engine::get_group_table().insert("racktuner", N_("Rack Tuner"));
    static const value_pair streaming_labels[] = {{"scale"}, {"stream"}, {0}};
    pmap.reg_non_midi_enum_par("racktuner.streaming", "Streaming Mode", streaming_labels, (int*)0, false, 1);
    static const value_pair tuning_labels[] = {{"(Chromatic)"},{"Standard"}, {"Standard/Es"}, {"Open E"}, {0}};
    pmap.reg_non_midi_enum_par("racktuner.tuning", "Tuning", tuning_labels, (int*)0, false, 0);
    pmap.reg_par_non_preset("racktuner.scale_lim", "Limit", 0, 3.0, 1.0, 10.0, 1.0);
    pmap.reg_par_non_preset("ui.tuner_reference_pitch", "?Tuner Reference Pitch", 0, 440, 427, 453, 0.1);
    //pmap.reg_par("racktuner.scale_lim", "Limit", &scale_lim, 3.0, 1.0, 10.0, 1.0); FIXME add in detail view?

    pmap.reg_par("ui.live_play_switcher", "Liveplay preset mode" , (bool*)0, false, false)->setSavable(false);
    pmap.reg_par("ui.racktuner", N_("Tuner on/off"), (bool*)0, true, false);
    pmap.reg_non_midi_par("system.show_tuner", (bool*)0, false);
    pmap.reg_non_midi_par("system.midi_in_preset", (bool*)0, false, false);
    pmap.reg_par_non_preset("ui.liveplay_brightness", "?liveplay_brightness", 0, 1.0, 0.5, 1.0, 0.01);
    pmap.reg_par_non_preset("ui.liveplay_background", "?liveplay_background", 0, 0.8, 0.0, 1.0, 0.01);
    pmap.reg_par("engine.mute", "Mute", 0, false);
    pmap.reg_non_midi_par("ui.mp_s_h", (bool*)0, false);

#ifndef NDEBUG
    // ------ time measurement (debug) ------
    gx_system::add_time_measurement();
#endif
    lock_rt_memory();

    engine.controller_map.signal_new_program().connect(
	sigc::mem_fun(this, &GxMachine::do_program_change));
}

GxMachine::~GxMachine() {
    delete sock;
#ifndef NDEBUG
    if (options.dump_parameter) {
	pmap.dump("json");
    }
#endif
}

void GxMachine::do_program_change(int pgm) {
    Glib::ustring bank = settings.get_current_bank();
    bool in_preset = !bank.empty();
    gx_system::PresetFile *f;
    if (in_preset) {
	f = settings.banks.get_file(bank);
	in_preset = pgm < f->size();
    }
    if (in_preset) {
	settings.load_preset(f, f->get_name(pgm));
	if (engine.get_state() == gx_engine::kEngineBypass) {
	    engine.set_state(gx_engine::kEngineOn);
	}
    } else if (engine.get_state() == gx_engine::kEngineOn) {
	engine.set_state(gx_engine::kEngineBypass);
    }
}

void GxMachine::set_state(GxEngineState state) {
    engine.set_state(state);
}

GxEngineState GxMachine::get_state() {
    return engine.get_state();
}

void GxMachine::init_plugin(PluginDef *p) {
    p->set_samplerate(engine.get_samplerate(), p);
}

void GxMachine::ladspaloader_update_plugins(
    const std::vector<gx_engine::Plugin*>& to_remove, LadspaLoader::pluginarray& ml,
    std::vector<PluginDef*>& pv) {
    engine.ladspaloader_update_plugins(to_remove, ml, pv);
}

bool GxMachine::ladspaloader_load(const gx_system::CmdlineOptions& options, LadspaLoader::pluginarray& p) {
    return engine.ladspaloader.load(options, p);
}

LadspaLoader::pluginarray::iterator GxMachine::ladspaloader_begin() {
    return engine.ladspaloader.begin();
}

LadspaLoader::pluginarray::iterator GxMachine::ladspaloader_end() {
    return engine.ladspaloader.end();
}

void GxMachine::ladspaloader_update_instance(PluginDef *pdef, plugdesc *pdesc) {
    engine.ladspaloader.update_instance(pdef, pdesc);
    if (pdef->register_params) {
	pmap.set_replace_mode(true);
	gx_engine::ParamRegImpl preg(&pmap);
	preg.plugin = pdef;
	pdef->register_params(preg);
	pmap.set_replace_mode(false);
    }
}

Plugin *GxMachine::pluginlist_lookup_plugin(const char *id) const {
    return engine.pluginlist.lookup_plugin(id);
}

bool GxMachine::load_unit(gx_gui::UiBuilderImpl& builder, PluginDef* pdef) {
    return builder.load_unit(pdef);
}

LadspaLoader::pluginarray::iterator GxMachine::ladspaloader_find(unsigned long uniqueid) {
    return engine.ladspaloader.find(uniqueid);
}

void GxMachine::ladspaloader_set_plugins(LadspaLoader::pluginarray& new_plugins) {
    engine.ladspaloader.set_plugins(new_plugins);
}

void GxMachine::pluginlist_append_rack(UiBuilderBase& ui) {
    engine.pluginlist.append_rack(ui);
}

void GxMachine::pluginlist_registerPlugin(Plugin *pl) {
    engine.pluginlist.registerPlugin(pl, pmap, gx_engine::get_group_table());
}

const std::string& GxMachine::conv_getIRFile(const char *id) {
    if (strcmp(id, "jconv") == 0) {
	return engine.stereo_convolver.getIRFile();
    } else {
	return engine.mono_convolver.getIRFile();
    }
}

float GxMachine::get_tuner_freq() {
    return engine.tuner.get_freq();
}

void GxMachine::set_oscilloscope_mul_buffer(int a, unsigned int b) {
    engine.oscilloscope.set_mul_buffer(a, b);
}

int GxMachine::get_oscilloscope_mul_buffer() {
    return engine.oscilloscope.get_mul_buffer();
}

float *GxMachine::get_oscilloscope_buffer() {
    return engine.oscilloscope.get_buffer();
}

void GxMachine::clear_oscilloscope_buffer() {
    engine.oscilloscope.clear_buffer();
}

bool GxMachine::oscilloscope_plugin_box_visible() {
    return engine.oscilloscope.plugin.get_box_visible();
}

sigc::signal<void, int>& GxMachine::signal_oscilloscope_post_pre() {
    return pmap[engine.oscilloscope.plugin.id_effect_post_pre()].signal_changed_int();
}

sigc::signal<void, bool>& GxMachine::signal_oscilloscope_visible() {
    return pmap[engine.oscilloscope.plugin.id_box_visible()].signal_changed_bool();
}

sigc::signal<int, bool>& GxMachine::signal_oscilloscope_activation() {
    return engine.oscilloscope.activation;
}

sigc::signal<void, unsigned int>& GxMachine::signal_oscilloscope_size_change() {
    return engine.oscilloscope.size_change;
}

void GxMachine::maxlevel_get(int channels, float *values) {
    for (int i = 0; i < channels; i++) {
	values[i] = engine.maxlevel.get(i);
    }
    engine.maxlevel.reset();
}

bool GxMachine::midiaudiobuffer_get_midistat() {
    return engine.midiaudiobuffer.get_midistat();
}

MidiAudioBuffer::Load GxMachine::midiaudiobuffer_jack_load_status() {
    return engine.midiaudiobuffer.jack_load_status();
}

gx_system::CmdlineOptions& GxMachine::get_options() const {
    return options;
}

void GxMachine::start_socket(sigc::slot<void> quit_mainloop, int port) {
    if (sock) {
	return;
    }
    sock = new MyService(settings, jack, quit_mainloop, port);
    sock->start();
}

sigc::signal<void>& GxMachine::signal_conv_settings_changed(const char *id) {
    if (strcmp(id, "jconv") == 0) {
	return engine.stereo_convolver.signal_settings_changed();
    } else {
	return engine.mono_convolver.signal_settings_changed();
    }
}

sigc::signal<void,const Glib::ustring&,const Glib::ustring&>& GxMachine::tuner_switcher_signal_display() {
    return tuner_switcher.signal_display();
}

sigc::signal<void,TunerSwitcher::SwitcherState>& GxMachine::tuner_switcher_signal_set_state() {
    return tuner_switcher.signal_set_state();
}

sigc::signal<void>& GxMachine::tuner_switcher_signal_selection_done() {
    return tuner_switcher.signal_selection_done();
}

sigc::signal<void,GxEngineState>& GxMachine::signal_state_change() {
    return engine.signal_state_change();
}

Glib::Dispatcher& GxMachine::signal_jack_load_change() {
    return engine.midiaudiobuffer.signal_jack_load_change();
}

void GxMachine::tuner_used_for_display(bool on) {
    engine.tuner.used_for_display(on);
}

void GxMachine::tuner_used_for_livedisplay(bool on) {
    engine.tuner.used_for_livedisplay(on);
}

const std::vector<std::string>& GxMachine::get_rack_unit_order(bool stereo) {
    return settings.get_rack_unit_order(stereo);
}

void GxMachine::remove_rack_unit(const std::string& unit, bool stereo) {
    settings.remove_rack_unit(unit, stereo);
    /*
    const std::vector<std::string>& l = GxMachine::get_rack_unit_order(stereo);
    cerr << "RU";
    for (std::vector<std::string>::const_iterator ii = l.begin(); ii != l.end(); ++ii) {
	cerr << " '" << *ii << "'";
    }
    cerr << endl;
    */
}

void GxMachine::insert_rack_unit(const std::string& unit, const std::string& before, bool stereo) {
    settings.insert_rack_unit(unit, before, stereo);
    /*
    const std::vector<std::string>& l = GxMachine::get_rack_unit_order(stereo);
    cerr << "IU";
    for (std::vector<std::string>::const_iterator ii = l.begin(); ii != l.end(); ++ii) {
	cerr << " '" << *ii << "'";
    }
    cerr << endl;
    */
}

// tuner_switcher
bool GxMachine::get_tuner_switcher_active() {
    return tuner_switcher.get_active();
}

void GxMachine::tuner_switcher_activate(bool v) {
    tuner_switcher.activate(v);
}

bool GxMachine::tuner_switcher_deactivate() {
    return tuner_switcher.deactivate();
}

// preset
bool GxMachine::setting_is_preset() {
    return settings.setting_is_preset();
}


const Glib::ustring& GxMachine::get_current_bank() {
    return settings.get_current_bank();
}

gx_system::PresetFileGui *GxMachine::get_current_bank_file() {
    return settings.get_current_bank_file()->get_guiwrapper();
}

const Glib::ustring& GxMachine::get_current_name() {
    return settings.get_current_name();
}

gx_system::PresetFileGui* GxMachine::get_bank_file(const Glib::ustring& bank) const {
    return settings.banks.get_file(bank)->get_guiwrapper();
}

Glib::ustring GxMachine::get_bank_name(int n) {
    return settings.banks.get_name(n);
}

void GxMachine::load_preset(gx_system::PresetFileGui *pf, const Glib::ustring& name) {
    settings.load_preset(pf, name);
}

void GxMachine::loadstate() {
    settings.loadstate();
}

int GxMachine::bank_size() {
    return settings.banks.size();
}

void GxMachine::create_default_scratch_preset() {
    settings.create_default_scratch_preset();
}

void GxMachine::set_statefilename(const std::string& fn) {
    settings.set_statefilename(fn);
}

void GxMachine::save_to_state(bool preserve_preset) {
    settings.save_to_state(preserve_preset);
}

Glib::RefPtr<gx_preset::PluginPresetList> GxMachine::load_plugin_preset_list(const Glib::ustring& id, bool factory) const {
    return settings.load_plugin_preset_list(id, factory);
}

void GxMachine::disable_autosave(bool v) {
    settings.disable_autosave(v);
}

sigc::signal<void>& GxMachine::signal_selection_changed() {
    return settings.signal_selection_changed();
}

sigc::signal<void>& GxMachine::signal_presetlist_changed() {
    return settings.signal_presetlist_changed();
}

gx_system::PresetFileGui* GxMachine::bank_insert_uri(const Glib::ustring& uri, bool move) {
    gx_system::PresetFile *f = settings.bank_insert_uri(uri, move);
    if (f) {
	return f->get_guiwrapper();
    } else {
	return 0;
    }
}

gx_system::PresetFileGui *GxMachine::bank_insert_new(const Glib::ustring& name) {
    gx_system::PresetFile *f = settings.bank_insert_new(name);
    if (f) {
	return f->get_guiwrapper();
    } else {
	return 0;
    }
}

bool GxMachine::rename_bank(const Glib::ustring& oldname, Glib::ustring& newname) {
    return settings.rename_bank(oldname, newname);
}

bool GxMachine::rename_preset(gx_system::PresetFileGui& pf, const Glib::ustring& oldname, const Glib::ustring& newname) {
    return settings.rename_preset(pf, oldname, newname);
}

void GxMachine::bank_reorder(const std::vector<Glib::ustring>& neworder) {
    settings.banks.reorder(neworder);
}

void GxMachine::reorder_preset(gx_system::PresetFileGui& pf, const std::vector<Glib::ustring>& neworder) {
    settings.reorder_preset(pf, neworder);
}

bool GxMachine::bank_check_reparse() {
    return settings.banks.check_reparse();
}

void GxMachine::erase_preset(gx_system::PresetFileGui& pf, const Glib::ustring& name) {
    settings.erase_preset(pf, name);
}

void GxMachine::bank_set_flag(gx_system::PresetFileGui *pf, int flag, bool v) {
    static_cast<gx_system::PresetFile*>(pf)->set_flag(flag, v);
    settings.banks.save();
}

std::string GxMachine::bank_get_filename(const Glib::ustring& bank) {
    return settings.banks.get_file(bank)->get_filename();
}

void GxMachine::bank_drag_begin() {
}

gx_system::PresetFileGui *GxMachine::bank_get_file(const Glib::ustring& bank) const {
    return settings.banks.get_file(bank)->get_guiwrapper();
}

bank_iterator GxMachine::bank_begin() {
    return bank_iterator(settings.banks.begin());
}

bank_iterator GxMachine::bank_end() {
    return bank_iterator(settings.banks.end());
}

void GxMachine::pf_append(gx_system::PresetFileGui& pf, const Glib::ustring& src, gx_system::PresetFileGui& pftgt, const Glib::ustring& name) {
    settings.append(pf, src, pftgt, name);
}

void GxMachine::pf_insert_before(gx_system::PresetFileGui& pf, const Glib::ustring& src, gx_system::PresetFileGui& pftgt, const Glib::ustring& pos, const Glib::ustring& name) {
    settings.insert_before(pf, src, pftgt, pos, name);
}

void GxMachine::pf_insert_after(gx_system::PresetFileGui& pf, const Glib::ustring& src, gx_system::PresetFileGui& pftgt, const Glib::ustring& pos, const Glib::ustring& name) {
    settings.insert_after(pf, src, pftgt, pos, name);
}

bool GxMachine::convert_preset(gx_system::PresetFileGui& pf) {
    return settings.convert_preset(pf);
}

bool GxMachine::bank_remove(const Glib::ustring& bank) {
    return settings.remove_bank(bank);
}

void GxMachine::bank_save() {
    settings.banks.save();
}

void GxMachine::pf_save(gx_system::PresetFileGui& pf, const Glib::ustring& name) {
    settings.save(pf, name);
}


// jack
gx_jack::GxJack *GxMachine::get_jack() {
    return &jack;
}

// pmap
Parameter& GxMachine::get_parameter(const char *p) {
    return pmap[p];
}

Parameter& GxMachine::get_parameter(const std::string& id) {
    return pmap[id];
}

void GxMachine::set_init_values() {
    pmap.set_init_values();
}

bool GxMachine::parameter_hasId(const char *p) {
    return pmap.hasId(p);
}

bool GxMachine::parameter_hasId(const std::string& id) {
    return pmap.hasId(id);
}

void GxMachine::reset_unit(Glib::ustring group_id) const {
    pmap.reset_unit(group_id, engine.pluginlist.lookup_plugin(group_id.c_str())->get_pdef()->groups);
}

bool GxMachine::parameter_unit_has_std_values(Glib::ustring group_id) const {
    return pmap.unit_has_std_values(group_id, engine.pluginlist.lookup_plugin(group_id.c_str())->get_pdef()->groups);
}

void GxMachine::set_parameter_value(const std::string& id, int value) {
    //cerr << "SVI " << id << ": " << value << endl;
    pmap[id].getInt().set(value);
}

void GxMachine::set_parameter_value(const std::string& id, bool value) {
    //cerr << "SVB " << id << ": " << value << endl;
    pmap[id].getBool().set(value);
}

void GxMachine::set_parameter_value(const std::string& id, float value) {
    //cerr << "SVF " << id << ": " << value << endl;
    pmap[id].getFloat().set(value);
}

void GxMachine::set_parameter_value(const std::string& id, const std::string& value) {
    //cerr << "SVF " << id << ": " << value << endl;
    pmap[id].getString().set(value);
}

int GxMachine::_get_parameter_value_int(const std::string& id) {
    return pmap[id].getInt().get_value();
}

int GxMachine::_get_parameter_value_bool(const std::string& id) {
    return pmap[id].getBool().get_value();
}

float GxMachine::_get_parameter_value_float(const std::string& id) {
    return pmap[id].getFloat().get_value();
}

std::string GxMachine::_get_parameter_value_string(const std::string& id) {
    return pmap[id].getString().get_value();
}

sigc::signal<void, int>& GxMachine::_signal_parameter_value_int(const std::string& id) {
    return pmap[id].signal_changed_int();
}

sigc::signal<void, bool>& GxMachine::_signal_parameter_value_bool(const std::string& id) {
    return pmap[id].signal_changed_bool();
}

sigc::signal<void, float>& GxMachine::_signal_parameter_value_float(const std::string& id) {
    return pmap[id].signal_changed_float();
}

// MidiControllerList
bool GxMachine::midi_get_config_mode(int *ctl) {
    if (engine.controller_map.get_config_mode()) {
	*ctl = engine.controller_map.get_current_control();
	return true;
    }
    return false;
}

void GxMachine::midi_set_config_mode(bool v, int ctl) {
    engine.controller_map.set_config_mode(v, ctl);
}

sigc::signal<void>& GxMachine::signal_midi_changed() {
    return engine.controller_map.signal_changed();
}

sigc::signal<void, int, int>& GxMachine::signal_midi_value_changed() {
    return engine.controller_map.signal_midi_value_changed();
}

void GxMachine::request_midi_value_update() {
    engine.controller_map.request_midi_value_update();
}

int GxMachine::midi_size() {
    return engine.controller_map.size();
}

midi_controller_list& GxMachine::midi_get(int n) {
    return engine.controller_map[n];
}

void GxMachine::midi_deleteParameter(Parameter& param, bool quiet) {
    engine.controller_map.deleteParameter(param, quiet);
}

void GxMachine::midi_set_current_control(int v) {
    engine.controller_map.set_current_control(v);
}

void GxMachine::midi_modifyCurrent(Parameter& param, float lower, float upper, bool toggle) {
    engine.controller_map.modifyCurrent(param, lower, upper, toggle);
}

int GxMachine::midi_param2controller(Parameter& param, const MidiController** p) {
    return engine.controller_map.param2controller(param, p);
}

// cheat
ConvolverMonoAdapter& GxMachine::get_mono_convolver() {
    return engine.mono_convolver;
}

ConvolverStereoAdapter& GxMachine::get_stereo_convolver() {
    return engine.stereo_convolver;
}


/****************************************************************
 ** GxMachineRemote
 */

#define START_NOTIFY(m)  { const jsonrpc_method_def& _md = start_call(RPNM_##m)

#define SEND()           assert(!_md.has_result); send(); }

#define START_CALL(m)    { const jsonrpc_method_def& _md = start_call(RPCM_##m)

#define START_RECEIVE(s) assert(_md.has_result); send(); { \
                           gx_system::JsonStringParser *jp = receive();\
                           if (!jp) { return s; }\
			   try {

#define END_RECEIVE(s)     } catch (const gx_system::JsonException& e) { report_rpc_error(jp, e, _md.name); } \
			 delete jp; s; }}


GxMachineRemote::GxMachineRemote(gx_system::CmdlineOptions& options_)
    : GxMachineBase(),
      options(options_),
      pmap(),
      pluginlist(),
      banks(),
      engine_state_change(),
      selection_changed(),
      presetlist_changed(),
      socket(),
      writebuf(),
      os(),
      jw(0),
      notify_list(),
      idle_conn(),
      rack_units(),
      midi_changed(),
      midi_value_changed(),
      current_bank(),
      current_preset(),
      bank_drag_get_counter(),
      bank_drag_get_path() {
    socket = Gio::Socket::create(Gio::SOCKET_FAMILY_IPV4, Gio::SOCKET_TYPE_STREAM, Gio::SOCKET_PROTOCOL_TCP);
    int flag = 1;
    setsockopt(socket->get_fd(), IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
    Glib::RefPtr<Gio::InetAddress> a = Gio::InetAddress::create("127.0.0.1");
    try {
	socket->connect(Gio::InetSocketAddress::create(a, 7000));
    } catch (Gio::Error e) {
	throw e;
    }
    socket->set_blocking(true);
    writebuf = new __gnu_cxx::stdio_filebuf<char>(socket->get_fd(), std::ios::out);
    os = new ostream(writebuf);
    jw = new gx_system::JsonWriter(os, false);

    START_CALL(parameterlist);
    START_RECEIVE();
    pmap.readJSON(*jp);
    END_RECEIVE();
    current_bank = pmap["system.current_bank"].getString().get_value();
    current_preset = pmap["system.current_preset"].getString().get_value();
    START_CALL(pluginlist);
    START_RECEIVE();
    pluginlist.readJSON(*jp, pmap);
    END_RECEIVE();
    START_CALL(banks);
    START_RECEIVE();
    banks.readJSON_remote(*jp);
    END_RECEIVE();
    START_CALL(get_midi_controller_map);
    START_RECEIVE();
    midi_controller_map.readJSON(*jp, pmap);
    END_RECEIVE();
    START_NOTIFY(listen);
    jw->write("preset");
    jw->write("state");
    //we don't need "freq"
    jw->write("display");
    jw->write("tuner");
    jw->write("presetlist_changed");
    jw->write("logger");
    jw->write("midi");
    SEND();
}

GxMachineRemote::~GxMachineRemote() {
    jw->close();
    delete jw;
    writebuf->close();
    delete os;
    delete writebuf;
}

void GxMachineRemote::param_signal_int(int v, IntParameter *p) {
    if (p->get_blocked()) {
	return;
    }
    //cerr << "SVI " << p->id() << ": " << v << endl;
    START_NOTIFY(set);
    jw->write(p->id());
    jw->write(v);
    SEND();
}

void GxMachineRemote::param_signal_bool(bool v, BoolParameter *p) {
    if (p->get_blocked()) {
	return;
    }
    //cerr << "SVB " << p->id() << ": " << v << endl;
    START_NOTIFY(set);
    jw->write(p->id());
    jw->write(v);
    SEND();
}

void GxMachineRemote::param_signal_float(float v, FloatParameter *p) {
    if (p->get_blocked()) {
	return;
    }
    //cerr << "SVF " << p->id() << ": " << v << endl;
    START_NOTIFY(set);
    jw->write(p->id());
    jw->write(v);
    SEND();
}

void GxMachineRemote::param_signal_string(const Glib::ustring& v, StringParameter *p) {
    if (p->get_blocked()) {
	return;
    }
    //cerr << "SVS " << p->id() << ": " << v << endl;
    START_NOTIFY(set);
    jw->write(p->id());
    jw->write(v);
    SEND();
}

void GxMachineRemote::parameter_changed(gx_system::JsonStringParser *jp) {
    Parameter& p = pmap[jp->current_value()];
    p.set_blocked(true);
    if (p.isFloat()) {
	float v;
	switch (jp->next()) {
	case gx_system::JsonParser::value_string:
	    v = dynamic_cast<FloatEnumParameter*>(&p)->idx_from_id(jp->current_value());
	    break;
	case gx_system::JsonParser::value_number:
	    v = jp->current_value_float();
	    break;
	default:
	    assert(false);
	    v = 0;
	}
	p.getFloat().set(v);
    } else if (p.isInt()) {
	int v;
	switch (jp->next()) {
	case gx_system::JsonParser::value_string:
	    v = p.getEnum().idx_from_id(jp->current_value());
	    break;
	case gx_system::JsonParser::value_number:
	    v = jp->current_value_int();
	    break;
	default:
	    assert(false);
	    v = 0;
	}
	p.getInt().set(v);
    } else if (p.isBool()) {
	jp->next(gx_system::JsonParser::value_number);
	p.getBool().set(jp->current_value_int());
    } else if (p.isString()) {
	jp->next(gx_system::JsonParser::value_string);
	p.getString().set(jp->current_value());
    } else if (p.isFile()) {
	cerr << "change file parameter " << p.id() << endl;
    } else {
	cerr << "change special type parameter " << p.id() << endl;
    }
    p.set_blocked(false);
}

void GxMachineRemote::handle_notify(gx_system::JsonStringParser *jp) {
    jp->next(gx_system::JsonParser::value_string);
    std::string method = jp->current_value();
    jp->next(gx_system::JsonParser::value_key); // "params"
    jp->next(gx_system::JsonParser::begin_array);
    if (method == "state_changed") {
	jp->next(gx_system::JsonParser::value_string);
	engine_state_change(string_to_engine_state(jp->current_value()));
    } else if (method == "message") {
	jp->next(gx_system::JsonParser::value_string);
	gx_system::GxMsgType msgtype = gx_system::kError;
	if (jp->current_value() == "info") {
	    msgtype = gx_system::kInfo;
	} else if (jp->current_value() == "warning") {
	    msgtype = gx_system::kWarning;
	}
	jp->next(gx_system::JsonParser::value_string);
	gx_system::Logger::get_logger().print(jp->current_value(), msgtype);
    } else if (method == "preset_changed") {
	jp->next();
	Glib::ustring new_bank = jp->current_value();
	jp->next(gx_system::JsonParser::value_string);
	Glib::ustring new_preset = jp->current_value();
	jp->next(gx_system::JsonParser::end_array);
	if (new_preset != current_preset || new_bank != current_bank) {
	    current_bank = new_bank;
	    current_preset = new_preset;
	    selection_changed();
	}
    } else if (method == "presetlist_changed") {
	START_CALL(banks);
	START_RECEIVE();
	banks.readJSON_remote(*jp);
	END_RECEIVE();
	presetlist_changed();
    } else if (method == "set") {
	while (jp->peek() != gx_system::JsonParser::end_array) {
	    jp->next(gx_system::JsonParser::value_string);
	    parameter_changed(jp);
	}
    } else if (method == "rack_units_changed") {
	jp->next(gx_system::JsonParser::begin_array);
	jp->next(gx_system::JsonParser::value_number);
	bool stereo = jp->current_value_int();
	std::vector<std::string>& l = (stereo ? rack_units.stereo : rack_units.mono);
	l.clear();
	while (jp->peek() != gx_system::JsonParser::end_array) {
	    jp->next(gx_system::JsonParser::value_string);
	    l.push_back(jp->current_value());
	}
	jp->next(gx_system::JsonParser::end_array);
	rack_unit_order_changed(stereo);
    } else if (method == "midi_changed") {
	midi_controller_map.readJSON(*jp, pmap);
	midi_changed();
    } else if (method == "midi_value_changed") {
	jp->next(gx_system::JsonParser::begin_array);
	jp->next(gx_system::JsonParser::value_number);
	int ctl = jp->current_value_int();
	jp->next(gx_system::JsonParser::value_number);
	int value = jp->current_value_int();
	jp->next(gx_system::JsonParser::end_array);
	midi_value_changed(ctl, value);
    } else {
	cerr << "> " << jp->get_string() << endl;
    }
}

bool GxMachineRemote::socket_input_handler(Glib::IOCondition cond) {
    if (cond == Glib::IO_HUP) {
	return false;
    }
    char buf[10000];
    gx_system::JsonStringParser *jp = new gx_system::JsonStringParser;
    while (true) {
	int n;
	try {
	    n = socket->receive(buf, sizeof(buf));
	    if (false && n > 0) {
		printf(">> %.*s", n, buf);
		fflush(stdout);
	    }
	} catch(Glib::Error e) {
	    delete jp;
	    return false;
	}
	if (n <= 0) {
	    delete jp;
	    return false;
	}
	char *p = buf;
	while (n-- > 0) {
	    jp->put(*p);
	    if (*p == '\n') {
		jp->start_parser();
		try {
		    jp->next(gx_system::JsonParser::begin_object);
		    jp->next(gx_system::JsonParser::value_key); // "jsonrpc"
		    jp->next(gx_system::JsonParser::value_string); // "2.0"
		    jp->next(gx_system::JsonParser::value_key); // "method"
		    handle_notify(jp);
		} catch (gx_system::JsonException e) {
		    cerr << "JsonException: " << e.what() << ": '" << jp->get_string() << "'" << endl;
		    assert(false);
		}
		if (n == 0) {
		    delete jp;
		    return true;
		}
		delete jp;
		jp = new gx_system::JsonStringParser;
	    }
	    p++;
	}
    }
}

const jsonrpc_method_def& GxMachineRemote::start_call(jsonrpc_method m) {
    const jsonrpc_method_def& md = jsonrpc_method_list[m];
    jw->begin_object();
    jw->write_key("jsonrpc");
    jw->write("2.0");
    if (md.has_result) {
	jw->write_key("id");
	jw->write("1");
    }
    jw->write_key("method");
    jw->write(md.name);
    jw->write_key("params");
    jw->begin_array();
    return md;
}

void GxMachineRemote::send() {
    jw->end_array();
    jw->end_object();
    *os << endl;
    jw->reset();
}

void GxMachineRemote::report_rpc_error(gx_system::JsonStringParser *jp, const gx_system::JsonException& e, const char *method) {
    if (method) {
	cerr << "RPC Error in " << method << "(): ";
    } else {
	cerr << "RPC Error: ";
    }
    cerr << e.what() << ": '" << jp->get_string() << "'" << endl;
    assert(false);
}

bool GxMachineRemote::idle_notify_handler() {
    for (unsigned int i = 0; i < notify_list.size(); ++i) {
	gx_system::JsonStringParser *jp = notify_list[i];
	handle_notify(jp);
	delete jp;
    }
    notify_list.clear();
    return false;
}

void GxMachineRemote::add_idle_handler() {
    if (!idle_conn.connected()) {
	idle_conn = Glib::signal_idle().connect(
	    sigc::mem_fun(this, &GxMachineRemote::idle_notify_handler));
    }
}

void GxMachineRemote::throw_error(gx_system::JsonStringParser *jp) {
    jp->next(gx_system::JsonParser::begin_object);
    int code = 0;
    Glib::ustring message;
    while (jp->peek() != gx_system::JsonParser::end_object) {
	jp->next(gx_system::JsonParser::value_key);
	if (jp->current_value() == "code") {
	    jp->next(gx_system::JsonParser::value_number);
	    code = jp->current_value_int();
	} else if (jp->current_value() == "message") {
	    jp->next(gx_system::JsonParser::value_string);
	    message = jp->current_value();
	}
    }
    jp->next(gx_system::JsonParser::end_object);
    cerr << jp->get_string() << std::flush;
    throw gx_system::JsonException(
	Glib::ustring::compose("RPC error %1 : %2", code, message));
}

gx_system::JsonStringParser *GxMachineRemote::receive(bool verbose) {
    char buf[10000];
    bool error = false;
    gx_system::JsonStringParser *jp_ret = 0;
    gx_system::JsonStringParser *jp = new gx_system::JsonStringParser;
    try {
	while (true) {
	    int n;
	    try {
		n = socket->receive(buf, sizeof(buf));
		if (verbose && n > 0) {
		    printf("## %.*s", n, buf);
		    fflush(stdout);
		}
	    } catch(Glib::Error e) {
		cerr << "Glib receive error" << endl;
		return 0;
	    }
	    if (n <= 0) {
		cerr << "Glib receive error n <= 0" << endl;
		return 0;
	    }
	    char *p = buf;
	    while (n-- > 0) {
		jp->put(*p);
		if (*p == '\n') {
		    jp->start_parser();
		    jp->next(gx_system::JsonParser::begin_object);
		    jp->next(gx_system::JsonParser::value_key); // "jsonrpc"
		    jp->next(gx_system::JsonParser::value_string); // "2.0"
		    jp->next(gx_system::JsonParser::value_key);
		    if (jp->current_value() == "id") {
			jp->next(); // id or null
			jp->next(gx_system::JsonParser::value_key);
			if (jp->current_value() == "error") {
			    error = true;
			}
			assert(jp_ret == 0);
			jp_ret = jp;
		    } else {
			assert(jp->current_value() == "method");
			notify_list.push_back(jp);
			add_idle_handler();
		    }
		    if (n == 0 && jp_ret) {
			if (error) {
			    throw_error(jp_ret);
			}
			return jp_ret;
		    }
		    jp = new gx_system::JsonStringParser;
		}
		p++;
	    }
	}
    } catch (const gx_system::JsonException& e) {
	report_rpc_error(jp, e);
	delete jp;
	return 0;
    }
}

bool GxMachineRemote::get_bool(gx_system::JsonStringParser *jp) {
    jp->next(gx_system::JsonParser::value_number);
    return bool(jp->current_value_int());
}

void GxMachineRemote::set_state(GxEngineState state) {
    START_NOTIFY(setstate);
    jw->write(engine_state_to_string(state));
    SEND();
}

GxEngineState GxMachineRemote::get_state() {
    START_CALL(getstate);
    START_RECEIVE(gx_engine::kEngineOff);
    jp->next(gx_system::JsonParser::value_string);
    return string_to_engine_state(jp->current_value());
    END_RECEIVE(return gx_engine::kEngineOff);
}

void GxMachineRemote::init_plugin(PluginDef *p) {
    cerr << "init_plugin()" << endl;
}


/*
** LadspaLoader
*/

bool GxMachineRemote::ladspaloader_load(const gx_system::CmdlineOptions& options, LadspaLoader::pluginarray& p) {
    cerr << "ladspaloader_load()" << endl;
    return false;
}

LadspaLoader::pluginarray::iterator GxMachineRemote::ladspaloader_begin() {
    cerr << "ladspaloader_begin()" << endl;
    LadspaLoader::pluginarray p;
    return p.begin();
}

LadspaLoader::pluginarray::iterator GxMachineRemote::ladspaloader_end() {
    cerr << "ladspaloader_end()" << endl;
    LadspaLoader::pluginarray p;
    return p.end();
}

void GxMachineRemote::ladspaloader_update_instance(PluginDef *pdef, plugdesc *pdesc) {
    cerr << "ladspaloader_update_instance()" << endl;
}

LadspaLoader::pluginarray::iterator GxMachineRemote::ladspaloader_find(unsigned long uniqueid) {
    LadspaLoader::pluginarray p;
    return p.begin();
}

void GxMachineRemote::ladspaloader_set_plugins(LadspaLoader::pluginarray& new_plugins) {
    cerr << "ladspaloader_set_plugins()" << endl;
}

void GxMachineRemote::ladspaloader_update_plugins(
    const std::vector<gx_engine::Plugin*>& to_remove, LadspaLoader::pluginarray& ml,
    std::vector<PluginDef*>& pv) {
    cerr << "ladspaloader_update_plugins()" << endl;
}

/*
** PluginList
*/

Plugin *GxMachineRemote::pluginlist_lookup_plugin(const char *id) const {
    return pluginlist.lookup_plugin(id);
}

void GxMachineRemote::pluginlist_append_rack(UiBuilderBase& ui) {
    pluginlist.append_rack(ui);
}

void GxMachineRemote::pluginlist_registerPlugin(Plugin *pl) {
    assert(false);
}

static const char *next_char_pointer(gx_system::JsonParser *jp) {
    switch (jp->next()) {
    case gx_system::JsonParser::value_string: return jp->current_value().c_str();
    case gx_system::JsonParser::value_null: return 0;
    default: jp->throw_unexpected(gx_system::JsonParser::value_string); return 0;
    }
}

static const std::string next_string(gx_system::JsonParser *jp) {
    if (jp->next() != gx_system::JsonParser::value_string) {
	jp->throw_unexpected(gx_system::JsonParser::value_string);
    }
    return jp->current_value();
}

int GxMachineRemote::load_remote_ui_static (const UiBuilder& builder) {
    GxMachineRemote *m = dynamic_cast<GxMachineRemote*>(&static_cast<const gx_gui::UiBuilderImpl*>(&builder)->main.get_machine());
    return m->load_remote_ui(builder);
}

int GxMachineRemote::load_remote_ui(const UiBuilder& builder) {
    START_CALL(plugin_load_ui);
    jw->write(builder.plugin->id);
    START_RECEIVE(-1);
    jp->next(gx_system::JsonParser::begin_array);
    while (jp->peek() != gx_system::JsonParser::end_array) {
	jp->next(gx_system::JsonParser::begin_array);
	jp->next(gx_system::JsonParser::value_string);
	if (jp->current_value() == "openTabBox") {
	    builder.openTabBox(next_char_pointer(jp));
	} else if (jp->current_value() == "openVerticalBox") {
	    builder.openVerticalBox(next_char_pointer(jp));
	} else if (jp->current_value() == "openVerticalBox1") {
	    builder.openVerticalBox1(next_char_pointer(jp));
	} else if (jp->current_value() == "openVerticalBox2") {
	    builder.openVerticalBox2(next_char_pointer(jp));
	} else if (jp->current_value() == "openHorizontalhideBox") {
	    builder.openHorizontalhideBox(next_char_pointer(jp));
	} else if (jp->current_value() == "openHorizontalBox") {
	    builder.openHorizontalBox(next_char_pointer(jp));
	} else if (jp->current_value() == "openHorizontalBox") {
	    builder.openHorizontalBox(next_char_pointer(jp));
	} else if (jp->current_value() == "insertSpacer") {
	    builder.insertSpacer();
	} else if (jp->current_value() == "set_next_flags") {
	    jp->next(gx_system::JsonParser::value_number);
	    builder.set_next_flags(jp->current_value_int());
	} else if (jp->current_value() == "create_small_rackknob") {
	    std::string id = next_string(jp);
	    builder.create_small_rackknob(id.c_str(), next_char_pointer(jp));
	} else if (jp->current_value() == "create_small_rackknobr") {
	    std::string id = next_string(jp);
	    builder.create_small_rackknobr(id.c_str(), next_char_pointer(jp));
	} else if (jp->current_value() == "create_master_slider") {
	    std::string id = next_string(jp);
	    builder.create_master_slider(id.c_str(), next_char_pointer(jp));
	} else if (jp->current_value() == "create_selector_no_caption") {
	    builder.create_selector_no_caption(next_char_pointer(jp));
	} else if (jp->current_value() == "create_selector") {
	    std::string id = next_string(jp);
	    builder.create_selector(id.c_str(), next_char_pointer(jp));
	} else if (jp->current_value() == "create_spin_value") {
	    std::string id = next_char_pointer(jp);
	    builder.create_spin_value(id.c_str(), next_char_pointer(jp));
	} else if (jp->current_value() == "create_switch_no_caption") {
	    std::string sw_type = next_char_pointer(jp);
	    builder.create_switch_no_caption(sw_type.c_str(), next_char_pointer(jp));
	} else if (jp->current_value() == "create_switch") {
	    std::string sw_type = next_char_pointer(jp);
	    std::string id = next_char_pointer(jp);
	    builder.create_switch(sw_type.c_str(), id.c_str(), next_char_pointer(jp));
	} else if (jp->current_value() == "create_port_display") {
	    std::string id = next_char_pointer(jp);
	    builder.create_port_display(id.c_str(), next_char_pointer(jp));
	} else if (jp->current_value() == "closeBox") {
	    builder.closeBox();
	} else if (jp->current_value() == "load_glade") {
	    jp->next(gx_system::JsonParser::value_string);
	    builder.load_glade(jp->current_value().c_str());
	} else {
	    cerr << "unknown uiBuilder function " << jp->current_value() << endl;
	    jp->skip_object();
	}
	jp->next(gx_system::JsonParser::end_array);
    }
    jp->next(gx_system::JsonParser::end_array);
    return 0;
    END_RECEIVE(return -1);
}

bool GxMachineRemote::load_unit(gx_gui::UiBuilderImpl& builder, PluginDef* pdef) {
    pdef->load_ui = load_remote_ui_static;
    return builder.load_unit(pdef);
}


/*
** Oscilloscope
*/

void GxMachineRemote::set_oscilloscope_mul_buffer(int a, unsigned int b) {
    cerr << "pluginlist_lookup_plugin()" << endl;
}

int GxMachineRemote::get_oscilloscope_mul_buffer() {
    cerr << "get_oscilloscope_mul_buffer()" << endl;
    return 1;
}

float *GxMachineRemote::get_oscilloscope_buffer() {
    cerr << "get_oscilloscope_buffer()" << endl;
    return 0;
}

void GxMachineRemote::clear_oscilloscope_buffer() {
    cerr << "clear_oscilloscope_buffer()" << endl;
}

bool GxMachineRemote::oscilloscope_plugin_box_visible() {
    cerr << "oscilloscope_plugin_box_visible()" << endl;
    return false;
}

sigc::signal<void, int>& GxMachineRemote::signal_oscilloscope_post_pre() {
    cerr << "signal_oscilloscope_post_pre()" << endl;
    static sigc::signal<void, int> x;
    return x;
}

sigc::signal<void, bool>& GxMachineRemote::signal_oscilloscope_visible() {
    cerr << "signal_oscilloscope_visible()" << endl;
    static sigc::signal<void, bool> x;
    return x;
}

sigc::signal<int, bool>& GxMachineRemote::signal_oscilloscope_activation() {
    cerr << "signal_oscilloscope_activation()" << endl;
    static sigc::signal<int, bool> x;
    return x;
}

sigc::signal<void, unsigned int>& GxMachineRemote::signal_oscilloscope_size_change() {
    cerr << "signal_oscilloscope_size_change()" << endl;
    static sigc::signal<void, unsigned int> x;
    return x;
}

const std::string& GxMachineRemote::conv_getIRFile(const char *id) {
    cerr << "conv_getIRFile()" << endl;
    static std::string s = "";
    return s;
}

float GxMachineRemote::get_tuner_freq() {
    //cerr << "get_tuner_freq()" << endl;
    START_CALL(get_tuner_freq);
    START_RECEIVE(0);
    jp->next(gx_system::JsonParser::value_number);
    return jp->current_value_float();
    END_RECEIVE(return 0);
}

void GxMachineRemote::maxlevel_get(int channels, float *values) {
    START_CALL(get_max_output_level);
    jw->write(channels);
    START_RECEIVE();
    jp->next(gx_system::JsonParser::begin_array);
    for (int i = 0; i < channels; i++) {
	if (jp->peek() != gx_system::JsonParser::end_array) {
	    jp->next(gx_system::JsonParser::value_number);
	    values[i] = jp->current_value_float();
	} else {
	    values[i] = 0.0;
	}
    }
    END_RECEIVE();
}

bool GxMachineRemote::midiaudiobuffer_get_midistat() {
    cerr << "midiaudiobuffer_get_midistat()" << endl;
    return false;
}

MidiAudioBuffer::Load GxMachineRemote::midiaudiobuffer_jack_load_status() {
    cerr << "midiaudiobuffer_jack_load_status()" << endl;
    START_CALL(get_jack_load_status);
    START_RECEIVE(MidiAudioBuffer::load_off);
    jp->next(gx_system::JsonParser::value_number);
    return static_cast<MidiAudioBuffer::Load>(jp->current_value_int());
    END_RECEIVE(return MidiAudioBuffer::load_off);
}

gx_system::CmdlineOptions& GxMachineRemote::get_options() const {
    return options;
}

void GxMachineRemote::start_socket(sigc::slot<void> quit_mainloop, int port) {
    assert(false);
}

sigc::signal<void>& GxMachineRemote::signal_conv_settings_changed(const char *id) {
    cerr << "signal_conv_settings_changed()" << endl;
    static sigc::signal<void> x;
    return x;
}

sigc::signal<void,const Glib::ustring&,const Glib::ustring&>& GxMachineRemote::tuner_switcher_signal_display() {
    cerr << "tuner_switcher_signal_display()" << endl;
    static sigc::signal<void,const Glib::ustring&,const Glib::ustring&> x;
    return x;
}

sigc::signal<void,TunerSwitcher::SwitcherState>& GxMachineRemote::tuner_switcher_signal_set_state() {
    cerr << "tuner_switcher_signal_set_state()" << endl;
    static sigc::signal<void,TunerSwitcher::SwitcherState> x;
    return x;
}

sigc::signal<void>& GxMachineRemote::tuner_switcher_signal_selection_done() {
    cerr << "tuner_switcher_signal_selection_done()" << endl;
    static sigc::signal<void> x;
    return x;
}

sigc::signal<void,GxEngineState>& GxMachineRemote::signal_state_change() {
    return engine_state_change;
}

Glib::Dispatcher& GxMachineRemote::signal_jack_load_change() {
    cerr << "signal_jack_load_change()" << endl;
    static Glib::Dispatcher x;
    return x;
}

void GxMachineRemote::tuner_used_for_display(bool on) {
    cerr << "tuner_used_for_display()" << endl;
    START_NOTIFY(tuner_used_for_display);
    jw->write(on);
    SEND();
}

void GxMachineRemote::tuner_used_for_livedisplay(bool on) {
    cerr << "tuner_used_for_livedisplay()" << endl;
    START_NOTIFY(tuner_used_for_livedisplay);
    jw->write(on);
    SEND();
}

const std::vector<std::string>& GxMachineRemote::get_rack_unit_order(bool stereo) {
    std::vector<std::string>& l = (stereo ? rack_units.stereo : rack_units.mono);
    l.clear();
    START_CALL(get_rack_unit_order);
    jw->write(stereo);
    START_RECEIVE(l);
    try {
	jp->next(gx_system::JsonParser::begin_array);
	while (jp->peek() != gx_system::JsonParser::end_array) {
	    jp->next(gx_system::JsonParser::value_string);
	    l.push_back(jp->current_value());
	}
	jp->next(gx_system::JsonParser::end_array);
    } catch (gx_system::JsonException e) {
	cerr << "JsonException: " << e.what() << ": '" << jp->get_string() << "'" << endl;
	assert(false);
    }
    return l;
    END_RECEIVE(return l);
}

void GxMachineRemote::remove_rack_unit(const std::string& unit, bool stereo) {
    cerr << "remove_rack_unit()" << endl;
    START_NOTIFY(remove_rack_unit);
    jw->write(unit);
    jw->write(stereo);
    SEND();
}

void GxMachineRemote::insert_rack_unit(const std::string& unit, const std::string& before, bool stereo) {
    cerr << "insert_rack_unit()" << endl;
    START_NOTIFY(insert_rack_unit);
    jw->write(unit);
    jw->write(before);
    jw->write(stereo);
    SEND();
}

// tuner_switcher
bool GxMachineRemote::get_tuner_switcher_active() {
    cerr << "get_tuner_switcher_active()" << endl;
    return false;
}

void GxMachineRemote::tuner_switcher_activate(bool v) {
    cerr << "tuner_switcher_activate()" << endl;
}

bool GxMachineRemote::tuner_switcher_deactivate() {
    cerr << "tuner_switcher_deactivate()" << endl;
    return false;
}

// preset
bool GxMachineRemote::setting_is_preset() {
    //cerr << "setting_is_preset()" << endl;
    return (!get_current_bank().empty());
}

static const Glib::ustring empty_string;

const Glib::ustring& GxMachineRemote::get_current_bank() {
    /*
    START_CALL(get);
    jw->write("system.current_bank");
    START_RECEIVE(empty_string);
    jp->next(gx_system::JsonParser::begin_object);
    jp->next(gx_system::JsonParser::value_key);
    jp->next(gx_system::JsonParser::value_string);
    return banks.get_file(jp->current_value())->get_name();
    END_RECEIVE(return empty_string);
    */
    return current_bank;
}

gx_system::PresetFileGui *GxMachineRemote::get_current_bank_file() {
    return get_bank_file(get_current_bank());
}

const Glib::ustring& GxMachineRemote::get_current_name() {
    /*
    gx_system::PresetFile *pf = get_current_bank_file();
    START_CALL(get);
    jw->write("system.current_preset");
    START_RECEIVE(empty_string);
    jp->next(gx_system::JsonParser::begin_object);
    jp->next(gx_system::JsonParser::value_key);
    jp->next(gx_system::JsonParser::value_string);
    return pf->get_name(pf->get_index(jp->current_value()));
    END_RECEIVE(return empty_string);
    */
    return current_preset;
}

gx_system::PresetFileGui* GxMachineRemote::get_bank_file(const Glib::ustring& bank) const { //FIXME
    return banks.get_file(bank)->get_guiwrapper();
}

Glib::ustring GxMachineRemote::get_bank_name(int n) {
    return banks.get_name(n);
}

void GxMachineRemote::load_preset(gx_system::PresetFileGui *pf, const Glib::ustring& name) {
    START_NOTIFY(setpreset);
    jw->write(pf->get_name());
    jw->write(name);
    SEND();
}

void GxMachineRemote::loadstate() {
    cerr << "loadstate()" << endl;
}

int GxMachineRemote::bank_size() {
    return banks.size();
}

void GxMachineRemote::create_default_scratch_preset() {
    cerr << "create_default_scratch_preset()" << endl;
}

void GxMachineRemote::set_statefilename(const std::string& fn) {
    cerr << "set_statefilename()" << endl;
}

void GxMachineRemote::save_to_state(bool preserve_preset) {
    cerr << "save_to_state()" << endl;
}

Glib::RefPtr<gx_preset::PluginPresetList> GxMachineRemote::load_plugin_preset_list(const Glib::ustring& id, bool factory) const {
    static gx_engine::MidiControllerList m;
    static std::string fname = "";
    static Glib::RefPtr<gx_preset::PluginPresetList> p = gx_preset::PluginPresetList::create(fname, const_cast<ParamMap&>(pmap), m);
    return p;
}

void GxMachineRemote::disable_autosave(bool v) {
    cerr << "disable_autosave()" << endl;
}

sigc::signal<void>& GxMachineRemote::signal_selection_changed() {
    return selection_changed;
}

sigc::signal<void>& GxMachineRemote::signal_presetlist_changed() {
    return presetlist_changed;
}

gx_system::PresetFileGui *GxMachineRemote::bank_insert_uri(const Glib::ustring& uri, bool move) {
    cerr << "bank_insert_uri()" << endl;
    START_CALL(bank_insert_content);
    jw->write(uri);
    Glib::RefPtr<Gio::File> rem = Gio::File::create_for_uri(uri);
    fstream f(rem->get_path().c_str());
    stringstream s;
    s << f.rdbuf();
    jw->write(s.str());
    START_RECEIVE(0);
    if (jp->peek() != gx_system::JsonParser::begin_object) {
	return 0;
    }
    gx_system::PresetFile *pf = new gx_system::PresetFile();
    pf->readJSON_remote(*jp);
    banks.insert(pf);
    return pf->get_guiwrapper();
    END_RECEIVE(return 0);
}

gx_system::PresetFileGui *GxMachineRemote::bank_insert_new(const Glib::ustring& newname) {
    cerr << "bank_insert_new()" << endl;
    START_CALL(bank_insert_new);
    jw->write(newname);
    START_RECEIVE(0);
    gx_system::PresetFile *pf = new gx_system::PresetFile();
    pf->readJSON_remote(*jp);
    banks.insert(pf);
    return pf->get_guiwrapper();
    END_RECEIVE(return 0);
}

bool GxMachineRemote::rename_bank(const Glib::ustring& oldname, Glib::ustring& newname) {
    cerr << "rename_bank()" << endl;
    START_CALL(rename_bank);
    jw->write(oldname);
    jw->write(newname);
    START_RECEIVE(false);
    jp->next(gx_system::JsonParser::begin_array);
    jp->next(gx_system::JsonParser::value_number);
    bool ret = jp->current_value_int();
    jp->next(gx_system::JsonParser::value_string);
    newname = jp->current_value();
    jp->next(gx_system::JsonParser::end_array);
    if (ret) {
	banks.get_file(oldname)->name = newname;
    }
    return ret;
    END_RECEIVE(return false);
}

bool GxMachineRemote::rename_preset(gx_system::PresetFileGui& pf, const Glib::ustring& oldname, const Glib::ustring& newname) {
    START_CALL(rename_preset);
    jw->write(pf.get_name());
    jw->write(oldname);
    jw->write(newname);
    START_RECEIVE(false);
    bool ret = get_bool(jp);
    if (ret) {
	int idx = pf.get_index(oldname);
	assert(idx >= 0);
	pf.entries[idx].name = newname;
    }
    return ret;
    END_RECEIVE(return false);
}

void GxMachineRemote::bank_reorder(const std::vector<Glib::ustring>& neworder) {
    cerr << "bank_reorder()" << endl;
    START_NOTIFY(bank_reorder);
    for (std::vector<Glib::ustring>::const_iterator i = neworder.begin(); i != neworder.end(); ++i) {
	jw->write(*i);
    }
    SEND();
    banks.reorder(neworder);
}

void GxMachineRemote::reorder_preset(gx_system::PresetFileGui& pf, const std::vector<Glib::ustring>& neworder) {
    cerr << "reorder_preset()" << endl;
    START_NOTIFY(reorder_preset);
    jw->write(pf.get_name());
    for (std::vector<Glib::ustring>::const_iterator i = neworder.begin(); i != neworder.end(); ++i) {
	jw->write(*i);
    }
    SEND();
    int n = 0;
    for (std::vector<Glib::ustring>::const_iterator i = neworder.begin(); i != neworder.end(); ++i) {
	pf.entries[n++].name = *i;
    }
    presetlist_changed();
}

bool GxMachineRemote::bank_check_reparse() {
    cerr << "bank_check_reparse()" << endl;
    START_CALL(bank_check_reparse);
    START_RECEIVE(false);
    return get_bool(jp);
    END_RECEIVE(return false);
}

void GxMachineRemote::erase_preset(gx_system::PresetFileGui& pf, const Glib::ustring& name) {
    cerr << "erase_preset()" << endl;
    START_NOTIFY(erase_preset);
    jw->write(pf.get_name());
    jw->write(name);
    SEND();
    for (gx_system::PresetFile::iterator i = pf.begin(); i != pf.end(); ++i) {
	if (i->name == name) {
	    pf.entries.erase(i);
	    break;
	}
    }
}

void GxMachineRemote::bank_set_flag(gx_system::PresetFileGui *pf, int flag, bool v) {
    cerr << "bank_set_flag()" << endl;
    START_NOTIFY(bank_set_flag);
    jw->write(pf->get_name());
    jw->write(flag);
    jw->write(v);
    SEND();
    pf->set_flag(flag, v);
}

std::string GxMachineRemote::bank_get_filename(const Glib::ustring& bank) {
    cerr << "bank_get_filename()" << endl;
    // FIXME Gtk3: bank_get_filename is used in DnD bank operation, and the
    // first request is due to an internal window of the DnD mechanism but
    // there seems to be no way to detect this. Skip this first request so
    // that no contents is send for a DnD reorder operation. Recheck for Gtk3
    switch (bank_drag_get_counter) {
    case 0:
	bank_drag_get_counter++;
	return "";
    case 1:
	bank_drag_get_counter++;
	START_CALL(bank_get_contents);
	jw->write(bank);
	START_RECEIVE(empty_string);
	jp->next(gx_system::JsonParser::begin_array);
	jp->next(gx_system::JsonParser::value_string);
	Glib::RefPtr<Gio::File> target = Gio::File::create_for_path(
	    options.get_temp_filepath(
		Gio::File::create_for_path(jp->current_value())->get_basename()));
	jp->next(gx_system::JsonParser::value_string);
	Glib::RefPtr<Gio::FileOutputStream> s = target->replace(
	    "", false, Gio::FILE_CREATE_REPLACE_DESTINATION);
	s->write(jp->current_value());
	s->close();
	jp->next(gx_system::JsonParser::end_array);
	bank_drag_get_path = target->get_path();
	END_RECEIVE(return empty_string);
    }
    return bank_drag_get_path;
}

void GxMachineRemote::bank_drag_begin() {
    bank_drag_get_counter = 0;
    bank_drag_get_path.clear();
}

gx_system::PresetFileGui *GxMachineRemote::bank_get_file(const Glib::ustring& bank) const {
    return banks.get_file(bank)->get_guiwrapper();
}

bank_iterator GxMachineRemote::bank_begin() {
    return banks.begin();
}

bank_iterator GxMachineRemote::bank_end() {
    return banks.end();
}

void GxMachineRemote::pf_append(gx_system::PresetFileGui& pf, const Glib::ustring& src, gx_system::PresetFileGui& pftgt, const Glib::ustring& name) {
    cerr << "pf_append()" << endl;
    START_NOTIFY(pf_append);
    jw->write(pf.get_name());
    jw->write(src);
    jw->write(pftgt.get_name());
    jw->write(name);
    SEND();
    pftgt.entries.push_back(gx_system::PresetFile::Position(name,0));
}

void GxMachineRemote::pf_insert_before(gx_system::PresetFileGui& pf, const Glib::ustring& src, gx_system::PresetFileGui& pftgt, const Glib::ustring& pos, const Glib::ustring& name) {
    cerr << "pf_insert_before()" << endl;
    START_NOTIFY(pf_insert_before);
    jw->write(pf.get_name());
    jw->write(src);
    jw->write(pftgt.get_name());
    jw->write(pos);
    jw->write(name);
    SEND();
    for (gx_system::PresetFile::iterator i = pftgt.begin(); i != pftgt.end(); ++i) {
	if (i->name == pos) {
	    pftgt.entries.insert(i, gx_system::PresetFile::Position(name, 0));
	    break;
	}
    }
}

void GxMachineRemote::pf_insert_after(gx_system::PresetFileGui& pf, const Glib::ustring& src, gx_system::PresetFileGui& pftgt, const Glib::ustring& pos, const Glib::ustring& name) {
    cerr << "pf_insert_after()" << endl;
    START_NOTIFY(pf_insert_after);
    jw->write(pf.get_name());
    jw->write(src);
    jw->write(pftgt.get_name());
    jw->write(pos);
    jw->write(name);
    SEND();
    for (gx_system::PresetFile::iterator i = pftgt.begin(); i != pftgt.end(); ++i) {
	if (i->name == pos) {
	    pftgt.entries.insert(++i, gx_system::PresetFile::Position(name, 0));
	    break;
	}
    }
}

bool GxMachineRemote::convert_preset(gx_system::PresetFileGui& pf) {
    cerr << "convert_preset()" << endl;
    START_CALL(convert_preset);
    jw->write(pf.get_name());
    START_RECEIVE(false);
    bool ret = get_bool(jp);
    if (ret) {
	pf.set_flag(gx_system::PRESET_FLAG_VERSIONDIFF, false);
    }
    return ret;
    END_RECEIVE(return false);
}

bool GxMachineRemote::bank_remove(const Glib::ustring& bank) {
    cerr << "bank_remove()" << endl;
    gx_system::PresetFile *f = banks.get_file(bank);
    START_CALL(bank_remove);
    jw->write(bank);
    START_RECEIVE(false);
    bool ret = get_bool(jp);
    if (ret) {
	banks.banklist.remove(f);
	delete f;
    }
    return ret;
    END_RECEIVE(return false);
}

void GxMachineRemote::bank_save() {
    cerr << "bank_save()" << endl;
    START_NOTIFY(bank_save);
    SEND();
}

void GxMachineRemote::pf_save(gx_system::PresetFileGui& pf, const Glib::ustring& name) {
    cerr << "pf_save()" << endl;
    START_NOTIFY(pf_save);
    jw->write(pf.get_name());
    jw->write(name);
    SEND();
}


// jack
gx_jack::GxJack *GxMachineRemote::get_jack() {
    return 0;
}

// pmap
Parameter& GxMachineRemote::get_parameter(const char *p) {
    return pmap[p];
}

Parameter& GxMachineRemote::get_parameter(const std::string& id) {
    return pmap[id];
}

void GxMachineRemote::set_init_values() {
    cerr << "set_init_values" << endl;
    for (ParamMap::iterator i = pmap.begin(); i != pmap.end(); ++i) {
	Parameter *p = i->second;
	if (p->isFloat()) {
	    FloatParameter& pf = p->getFloat();
	    pf.signal_changed()(pf.get_value());
	} else if (p->isInt()) {
	    IntParameter& pi = p->getInt();
	    pi.signal_changed()(pi.get_value());
	} else if (p->isBool()) {
	    BoolParameter& pb = p->getBool();
	    pb.signal_changed()(pb.get_value());
	} else if (p->isString()) {
	    StringParameter& ps = p->getString();
	    ps.signal_changed()(ps.get_value());
	} else if (p->isFile()) {
	    FileParameter& fp = p->getFile();
	    fp.signal_changed()();
	}
    }
    selection_changed(); //FIXME
    Glib::signal_io().connect(
	sigc::mem_fun(this, &GxMachineRemote::socket_input_handler),
	socket->get_fd(), Glib::IO_IN|Glib::IO_HUP);
    for (ParamMap::iterator i = pmap.begin(); i != pmap.end(); ++i) {
	if (i->second->isInt()) {
	    i->second->getInt().signal_changed().connect(
		sigc::bind(sigc::mem_fun(this, &GxMachineRemote::param_signal_int), &i->second->getInt()));
	} else if (i->second->isBool()) {
	    i->second->getBool().signal_changed().connect(
		sigc::bind(sigc::mem_fun(this, &GxMachineRemote::param_signal_bool), &i->second->getBool()));
	} else if (i->second->isFloat()) {
	    i->second->getFloat().signal_changed().connect(
		sigc::bind(sigc::mem_fun(this, &GxMachineRemote::param_signal_float), &i->second->getFloat()));
	} else if (i->second->isString()) {
	    i->second->getString().signal_changed().connect(
		sigc::bind(sigc::mem_fun(this, &GxMachineRemote::param_signal_string), &i->second->getString()));
	}
    }
}

bool GxMachineRemote::parameter_hasId(const char *p) {
    return pmap.hasId(p);
}

bool GxMachineRemote::parameter_hasId(const std::string& id) {
    return pmap.hasId(id);
}

void GxMachineRemote::reset_unit(Glib::ustring group_id) const {
    pmap.reset_unit(group_id, 0);
}

bool GxMachineRemote::parameter_unit_has_std_values(Glib::ustring group_id) const {
    return pmap.unit_has_std_values(group_id, 0);
}

void GxMachineRemote::set_parameter_value(const std::string& id, int value) {
    pmap[id].getInt().set(value);
}

void GxMachineRemote::set_parameter_value(const std::string& id, bool value) {
    pmap[id].getBool().set(value);
}

void GxMachineRemote::set_parameter_value(const std::string& id, float value) {
    pmap[id].getFloat().set(value);
}

void GxMachineRemote::set_parameter_value(const std::string& id, const std::string& value) {
    if (!pmap[id].getString().set(value)) {
	return;
    }
    cerr << "SVS " << id << ": " << value << endl;
    START_NOTIFY(set);
    jw->write(id);
    jw->write(value);
    SEND();
}

int GxMachineRemote::_get_parameter_value_int(const std::string& id) {
    START_CALL(get);
    jw->write(id);
    START_RECEIVE(0);
    jp->next(gx_system::JsonParser::begin_object);
    jp->next(gx_system::JsonParser::value_key);
    int v;
    switch (jp->next()) {
    case gx_system::JsonParser::value_string:
	v = pmap[id].getEnum().idx_from_id(jp->current_value());
	break;
    case gx_system::JsonParser::value_number:
	v = jp->current_value_int();
	break;
    default:
	assert(false);
	return 0;
    }
    //cerr << "GVI " << id << ": " << v << endl;
    return v;
    END_RECEIVE(return 0);
}

int GxMachineRemote::_get_parameter_value_bool(const std::string& id) {
    START_CALL(get);
    jw->write(id);
    START_RECEIVE(false);
    jp->next(gx_system::JsonParser::begin_object);
    jp->next(gx_system::JsonParser::value_key);
    jp->next(gx_system::JsonParser::value_number);
    //cerr << "GVB " << id << ": " << jp->current_value_int() << endl;
    return jp->current_value_int();
    END_RECEIVE(return false);
}

float GxMachineRemote::_get_parameter_value_float(const std::string& id) {
    START_CALL(get);
    jw->write(id);
    START_RECEIVE(0);
    jp->next(gx_system::JsonParser::begin_object);
    jp->next(gx_system::JsonParser::value_key);
    float v;
    switch (jp->next()) {
    case gx_system::JsonParser::value_string:
	v = dynamic_cast<FloatEnumParameter*>(&pmap[id])->idx_from_id(jp->current_value());
	break;
    case gx_system::JsonParser::value_number:
	v = jp->current_value_float();
	break;
    default:
	assert(false);
	return 0;
    }
    //cerr << "GVF " << id << ": " << jp->current_value_float() << endl;
    return v;
    END_RECEIVE(return 0);
}

std::string GxMachineRemote::_get_parameter_value_string(const std::string& id) {
    START_CALL(get);
    jw->write(id);
    START_RECEIVE(empty_string);
    jp->next(gx_system::JsonParser::begin_object);
    jp->next(gx_system::JsonParser::value_key);
    jp->next(gx_system::JsonParser::value_string);
    //cerr << "GVS " << id << ": " << jp->current_value() << endl;
    return jp->current_value();
    END_RECEIVE(return empty_string);
}

sigc::signal<void, int>& GxMachineRemote::_signal_parameter_value_int(const std::string& id) {
    return pmap[id].signal_changed_int();
}

sigc::signal<void, bool>& GxMachineRemote::_signal_parameter_value_bool(const std::string& id) {
    return pmap[id].signal_changed_bool();
}

sigc::signal<void, float>& GxMachineRemote::_signal_parameter_value_float(const std::string& id) {
    return pmap[id].signal_changed_float();
}

// MidiControllerList
bool GxMachineRemote::midi_get_config_mode(int *ctl) {
    //cerr << "midi_get_config_mode()" << endl;
    START_CALL(midi_get_config_mode);
    START_RECEIVE(false);
    jp->next(gx_system::JsonParser::begin_array);
    bool ret = get_bool(jp);
    jp->next(gx_system::JsonParser::value_number);
    if (ret && ctl) {
	*ctl = jp->current_value_int();
    }
    jp->next(gx_system::JsonParser::end_array);
    return ret;
    END_RECEIVE(return false);
}

void GxMachineRemote::midi_set_config_mode(bool v, int ctl) {
    cerr << "midi_set_config_mode(" << v << "," << ctl << ")" << endl;
    START_NOTIFY(midi_set_config_mode);
    jw->write(v);
    jw->write(ctl);
    SEND();
}

sigc::signal<void>& GxMachineRemote::signal_midi_changed() {
    cerr << "signal_midi_changed()" << endl;
    return midi_changed;
}

sigc::signal<void, int, int>& GxMachineRemote::signal_midi_value_changed() {
    cerr << "signal_midi_value_changed()" << endl;
    return midi_value_changed;
}

void GxMachineRemote::request_midi_value_update() {
    cerr << "request_midi_value_update()" << endl;
    START_NOTIFY(request_midi_value_update);
    SEND();
}

int GxMachineRemote::midi_size() {
    return midi_controller_map.size();
}

midi_controller_list& GxMachineRemote::midi_get(int n) {
    cerr << "midi_get()" << endl;
    return midi_controller_map[n];
}

void GxMachineRemote::midi_deleteParameter(Parameter& param, bool quiet) {
    cerr << "midi_deleteParameter()" << endl;
    START_NOTIFY(midi_deleteParameter);
    jw->write(param.id());
    jw->write(quiet);
    SEND();
}

void GxMachineRemote::midi_set_current_control(int v) {
    cerr << "midi_set_current_control()" << endl;
    START_NOTIFY(midi_set_current_control);
    jw->write(v);
    SEND();
}

void GxMachineRemote::midi_modifyCurrent(Parameter& param, float lower, float upper, bool toggle) {
    cerr << "midi_modifyCurrent()" << endl;
    START_NOTIFY(midi_modifyCurrent);
    jw->write(param.id());
    jw->write(lower);
    jw->write(upper);
    jw->write(toggle);
    SEND();
}

int GxMachineRemote::midi_param2controller(Parameter& param, const MidiController** p) {
    cerr << "midi_param2controller()" << endl;
    return midi_controller_map.param2controller(param, p);
}

// cheat
ConvolverMonoAdapter& GxMachineRemote::get_mono_convolver() {
    cerr << "get_mono_convolver()" << endl;
    return *(ConvolverMonoAdapter*)0;
}

ConvolverStereoAdapter& GxMachineRemote::get_stereo_convolver() {
    cerr << "get_stereo_convolver()" << endl;
    return *(ConvolverStereoAdapter*)0;
}


} // namespace gx_engine
