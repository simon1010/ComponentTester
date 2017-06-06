#pragma once

#include "IDispatchComponent.h"
#include <tuple>
#include <fstream>

static const char* sc_ResFileName = "/Users/newuser/Documents/Programming/DSpatch/Logging/ResistorLog.csv";
std::ofstream ResLog;

class CResistor : public IDispatchComponent {
  static const int sc_nPortsNo = 2;
public:
  CResistor() : IDispatchComponent(sc_nPortsNo) {
    ResLog.open(sc_ResFileName, std::ios_base::out);

    mv_dfResistance = 10000.; // ohm
    mv_dfVoltageAcrossResistor = NAN;
    mv_dfCurrent = NAN;
  }

protected:
  virtual void Process_(DspSignalBus &inputs, DspSignalBus &outputs) override {

    double lv_P0_VoltageIn = NAN;
    double lv_P0_CurrentIn = NAN;
    double lv_P1_VoltageIn = NAN;
    double lv_P1_CurrentIn = NAN;
    double lv_Reader = NAN;

    // Read all inputs
    if (inputs.GetValue(mv_Ports[0].mv_sVoltage_IN, lv_Reader)) lv_P0_VoltageIn = lv_Reader;
    if (inputs.GetValue(mv_Ports[0].mv_sCurrent_IN, lv_Reader)) lv_P0_CurrentIn = lv_Reader;
    if (inputs.GetValue(mv_Ports[1].mv_sVoltage_IN, lv_Reader)) lv_P1_VoltageIn = lv_Reader;
    if (inputs.GetValue(mv_Ports[1].mv_sCurrent_IN, lv_Reader)) lv_P1_CurrentIn = lv_Reader;

    // first compute equivalent upstream resistance and voltage drop
    if (!isnan(lv_P0_CurrentIn) && !isnan(lv_P0_VoltageIn)) {
      const double lc_dfVoltageDrop = mf_ComputeVoltageDrop(lv_P0_CurrentIn, lv_P0_VoltageIn);
      outputs.SetValue(mv_Ports[1].mv_sVoltage_OUT, lc_dfVoltageDrop);
      outputs.SetValue(mv_Ports[1].mv_sCurrent_OUT, mv_dfCurrent);
      mv_dfVoltageAcrossResistor = lc_dfVoltageDrop;
      ResLog << "P1," << lc_dfVoltageDrop << "," << mv_dfCurrent << std::endl;
      return;
    }
    else if (!isnan(lv_P1_CurrentIn) && !isnan(lv_P1_VoltageIn)) {
      const double lc_dfVoltageDrop = mf_ComputeVoltageDrop(lv_P1_CurrentIn, lv_P1_VoltageIn);
      outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, lc_dfVoltageDrop);
      outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, mv_dfCurrent);
      mv_dfVoltageAcrossResistor = lc_dfVoltageDrop;
      ResLog << "P0," << lc_dfVoltageDrop << "," << mv_dfCurrent << std::endl;
      return;
    }

    if (!isnan(lv_P0_VoltageIn) && !isnan(lv_P1_VoltageIn)) {
      const double lc_dfVoltageDifference = (lv_P0_VoltageIn - lv_P1_VoltageIn);
      mv_dfCurrent = lc_dfVoltageDifference / mv_dfResistance;

      outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, lc_dfVoltageDifference);
      outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, mv_dfCurrent);
      outputs.SetValue(mv_Ports[1].mv_sVoltage_OUT, lc_dfVoltageDifference);
      outputs.SetValue(mv_Ports[1].mv_sCurrent_OUT, mv_dfCurrent);
      mv_dfVoltageAcrossResistor = lc_dfVoltageDifference;
      ResLog << "P0," << lc_dfVoltageDifference << "," << mv_dfCurrent << std::endl;
      ResLog << "P1," << lc_dfVoltageDifference << "," << mv_dfCurrent << std::endl;
      return;
    }

    // we don't do it the opposite way around so we don't create a race-condition
    /*if(!isnan(lv_P0_VoltageIn))
    {
      outputs.SetValue(mv_Ports[1].mv_sVoltage_OUT, lv_P0_VoltageIn);
      outputs.SetValue(mv_Ports[1].mv_sCurrent_OUT, mv_dfCurrent);
      return;
    }*/

    if(!isnan(lv_P1_VoltageIn))
    {
      outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, lv_P1_VoltageIn);
      outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, mv_dfCurrent);
      return;
    }
  }

private:
  const double mf_ComputeVoltageDrop(const double &ac_dfCurrent, const double &ac_dfVoltage);

  double mv_dfVoltageAcrossResistor;
  double mv_dfCurrent;
  double mv_dfResistance;
  bool mv_bFlowFrom0to1; // TODO: organicity, this will update the current correctly everywhere
};

const double CResistor::mf_ComputeVoltageDrop(const double &ac_dfCurrent, const double &ac_dfVoltage) {
  double lv_dfEquivResistance = 0.;
  if(ac_dfCurrent != 0. && ac_dfVoltage != 0.)
    lv_dfEquivResistance = ac_dfVoltage / ac_dfCurrent;

  // apply the voltage divider rule
  const double lc_dfDividerVoltage = (ac_dfVoltage * mv_dfResistance) / (mv_dfResistance + lv_dfEquivResistance);
  const double lc_dfAdjustedCurrent = (lc_dfDividerVoltage / mv_dfResistance);

  // only update the output voltage if the current is different in the downstream
  if (ac_dfCurrent != mv_dfCurrent && lc_dfAdjustedCurrent != mv_dfCurrent) {
    // Update the current
    mv_dfCurrent = lc_dfAdjustedCurrent;
    return lc_dfDividerVoltage;
  }
  else {
    mv_dfCurrent = ac_dfVoltage/mv_dfResistance;
    return ac_dfVoltage;
  }
}