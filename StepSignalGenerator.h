#include "IDispatchComponent.h"
#include <tuple>
#include <functional>

#ifdef max

#undef max
#endif // max

#include <limits>

class CStepSignalGenerator : public IDispatchComponent {
public:
  CStepSignalGenerator(const double ac_dfHighVoltage = 5.0):mc_dfHighVoltage(ac_dfHighVoltage) {
    mv_nCounter = 10; // TODO make configurable, depending on time maybe?
  }

protected:
  virtual void Process_(DspSignalBus &inputs, DspSignalBus &outputs) {
    if (mf_bOutputHigh()) {
      std::cout << "generating: " << mc_dfHighVoltage << " V" << std::endl;
      outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, mc_dfHighVoltage); // TODO configure step value
      outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, std::numeric_limits<double>::max()); // TODO based on current in
    }
    else {
      std::cout << "generating: " << 0.0 << " V" << std::endl;
      outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, 0.0);
      outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, 0.0);
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
  const double mc_dfHighVoltage;
};

#ifdef max

#undef max
#endif // max

