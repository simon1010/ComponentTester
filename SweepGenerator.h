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
          const double ac_dfAmplitude = 5.0         // V
          , const double ac_dfStartFreq = 5.       // Hz
          , const double ac_dfStopFreq = 500.       // Hz
          , const double ac_dfSweepDuration = 2.)   // 2s
          : mc_dfAmplitude    ( ac_dfAmplitude    )
          , mc_dfStartFreq    ( ac_dfStartFreq    )
          , mc_dfStopFreq     ( ac_dfStopFreq     )
          , mc_dfSweepDuration( ac_dfSweepDuration)
  {
    SweepLog.open(sc_FileName, std::ios_base::out);
    mv_tElapsedTime = 0;
    mv_dfCurrentOut = NAN;
    mv_nIncrement = 1;
    mv_bIncrement = true;
  }

protected:
  virtual void Process_(DspSignalBus &inputs, DspSignalBus &outputs) {

    // Current
    double lv_dfMaxCurrent = NAN;
    bool lv_bHaveMaxCurrent = inputs.GetValue(mv_Ports[0].mv_sCurrent_IN, lv_dfMaxCurrent);
    if (lv_bHaveMaxCurrent && !isnan(lv_dfMaxCurrent)) {
      mv_dfCurrentOut = lv_dfMaxCurrent;
    }
    //mv_tElapsedTime += mv_nTickDuration;//194000;
    _super::Process_(inputs, outputs);
    double lv_VoltageOut = mf_dfGetSweep(19400);//mc_dfAmplitude * sin(2 * M_PI * 10 * mv_tElapsedTime * pow(10, -9));//mf_dfGetSweep(mv_nTickDuration);

    SweepLog << lv_VoltageOut  << "," << mv_dfCurrentOut << std::endl;

    outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, lv_VoltageOut);
    outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, mv_dfCurrentOut);
  }

private:
  typedef IDispatchComponent _super;

  const double mc_dfAmplitude;
  const double mc_dfStartFreq;
  const double mc_dfStopFreq;
  const double mc_dfSweepDuration;
  double mv_dfCurrentOut;
  bool mv_bIncrement;
  int mv_nIncrement;
  double mv_tElapsedTime;

  double mf_dfGetSweep(const int64_t ac_nTickDuration) {
    // Handle sweep up and then down smoothly
    if (mv_bIncrement && mv_tElapsedTime >= (mc_dfSweepDuration / 2)) {
      mv_bIncrement = false;
      mv_nIncrement = -1;
    } else if (!mv_bIncrement && mv_tElapsedTime <= 0) {
      mv_bIncrement = true;
      mv_nIncrement = 1;
    }

    mv_tElapsedTime += ac_nTickDuration * pow(10, -9) * mv_nIncrement; // from ns to s => may produce rounding error
    double delta = mv_tElapsedTime / mc_dfSweepDuration;
    double t = mc_dfSweepDuration * delta;

    double phase = 2 * M_PI * t * (mc_dfStartFreq + (mc_dfStopFreq - mc_dfStartFreq) * delta / 2);
    while (phase > 2 * M_PI) phase = fmod(phase, (2 * M_PI));

    return mc_dfAmplitude * sin(phase);
  }
};

