// generated from file '../src/faust/drumseq.dsp' by dsp2cc:
// Code generated with Faust 0.9.90 (http://faust.grame.fr)


namespace drumseq {


Dsp::Dsp() {
}

Dsp::~Dsp() {
}

inline void Dsp::clear_state_f()
{
	for (int i=0; i<2; i++) iVec0[i] = 0;
	for (int i=0; i<2; i++) fVec1[i] = 0;
	for (int i=0; i<2; i++) fRec0[i] = 0;
	for (int i=0; i<2; i++) iRec5[i] = 0;
	for (int i=0; i<2; i++) fRec4[i] = 0;
	for (int i=0; i<3; i++) fRec3[i] = 0;
	for (int i=0; i<2; i++) fVec2[i] = 0;
	for (int i=0; i<2; i++) fRec2[i] = 0;
	for (int i=0; i<3; i++) fRec1[i] = 0;
	for (int i=0; i<2; i++) fRec8[i] = 0;
	for (int i=0; i<2; i++) iRec6[i] = 0;
	for (int i=0; i<2; i++) fRec7[i] = 0;
	for (int i=0; i<2; i++) fRec13[i] = 0;
	for (int i=0; i<2; i++) fRec14[i] = 0;
	for (int i=0; i<2; i++) fRec15[i] = 0;
	for (int i=0; i<2; i++) fRec11[i] = 0;
	for (int i=0; i<2; i++) fRec12[i] = 0;
	for (int i=0; i<2; i++) fRec10[i] = 0;
	for (int i=0; i<3; i++) fRec9[i] = 0;
	for (int i=0; i<2; i++) fRec18[i] = 0;
	for (int i=0; i<2; i++) iRec16[i] = 0;
	for (int i=0; i<2; i++) fRec17[i] = 0;
	for (int i=0; i<2; i++) fRec19[i] = 0;
	for (int i=0; i<2; i++) fRec20[i] = 0;
	for (int i=0; i<2; i++) fVec3[i] = 0;
	for (int i=0; i<2; i++) fRec23[i] = 0;
	for (int i=0; i<2; i++) iRec21[i] = 0;
	for (int i=0; i<2; i++) fRec22[i] = 0;
	for (int i=0; i<2; i++) fRec26[i] = 0;
	for (int i=0; i<2; i++) fRec27[i] = 0;
	for (int i=0; i<2; i++) fRec28[i] = 0;
	for (int i=0; i<2; i++) fRec24[i] = 0;
	for (int i=0; i<2; i++) fRec25[i] = 0;
	for (int i=0; i<2; i++) fRec30[i] = 0;
	for (int i=0; i<2; i++) fRec31[i] = 0;
	for (int i=0; i<2; i++) fRec32[i] = 0;
	for (int i=0; i<2; i++) fRec33[i] = 0;
	for (int i=0; i<2; i++) iRec34[i] = 0;
	for (int i=0; i<2; i++) fRec35[i] = 0;
	for (int i=0; i<2; i++) fVec4[i] = 0;
	for (int i=0; i<2; i++) fRec36[i] = 0;
	for (int i=0; i<2; i++) fRec41[i] = 0;
	for (int i=0; i<2; i++) fVec5[i] = 0;
	for (int i=0; i<64; i++) fVec6[i] = 0;
	for (int i=0; i<2; i++) fRec42[i] = 0;
	for (int i=0; i<2; i++) fRec43[i] = 0;
	for (int i=0; i<2; i++) fRec39[i] = 0;
	for (int i=0; i<2; i++) fRec40[i] = 0;
	for (int i=0; i<2; i++) fRec46[i] = 0;
	for (int i=0; i<2; i++) fRec47[i] = 0;
	for (int i=0; i<2; i++) fRec44[i] = 0;
	for (int i=0; i<2; i++) fRec45[i] = 0;
	for (int i=0; i<2; i++) fRec50[i] = 0;
	for (int i=0; i<2; i++) fRec51[i] = 0;
	for (int i=0; i<2; i++) fRec48[i] = 0;
	for (int i=0; i<2; i++) fRec49[i] = 0;
	for (int i=0; i<2; i++) fRec54[i] = 0;
	for (int i=0; i<2; i++) fRec55[i] = 0;
	for (int i=0; i<2; i++) fRec52[i] = 0;
	for (int i=0; i<2; i++) fRec53[i] = 0;
	for (int i=0; i<2; i++) fVec7[i] = 0;
	for (int i=0; i<2; i++) fRec38[i] = 0;
	for (int i=0; i<3; i++) fRec37[i] = 0;
	for (int i=0; i<2; i++) fRec58[i] = 0;
	for (int i=0; i<2; i++) iRec56[i] = 0;
	for (int i=0; i<2; i++) fRec57[i] = 0;
	for (int i=0; i<2; i++) iRec59[i] = 0;
	for (int i=0; i<2; i++) fRec60[i] = 0;
	for (int i=0; i<2; i++) fRec62[i] = 0;
	for (int i=0; i<3; i++) fRec61[i] = 0;
	for (int i=0; i<2; i++) fRec63[i] = 0;
	for (int i=0; i<2; i++) fRec65[i] = 0;
	for (int i=0; i<3; i++) fRec64[i] = 0;
	for (int i=0; i<2; i++) fVec8[i] = 0;
	for (int i=0; i<2; i++) fRec68[i] = 0;
	for (int i=0; i<2; i++) iRec66[i] = 0;
	for (int i=0; i<2; i++) fRec67[i] = 0;
	for (int i=0; i<2; i++) fRec70[i] = 0;
	for (int i=0; i<3; i++) fRec69[i] = 0;
	for (int i=0; i<2; i++) iRec71[i] = 0;
	for (int i=0; i<2; i++) fRec72[i] = 0;
	for (int i=0; i<2; i++) iRec73[i] = 0;
	for (int i=0; i<2; i++) fRec74[i] = 0;
	for (int i=0; i<2; i++) fRec75[i] = 0;
	for (int i=0; i<2; i++) fRec76[i] = 0;
	for (int i=0; i<2; i++) iRec77[i] = 0;
	for (int i=0; i<2; i++) fRec78[i] = 0;
	for (int i=0; i<2; i++) fRec79[i] = 0;
	for (int i=0; i<2; i++) fRec80[i] = 0;
	for (int i=0; i<2; i++) fRec81[i] = 0;
	for (int i=0; i<2; i++) fRec82[i] = 0;
}

inline void Dsp::init(unsigned int samplingFreq)
{
	fSamplingFreq = samplingFreq;
	fConst0 = min(1.92e+05, max(1.0, (double)fSamplingFreq));
	fConst1 = double(fConst0);
	fConst2 = (1.0 / double(int((0.03 * fConst1))));
	fConst3 = (3.141592653589793 / fConst0);
	fConst4 = tan((1570.7963267948965 / fConst0));
	fConst5 = (1.0 / faustpower<2>(fConst4));
	fConst6 = (2 * (1 - fConst5));
	fConst7 = (1.0 / fConst4);
	fConst8 = (1 + ((fConst7 - 1.0000000000000004) / fConst4));
	fConst9 = (1.0 / (1 + ((1.0000000000000004 + fConst7) / fConst4)));
	fConst10 = (0 - fConst7);
	fConst11 = (1 + fConst7);
	fConst12 = (1.396983862573739e-11 / fConst11);
	fConst13 = (0 - ((1 - fConst7) / fConst11));
	fConst14 = (2 * (0 - fConst5));
	fConst15 = (1.0 / double(int((0.01 * fConst1))));
	fConst16 = (1.0 / fConst0);
	fConst17 = (1 - (1.0 / pow(1e+03,fConst16)));
	fConst18 = (1e+02 / fConst0);
	fConst19 = tan((3141.592653589793 / fConst0));
	fConst20 = (2 * (1 - (1.0 / faustpower<2>(fConst19))));
	fConst21 = (1.0 / fConst19);
	fConst22 = (1 + ((fConst21 - 1.0000000000000004) / fConst19));
	fConst23 = (1.0 / (1 + ((1.0000000000000004 + fConst21) / fConst19)));
	fConst24 = (1.0 / double(int((0.02 * fConst1))));
	fConst25 = (31.41592653589793 / fConst0);
	fConst26 = cos(fConst25);
	fConst27 = sin(fConst25);
	fConst28 = (0 - fConst27);
	fConst29 = (6.283185307179586 / fConst0);
	fConst30 = (1 + fConst21);
	fConst31 = (1.0 / fConst30);
	fConst32 = (0 - ((1 - fConst21) / fConst30));
	fConst33 = (1.0 / double(int((0.005 * fConst1))));
	fConst34 = (1 - (1.0 / pow(1e+03,(2.5 / fConst0))));
	fConst35 = (2e+02 / fConst0);
	fConst36 = (1 - (1.0 / pow(1e+03,fConst18)));
	fConst37 = (5.340707511102648 / fConst0);
	fConst38 = (1.0 / fConst1);
	fConst39 = (5.026548245743669 / fConst0);
	fConst40 = (1.6e+03 / fConst0);
	fConst41 = max((double)0, min((double)2047, (0.0003125 * fConst1)));
	iConst42 = int(fConst41);
	iConst43 = int((1 + iConst42));
	fConst44 = floor(fConst41);
	fConst45 = (fConst41 - fConst44);
	fConst46 = ((1 + fConst44) - fConst41);
	fConst47 = (4.572399611740729 / fConst0);
	fConst48 = cos(fConst47);
	fConst49 = sin(fConst47);
	fConst50 = (0 - fConst49);
	fConst51 = (0.72772 * fConst1);
	fConst52 = (20.327952464404543 / fConst0);
	fConst53 = cos(fConst52);
	fConst54 = sin(fConst52);
	fConst55 = (0 - fConst54);
	fConst56 = (3.235294117647059 * fConst1);
	fConst57 = (326.7256359733385 / fConst0);
	fConst58 = cos(fConst57);
	fConst59 = sin(fConst57);
	fConst60 = (0 - fConst59);
	fConst61 = (52.0 * fConst1);
	fConst62 = (1498.2980347889782 / fConst0);
	fConst63 = cos(fConst62);
	fConst64 = sin(fConst62);
	fConst65 = (0 - fConst64);
	fConst66 = (238.46153846153845 * fConst1);
	fConst67 = (1 - (1.0 / pow(1e+03,(1e+01 / fConst0))));
	fConst68 = (1.0 / double(int((0.1 * fConst1))));
	fConst69 = (2.0 / fConst0);
	fConst70 = tan((22116.812281272145 / fConst0));
	fConst71 = (2 * (1 - (1.0 / faustpower<2>(fConst70))));
	fConst72 = (1.0 / fConst70);
	fConst73 = (1 + ((fConst72 - 1.0000000000000004) / fConst70));
	fConst74 = (1.0 / (1 + ((1.0000000000000004 + fConst72) / fConst70)));
	fConst75 = (1 + fConst72);
	fConst76 = (4.656612875245797e-10 / fConst75);
	fConst77 = (0 - ((1 - fConst72) / fConst75));
	fConst78 = (1.0 / double(int((0.0005 * fConst1))));
	fConst79 = (1 - (1.0 / pow(1e+03,(5.0 / fConst0))));
	fConst80 = (1 - pow(1.0,(1e+03 / fConst0)));
	fConst81 = (2e+03 / fConst0);
	fConst82 = tan((1643.0529578274618 / fConst0));
	fConst83 = (1.0 / faustpower<2>(fConst82));
	fConst84 = (2 * (1 - fConst83));
	fConst85 = (1.0 / fConst82);
	fConst86 = (1 + ((fConst85 - 1.0000000000000004) / fConst82));
	fConst87 = (1.0 / (1 + ((1.0000000000000004 + fConst85) / fConst82)));
	fConst88 = (0 - fConst85);
	fConst89 = (1 + fConst85);
	fConst90 = (4.656612875245797e-10 / fConst89);
	fConst91 = (0 - ((1 - fConst85) / fConst89));
	fConst92 = (2 * (0 - fConst83));
	fConst93 = (1 - (1.0 / pow(1e+03,(5.46448087431694 / fConst0))));
	fConst94 = (1 - (1.0 / pow(1e+03,(13.333333333333334 / fConst0))));
	fConst95 = (1162.3892818282234 / fConst0);
	fConst96 = cos(fConst95);
	fConst97 = sin(fConst95);
	fConst98 = (0 - fConst97);
	fConst99 = (1 - (1.0 / pow(1e+03,(18.181818181818183 / fConst0))));
	fConst100 = (2073.4511513692632 / fConst0);
	fConst101 = cos(fConst100);
	fConst102 = sin(fConst100);
	fConst103 = (0 - fConst102);
	IOTA = 0;
	clear_state_f();
}

void always_inline Dsp::compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0)
{
#define fcheckbox0 (*fcheckbox0_)
#define fslider1 (*fslider1_)
#define fcheckbox1 (*fcheckbox1_)
#define fslider2 (*fslider2_)
#define fcheckbox2 (*fcheckbox2_)
#define fcheckbox3 (*fcheckbox3_)
	double 	fSlow0 = double(fcheckbox0);
	double 	fSlow1 = (0.0010000000000000009 * double(fslider0));
	double 	fSlow2 = (0.0010000000000000009 * double(fslider1));
	double 	fSlow3 = double(fcheckbox1);
	double 	fSlow4 = double(fslider2);
	double 	fSlow5 = (fConst37 * fSlow4);
	double 	fSlow6 = cos(fSlow5);
	double 	fSlow7 = sin(fSlow5);
	double 	fSlow8 = (0 - fSlow7);
	double 	fSlow9 = (0.6538461538461537 * fSlow4);
	double 	fSlow10 = double(max(1e-07, fabs((0.9 * fSlow4))));
	double 	fSlow11 = (fConst38 * fSlow10);
	double 	fSlow12 = (fSlow11 - 1);
	double 	fSlow13 = (1 - (fConst1 / fSlow10));
	double 	fSlow14 = (fConst29 * fSlow4);
	double 	fSlow15 = cos(fSlow14);
	double 	fSlow16 = sin(fSlow14);
	double 	fSlow17 = (0 - fSlow16);
	double 	fSlow18 = (fConst39 * fSlow4);
	double 	fSlow19 = cos(fSlow18);
	double 	fSlow20 = sin(fSlow18);
	double 	fSlow21 = (0 - fSlow20);
	double 	fSlow22 = double(fcheckbox2);
	double 	fSlow23 = (0.0010000000000000009 * double(fslider3));
	double 	fSlow24 = double(fcheckbox3);
	double 	fSlow25 = (0.0010000000000000009 * double(fslider4));
	double 	fSlow26 = (0.0010000000000000009 * double(fslider5));
	for (int i=0; i<count; i++) {
		iVec0[0] = 1;
		fVec1[0] = fSlow0;
		int iTemp0 = (fSlow0 > fVec1[1]);
		fRec0[0] = ((iTemp0 + fRec0[1]) - (fConst2 * (fRec0[1] > 0.0)));
		double fTemp1 = tan((fConst3 * (500 + (5500 * fRec0[0]))));
		double fTemp2 = (1.0 / fTemp1);
		double fTemp3 = (1 + ((1.0000000000000004 + fTemp2) / fTemp1));
		double fTemp4 = (1 + fTemp2);
		iRec5[0] = (12345 + (1103515245 * iRec5[1]));
		fRec4[0] = ((fConst13 * fRec4[1]) + (fConst12 * ((fConst7 * iRec5[0]) + (fConst10 * iRec5[1]))));
		fRec3[0] = (fRec4[0] - (fConst9 * ((fConst8 * fRec3[2]) + (fConst6 * fRec3[1]))));
		double fTemp5 = ((fConst14 * fRec3[1]) + (fConst5 * (fRec3[2] + fRec3[0])));
		fVec2[0] = fTemp5;
		fRec2[0] = ((fRec2[1] * (0 - ((1 - fTemp2) / fTemp4))) + (fConst9 * ((fVec2[0] + fVec2[1]) / fTemp4)));
		fRec1[0] = (fRec2[0] - (((fRec1[2] * (1 + ((fTemp2 - 1.0000000000000004) / fTemp1))) + (2 * (fRec1[1] * (1 - (1.0 / faustpower<2>(fTemp1)))))) / fTemp3));
		fRec8[0] = ((iTemp0 + fRec8[1]) - (fConst15 * (fRec8[1] > 0.0)));
		int iTemp6 = (fRec8[0] > 0.0);
		int iTemp7 = (iTemp6 > 0);
		iRec6[0] = (iTemp7 & (iRec6[1] | (fRec7[1] >= 1)));
		int iTemp8 = ((iTemp6 <= 0) & (fRec7[1] > 0));
		fRec7[0] = (((fConst18 * (((iRec6[1] == 0) & iTemp7) & (fRec7[1] < 1))) + (fRec7[1] * (1 - (fConst17 * iTemp8)))) * ((iTemp8 == 0) | (fRec7[1] >= 1e-06)));
		fRec13[0] = ((iTemp0 + fRec13[1]) - (fConst24 * (fRec13[1] > 0.0)));
		fRec14[0] = ((fConst27 * fRec15[1]) + (fConst26 * fRec14[1]));
		fRec15[0] = ((1 + ((fConst26 * fRec15[1]) + (fConst28 * fRec14[1]))) - iVec0[1]);
		double fTemp9 = (fConst29 * ((19.23076923076923 * fRec14[0]) + (50 * (1 + fRec13[0]))));
		double fTemp10 = cos(fTemp9);
		double fTemp11 = sin(fTemp9);
		fRec11[0] = ((fRec12[1] * fTemp11) + (fRec11[1] * fTemp10));
		fRec12[0] = ((1 + ((fRec12[1] * fTemp10) + (fRec11[1] * (0 - fTemp11)))) - iVec0[1]);
		fRec10[0] = ((fConst32 * fRec10[1]) + (fConst31 * (fRec11[1] + fRec11[0])));
		fRec9[0] = (fRec10[0] - (fConst23 * ((fConst22 * fRec9[2]) + (fConst20 * fRec9[1]))));
		fRec18[0] = ((iTemp0 + fRec18[1]) - (fConst33 * (fRec18[1] > 0.0)));
		int iTemp12 = (fRec18[0] > 0.0);
		int iTemp13 = (iTemp12 > 0);
		iRec16[0] = (iTemp13 & (iRec16[1] | (fRec17[1] >= 1)));
		int iTemp14 = ((iTemp12 <= 0) & (fRec17[1] > 0));
		fRec17[0] = (((fConst35 * (((iRec16[1] == 0) & iTemp13) & (fRec17[1] < 1))) + (fRec17[1] * (1 - (fConst34 * iTemp14)))) * ((iTemp14 == 0) | (fRec17[1] >= 1e-06)));
		fRec19[0] = (fSlow1 + (0.999 * fRec19[1]));
		fRec20[0] = (fSlow2 + (0.999 * fRec20[1]));
		fVec3[0] = fSlow3;
		fRec23[0] = (((fSlow3 > fVec3[1]) + fRec23[1]) - (fConst33 * (fRec23[1] > 0.0)));
		int iTemp15 = (fRec23[0] > 0.0);
		int iTemp16 = (iTemp15 > 0);
		iRec21[0] = (iTemp16 & (iRec21[1] | (fRec22[1] >= 1)));
		int iTemp17 = (iTemp15 <= 0);
		int iTemp18 = (iTemp17 & (fRec22[1] > 0));
		fRec22[0] = (((iTemp18 == 0) | (fRec22[1] >= 1e-06)) * ((fConst35 * (((iRec21[1] == 0) & iTemp16) & (fRec22[1] < 1))) + (fRec22[1] * (1 - (fConst36 * iTemp18)))));
		fRec26[0] = ((fSlow7 * fRec27[1]) + (fSlow6 * fRec26[1]));
		fRec27[0] = ((1 + ((fSlow6 * fRec27[1]) + (fSlow8 * fRec26[1]))) - iVec0[1]);
		double fTemp19 = (fSlow12 + fRec28[1]);
		double fTemp20 = (fSlow11 + fRec28[1]);
		int iTemp21 = int((fTemp19 < 0));
		fRec28[0] = ((iTemp21)?fTemp20:fTemp19);
		double 	fRec29 = ((iTemp21)?fTemp20:(fSlow11 + (fRec28[1] + (fSlow13 * fTemp19))));
		double fTemp22 = (fConst29 * (((2 * fRec29) + (fSlow9 * fRec26[0])) - 1));
		double fTemp23 = sin(fTemp22);
		double fTemp24 = cos(fTemp22);
		fRec24[0] = ((fRec24[1] * fTemp24) + (fRec25[1] * fTemp23));
		fRec25[0] = ((1 + ((fRec25[1] * fTemp24) + (fRec24[1] * (0 - fTemp23)))) - iVec0[1]);
		fRec30[0] = ((fSlow16 * fRec31[1]) + (fSlow15 * fRec30[1]));
		fRec31[0] = ((1 + ((fSlow15 * fRec31[1]) + (fSlow17 * fRec30[1]))) - iVec0[1]);
		fRec32[0] = ((fSlow20 * fRec33[1]) + (fSlow19 * fRec32[1]));
		fRec33[0] = ((1 + ((fSlow19 * fRec33[1]) + (fSlow21 * fRec32[1]))) - iVec0[1]);
		iRec34[0] = (iTemp16 & (iRec34[1] | (fRec35[1] >= 1)));
		int iTemp25 = (iTemp17 & (fRec35[1] > 0));
		fRec35[0] = (((fConst35 * (((iRec34[1] == 0) & iTemp16) & (fRec35[1] < 1))) + (fRec35[1] * (1 - (fConst34 * iTemp25)))) * ((iTemp25 == 0) | (fRec35[1] >= 1e-06)));
		fVec4[0] = fSlow22;
		int iTemp26 = (fSlow22 > fVec4[1]);
		fRec36[0] = ((iTemp26 + fRec36[1]) - (fConst15 * (fRec36[1] > 0.0)));
		double fTemp27 = tan((fConst3 * (12000 - (3000 * fRec36[0]))));
		double fTemp28 = (1.0 / fTemp27);
		double fTemp29 = (1 + ((1.0000000000000004 + fTemp28) / fTemp27));
		double fTemp30 = faustpower<2>(fTemp27);
		double fTemp31 = (1.0 / fTemp30);
		double fTemp32 = (1 + fTemp28);
		double fTemp33 = (fConst40 + fRec41[1]);
		fRec41[0] = (fTemp33 - floor(fTemp33));
		double fTemp34 = faustpower<2>(((2 * fRec41[0]) - 1));
		fVec5[0] = fTemp34;
		double fTemp35 = (iVec0[1] * (fVec5[0] - fVec5[1]));
		fVec6[IOTA&63] = fTemp35;
		double fTemp36 = (0 - (0.00015625 * (((fConst46 * fVec6[(IOTA-iConst42)&63]) + (fConst45 * fVec6[(IOTA-iConst43)&63])) - fVec6[IOTA&63])));
		fRec42[0] = ((fConst49 * fRec43[1]) + (fConst48 * fRec42[1]));
		fRec43[0] = ((1 + ((fConst48 * fRec43[1]) + (fConst50 * fRec42[1]))) - iVec0[1]);
		double fTemp37 = (fConst29 * (2094.4 + (fConst51 * (fRec42[0] * fTemp36))));
		double fTemp38 = cos(fTemp37);
		double fTemp39 = sin(fTemp37);
		fRec39[0] = ((fRec40[1] * fTemp39) + (fRec39[1] * fTemp38));
		fRec40[0] = ((1 + ((fRec40[1] * fTemp38) + (fRec39[1] * (0 - fTemp39)))) - iVec0[1]);
		fRec46[0] = ((fConst54 * fRec47[1]) + (fConst53 * fRec46[1]));
		fRec47[0] = ((1 + ((fConst53 * fRec47[1]) + (fConst55 * fRec46[1]))) - iVec0[1]);
		double fTemp40 = (fConst29 * (4944.0 + (fConst56 * (fRec46[0] * fTemp36))));
		double fTemp41 = cos(fTemp40);
		double fTemp42 = sin(fTemp40);
		fRec44[0] = ((fRec45[1] * fTemp42) + (fRec44[1] * fTemp41));
		fRec45[0] = ((1 + ((fRec45[1] * fTemp41) + (fRec44[1] * (0 - fTemp42)))) - iVec0[1]);
		fRec50[0] = ((fConst59 * fRec51[1]) + (fConst58 * fRec50[1]));
		fRec51[0] = ((1 + ((fConst58 * fRec51[1]) + (fConst60 * fRec50[1]))) - iVec0[1]);
		double fTemp43 = (fConst29 * (3847.9999999999995 + (fConst61 * (fRec50[0] * fTemp36))));
		double fTemp44 = cos(fTemp43);
		double fTemp45 = sin(fTemp43);
		fRec48[0] = ((fRec49[1] * fTemp45) + (fRec48[1] * fTemp44));
		fRec49[0] = ((1 + ((fRec49[1] * fTemp44) + (fRec48[1] * (0 - fTemp45)))) - iVec0[1]);
		fRec54[0] = ((fConst64 * fRec55[1]) + (fConst63 * fRec54[1]));
		fRec55[0] = ((1 + ((fConst63 * fRec55[1]) + (fConst65 * fRec54[1]))) - iVec0[1]);
		double fTemp46 = (fConst29 * (2144.0 + (fConst66 * (fRec54[0] * fTemp36))));
		double fTemp47 = cos(fTemp46);
		double fTemp48 = sin(fTemp46);
		fRec52[0] = ((fRec53[1] * fTemp48) + (fRec52[1] * fTemp47));
		fRec53[0] = ((1 + ((fRec53[1] * fTemp47) + (fRec52[1] * (0 - fTemp48)))) - iVec0[1]);
		double fTemp49 = (((fRec52[0] + fRec48[0]) + fRec44[0]) + fRec39[0]);
		fVec7[0] = fTemp49;
		fRec38[0] = ((fRec38[1] * (0 - ((1 - fTemp28) / fTemp32))) + (((fVec7[1] * (0 - fTemp28)) + (fVec7[0] / fTemp27)) / fTemp32));
		fRec37[0] = (fRec38[0] - (((fRec37[2] * (1 + ((fTemp28 - 1.0000000000000004) / fTemp27))) + (2 * (fRec37[1] * (1 - fTemp31)))) / fTemp29));
		fRec58[0] = ((iTemp26 + fRec58[1]) - (fConst33 * (fRec58[1] > 0.0)));
		int iTemp50 = (fRec58[0] > 0.0);
		int iTemp51 = (iTemp50 > 0);
		iRec56[0] = (iTemp51 & (iRec56[1] | (fRec57[1] >= 1)));
		int iTemp52 = (iTemp50 <= 0);
		int iTemp53 = (iTemp52 & (fRec57[1] > 0));
		fRec57[0] = (((iTemp53 == 0) | (fRec57[1] >= 1e-06)) * ((fConst35 * (((iRec56[1] == 0) & iTemp51) & (fRec57[1] < 1))) + (fRec57[1] * (1 - (fConst36 * iTemp53)))));
		iRec59[0] = (iTemp51 & (iRec59[1] | (fRec60[1] >= 1)));
		int iTemp54 = (iTemp52 & (fRec60[1] > 0));
		fRec60[0] = (((fConst35 * (((iRec59[1] == 0) & iTemp51) & (fRec60[1] < 1))) + (fRec60[1] * (1 - (fConst67 * iTemp54)))) * ((iTemp54 == 0) | (fRec60[1] >= 1e-06)));
		fRec62[0] = ((iTemp26 + fRec62[1]) - (fConst68 * (fRec62[1] > 0.0)));
		double fTemp55 = (9000 + (6000 * fRec62[0]));
		double fTemp56 = (0 - (3.141592653589793 * fTemp55));
		fRec61[0] = (0 - ((((fRec61[1] * (0 - (fConst69 * fTemp56))) * cos((fConst29 * fTemp55))) + (fRec61[2] * faustpower<2>((fConst16 * fTemp56)))) - fVec7[0]));
		fRec63[0] = (fSlow23 + (0.999 * fRec63[1]));
		fRec65[0] = ((fConst77 * fRec65[1]) + (fConst76 * (iRec5[1] + iRec5[0])));
		fRec64[0] = (fRec65[0] - (fConst74 * ((fConst73 * fRec64[2]) + (fConst71 * fRec64[1]))));
		fVec8[0] = fSlow24;
		fRec68[0] = (((fSlow24 > fVec8[1]) + fRec68[1]) - (fConst78 * (fRec68[1] > 0.0)));
		int iTemp57 = (fRec68[0] > 0.0);
		int iTemp58 = (iTemp57 > 0);
		iRec66[0] = (iTemp58 & (iRec66[1] | (fRec67[1] >= 1)));
		int iTemp59 = (iTemp57 <= 0);
		int iTemp60 = (iTemp59 & (fRec67[1] > 0));
		fRec67[0] = (((iTemp60 == 0) | (fRec67[1] >= 1e-06)) * ((fConst81 * (((iRec66[1] == 0) & iTemp58) & (fRec67[1] < 1))) + (fRec67[1] * (0 - (((fConst80 * (iRec66[1] & (fRec67[1] > 1.0))) + (fConst79 * iTemp60)) - 1)))));
		fRec70[0] = ((fConst91 * fRec70[1]) + (fConst90 * ((fConst85 * iRec5[0]) + (fConst88 * iRec5[1]))));
		fRec69[0] = (fRec70[0] - (fConst87 * ((fConst86 * fRec69[2]) + (fConst84 * fRec69[1]))));
		iRec71[0] = (iTemp58 & (iRec71[1] | (fRec72[1] >= 1)));
		int iTemp61 = (iTemp59 & (fRec72[1] > 0));
		fRec72[0] = (((iTemp61 == 0) | (fRec72[1] >= 1e-06)) * ((fConst81 * (((iRec71[1] == 0) & iTemp58) & (fRec72[1] < 1))) + (fRec72[1] * (0 - (((fConst80 * (iRec71[1] & (fRec72[1] > 1.0))) + (fConst93 * iTemp61)) - 1)))));
		iRec73[0] = (iTemp58 & (iRec73[1] | (fRec74[1] >= 1)));
		int iTemp62 = (iTemp59 & (fRec74[1] > 0));
		fRec74[0] = (((iTemp62 == 0) | (fRec74[1] >= 1e-06)) * ((fConst81 * (((iRec73[1] == 0) & iTemp58) & (fRec74[1] < 1))) + (fRec74[1] * (0 - (((fConst80 * (iRec73[1] & (fRec74[1] > 1.0))) + (fConst94 * iTemp62)) - 1)))));
		fRec75[0] = ((fConst97 * fRec76[1]) + (fConst96 * fRec75[1]));
		fRec76[0] = ((1 + ((fConst96 * fRec76[1]) + (fConst98 * fRec75[1]))) - iVec0[1]);
		iRec77[0] = (iTemp58 & (iRec77[1] | (fRec78[1] >= 1)));
		int iTemp63 = (iTemp59 & (fRec78[1] > 0));
		fRec78[0] = (((iTemp63 == 0) | (fRec78[1] >= 1e-06)) * ((fConst81 * (((iRec77[1] == 0) & iTemp58) & (fRec78[1] < 1))) + (fRec78[1] * (0 - (((fConst80 * (iRec77[1] & (fRec78[1] > 1.0))) + (fConst99 * iTemp63)) - 1)))));
		fRec79[0] = ((fConst102 * fRec80[1]) + (fConst101 * fRec79[1]));
		fRec80[0] = ((1 + ((fConst101 * fRec80[1]) + (fConst103 * fRec79[1]))) - iVec0[1]);
		fRec81[0] = (fSlow25 + (0.999 * fRec81[1]));
		fRec82[0] = (fSlow26 + (0.999 * fRec82[1]));
		output0[i] = (FAUSTFLOAT)((double)input0[i] + (2 * (pow(10,(0.05 * fRec82[0])) * ((0.1 * (pow(10,(0.05 * fRec81[0])) * ((5.0 * (((0.25 + fRec79[0]) * fRec78[0]) + ((0.25 + fRec75[0]) * fRec74[0]))) + ((fConst87 * (fRec72[0] * ((fConst92 * fRec69[1]) + (fConst83 * (fRec69[2] + fRec69[0]))))) + (fConst74 * (fRec67[0] * (fRec64[0] + (fRec64[2] + (2 * fRec64[1]))))))))) + ((0.5 * (pow(10,(0.05 * fRec63[0])) * (((fRec61[0] - fRec61[2]) * fRec60[0]) + ((sqrt(fRec57[0]) * ((2 * (fRec37[1] * (0 - fTemp31))) + ((fRec37[2] + fRec37[0]) / fTemp30))) / fTemp29)))) + ((((0.25 * (fRec35[0] * ((0.5 * (fRec32[0] + fRec30[0])) + (5 * fRec24[0])))) + (3 * fRec22[0])) * pow(10,(0.05 * fRec20[0]))) + (pow(10,(0.05 * fRec19[0])) * ((fConst23 * (fRec17[0] * (fRec9[0] + (fRec9[2] + (2 * fRec9[1]))))) + ((fRec7[0] * (fRec1[0] + (fRec1[2] + (2 * fRec1[1])))) / fTemp3)))))))));
		// post processing
		fRec82[1] = fRec82[0];
		fRec81[1] = fRec81[0];
		fRec80[1] = fRec80[0];
		fRec79[1] = fRec79[0];
		fRec78[1] = fRec78[0];
		iRec77[1] = iRec77[0];
		fRec76[1] = fRec76[0];
		fRec75[1] = fRec75[0];
		fRec74[1] = fRec74[0];
		iRec73[1] = iRec73[0];
		fRec72[1] = fRec72[0];
		iRec71[1] = iRec71[0];
		fRec69[2] = fRec69[1]; fRec69[1] = fRec69[0];
		fRec70[1] = fRec70[0];
		fRec67[1] = fRec67[0];
		iRec66[1] = iRec66[0];
		fRec68[1] = fRec68[0];
		fVec8[1] = fVec8[0];
		fRec64[2] = fRec64[1]; fRec64[1] = fRec64[0];
		fRec65[1] = fRec65[0];
		fRec63[1] = fRec63[0];
		fRec61[2] = fRec61[1]; fRec61[1] = fRec61[0];
		fRec62[1] = fRec62[0];
		fRec60[1] = fRec60[0];
		iRec59[1] = iRec59[0];
		fRec57[1] = fRec57[0];
		iRec56[1] = iRec56[0];
		fRec58[1] = fRec58[0];
		fRec37[2] = fRec37[1]; fRec37[1] = fRec37[0];
		fRec38[1] = fRec38[0];
		fVec7[1] = fVec7[0];
		fRec53[1] = fRec53[0];
		fRec52[1] = fRec52[0];
		fRec55[1] = fRec55[0];
		fRec54[1] = fRec54[0];
		fRec49[1] = fRec49[0];
		fRec48[1] = fRec48[0];
		fRec51[1] = fRec51[0];
		fRec50[1] = fRec50[0];
		fRec45[1] = fRec45[0];
		fRec44[1] = fRec44[0];
		fRec47[1] = fRec47[0];
		fRec46[1] = fRec46[0];
		fRec40[1] = fRec40[0];
		fRec39[1] = fRec39[0];
		fRec43[1] = fRec43[0];
		fRec42[1] = fRec42[0];
		IOTA = IOTA+1;
		fVec5[1] = fVec5[0];
		fRec41[1] = fRec41[0];
		fRec36[1] = fRec36[0];
		fVec4[1] = fVec4[0];
		fRec35[1] = fRec35[0];
		iRec34[1] = iRec34[0];
		fRec33[1] = fRec33[0];
		fRec32[1] = fRec32[0];
		fRec31[1] = fRec31[0];
		fRec30[1] = fRec30[0];
		fRec25[1] = fRec25[0];
		fRec24[1] = fRec24[0];
		fRec28[1] = fRec28[0];
		fRec27[1] = fRec27[0];
		fRec26[1] = fRec26[0];
		fRec22[1] = fRec22[0];
		iRec21[1] = iRec21[0];
		fRec23[1] = fRec23[0];
		fVec3[1] = fVec3[0];
		fRec20[1] = fRec20[0];
		fRec19[1] = fRec19[0];
		fRec17[1] = fRec17[0];
		iRec16[1] = iRec16[0];
		fRec18[1] = fRec18[0];
		fRec9[2] = fRec9[1]; fRec9[1] = fRec9[0];
		fRec10[1] = fRec10[0];
		fRec12[1] = fRec12[0];
		fRec11[1] = fRec11[0];
		fRec15[1] = fRec15[0];
		fRec14[1] = fRec14[0];
		fRec13[1] = fRec13[0];
		fRec7[1] = fRec7[0];
		iRec6[1] = iRec6[0];
		fRec8[1] = fRec8[0];
		fRec1[2] = fRec1[1]; fRec1[1] = fRec1[0];
		fRec2[1] = fRec2[0];
		fVec2[1] = fVec2[0];
		fRec3[2] = fRec3[1]; fRec3[1] = fRec3[0];
		fRec4[1] = fRec4[0];
		iRec5[1] = iRec5[0];
		fRec0[1] = fRec0[0];
		fVec1[1] = fVec1[0];
		iVec0[1] = iVec0[0];
	}
#undef fcheckbox0
#undef fslider1
#undef fcheckbox1
#undef fslider2
#undef fcheckbox2
#undef fcheckbox3
}

