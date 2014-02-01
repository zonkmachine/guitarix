#! /bin/bash

# use to copy the LV2 bundle gx_sceleton.lv2 to gx_${name}.lv2
# generate a C++ file from the faust.dsp source (by dsp2cc)
# generate a acording name.ttl file
# were ${name} is the first arg on comandline, usualy the dsp file.
# the dsp file needs to be stored in the faust folder.

########################## functions ###################################

function usage() {
  echo "usage: make_lv2_bundle {options} <cc|cpp|dsp-file> [<plugin class>] "
  echo "options:"
  echo "    -h:   show this help"
  echo "    -s:   faust use single precision"
  echo "    -d:   faust use double precision (default)"
  echo "    -V:   faust use vectorize"
  echo "    -S x: faust use vector size x"
  echo "    -p /: path to source file"
  echo "    -c $: color to use in the MOD jason file"
  exit 1
}

function get_basename_ext() {
case "$extension" in
  dsp)
    if [ "$2" = "" ]; then
      bname="$(basename "$1" .dsp)"
    else
      bname="$(basename "$1" .dsp)"
      effect_name="$2"
    fi;;
  cc)
    if [ "$2" = "" ]; then
      bname="$(basename "$1" .cc)"
    else
      bname="$(basename "$1" .cc)"
      effect_name="$2"
    fi;;
  cpp)
    if [ "$2" = "" ]; then
      bname="$(basename "$1" .cpp)"
    else
      bname="$(basename "$1" .cpp)"
      effect_name="$2"
    fi;;
  *)
   echo -e $RED"file extension not supported"$NONE ; usage;;
esac
}

function select_plugin_type() {
  echo -e $RED'Please select a Plugin type from the list'$NONE
  select type in "${Plugin_types[@]}" ; do 
    if [[ $type == "quit" ]]; then
      break
    fi
    effect_name="$type"
    if [ ! -z "$type" ]; then
      break;
    else
      echo -e $RED"Select a number from the list!"$NONE
    fi
  done
}

function check_plugin_type() { 
  for I in "${Plugin_types[@]}"; do
    [[ $I == $effect_name ]] && return 0
  done
  select_plugin_type
  return 0
}

function check_dir() {
  if [ ! -d gx_${bname}.lv2 ]; then
    mkdir -p gx_${bname}.lv2
  else
    echo -e $RED"Directory gx_${bname}.lv2 allready exist, should we remove it? Yes/No"$NONE
    select yn in "Yes" "No"; do
      case $yn in
        Yes ) rm -rf gx_${bname}.lv2; mkdir -p gx_${bname}.lv2; break;;
        No ) echo  -e $RED"exit"$NONE; exit;;
      esac
    done
  fi
}

function dsptocc() {
  if [ -f ${faustdir}/"$bname.$extension" ] ; then
    if [ $extension == "dsp" ] ; then
      echo -e $BLUE"generate $bname.cc and copy to gx_$bname.lv2"$NONE
      set -e
      "$tooldir"/dsp2cc  --init-type=plugin-lv2  \
        $prec "${faustopt[@]}" -o gx_${bname}.lv2/"$bname.cc" ${faustdir}/"$bname.dsp"
    else
      echo -e $BLUE"copy $bname.$extension to gx_$bname.lv2/$bname.cc"$NONE
      cp -r ${faustdir}/"$bname.$extension" gx_${bname}.lv2/"$bname.cc"
    fi
  else
    echo  -e $RED"error: "${faustdir}/"$bname.$extension not found"$NONE; exit;
  fi
}


