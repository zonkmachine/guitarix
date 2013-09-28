/*
 * Copyright (C) 2012 Hermann Meyer, Andreas Degert, Pete Shorthose, Steve Poskitt
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
 * --------------------------------------------------------------------------
 */

//#include <zita-resampler/resampler.h>
//#include <cassert>
////////////////////////////// LOCAL INCLUDES //////////////////////////
#include "gx_resampler.h"
#include "gx_common.h"      // faust support and denormal protection (SSE)
#include "gx_fuzz.h"        // define struct PortIndex
#include "gx_pluginlv2.h"   // define struct PluginLV2
#include "bmfp.cc"    // dsp class generated by faust -> dsp2cc
#include "lowpass_up.cc"    // dsp class generated by faust -> dsp2cc
#ifndef __SSE__
#include "noiser.cc"
#endif

////////////////////////////// PLUG-IN CLASS ///////////////////////////

class Gx_fuzz_
{
private:
  // pointer to buffer
  float*      output;
  float*      input;
  // pointer to dsp class
  PluginLV2*  fuzz;
  PluginLV2*  pass;
#ifndef __SSE__
  PluginLV2*  wn;
#endif
  // private functions
  inline void run_dsp_(uint32_t n_samples);
  inline void connect_(uint32_t port,void* data);
  inline void init_dsp_(uint32_t rate);
  inline void connect_all__ports(uint32_t port, void* data);
  inline void activate_f();
  inline void clean_up();
  inline void deactivate_f();
  gx_resample::SimpleResampler resamp;
  
public:
  // LV2 Descriptor
  static const LV2_Descriptor descriptor;
  // static wrapper to private functions
  static void deactivate(LV2_Handle instance);
  static void cleanup(LV2_Handle instance);
  static void run(LV2_Handle instance, uint32_t n_samples);
  static void activate(LV2_Handle instance);
  static void connect_port(LV2_Handle instance, uint32_t port, void* data);
  static LV2_Handle instantiate(const LV2_Descriptor* descriptor,
                                double rate, const char* bundle_path,
                                const LV2_Feature* const* features);
  Gx_fuzz_();
  ~Gx_fuzz_();
};

// constructor
Gx_fuzz_::Gx_fuzz_() :
  output(NULL),
  input(NULL),
  fuzz(bmfp::plugin()),
  pass(lowpass_up::plugin()) {};

// destructor
Gx_fuzz_::~Gx_fuzz_()
{
  // just to be sure the plug have given free the allocated mem
  // it didn't hurd if the mem is already given free by clean_up()
  if (fuzz->activate_plugin !=0)
    fuzz->activate_plugin(false, fuzz);
  // delete DSP class
  fuzz->delete_instance(fuzz);
  if (pass->activate_plugin !=0)
    pass->activate_plugin(false, pass);
  // delete DSP class
  pass->delete_instance(pass);
};

///////////////////////// PRIVATE CLASS  FUNCTIONS /////////////////////

void Gx_fuzz_::init_dsp_(uint32_t rate)
{
  AVOIDDENORMALS(); // init the SSE denormal protection
  resamp.setup(rate, 4);
  fuzz->set_samplerate(4*rate, fuzz); // init the DSP class
  pass->set_samplerate(rate, pass); // init the DSP class
#ifndef __SSE__
  wn = noiser::plugin();
  wn->set_samplerate(rate, wn);
#endif
}

// connect the Ports used by the plug-in class
void Gx_fuzz_::connect_(uint32_t port,void* data)
{
  switch ((PortIndex)port)
    {
    case EFFECTS_OUTPUT:
      output = static_cast<float*>(data);
      break;
    case EFFECTS_INPUT:
      input = static_cast<float*>(data);
      break;
    default:
      break;
    }
}

void Gx_fuzz_::activate_f()
{
  // allocate the internal DSP mem
  if (fuzz->activate_plugin !=0)
    fuzz->activate_plugin(true, fuzz);
  if (pass->activate_plugin !=0)
    pass->activate_plugin(true, pass);
}

void Gx_fuzz_::clean_up()
{
  // delete the internal DSP mem
  if (fuzz->activate_plugin !=0)
    fuzz->activate_plugin(false, fuzz);
  if (pass->activate_plugin !=0)
    pass->activate_plugin(false, pass);
}

void Gx_fuzz_::deactivate_f()
{
  // delete the internal DSP mem
  if (fuzz->activate_plugin !=0)
    fuzz->activate_plugin(false, fuzz);
  if (pass->activate_plugin !=0)
    pass->activate_plugin(false, pass);
}

void Gx_fuzz_::run_dsp_(uint32_t n_samples)
{
#ifndef __SSE__
  wn->mono_audio(static_cast<int>(n_samples), input, input, wn);;
#endif
  pass->mono_audio(n_samples, input, output, pass);
  float buf[resamp.get_max_out_size(n_samples)];
  int n = resamp.up(n_samples, output, buf);
  fuzz->mono_audio(n, buf, buf, fuzz);
  resamp.down(n_samples,buf, output);
}

void Gx_fuzz_::connect_all__ports(uint32_t port, void* data)
{
  // connect the Ports used by the plug-in class
  connect_(port,data); 
  // connect the Ports used by the DSP class
  fuzz->connect_ports(port,  data, fuzz);
}

////////////////////// STATIC CLASS  FUNCTIONS  ////////////////////////

LV2_Handle 
Gx_fuzz_::instantiate(const LV2_Descriptor* descriptor,
                            double rate, const char* bundle_path,
                            const LV2_Feature* const* features)
{
  // init the plug-in class
  Gx_fuzz_ *self = new Gx_fuzz_();
  if (!self)
    {
      return NULL;
    }

  self->init_dsp_((uint32_t)rate);

  return (LV2_Handle)self;
}

void Gx_fuzz_::connect_port(LV2_Handle instance, 
                                   uint32_t port, void* data)
{
  // connect all ports
  static_cast<Gx_fuzz_*>(instance)->connect_all__ports(port, data);
}

void Gx_fuzz_::activate(LV2_Handle instance)
{
  // allocate needed mem
  static_cast<Gx_fuzz_*>(instance)->activate_f();
}

void Gx_fuzz_::run(LV2_Handle instance, uint32_t n_samples)
{
  // run dsp
  static_cast<Gx_fuzz_*>(instance)->run_dsp_(n_samples);
}

void Gx_fuzz_::deactivate(LV2_Handle instance)
{
  // free allocated mem
  static_cast<Gx_fuzz_*>(instance)->deactivate_f();
}

void Gx_fuzz_::cleanup(LV2_Handle instance)
{
  // well, clean up after us
  Gx_fuzz_* self = static_cast<Gx_fuzz_*>(instance);
  self->clean_up();
  delete self;
}

const LV2_Descriptor Gx_fuzz_::descriptor =
{
  GXPLUGIN_URI "#fuzz_",
  Gx_fuzz_::instantiate,
  Gx_fuzz_::connect_port,
  Gx_fuzz_::activate,
  Gx_fuzz_::run,
  Gx_fuzz_::deactivate,
  Gx_fuzz_::cleanup,
  NULL
};

////////////////////////// LV2 SYMBOL EXPORT ///////////////////////////

extern "C"
LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
  switch (index)
    {
    case 0:
      return &Gx_fuzz_::descriptor;
    default:
      return NULL;
    }
}

///////////////////////////// FIN //////////////////////////////////////
