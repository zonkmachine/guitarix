v 20130925 2
C 40000 40000 0 0 0 title-B.sym
C 47800 44000 1 90 0 resistor-2.sym
{
T 47450 44400 5 10 0 0 90 0 1
device=RESISTOR
T 48200 44700 5 10 1 1 180 0 1
refdes=R4
T 47900 44200 5 10 1 1 0 0 1
value=390
}
C 44900 45700 1 0 0 input-1.sym
{
T 44900 46000 5 10 0 0 0 0 1
device=INPUT
T 44900 45700 5 10 1 1 0 0 1
refdes=IN1
}
C 49700 45800 1 180 1 output-1.sym
{
T 49800 45500 5 10 0 0 180 6 1
device=OUTPUT
T 49900 45800 5 10 1 1 180 6 1
refdes=OUT1
}
C 46700 46300 1 90 0 resistor-2.sym
{
T 46350 46700 5 10 0 0 90 0 1
device=RESISTOR
T 46400 47000 5 10 1 1 180 0 1
refdes=R2
T 46000 46500 5 10 1 1 0 0 1
value=430k
}
C 47800 46300 1 90 0 resistor-2.sym
{
T 47450 46700 5 10 0 0 90 0 1
device=RESISTOR
T 47500 47100 5 10 1 1 180 0 1
refdes=R3
T 47200 46600 5 10 1 1 0 0 1
value=10k
}
C 49200 46500 1 180 0 capacitor-1.sym
{
T 49000 45800 5 10 0 0 180 0 1
device=CAPACITOR
T 48400 46400 5 10 1 1 0 0 1
refdes=C2
T 49000 45600 5 10 0 0 180 0 1
symversion=0.1
T 49300 46600 5 10 1 1 180 0 1
value=0.002u
}
C 47200 45300 1 0 0 npn-2.sym
{
T 47800 45800 5 10 0 0 0 0 1
device=NPN_TRANSISTOR
T 47600 45800 5 10 1 1 0 0 1
refdes=T1
T 47200 45300 5 10 0 1 0 0 1
value=Vt=26mV,Is=20.3fA,Bf=1430,Br=4
T 47700 45600 5 10 1 1 0 0 1
model-name=2N5133
}
C 46700 44000 1 90 0 resistor-2.sym
{
T 46350 44400 5 10 0 0 90 0 1
device=RESISTOR
T 46400 44400 5 10 1 1 180 0 1
refdes=R1
T 46800 44100 5 10 1 1 0 0 1
value=43k
}
C 49300 43700 1 0 1 gnd-1.sym
C 46700 43700 1 0 1 gnd-1.sym
C 47800 43700 1 0 1 gnd-1.sym
C 46600 46000 1 180 0 capacitor-1.sym
{
T 46400 45300 5 10 0 0 180 0 1
device=CAPACITOR
T 45800 45900 5 10 1 1 0 0 1
refdes=C1
T 46400 45100 5 10 0 0 180 0 1
symversion=0.1
T 46400 45500 5 10 1 1 180 0 1
value=0.002u
}
C 49100 45200 1 270 1 resistor-variable-2.sym
{
T 50000 46000 5 10 0 1 90 2 1
device=VARIABLE_RESISTOR
T 48850 45900 5 10 1 1 180 6 1
refdes=P1
T 51400 45400 5 10 1 1 180 0 1
value=value=100k,var=Scream
}
C 47500 47200 1 0 0 9V-plus-1.sym
{
T 47500 47200 5 10 0 0 0 0 1
device=VOLTAGE_SOURCE
T 47800 47200 5 10 1 1 0 0 1
value=+9V
T 47500 47600 5 10 1 1 0 0 1
refdes=Vdc
}
N 46600 45800 47200 45800 4
N 47700 46300 48300 46300 4
N 46600 44900 46600 46300 4
N 47700 44900 47700 45300 4
N 46600 47200 47700 47200 4
N 49200 46300 49200 46100 4
C 49300 44000 1 90 0 resistor-2.sym
{
T 48950 44400 5 10 0 0 90 0 1
device=RESISTOR
T 49700 44700 5 10 1 1 180 0 1
refdes=R5
T 49400 44200 5 10 1 1 0 0 1
value=10k
}
N 49200 44900 49200 45200 4