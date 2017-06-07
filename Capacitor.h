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
    mv_dfCapVoltage            = NAN;
    mv_dfCurrent               = NAN;
    mv_EquivalentResistance    = NAN;
    mv_dfPreviousOutputVoltage = 0.;
    mv_dfoutputVoltage         = 0.;
  }

protected:
  virtual void Process_(DspSignalBus &inputs, DspSignalBus &outputs) override {

    // inputs
    double lv_P0_VoltageIn = NAN;
    double lv_P0_CurrentIn = NAN;
    double lv_P1_VoltageIn = NAN;
    double lv_P1_CurrentIn = NAN;

    // outputs
    double lv_P0_VoltageOut = NAN;
    double lv_P0_CurrentOut = NAN;
    double lv_P1_VoltageOut = NAN;
    double lv_P1_CurrentOut = NAN;
    double lv_Reader = NAN;

    // Read all inputs
    if (inputs.GetValue(mv_Ports[0].mv_sVoltage_IN, lv_Reader)) lv_P0_VoltageIn = lv_Reader;
    if (inputs.GetValue(mv_Ports[0].mv_sCurrent_IN, lv_Reader)) lv_P0_CurrentIn = lv_Reader;
    if (inputs.GetValue(mv_Ports[1].mv_sVoltage_IN, lv_Reader)) lv_P1_VoltageIn = lv_Reader;
    if (inputs.GetValue(mv_Ports[1].mv_sCurrent_IN, lv_Reader)) lv_P1_CurrentIn = lv_Reader;

    if(!isnan(lv_P0_VoltageIn) && !isnan(lv_P1_VoltageIn) && !isnan(lv_P0_CurrentIn))
    {
      // we should not divide by 0.
      if(lv_P0_CurrentIn != 0.)
        mv_EquivalentResistance = lv_P0_VoltageIn / lv_P0_CurrentIn;
      else
        mv_EquivalentResistance = 0.;

      mv_dfCapVoltage = lv_P0_VoltageIn - lv_P1_VoltageIn; // Vth
      mf_DoCapacitor(lv_P0_VoltageIn, lv_P1_VoltageIn, inputs, outputs);

      lv_P0_VoltageOut = mv_dfoutputVoltage;
      lv_P0_CurrentOut = mv_dfCurrent;
      lv_P1_VoltageOut = mv_dfoutputVoltage;
      lv_P1_CurrentOut = mv_dfCurrent;
    }
    else
    {
      lv_P0_VoltageOut = lv_P1_VoltageIn;
      lv_P0_CurrentOut = lv_P1_CurrentIn;
      lv_P1_VoltageOut = lv_P0_VoltageIn;
      lv_P1_CurrentOut = lv_P0_CurrentIn;
    }

    outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, lv_P0_VoltageOut);
    outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, lv_P0_CurrentOut);
    outputs.SetValue(mv_Ports[1].mv_sVoltage_OUT, lv_P1_VoltageOut);
    outputs.SetValue(mv_Ports[1].mv_sCurrent_OUT, lv_P1_CurrentOut);

/*
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
    }*/
  }

private:
  typedef IDispatchComponent _super;

  const double mc_dfCapacitance;
  double mv_dfCurrent;
  double mv_dfCapVoltage;
  double mv_dfoutputVoltage;
  double mv_dfPreviousOutputVoltage;
  double mv_dfResistance;
  double mv_EquivalentResistance;

  void mf_DoCapacitor(const double ac_P0_VoltageIn, const double ac_P1_VoltageIn, DspSignalBus &inputs, DspSignalBus &outputs)
  {
    _super::Process_(inputs, outputs);
    mv_dfResistance = mv_nTickDuration / mc_dfCapacitance; // dt / C = Rth
    if(!isnan(mv_dfCapVoltage))
    {
      // first trip
      if(isnan(mv_dfPreviousOutputVoltage))
      {
        mv_dfPreviousOutputVoltage = 0.;//mv_dfResistance / (mv_dfResistance + mv_EquivalentResistance) * mv_dfCapVoltage; // may not be correct, better 0. ?
      }
      mv_dfoutputVoltage = ((mv_dfResistance / (mv_dfResistance + mv_EquivalentResistance)) * (mv_dfCapVoltage - mv_dfPreviousOutputVoltage)) + mv_dfPreviousOutputVoltage;
      mv_dfPreviousOutputVoltage = mv_dfoutputVoltage;
      mv_dfCurrent = -mv_dfCapVoltage / mv_dfResistance;//mv_dfoutputVoltage / (mv_dfResistance + mv_EquivalentResistance);
    }

    CapLog << /*mv_dfResistance * mv_dfCurrent*/mv_dfoutputVoltage << "," << mv_dfCurrent << std::endl;
  }
};

#endif //DSPATCH_CAPACITOR_H
