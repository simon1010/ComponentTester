//
// Created by Simon B. Szabolcs on 6/3/17.
//

#ifndef DSPATCH_SWEEPGENERATOR_H
#define DSPATCH_SWEEPGENERATOR_H

#endif //DSPATCH_SWEEPGENERATOR_H

#include "IDispatchComponent.h"
#include <cmath>
#include <fstream>

static const char *sc_FileName = "/Users/newuser/Documents/Programming/DSpatch/Logging/SweepLog.csv";
std::ofstream SweepLog;

class CSweepGenerator : public IDispatchComponent {
public:
  CSweepGenerator(
          const double ac_dfAmplitude       = 5.0   // V
          , const double ac_dfStartFreq     = 50.   // Hz
          , const double ac_dfStopFreq      = 200. // Hz
          , const double ac_dfSweepDuration = 2.)   // 2s
          : mc_dfAmplitude    ( ac_dfAmplitude     )
          , mc_dfStartFreq    ( ac_dfStartFreq     )//[ac_dfStartFreq    ](){return ac_dfStartFreq     * pow(10, 9);}()) // adjusted for ns
          , mc_dfStopFreq     ( ac_dfStopFreq      )//[ac_dfStopFreq     ](){return ac_dfStopFreq      * pow(10, 9);}()) // adjusted for ns
          , mc_dfSweepDuration( ac_dfSweepDuration )//[ac_dfSweepDuration](){return ac_dfSweepDuration * pow(10, 9);}()) // ns
  {
    SweepLog.open(sc_FileName, std::ios_base::out);

    mv_dfCurrentOut = NAN;
    mv_nIncrement = 1;

  }

protected:
  virtual void Process_(DspSignalBus &inputs, DspSignalBus &outputs) {

    // Current
    double lv_dfMaxCurrent = NAN;
    bool lv_bHaveMaxCurrent = inputs.GetValue(mv_Ports[0].mv_sCurrent_IN, lv_dfMaxCurrent);
    if (lv_bHaveMaxCurrent && !isnan(lv_dfMaxCurrent)) {
      mv_dfCurrentOut = lv_dfMaxCurrent;
    }

    _super::Process_(inputs, outputs);
    double lv_VoltageOut = mf_dfGetSweep(mv_nTickDuration);//mc_dfAmplitude * sin(mv_nCounter);
    //mv_nCounter += mv_nIncrement;

    //std::cout << "generating: " << lv_VoltageOut << " V and " << mv_dfCurrentOut << " A" << std::endl;
    SweepLog << lv_VoltageOut /* << "," << mv_dfCurrentOut */ << std::endl;

    outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, lv_VoltageOut);
    outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, mv_dfCurrentOut);
  }

private:
  typedef IDispatchComponent _super;

  int mv_nIncrement;
  int mv_nCounter;
  const double mc_dfAmplitude;
  const double mc_dfStartFreq;
  const double mc_dfStopFreq;
  const double mc_dfSweepDuration;
  double mv_dfCurrentOut;

  double mf_dfGetSweep(const int64_t ac_nTickDuration) {

    // Todo direction: mv_nIncrement : + - 1

    if(mv_nCounter < 4000)
      mv_nCounter++;
    else
      mv_nCounter = 0;

    double delta = mv_nCounter/ (float)4000;
    double t = mc_dfSweepDuration * delta;

    double phase = 2 * M_PI * t * (mc_dfStartFreq + (mc_dfStopFreq - mc_dfStartFreq) * delta / 2);
    while (phase > 2 * M_PI) phase = fmod(phase, (2 * M_PI)); // optional
    //printf("%f %f %f", t, phase * 180 / M_PI, 3 * sin(phase));

    return mc_dfAmplitude * sin(phase);
  }
};//