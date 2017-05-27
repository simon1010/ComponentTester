#pragma once

#include <DSPatch.h>
#include <iostream>
#include <string>
using namespace std;

typedef std::tuple<double, double> t_FlowData;

class IDispatchComponent : public DspComponent {
protected:
  IDispatchComponent();
  const string mc_sCompID;
  static int sv_nCompNr;

  // convenience
public:
  const string mc_sCurrent_IN;
  const string mc_sCurrent_OUT;
  const string mc_sVoltage_IN;
  const string mc_sVoltage_OUT;
};

int IDispatchComponent::sv_nCompNr = 0;

IDispatchComponent::IDispatchComponent()
        : mc_sCompID([]{sv_nCompNr++;return std::to_string(sv_nCompNr);}())
        , mc_sCurrent_IN ([this]{return mc_sCompID + "II";}())
        , mc_sCurrent_OUT([this]{return mc_sCompID + "IO";}())
        , mc_sVoltage_IN ([this]{return mc_sCompID + "UI";}())
        , mc_sVoltage_OUT([this]{return mc_sCompID + "UO";}())
{
  AddInput_ (mc_sCurrent_IN ); // Current in
  AddInput_ (mc_sVoltage_IN); // Voltage in
  AddOutput_(mc_sCurrent_OUT ); // Current source
  AddOutput_(mc_sVoltage_OUT); // Voltage source
}
