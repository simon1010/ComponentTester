#include "IDispatchComponent.h"
#include <tuple>
#include <functional>

#ifdef max

#undef max
#endif // max

#include <limits>

class CStepSignalGenerator : public IDispatchComponent
{
public:
  CStepSignalGenerator()
  {
    mv_nCounter = 10; // TODO make configurable, depending on time maybe?
  }

protected:
  virtual void Process_(DspSignalBus& inputs, DspSignalBus& outputs)
  {
    if (mf_bOutputHigh())
      outputs.SetValue(0, std::make_tuple(std::numeric_limits<double>::max(), 5.0)); // TODO configure step value
    else
      outputs.SetValue(0, std::make_tuple(0.0, 0.0));
  }

private:
  const char * mf_DerivedGetCompID() override
  {
    char lv_sBuf[3];
    _itoa_s(sv_nCompNr, lv_sBuf, 10);

    char lv_sID[6];
    strcat_s(&lv_sID[0], 6, "R");
    strcat_s(&lv_sID[1], 6, lv_sBuf);

    return "SG1";
  }

  bool mf_bOutputHigh()
  {
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

