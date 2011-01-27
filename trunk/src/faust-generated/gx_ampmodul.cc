namespace gx_ampmodul {
// generated from file '../src/faust/gx_ampmodul.dsp'

FAUSTFLOAT 	fslider0;
double 	fConst0;
double 	fConst1;
double 	fConst2;
FAUSTFLOAT 	fslider1;
double 	fRec3[2];
double 	fConst3;
double 	fConst4;
double 	fConst5;
FAUSTFLOAT 	fslider2;
double 	fRec10[2];
FAUSTFLOAT 	fslider3;
double 	fRec11[2];
double 	fConst6;
double 	fConst7;
double 	fConst8;
double 	fVec0[2];
double 	fConst9;
double 	fRec12[2];
double 	fRec9[2];
double 	fConst10;
double 	fConst11;
double 	fRec8[2];
double 	fConst12;
double 	fRec7[2];
double 	fConst13;
double 	fConst14;
double 	fConst15;
double 	fVec1[2];
double 	fConst16;
double 	fRec13[2];
double 	fRec6[2];
double 	fRec5[2];
double 	fRec4[2];
double 	fConst17;
double 	fConst18;
double 	fConst19;
double 	fVec2[2];
double 	fConst20;
double 	fRec14[2];
double 	fRec2[2];
double 	fRec1[2];
double 	fRec0[6];
double 	fVec3[2];
double 	fRec24[2];
double 	fRec23[2];
double 	fRec22[2];
double 	fVec4[2];
double 	fRec21[2];
double 	fVec5[2];
double 	fRec25[2];
double 	fRec20[2];
double 	fRec19[2];
double 	fVec6[2];
double 	fRec18[2];
double 	fVec7[2];
double 	fRec26[2];
double 	fRec17[2];
double 	fRec16[2];
double 	fRec15[6];
int	fSamplingFreq;

void init(int samplingFreq)
{
	fSamplingFreq = samplingFreq;
	fConst0 = (1.0 / tan((97.38937226128358 / fSamplingFreq)));
	fConst1 = (1 + fConst0);
	fConst2 = (0 - ((1 - fConst0) / fConst1));
	for (int i=0; i<2; i++) fRec3[i] = 0;
	fConst3 = (1.0 / tan((20517.741620594938 / fSamplingFreq)));
	fConst4 = (1 + fConst3);
	fConst5 = (0 - ((1 - fConst3) / fConst4));
	for (int i=0; i<2; i++) fRec10[i] = 0;
	for (int i=0; i<2; i++) fRec11[i] = 0;
	fConst6 = (1.0 / tan((270.1769682087222 / fSamplingFreq)));
	fConst7 = (1 + fConst6);
	fConst8 = (0 - ((1 - fConst6) / fConst7));
	for (int i=0; i<2; i++) fVec0[i] = 0;
	fConst9 = (0.027 / fConst7);
	for (int i=0; i<2; i++) fRec12[i] = 0;
	for (int i=0; i<2; i++) fRec9[i] = 0;
	fConst10 = (0 - fConst0);
	fConst11 = (0.025 / fConst1);
	for (int i=0; i<2; i++) fRec8[i] = 0;
	fConst12 = (1.0 / fConst4);
	for (int i=0; i<2; i++) fRec7[i] = 0;
	fConst13 = (1.0 / tan((414.6902302738527 / fSamplingFreq)));
	fConst14 = (1 + fConst13);
	fConst15 = (0 - ((1 - fConst13) / fConst14));
	for (int i=0; i<2; i++) fVec1[i] = 0;
	fConst16 = (0.015 / fConst14);
	for (int i=0; i<2; i++) fRec13[i] = 0;
	for (int i=0; i<2; i++) fRec6[i] = 0;
	for (int i=0; i<2; i++) fRec5[i] = 0;
	for (int i=0; i<2; i++) fRec4[i] = 0;
	fConst17 = (1.0 / tan((609.4689747964198 / fSamplingFreq)));
	fConst18 = (1 + fConst17);
	fConst19 = (0 - ((1 - fConst17) / fConst18));
	for (int i=0; i<2; i++) fVec2[i] = 0;
	fConst20 = (0.0082 / fConst18);
	for (int i=0; i<2; i++) fRec14[i] = 0;
	for (int i=0; i<2; i++) fRec2[i] = 0;
	for (int i=0; i<2; i++) fRec1[i] = 0;
	for (int i=0; i<6; i++) fRec0[i] = 0;
	for (int i=0; i<2; i++) fVec3[i] = 0;
	for (int i=0; i<2; i++) fRec24[i] = 0;
	for (int i=0; i<2; i++) fRec23[i] = 0;
	for (int i=0; i<2; i++) fRec22[i] = 0;
	for (int i=0; i<2; i++) fVec4[i] = 0;
	for (int i=0; i<2; i++) fRec21[i] = 0;
	for (int i=0; i<2; i++) fVec5[i] = 0;
	for (int i=0; i<2; i++) fRec25[i] = 0;
	for (int i=0; i<2; i++) fRec20[i] = 0;
	for (int i=0; i<2; i++) fRec19[i] = 0;
	for (int i=0; i<2; i++) fVec6[i] = 0;
	for (int i=0; i<2; i++) fRec18[i] = 0;
	for (int i=0; i<2; i++) fVec7[i] = 0;
	for (int i=0; i<2; i++) fRec26[i] = 0;
	for (int i=0; i<2; i++) fRec17[i] = 0;
	for (int i=0; i<2; i++) fRec16[i] = 0;
	for (int i=0; i<6; i++) fRec15[i] = 0;
}

void compute(int count, float *input0, float *input1, float *output0, float *output1)
{
	double 	fSlow0 = fslider0;
	double 	fSlow1 = (0.0010000000000000009 * pow(10,(0.05 * fslider1)));
	double 	fSlow2 = (0.0010000000000000009 * pow(10,(0.05 * fslider2)));
	double 	fSlow3 = (0.0010000000000000009 * pow(10,(0.05 * fslider3)));
	for (int i=0; i<count; i++) {
		fRec3[0] = (fSlow1 + (0.999 * fRec3[1]));
		fRec10[0] = (fSlow2 + (0.999 * fRec10[1]));
		fRec11[0] = (fSlow3 + (0.999 * fRec11[1]));
		double fTemp0 = (fRec9[1] - 2.5e+02);
		fVec0[0] = fTemp0;
		fRec12[0] = ((fConst9 * (fVec0[0] + fVec0[1])) + (fConst8 * fRec12[1]));
		fRec9[0] = Ftube2(0, (fRec12[0] + (((double)input0[i] * fRec11[0]) * fRec10[0])));
		fRec8[0] = ((fConst11 * ((fConst10 * fRec9[1]) + (fConst0 * fRec9[0]))) + (fConst2 * fRec8[1]));
		fRec7[0] = ((fConst12 * (fRec8[0] + fRec8[1])) + (fConst5 * fRec7[1]));
		double fTemp1 = (fRec6[1] - 2.5e+02);
		fVec1[0] = fTemp1;
		fRec13[0] = ((fConst16 * (fVec1[0] + fVec1[1])) + (fConst15 * fRec13[1]));
		fRec6[0] = Ftube2(1, (fRec13[0] + (fRec10[0] * fRec7[0])));
		fRec5[0] = ((fConst11 * ((fConst10 * fRec6[1]) + (fConst0 * fRec6[0]))) + (fConst2 * fRec5[1]));
		fRec4[0] = ((fConst12 * (fRec5[0] + fRec5[1])) + (fConst5 * fRec4[1]));
		double fTemp2 = (fRec2[1] - 2.5e+02);
		fVec2[0] = fTemp2;
		fRec14[0] = ((fConst20 * (fVec2[0] + fVec2[1])) + (fConst19 * fRec14[1]));
		fRec2[0] = Ftube2(1, (fRec14[0] + (fRec4[0] * fRec3[0])));
		fRec1[0] = ((fConst11 * ((fConst10 * fRec2[1]) + (fConst0 * fRec2[0]))) + (fConst2 * fRec1[1]));
		fRec0[0] = (fRec1[0] - (fSlow0 * fRec0[5]));
		output0[i] = (FAUSTFLOAT)fRec0[0];
		double fTemp3 = (fRec23[1] - 2.5e+02);
		fVec3[0] = fTemp3;
		fRec24[0] = ((fConst9 * (fVec3[0] + fVec3[1])) + (fConst8 * fRec24[1]));
		fRec23[0] = Ftube(0, (fRec24[0] + ((double)input1[i] * fRec11[0])));
		fRec22[0] = ((fConst11 * ((fConst10 * fRec23[1]) + (fConst0 * fRec23[0]))) + (fConst2 * fRec22[1]));
		double fTemp4 = (fRec10[0] * fRec22[0]);
		fVec4[0] = fTemp4;
		fRec21[0] = ((fConst12 * (fVec4[0] + fVec4[1])) + (fConst5 * fRec21[1]));
		double fTemp5 = (fRec20[1] - 2.5e+02);
		fVec5[0] = fTemp5;
		fRec25[0] = ((fConst16 * (fVec5[0] + fVec5[1])) + (fConst15 * fRec25[1]));
		fRec20[0] = Ftube(1, (fRec25[0] + fRec21[0]));
		fRec19[0] = ((fConst11 * ((fConst10 * fRec20[1]) + (fConst0 * fRec20[0]))) + (fConst2 * fRec19[1]));
		double fTemp6 = (fRec10[0] * fRec19[0]);
		fVec6[0] = fTemp6;
		fRec18[0] = ((fConst12 * (fVec6[0] + fVec6[1])) + (fConst5 * fRec18[1]));
		double fTemp7 = (fRec17[1] - 2.5e+02);
		fVec7[0] = fTemp7;
		fRec26[0] = ((fConst20 * (fVec7[0] + fVec7[1])) + (fConst19 * fRec26[1]));
		fRec17[0] = Ftube(1, (fRec26[0] + fRec18[0]));
		fRec16[0] = ((fConst11 * ((fConst10 * fRec17[1]) + (fConst0 * fRec17[0]))) + (fConst2 * fRec16[1]));
		fRec15[0] = ((fRec3[0] * fRec16[0]) - (fSlow0 * fRec15[5]));
		output1[i] = (FAUSTFLOAT)fRec15[0];
		// post processing
		for (int i=5; i>0; i--) fRec15[i] = fRec15[i-1];
		fRec16[1] = fRec16[0];
		fRec17[1] = fRec17[0];
		fRec26[1] = fRec26[0];
		fVec7[1] = fVec7[0];
		fRec18[1] = fRec18[0];
		fVec6[1] = fVec6[0];
		fRec19[1] = fRec19[0];
		fRec20[1] = fRec20[0];
		fRec25[1] = fRec25[0];
		fVec5[1] = fVec5[0];
		fRec21[1] = fRec21[0];
		fVec4[1] = fVec4[0];
		fRec22[1] = fRec22[0];
		fRec23[1] = fRec23[0];
		fRec24[1] = fRec24[0];
		fVec3[1] = fVec3[0];
		for (int i=5; i>0; i--) fRec0[i] = fRec0[i-1];
		fRec1[1] = fRec1[0];
		fRec2[1] = fRec2[0];
		fRec14[1] = fRec14[0];
		fVec2[1] = fVec2[0];
		fRec4[1] = fRec4[0];
		fRec5[1] = fRec5[0];
		fRec6[1] = fRec6[0];
		fRec13[1] = fRec13[0];
		fVec1[1] = fVec1[0];
		fRec7[1] = fRec7[0];
		fRec8[1] = fRec8[0];
		fRec9[1] = fRec9[0];
		fRec12[1] = fRec12[0];
		fVec0[1] = fVec0[0];
		fRec11[1] = fRec11[0];
		fRec10[1] = fRec10[0];
		fRec3[1] = fRec3[0];
	}
}

static struct RegisterParams { RegisterParams(); } RegisterParams;
RegisterParams::RegisterParams()
{
	registerVar("ampmodul.level","","S","",&fslider3, -2e+01, -4e+01, 4.0, 0.1);
	registerVar("ampmodul.amp2.stage1.tube1","","S","",&fslider2, 0.0, -2e+01, 2e+01, 0.1);
	registerVar("ampmodul.amp2.stage2.tube2","","S","",&fslider1, 6.0, -2e+01, 2e+01, 0.1);
	registerVar("ampmodul.feedback","","S","",&fslider0, 0.0, -1.0, 1.0, 0.01);
	registerInit("ampmodul", init);
}

} // end namespace gx_ampmodul
