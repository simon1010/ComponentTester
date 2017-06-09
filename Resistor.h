#pragma once

#include "IDispatchComponent.h"
#include <tuple>
#include <fstream>

static const char *sc_ResFileName = "/Users/newuser/Documents/Programming/DSpatch/Logging/ResistorLog";
std::ofstream ResLog;

class CResistor : public IDispatchComponent {
  static const int sc_nPortsNo = 2;
public:
  CResistor(const double ac_dfResistance = 100.) : IDispatchComponent(sc_nPortsNo) {
    //ResLog.open(sc_ResFileName, std::ios_base::out);

    mv_dfResistance = ac_dfResistance; // ohm
    mv_dfVoltageAcrossResistor = NAN;
    mv_dfCurrent = NAN;
    //mv_bCircuitStable = false;
  }

protected:
  virtual void Process_(DspSignalBus &inputs, DspSignalBus &outputs) override {

    ResLog.open(sc_ResFileName + mc_sCompID + ".csv", std::ios_base::out | std::ios_base::app);

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

    // only do calculations if lv_P0_VoltageIn && lv_P1_VoltageIn are different from their previous values

    if (!isnan(lv_P0_VoltageIn) && !isnan(lv_P1_VoltageIn)) {
      mv_dfVoltageAcrossResistor = lv_P0_VoltageIn - lv_P1_VoltageIn;
      mv_dfCurrent = mv_dfVoltageAcrossResistor / mv_dfResistance;

      double lv_dfEquivResistance = NAN;
      double lc_dfDividerVoltage = NAN;
      if (lv_P1_CurrentIn != lv_P0_CurrentIn && !mv_bCircuitStable) {
        if (!isnan(lv_P0_CurrentIn)) {
          lv_dfEquivResistance = lv_P0_VoltageIn / lv_P0_CurrentIn;

          // apply the voltage divider rule
          lc_dfDividerVoltage = (lv_P0_VoltageIn * mv_dfResistance) / (mv_dfResistance + lv_dfEquivResistance);
          mv_bCircuitStable = true;
        } else if (!isnan(lv_P1_CurrentIn)) {
          lv_dfEquivResistance = lv_P1_VoltageIn / lv_P1_CurrentIn;

          // apply the voltage divider rule
          lc_dfDividerVoltage = (lv_P1_VoltageIn * mv_dfResistance) / (mv_dfResistance + lv_dfEquivResistance);
          mv_bCircuitStable = true;
        }
      } else if (mv_bCircuitStable) {
        mv_bCircuitStable = false;
      }
      const double lc_dfAdjustedCurrent = (lc_dfDividerVoltage / mv_dfResistance);

      mv_dfVoltageAcrossResistor = !isnan(lc_dfDividerVoltage) ? lc_dfDividerVoltage : mv_dfVoltageAcrossResistor;
      mv_dfCurrent = !isnan(lc_dfAdjustedCurrent) ? lc_dfAdjustedCurrent : mv_dfCurrent;

      lv_P0_VoltageOut = mv_dfVoltageAcrossResistor;
      lv_P1_VoltageOut = mv_dfVoltageAcrossResistor;
      lv_P0_CurrentOut = mv_dfCurrent;
      lv_P1_CurrentOut = mv_dfCurrent;

      ResLog << mv_dfVoltageAcrossResistor << "," << mv_dfCurrent << (mv_bCircuitStable ? ",true" : ",false") << std::endl;
      ResLog.close();
    }
    else {
      // make transparent
      lv_P0_VoltageOut = lv_P1_VoltageIn;
      lv_P0_CurrentOut = lv_P1_CurrentIn;
      lv_P1_VoltageOut = lv_P0_VoltageIn;
      lv_P1_CurrentOut = lv_P0_CurrentIn;
    }

    outputs.SetValue(mv_Ports[0].mv_sVoltage_OUT, lv_P0_VoltageOut);
    outputs.SetValue(mv_Ports[0].mv_sCurrent_OUT, lv_P0_CurrentOut);
    outputs.SetValue(mv_Ports[1].mv_sVoltage_OUT, lv_P1_VoltageOut);
    outputs.SetValue(mv_Ports[1].mv_sCurrent_OUT, lv_P1_CurrentOut);
  }

private:


  double mv_dfVoltageAcrossResistor;
  double mv_dfCurrent;
  double mv_dfResistance;
};

