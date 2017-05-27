#pragma once

#include "IDispatchComponent.h"
#include <tuple>

typedef std::tuple<double, double> t_FlowData;

class CResistor : public IDispatchComponent {
public:
  CResistor() {
    mv_dfResistance = 10.; // ohm
    mv_dfVoltageAcrossResistor = 0.;
  }

protected:

  const std::string mf_DerivedGetCompID() override {
    return "R";
  }

  // TODO: Think of equivalent upstream resistance!
  virtual void Process_(DspSignalBus &inputs, DspSignalBus &output) {
    // Get data from both sides
    t_FlowData lv_Inp, lv_Out;
    inputs.GetValue(0, lv_Inp);
    output.GetValue(0, lv_Out);

    // Process it
    const double lc_InputVoltage = std::get<1>(lv_Inp);
    const double lc_OutputVoltage = std::get<1>(lv_Out);

    mv_dfVoltageAcrossResistor = lc_InputVoltage - lc_OutputVoltage; // todo: fucked up
    const double lc_AllowedCurrent = mv_dfVoltageAcrossResistor / mv_dfResistance; // U = R x I, I = U / R

    // Update circuit
    inputs.SetValue(0, std::make_tuple(lc_AllowedCurrent, lc_InputVoltage));
    inputs.SetValue(0, std::make_tuple(lc_AllowedCurrent, lc_OutputVoltage));
  }

private:
  double mv_dfVoltageAcrossResistor;
  double mv_dfResistance;
};
