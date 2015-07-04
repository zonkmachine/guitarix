// generated automatically
// DO NOT MODIFY!
declare id "fuzzdrive";
declare name "Fuzz Drive";
declare category "Distortion";
declare shortname "Fuzz Drive";
declare description "Fuzz Distortion";

import("filter.lib");

    LogPot(a, x) = if(a, (exp(a * x) - 1) / (exp(a) - 1), x);
    Inverted(b, x) = if(b, 1 - x, x);
    s = 0.993;

    wet = vslider("wet_dry[name:wet/dry][tooltip:percentage of processed signal in output signal]",  100, 0, 100, 1) : /(100);
    dry = 1 - wet;

    Level = vslider("Level[name:Level]", 0.5, 0, 1, 0.01) : Inverted(0) : LogPot(0) : smooth(s);
    
    Fuzz = 0.5 ; //vslider("Fuzz[name:Tone]", 0.5, 0, 1, 0.01) : Inverted(1) : LogPot(0) : smooth(s);

    Distortion = vslider("Distortion[name:Drive]", 0.5, 0, 0.99, 0.01) : Inverted(1) : LogPot(0) : smooth(s);

    clip(x) = 0.4 * (min(2.7514,max(-2.4514,x)));

fuzz =  iir((b0/a0,b1/a0,b2/a0,b3/a0,b4/a0,b5/a0,b6/a0,b7/a0,b8/a0),(a1/a0,a2/a0,a3/a0,a4/a0,a5/a0,a6/a0,a7/a0,a8/a0)) : clip with {
    fs = float(SR);
   
    b0 = Fuzz*Level*pow(fs,3)*(fs*(fs*(fs*(fs*(1.29594227235659e-40*fs - 8.87073657190112e-34) - 1.64126469795529e-30) - 1.58018238163299e-26) - 1.52868099778786e-23) - 4.36734685990874e-21) + Level*pow(fs,2)*(fs*(fs*(fs*(fs*(fs*(1.17227175222998e-40*fs - 8.60503380045389e-34) + 3.96129387282593e-28) + 4.46652612440854e-25) + 6.72185939565773e-21) + 2.17087377052947e-18) - 1.32343844239659e-17);

    b1 = Fuzz*Level*pow(fs,3)*(pow(fs,2)*(fs*(fs*(-1.03675381788527e-39*fs + 5.32244194314067e-33) + 6.56505879182116e-30) + 3.16036476326599e-26) - 8.73469371981747e-21) + Level*pow(fs,2)*(fs*(pow(fs,2)*(fs*(fs*(-9.37817401783986e-40*fs + 5.16302028027233e-33) - 1.58451754913037e-27) - 8.93305224881709e-25) + 4.34174754105893e-18) - 5.29375376958635e-17);

    b2 = Fuzz*Level*pow(fs,3)*(fs*(fs*(fs*(fs*(3.62863836259845e-39*fs - 1.24190312006616e-32) - 6.56505879182116e-30) + 3.16036476326599e-26) + 6.11472399115143e-23) + 8.73469371981747e-21) + Level*pow(fs,2)*(fs*(fs*(fs*(fs*(fs*(3.28236090624395e-39*fs - 1.20470473206354e-32) + 1.58451754913037e-27) - 8.93305224881709e-25) - 2.68874375826309e-20) - 4.34174754105893e-18) - 5.29375376958635e-17);

    b3 = Fuzz*Level*pow(fs,3)*(pow(fs,2)*(fs*(fs*(-7.2572767251969e-39*fs + 1.24190312006616e-32) - 6.56505879182116e-30) - 9.48109428979796e-26) + 2.62040811594524e-20) + Level*pow(fs,2)*(fs*(pow(fs,2)*(fs*(fs*(-6.5647218124879e-39*fs + 1.20470473206354e-32) + 1.58451754913037e-27) + 2.67991567464513e-24) - 1.30252426231768e-17) + 5.29375376958635e-17);

    b4 = Fuzz*Level*pow(fs,4)*(pow(fs,2)*(9.07159590649613e-39*pow(fs,2) + 1.64126469795529e-29) - 9.17208598672714e-23) + Level*pow(fs,2)*(pow(fs,2)*(pow(fs,2)*(8.20590226560988e-39*pow(fs,2) - 3.96129387282593e-27) + 4.03311563739464e-20) + 1.32343844239659e-16);

    b5 = Fuzz*Level*pow(fs,3)*(pow(fs,2)*(fs*(fs*(-7.2572767251969e-39*fs - 1.24190312006616e-32) - 6.56505879182116e-30) + 9.48109428979796e-26) - 2.62040811594524e-20) + Level*pow(fs,2)*(fs*(pow(fs,2)*(fs*(fs*(-6.5647218124879e-39*fs - 1.20470473206354e-32) + 1.58451754913037e-27) - 2.67991567464513e-24) + 1.30252426231768e-17) + 5.29375376958635e-17);

    b6 = Fuzz*Level*pow(fs,3)*(fs*(fs*(fs*(fs*(3.62863836259845e-39*fs + 1.24190312006616e-32) - 6.56505879182116e-30) - 3.16036476326599e-26) + 6.11472399115143e-23) - 8.73469371981747e-21) + Level*pow(fs,2)*(fs*(fs*(fs*(fs*(fs*(3.28236090624395e-39*fs + 1.20470473206354e-32) + 1.58451754913037e-27) + 8.93305224881709e-25) - 2.68874375826309e-20) + 4.34174754105893e-18) - 5.29375376958635e-17);

    b7 = Fuzz*Level*pow(fs,3)*(pow(fs,2)*(fs*(fs*(-1.03675381788527e-39*fs - 5.32244194314067e-33) + 6.56505879182116e-30) - 3.16036476326599e-26) + 8.73469371981747e-21) + Level*pow(fs,2)*(fs*(pow(fs,2)*(fs*(fs*(-9.37817401783986e-40*fs - 5.16302028027233e-33) - 1.58451754913037e-27) + 8.93305224881709e-25) - 4.34174754105893e-18) - 5.29375376958635e-17);

    b8 = Fuzz*Level*pow(fs,3)*(fs*(fs*(fs*(fs*(1.29594227235659e-40*fs + 8.87073657190112e-34) - 1.64126469795529e-30) + 1.58018238163299e-26) - 1.52868099778786e-23) + 4.36734685990874e-21) + Level*pow(fs,2)*(fs*(fs*(fs*(fs*(fs*(1.17227175222998e-40*fs + 8.60503380045389e-34) + 3.96129387282593e-28) - 4.46652612440854e-25) + 6.72185939565773e-21) - 2.17087377052947e-18) - 1.32343844239659e-17);

    a0 = Fuzz*(Fuzz*pow(fs,2)*(fs*(fs*(fs*(fs*(fs*(-1.20954612086615e-39*fs - 2.86589792702458e-34) - 6.29844123704863e-30) - 6.96590249983833e-27) - 2.97472992156056e-24) - 4.85407145719168e-22) - 2.26821940920998e-20) + fs*(fs*(fs*(fs*(fs*(fs*(fs*(2.45020046020493e-40*fs - 1.12017008405057e-33) - 2.68390877243249e-29) - 2.76343706770825e-26) - 1.93946085329172e-23) - 8.29607943811672e-21) - 1.41480755159492e-18) - 6.87339214912114e-17)) + fs*(fs*(fs*(fs*(fs*(fs*(fs*(1.21134747730432e-39*fs + 1.63282987404348e-33) + 4.4344097371444e-29) + 1.07298568411666e-25) + 1.73266579347222e-22) + 1.31102852092565e-19) + 4.74767687606168e-17) + 7.12034665177176e-15) + 3.12426915869143e-13;

    a1 = Fuzz*(Fuzz*pow(fs,2)*(fs*(pow(fs,2)*(fs*(fs*(9.6763689669292e-39*fs + 1.71953875621475e-33) + 2.51937649481945e-29) + 1.39318049996767e-26) - 9.70814291438336e-22) - 9.0728776368399e-20) + fs*(fs*(fs*(pow(fs,2)*(fs*(fs*(-1.96016036816394e-39*fs + 6.7210205043034e-33) + 1.073563508973e-28) + 5.52687413541649e-26) - 1.65921588762334e-20) - 5.65923020637969e-18) - 4.12403528947268e-16)) + fs*(fs*(fs*(pow(fs,2)*(fs*(fs*(-9.69077981843452e-39*fs - 9.79697924426087e-33) - 1.77376389485776e-28) - 2.14597136823331e-25) + 2.62205704185131e-19) + 1.89907075042467e-16) + 4.27220799106305e-14) + 2.49941532695314e-12;

    a2 = Fuzz*(Fuzz*pow(fs,2)*(fs*(fs*(fs*(fs*(fs*(-3.38672913842522e-38*fs - 4.01225709783442e-33) - 2.51937649481945e-29) + 1.39318049996767e-26) + 1.18989196862423e-23) + 9.70814291438336e-22) - 9.0728776368399e-20) + fs*(fs*(fs*(fs*(fs*(fs*(fs*(6.86056128857379e-39*fs - 1.56823811767079e-32) - 1.073563508973e-28) + 5.52687413541649e-26) + 7.75784341316688e-23) + 1.65921588762334e-20) - 5.65923020637969e-18) - 9.62274900876959e-16)) + fs*(fs*(fs*(fs*(fs*(fs*(fs*(3.39177293645208e-38*fs + 2.28596182366087e-32) + 1.77376389485776e-28) - 2.14597136823331e-25) - 6.93066317388889e-22) - 2.62205704185131e-19) + 1.89907075042467e-16) + 9.96848531248046e-14) + 8.74795364433599e-12;

    a3 = Fuzz*(Fuzz*pow(fs,2)*(fs*(pow(fs,2)*(fs*(fs*(6.77345827685044e-38*fs + 4.01225709783442e-33) - 2.51937649481945e-29) - 4.179541499903e-26) + 2.91244287431501e-21) + 9.0728776368399e-20) + fs*(fs*(fs*(pow(fs,2)*(fs*(fs*(-1.37211225771476e-38*fs + 1.56823811767079e-32) - 1.073563508973e-28) - 1.65806224062495e-25) + 4.97764766287003e-20) + 5.65923020637969e-18) - 9.62274900876959e-16)) + fs*(fs*(fs*(pow(fs,2)*(fs*(fs*(-6.78354587290416e-38*fs - 2.28596182366087e-32) + 1.77376389485776e-28) + 6.43791410469994e-25) - 7.86617112555392e-19) - 1.89907075042467e-16) + 9.96848531248046e-14) + 1.7495907288672e-11;

    a4 = Fuzz*(Fuzz*pow(fs,2)*(pow(fs,2)*(pow(fs,2)*(-8.46682284606305e-38*pow(fs,2) + 6.29844123704863e-29) - 1.78483795293634e-23) + 2.26821940920998e-19) + pow(fs,2)*(pow(fs,2)*(pow(fs,2)*(1.71514032214345e-38*pow(fs,2) + 2.68390877243249e-28) - 1.16367651197503e-22) + 1.41480755159492e-17)) + pow(fs,2)*(pow(fs,2)*(pow(fs,2)*(8.47943234113021e-38*pow(fs,2) - 4.4344097371444e-28) + 1.03959947608333e-21) - 4.74767687606168e-16) + 2.186988411084e-11;

    a5 = Fuzz*(Fuzz*pow(fs,2)*(fs*(pow(fs,2)*(fs*(fs*(6.77345827685044e-38*fs - 4.01225709783442e-33) - 2.51937649481945e-29) + 4.179541499903e-26) - 2.91244287431501e-21) + 9.0728776368399e-20) + fs*(fs*(fs*(pow(fs,2)*(fs*(fs*(-1.37211225771476e-38*fs - 1.56823811767079e-32) - 1.073563508973e-28) + 1.65806224062495e-25) - 4.97764766287003e-20) + 5.65923020637969e-18) + 9.62274900876959e-16)) + fs*(fs*(fs*(pow(fs,2)*(fs*(fs*(-6.78354587290416e-38*fs + 2.28596182366087e-32) + 1.77376389485776e-28) - 6.43791410469994e-25) + 7.86617112555392e-19) - 1.89907075042467e-16) - 9.96848531248046e-14) + 1.7495907288672e-11;

    a6 = Fuzz*(Fuzz*pow(fs,2)*(fs*(fs*(fs*(fs*(fs*(-3.38672913842522e-38*fs + 4.01225709783442e-33) - 2.51937649481945e-29) - 1.39318049996767e-26) + 1.18989196862423e-23) - 9.70814291438336e-22) - 9.0728776368399e-20) + fs*(fs*(fs*(fs*(fs*(fs*(fs*(6.86056128857379e-39*fs + 1.56823811767079e-32) - 1.073563508973e-28) - 5.52687413541649e-26) + 7.75784341316688e-23) - 1.65921588762334e-20) - 5.65923020637969e-18) + 9.62274900876959e-16)) + fs*(fs*(fs*(fs*(fs*(fs*(fs*(3.39177293645208e-38*fs - 2.28596182366087e-32) + 1.77376389485776e-28) + 2.14597136823331e-25) - 6.93066317388889e-22) + 2.62205704185131e-19) + 1.89907075042467e-16) - 9.96848531248046e-14) + 8.74795364433599e-12;

    a7 = Fuzz*(Fuzz*pow(fs,2)*(fs*(pow(fs,2)*(fs*(fs*(9.6763689669292e-39*fs - 1.71953875621475e-33) + 2.51937649481945e-29) - 1.39318049996767e-26) + 9.70814291438336e-22) - 9.0728776368399e-20) + fs*(fs*(fs*(pow(fs,2)*(fs*(fs*(-1.96016036816394e-39*fs - 6.7210205043034e-33) + 1.073563508973e-28) - 5.52687413541649e-26) + 1.65921588762334e-20) - 5.65923020637969e-18) + 4.12403528947268e-16)) + fs*(fs*(fs*(pow(fs,2)*(fs*(fs*(-9.69077981843452e-39*fs + 9.79697924426087e-33) - 1.77376389485776e-28) + 2.14597136823331e-25) - 2.62205704185131e-19) + 1.89907075042467e-16) - 4.27220799106305e-14) + 2.49941532695314e-12;

    a8 = Fuzz*(Fuzz*pow(fs,2)*(fs*(fs*(fs*(fs*(fs*(-1.20954612086615e-39*fs + 2.86589792702458e-34) - 6.29844123704863e-30) + 6.96590249983833e-27) - 2.97472992156056e-24) + 4.85407145719168e-22) - 2.26821940920998e-20) + fs*(fs*(fs*(fs*(fs*(fs*(fs*(2.45020046020493e-40*fs + 1.12017008405057e-33) - 2.68390877243249e-29) + 2.76343706770825e-26) - 1.93946085329172e-23) + 8.29607943811672e-21) - 1.41480755159492e-18) + 6.87339214912114e-17)) + fs*(fs*(fs*(fs*(fs*(fs*(fs*(1.21134747730432e-39*fs - 1.63282987404348e-33) + 4.4344097371444e-29) - 1.07298568411666e-25) + 1.73266579347222e-22) - 1.31102852092565e-19) + 4.74767687606168e-17) - 7.12034665177176e-15) + 3.12426915869143e-13;
};


