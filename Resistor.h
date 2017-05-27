#pragma once

#include "IDispatchComponent.h"
#include <tuple>

class CResistor : public IDispatchComponent {
  static const int sc_nPortsNo = 2;
public:
  CResistor() : IDispatchComponent(sc_nPortsNo) {
    mv_dfResistance = 10.; // ohm
    mv_dfVoltageAcrossResistor = 0.;
  }

protected:

  // TODO: Think of equivalent upstream resistance!
  virtual void Process_(DspSignalBus &inputs, DspSignalBus &output) override {

    double U1, U2;
    double lv_dfCurrent;
    if (inputs.GetValue(mv_Ports[0].mv_sVoltage_IN, U1) && inputs.GetValue(mv_Ports[1].mv_sVoltage_IN, U2)) {
      mv_dfVoltageAcrossResistor = U1 - U2;
      lv_dfCurrent = mv_dfVoltageAcrossResistor / mv_dfResistance;
      std::cout << "Voltage Across R:"  << mv_dfVoltageAcrossResistor << "V, I:" << lv_dfCurrent << std::endl;

      output.SetValue(mv_Ports[0].mv_sCurrent_OUT, lv_dfCurrent);
      output.SetValue(mv_Ports[1].mv_sCurrent_OUT, lv_dfCurrent);
    }
  }

private:
  double mv_dfVoltageAcrossResistor;
  double mv_dfResistance;
};