#include "IDispatchComponent.h"
#include <tuple>
#include <functional>

#ifdef max

#undef max
#endif // max

#include <limits>

class CStepSignalGenerator : public IDispatchComponent {
public:
  CStepSignalGenerator() {
    mv_nCounter = 10; // TODO make configurable, depending on time maybe?
  }

protected:
  virtual void Process_(DspSignalBus &inputs, DspSignalBus &outputs) {
    if (mf_bOutputHigh()) {
      outputs.SetValue(mc_sVoltage_OUT, 5.0); // TODO configure step value
      outputs.SetValue(mc_sCurrent_OUT, std::numeric_limits<double>::max()); // TODO based on current in
    }
    else {
      outputs.SetValue(mc_sVoltage_OUT, 0.0);
      outputs.SetValue(mc_sCurrent_OUT, 0.0);
    }
  }

private:

  bool mf_bOutputHigh() {
    if (mv_nCounter > 0)
      mv_nCounter--;

    return mv_nCounter == 0;
  }

  // The number of ticks to wait untill trigger, todo? triggered step?  
  int mv_nCounter;
};

#ifdef max

#undef max
#endif // max

