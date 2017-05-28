#include "IDispatchComponent.h"
#include <tuple>
#include <functional>
#include <cmath>

class CStepSignalGenerator : public IDispatchComponent {
public:
  CStepSignalGenerator(const double ac_dfHighVoltage = 5.0):mc_dfHighVoltage(ac_dfHighVoltage) {
    mv_nCounter = 0; // TODO make configurable, depending on time maybe?
    mv_dfCurrentOut = NAN;
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

    // TODO configure step value
    double lv_VoltageOut = mf_bOutputHigh()   ? mc_dfHighVoltage : NAN;
    //double lv_CurrentOut = lv_bHaveMaxCurrent ? lv_dfMaxCurrent  : NAN;

    std::cout << "generating: " << lv_VoltageOut << " V and " << mv_dfCurrentOut << " A" << std::endl;
    outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, lv_VoltageOut);
    outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, mv_dfCurrentOut);
  }

private:

  bool mf_bOutputHigh() {
    if (mv_nCounter > 0)
      mv_nCounter--;

    return mv_nCounter == 0;
  }

  // The number of ticks to wait until trigger
  int mv_nCounter;
  const double mc_dfHighVoltage;
  double mv_dfCurrentOut;
};