function copy_sceleton() {
  grep 'stereo_audio' gx_${bname}.lv2/$bname.cc >/dev/null && stereo="true"  || stereo="false"
  if [ "$stereo" == "true" ] ; then
    grep 'stereo_audio = 0;' gx_${bname}.lv2/$bname.cc >/dev/null && stereo="false"  || stereo="true"
  fi

  if [ "$stereo" == "false" ] ; then
    echo "copy gx_sceleton.lv2 to gx_$bname.lv2 and rename/replace strings to $bname"
    cp -r gx_sceleton.lv2/* gx_${bname}.lv2/ 
    j=2
  else
    echo "copy gx_sceleton_stereo.lv2 to gx_$bname.lv2 and rename/replace strings to $bname"
    cp -r gx_sceleton_stereo.lv2/* gx_${bname}.lv2/ 
    j=4
  fi
  cd ./gx_${bname}.lv2 && rename 's/sceleton/'${bname}'/g' * && sed -i 's/sceleton/'${bname}'/g' *
}

function grep_ports_enums() {
  echo "grep ports and copy them to  gx_$bname.h"
  cat "$bname.cc" | sed -n '/enum/,/PortIndex/p' |  sed '/enum/d;/PortIndex/d;/{/d;/}/d'>ports

  if [ "$stereo" == "false" ] ; then
    sed -i -e '/EFFECTS_INPUT/r ports' "gx_$bname.h"
  else
    sed -i -e '/EFFECTS_INPUT1/r ports' "gx_$bname.h"
  fi
  echo "grep ports values and enums and copy them to gx_$bname.ttl"
  cat "$bname.cc" | sed -n '/data;/{p;g;1!p;};h' | sed 's/ , /\n/;s/.*\n//;s/case//g;s/,/ ;/g;s/://g;s/	 //g;s/  //g;s/ //g;s/$/;/' | sed '$!N;s/\n//'>ports
  cat "$bname.cc" | sed -n '/value_pair/{p;n;1!p;};h' | sed 's/{/\n/;s/.*\n//;s/ , /\n/;s/.*\n//;s/case//g;s/}//g;s/{//g;s/;//g;s/,/ ;/g;s/://g;s/	 //g;s/;0//g;s/  //g;s/ //g;s/$/;/' > enums
}

function make_jason() {
  echo -e "generate "$BLUE"/modgui/data-gx${bname}.jason"$NONE
  mkdir -p modgui
  echo '{
    "author": "Guitarix", 
    "color": "'$color'", 
    "controls": [' > modgui/data-gx${bname}.jason
  
  while IFS=$';' read -r -a myPorts; do
    jports+='        {
            "name": "'${myPorts[4]}'", 
            "symbol": "'${myPorts[4]}'"
        },\n' 
  done < ports 
  jports=${jports%%???}
  echo -e "$jports" >> modgui/data-gx${bname}.jason
  echo '    ], 
    "label": "gx'$bname'"
}' >> modgui/data-gx${bname}.jason
}

function make_html() {
  echo -e "generate "$BLUE"/modgui/icon-gx${bname}.html"$NONE
  if [ "$stereo" == "false" ] ; then
    j=$[j-2]
  else
    j=$[j-4]
  fi
  mod_knobs="mod-one-knobs"
  case "$j" in
    1) mod_knobs="mod-one-knobs";;
    2) mod_knobs="mod-two-knobs";;
    3) mod_knobs="mod-three-knobs";;
    4) mod_knobs="mod-four-knobs";;
    5) mod_knobs="mod-five-knobs";;
    6) mod_knobs="mod-six-knobs";;
    7) mod_knobs="mod-seven-knobs";;
    8) mod_knobs="mod-eight-knobs";;
  esac

echo '<div class="mod-pedal mod-pedal-boxy '${mod_knobs}' mod-{{color}}">
    <div mod-role="drag-handle" class="mod-drag-handle"></div>
    <div class="mod-plugin-author"><h1>{{author}}</h1></div>
    <div class="mod-plugin-name"><h1>{{label}}</h1></div>
    <div class="mod-light on" mod-role="bypass-light"></div>
    <div class="mod-control-group clearfix">
        {{#controls}}
        <div class="mod-knob">
            <div class="mod-knob-image" mod-role="input-control-port" mod-port-symbol="{{symbol}}"></div>
            <span class="mod-knob-title">{{name}}</span>
        </div>
        {{/controls}}
    </div>
    <div class="mod-footswitch" mod-role="bypass"></div>  
    <div class="mod-pedal-input">
        {{#effect.ports.audio.input}}
        <div class="mod-input mod-input-disconnected" title="{{name}}" mod-role="input-audio-port" mod-port-symbol="{{symbol}}">
            <div class="mod-pedal-input-image"></div>
        </div>
        {{/effect.ports.audio.input}}
        {{#effect.ports.midi.input}}
        <div class="mod-input mod-input-disconnected" title="{{name}}" mod-role="input-midi-port" mod-port-symbol="{{symbol}}">
            <div class="mod-pedal-input-image"></div>
        </div>
        {{/effect.ports.midi.input}}
    </div>
    <div class="mod-pedal-output">
        {{#effect.ports.audio.output}}
        <div class="mod-output mod-output-disconnected" title="{{name}}" mod-role="output-audio-port" mod-port-symbol="{{symbol}}">
            <div class="mod-pedal-output-image"></div>
        </div>
        {{/effect.ports.audio.output}}
        {{#effect.ports.midi.output}}
        <div class="mod-output mod-output-disconnected" title="{{name}}" mod-role="output-midi-port" mod-port-symbol="{{symbol}}">
            <div class="mod-pedal-input-image"></div>
        </div>
        {{/effect.ports.midi.output}}
    </div>
</div>' > modgui/icon-gx$bname.html
}

function make_ttl() {
  match=0

  while IFS=$';' read -r -a myPorts; do
    while IFS=$';' read -r -a myEnum; do
      if [ -z "${myEnum[0]}" ]; then
        match=0
      else
        a=${#myEnum[@]} 
        if (($a==1)); then
          if [ ${myPorts[4]} == ${myEnum[0]} ]; then
            match=1
            sed -i -e "1,2d" enums
            break
          fi
        else
          match=0
          FLOAT=${myPorts[1]}
          i=${FLOAT/\.*}
          for (( c=0; c<a; c++ )); do 
            enum_var1+="        lv2:scalePoint [rdfs:label "${myEnum[c]}"; rdf:value "${i}"];\n"
            i=$[i+1]
          done
        fi
      fi
    done < enums
    echo '      , [
        a lv2:InputPort ,
            lv2:ControlPort ;
        lv2:index' $j' ;
        lv2:symbol "'${myPorts[4]}'" ;
        lv2:name "'${myPorts[4]}'" ;'
    echo '        lv2:default '${myPorts[0]}' ;
        lv2:minimum '${myPorts[1]}' ;
        lv2:maximum '${myPorts[2]}' ;'
    if (($match==1)); then
      echo '        lv2:portProperty lv2:integer;
        lv2:portProperty lv2:enumeration ; '
      echo -e "$enum_var1"
      enum_var1=""
      match=0
    fi
    echo -n '    ]'
    j=$[j+1]
    enum_var1=""
  done < ports >> gx_$bname.ttl
  echo ";
    mod:gui [
        a mod:Gui;
        mod:resourcesDirectory <modgui>;
        mod:iconTemplate <modgui/icon-gx$bname.html>;
        mod:templateData <modgui/data-gx$bname.json>;
        mod:screenshot <modgui/screenshot-gx$bname.png>;
        mod:thumbnail <modgui/thumb-gx$bname.png>;
    ]." >> gx_$bname.ttl
  if [ ! -z "$effect_name" ]; then
    echo -e "set plugin class to "$BLUE"$effect_name"$NONE
    sed -i 's/EffectPlugin/'${effect_name}'/g'  gx_$bname.ttl
  fi
  rm -rf ports
  rm -rf enums
}

function byby() {
  echo -e $BLUE"Okay, gx_${bname}.lv2 is done"$NONE
  echo  -e $BLUE"Please add gx_${bname}.lv2 to the wscript in the LV2 directory
and run ./waf build in order to build the plug"$NONE
}

########################## supported plugin types ######################

Plugin_types=(
        'DelayPlugin'
            'ReverbPlugin'
        'DistortionPlugin'
            'WaveshaperPlugin'
        'DynamicsPlugin'
            'AmplifierPlugin'
            'CompressorPlugin'
            'EnvelopePlugin'
            'ExpanderPlugin'
            'GatePlugin'
            'LimiterPlugin'
        'FilterPlugin'
            'AllpassPlugin'
            'BandpassPlugin'
            'CombPlugin'
            'EQPlugin'
               'MultiEQPlugin'
                'ParaEQPlugin'
            'HighpassPlugin'
            'LowpassPlugin'
        'GeneratorPlugin'
            'ConstantPlugin'
            'InstrumentPlugin'
            'OscillatorPlugin'
        'ModulatorPlugin'
            'ChorusPlugin'
            'FlangerPlugin'
            'PhaserPlugin'
        'ReverbPlugin'
        'SimulatorPlugin'
            'ReverbPlugin'
        'SpatialPlugin'
        'SpectralPlugin'
            'PitchPlugin'
        'UtilityPlugin'
            'AnalyserPlugin'
            'ConverterPlugin'
            'FunctionPlugin'
            'MixerPlugin' )



###################### global variables ################################

BLUE="\033[1;34m"
RED="\033[1;31m"
NONE="\033[0m"

tooldir=../../tools
faustdir=./faust
instdir="$tooldir"/.."$(dirname "$0")"
prec="--double"
faustopt=()
copy=0
bname=""
effect_name=""
color="yellow"

############################# main #####################################

while getopts hsdVSp:c OPT; do
  case "$OPT" in
  h) usage;;
  s) prec="--float";;
  d) prec="--double";;
  V) faustopt+=(--vectorize);;
  S) faustopt+=(--add="-vs $OPTARG");;
  p) faustdir=$OPTARG;;
  c) color=$OPTARG;;
  \?) usage;;
  esac
done

shift $(expr $OPTIND - 1)
[ "$1" = "" ] && usage
extension="${1##*.}"

######################## function calls ################################

get_basename_ext "$1" "$2"
check_plugin_type && echo -e "Plugin type "$BLUE"$effect_name"$NONE
check_dir
dsptocc
copy_sceleton
grep_ports_enums
make_jason
make_ttl
make_html
byby

############################ EOF #######################################

exit
