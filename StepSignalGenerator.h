#include "IDispatchComponent.h"
#include <tuple>
#include <functional>
#include <cmath>

#include <fstream>

static const char* sc_StepFileName = "/Users/newuser/Documents/Programming/DSpatch/Logging/StepLog.csv";
std::ofstream StepLog;

class CStepSignalGenerator : public IDispatchComponent {
public:
  CStepSignalGenerator(
          const double ac_dfHighVoltage = 5.0    // V
          , const double ac_dfHoldOffTime = 1. ) // s
          : mc_dfHighVoltage(ac_dfHighVoltage)
          , mc_dfHoldOffTime(ac_dfHoldOffTime)
  {
    StepLog.open(sc_StepFileName, std::ios_base::out);
    mv_nCounter = 0.;
    mv_dfCurrentOut = NAN;
    updown = false;
  }

protected:
  virtual void Process_(DspSignalBus &inputs, DspSignalBus &outputs) {
    // See if the circuit computed it's maximum current for the source
    double lv_dfMaxCurrent = NAN;
    bool lv_bHaveMaxCurrent = inputs.GetValue(mv_Ports[0].mv_sCurrent_IN, lv_dfMaxCurrent);

    if(lv_bHaveMaxCurrent && !isnan(lv_dfMaxCurrent))
    {
      mv_dfCurrentOut = lv_dfMaxCurrent;
    }

    // Update time with call to Super
    _super::Process_(inputs, outputs);
    double lv_VoltageOut = mf_bOutputHigh() ? mc_dfHighVoltage : 0.;

    StepLog << lv_VoltageOut << "," << mv_dfCurrentOut << std::endl;
    outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, lv_VoltageOut);
    outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, mv_dfCurrentOut);
  }

private:
  typedef IDispatchComponent _super;

  bool mf_bOutputHigh() {
    mv_nCounter += (mv_nTickDuration * pow(10, -9)); // take time in seconds
    if(mv_nCounter >= mc_dfHoldOffTime) {
      mv_nCounter = 0;
      updown = !updown;
    }

    return updown;//mv_nCounter >= mc_dfHoldOffTime; //updown;
  }
  bool updown;
  double mv_nCounter;
  // The number of ticks to wait until trigger
  double mv_dfCurrentOut;
  const double mc_dfHighVoltage;
  const double mc_dfHoldOffTime;
};