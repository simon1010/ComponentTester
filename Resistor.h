#pragma once

#include "IDispatchComponent.h"
#include <tuple>

class CResistor : public IDispatchComponent
{
public:
  CResistor()
  {
    mv_dfResistance = 10.; // ohm
    mv_dfVoltageAcrossResistor = 0.;
  }

protected:

  // TODO: Think of equivalent upstream resistance!
  virtual void Process_(DspSignalBus& inputs, DspSignalBus& output) override
  {

  }

private:
  double mv_dfVoltageAcrossResistor;
  double mv_dfResistance;
};