dist = iir((b0/a0,b1/a0,b2/a0,b3/a0),(a1/a0,a2/a0,a3/a0)) : clip with {
    fs = float(SR);

    b0 = 3.45357917689612e-13*Distortion*Level*0.1*pow(fs,3) + Level*0.1*pow(fs,2)*(1.79347857986842e-13*fs + 3.45357917689612e-12);

    b1 = -1.03607375306884e-12*Distortion*Level*0.1*pow(fs,3) + Level*0.1*pow(fs,2)*(-5.38043573960525e-13*fs - 3.45357917689612e-12);

    b2 = 1.03607375306884e-12*Distortion*Level*0.1*pow(fs,3) + Level*0.1*pow(fs,2)*(5.38043573960525e-13*fs - 3.45357917689612e-12);

    b3 = -3.45357917689612e-13*Distortion*Level*0.1*pow(fs,3) + Level*0.1*pow(fs,2)*(-1.79347857986842e-13*fs + 3.45357917689612e-12);

    a0 = Distortion*fs*(fs*(1.2099049942893e-14*fs + 2.51801622165627e-10) + 2.49252253398618e-8) + fs*(fs*(3.57407648445144e-16*fs + 2.81736895109737e-13) + 2.53046873750701e-9) + 2.49252253398618e-7;

    a1 = Distortion*fs*(fs*(-3.62971498286789e-14*fs - 2.51801622165627e-10) + 2.49252253398618e-8) + fs*(fs*(-1.07222294533543e-15*fs - 2.81736895109737e-13) + 2.53046873750701e-9) + 7.47756760195853e-7;

    a2 = Distortion*fs*(fs*(3.62971498286789e-14*fs - 2.51801622165627e-10) - 2.49252253398618e-8) + fs*(fs*(1.07222294533543e-15*fs - 2.81736895109737e-13) - 2.53046873750701e-9) + 7.47756760195853e-7;

    a3 = Distortion*fs*(fs*(-1.2099049942893e-14*fs + 2.51801622165627e-10) - 2.49252253398618e-8) + fs*(fs*(-3.57407648445144e-16*fs + 2.81736895109737e-13) - 2.53046873750701e-9) + 2.49252253398618e-7;
};

process = _<:*(dry),(*(wet) : fuzz : dist ):>_;
