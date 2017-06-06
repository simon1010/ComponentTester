//
// Created by Simon B. Szabolcs on 5/27/17.
//

#ifndef DSPATCH_CAPACITOR_H
#define DSPATCH_CAPACITOR_H

#include "IDispatchComponent.h"

// TODO: test out trapezoidal approximation!

#include <fstream>

static const char* sc_CapFileName = "/Users/newuser/Documents/Programming/DSpatch/Logging/CapacitorLog.csv";
std::ofstream CapLog;

class CCapacitor : public IDispatchComponent {
  static const int sc_nPortsNo = 2;
public:
  CCapacitor(const double ac_dfCapacitance = 100. /* <uF> */ ): IDispatchComponent(sc_nPortsNo), mc_dfCapacitance(ac_dfCapacitance){
    CapLog.open(sc_CapFileName, std::ios_base::out);
    mv_dfCapVoltage         = NAN;
    mv_dfCurrent            = NAN;
    mv_EquivalentResistance = NAN;
    mv_dfPrevInputVoltage   = NAN;
    mv_nElapsedTime         = 0;
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

    if(!isnan(lv_P0_VoltageIn) && !isnan(lv_P1_VoltageIn))
    {
      // TODO: must consider current direction somehow
      mv_dfCapVoltage = lv_P0_VoltageIn - lv_P1_VoltageIn;
    }

    if (!isnan(lv_P0_CurrentIn) && !isnan(lv_P0_VoltageIn)) {
      const double lc_dfVoltageDrop = 0.;
      if(lv_P0_CurrentIn != 0.) {
        mf_ComputeVoltageDrop(lv_P0_CurrentIn, lv_P0_VoltageIn);
      }
      else{
        mv_EquivalentResistance = 0.;
        mv_dfCurrent = 0.;
      }

      mf_DoCapacitor(lv_P0_VoltageIn,lv_P1_VoltageIn, inputs, outputs );

      outputs.SetValue(mv_Ports[1].mv_sVoltage_OUT, lc_dfVoltageDrop);
      outputs.SetValue(mv_Ports[1].mv_sCurrent_OUT, mv_dfCurrent);
      mv_dfCapVoltage = lc_dfVoltageDrop;
      return;
    }
    else if (!isnan(lv_P1_CurrentIn) && !isnan(lv_P1_VoltageIn)) {
      const double lc_dfVoltageDrop = 0.;
      if(lv_P1_CurrentIn != 0.){
        mf_ComputeVoltageDrop(lv_P1_CurrentIn, lv_P1_VoltageIn);
      } else{
        mv_EquivalentResistance = 0.;
        mv_dfCurrent = 0.;
      }

      mf_DoCapacitor(lv_P0_VoltageIn,lv_P1_VoltageIn, inputs, outputs );

      outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, lc_dfVoltageDrop);
      outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, mv_dfCurrent);
      mv_dfCapVoltage = lc_dfVoltageDrop;
      return;
    }

    if (!isnan(lv_P0_VoltageIn) && !isnan(lv_P1_VoltageIn)) {
      const double lc_dfVoltageDifference = (lv_P0_VoltageIn - lv_P1_VoltageIn);
      mv_dfCurrent = lc_dfVoltageDifference / mv_dfResistance;

      // debateable position here?
      mf_DoCapacitor(lv_P0_VoltageIn,lv_P1_VoltageIn, inputs, outputs );

      outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, lc_dfVoltageDifference);
      outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, mv_dfCurrent);
      outputs.SetValue(mv_Ports[1].mv_sVoltage_OUT, lc_dfVoltageDifference);
      outputs.SetValue(mv_Ports[1].mv_sCurrent_OUT, mv_dfCurrent);
      mv_dfCapVoltage = lc_dfVoltageDifference;
      return;
    }

    mf_DoCapacitor(lv_P0_VoltageIn,lv_P1_VoltageIn, inputs, outputs );
    if(!isnan(lv_P1_VoltageIn))
    {
      outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, lv_P1_VoltageIn);
      outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, mv_dfCurrent);
      return;
    }
  }

private:
  typedef IDispatchComponent _super;

  const double mc_dfCapacitance;
  double mv_dfCurrent;
  double mv_dfCapVoltage;
  double mv_dfResistance;
  double mv_EquivalentResistance;
  double mv_dfPrevInputVoltage;

  double mv_nElapsedTime;

  const double mf_ComputeVoltageDrop(const double &ac_dfCurrent, const double &ac_dfVoltage)
  {
    mv_EquivalentResistance = ac_dfVoltage / ac_dfCurrent;

    // apply the voltage divider rule
    const double lc_dfDividerVoltage = (ac_dfVoltage * mv_dfResistance) / (mv_dfResistance + mv_EquivalentResistance);
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

  void mf_DoCapacitor(const double ac_P0_VoltageIn, const double ac_P1_VoltageIn, DspSignalBus &inputs, DspSignalBus &outputs)
  {
    // reset mv_nElapsedTime such that the capacitor can operate in AC mode.
    // we do a reset when we observe a change in charging current direction

    if(!isnan(ac_P0_VoltageIn))
    {
      if(isnan(mv_dfPrevInputVoltage))
        mv_dfPrevInputVoltage = ac_P0_VoltageIn;
      else if (mv_dfPrevInputVoltage - ac_P0_VoltageIn != 0) {
        mv_dfPrevInputVoltage = ac_P0_VoltageIn;
        mv_nElapsedTime = 0;
      }
    }

    _super::Process_(inputs, outputs);
    mv_nElapsedTime += (!isnan(ac_P0_VoltageIn)  && !isnan(ac_P1_VoltageIn)) ? mv_nTickDuration : 0.; // ns
    mv_dfResistance = (mv_nElapsedTime/1000) / mc_dfCapacitance;
    //mv_dfResistance = mv_dfResistance * pow(10, -6); // back to seconds, I reckon
    if(!isnan(mv_dfCapVoltage))
    {
      mv_dfCurrent = mv_dfCapVoltage / (mv_dfResistance + mv_EquivalentResistance);
    }

    CapLog << mv_dfResistance * mv_dfCurrent << "," << mv_dfCurrent << std::endl;
  }
};

#endif //DSPATCH_CAPACITOR_H
