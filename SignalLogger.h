#include "IDispatchComponent.h"
#include <iostream>
#include <tuple>

class CSignalLogger : public IDispatchComponent {
public:
  CSignalLogger() {
  }

protected:

  virtual void Process_(DspSignalBus &inputs, DspSignalBus &) {
    // create a local stack variable to hold input value
    double CurrentIN;
    double VoltageIN;

    // get the value from inputs bus and print it
    inputs.GetValue(mv_Ports[0].mv_sCurrent_IN, CurrentIN);
    inputs.GetValue(mv_Ports[0].mv_sVoltage_IN, VoltageIN);
    std::cout << "I: " << CurrentIN << " U: " << VoltageIN << std::endl;
  }
};