int Dsp::register_par(const ParamReg& reg)
{
	reg.registerVar("seq.gain","","S",N_("Volume level in decibels"),&fslider5, -2e+01, -6e+01, 4e+01, 0.1);
	reg.registerVar("seq.hat_closed.dsp.Gain","","S",N_("Volume level in decibels"),&fslider3, -2e+01, -6e+01, 4e+01, 0.1);
	fcheckbox2_ = reg.registerNonMidiSharedVar("seq.hat_closed.dsp.gate",&fcheckbox2, false, true, 0.0, 0.0, 1.0, 1.0);
	fcheckbox0_ = reg.registerNonMidiSharedVar("seq.kick.dsp.gate",&fcheckbox0, false, true, 0.0, 0.0, 1.0, 1.0);
	reg.registerVar("seq.kick.dsp.Gain","","S",N_("Volume level in decibels"),&fslider0, -2e+01, -6e+01, 4e+01, 0.1);
	reg.registerVar("seq.snare.dsp.Gain","","S",N_("Volume level in decibels"),&fslider4, -2e+01, -6e+01, 4e+01, 0.1);
	fcheckbox3_ = reg.registerNonMidiSharedVar("seq.snare.dsp.gate",&fcheckbox3, false, true, 0.0, 0.0, 1.0, 1.0);
	fslider1_ = reg.registerNonMidiSharedVar("seq.tom.dsp.Gainf",&fslider1, false, true, -2e+01, -6e+01, 4e+01, 0.1);
	fslider2_ = reg.registerNonMidiSharedVar("seq.tom.dsp.freq",&fslider2, false, true, 9e+01, 9e+01, 1.5e+02, 1.0);
	fcheckbox1_ = reg.registerNonMidiSharedVar("seq.tom.dsp.gate",&fcheckbox1, false, true, 0.0, 0.0, 1.0, 1.0);
	return 0;
}


} // end namespace drumseq