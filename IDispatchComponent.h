#pragma once

#include <DSPatch.h>
#include <iostream>
#include <string>
#include <cmath>

#ifdef max
#undef max
#endif // max

#include <limits>
#include <chrono>

using namespace std;

struct Port
{
  string mv_sCurrent_IN;
  string mv_sCurrent_OUT;
  string mv_sVoltage_IN;
  string mv_sVoltage_OUT;
};

typedef std::vector<Port> t_Ports;

class IDispatchComponent : public DspComponent {
protected:
  IDispatchComponent(const int ac_nPortCount = 1);

  static bool mv_bCircuitStable;

  virtual void Process_(DspSignalBus&, DspSignalBus&) override {
   if(!mv_bMaidenTrip)
    {
      mv_nTickDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - mv_LastTime).count();
    }
    else
    {
      mv_bMaidenTrip = false;
    }
    mv_LastTime = std::chrono::steady_clock::now();
  }

  const string mc_sCompID;
  static int sv_nCompNr;
  int64_t mv_nTickDuration;
  std::chrono::steady_clock::time_point mv_LastTime;
  bool mv_bMaidenTrip;
  // convenience
public:
  t_Ports mv_Ports;
};

int IDispatchComponent::sv_nCompNr = 0;
bool IDispatchComponent::mv_bCircuitStable = false;

IDispatchComponent::IDispatchComponent(const int ac_nPortCount)
        : mc_sCompID([]{sv_nCompNr++;return std::to_string(sv_nCompNr);}()), mv_nTickDuration(NAN)
{
  mv_bMaidenTrip = true;
  mv_Ports.resize(ac_nPortCount);

  // Connection ID format: <ID>_<ConnectionType>_<PortNr> (E.g. 1_0_II, first component, port 0, Current input )
  for(int i = 0; i < ac_nPortCount; ++i) {
    mv_Ports[i].mv_sCurrent_IN  = ([this, i] { return mc_sCompID + "_" + std::to_string(i) + "_II"; }());
    AddInput_ (mv_Ports[i].mv_sCurrent_IN); // Current in

    mv_Ports[i].mv_sCurrent_OUT = ([this, i] { return mc_sCompID + "_" + std::to_string(i) + "_IO"; }());
    AddOutput_(mv_Ports[i].mv_sCurrent_OUT); // Current source

    mv_Ports[i].mv_sVoltage_IN  = ([this, i] { return mc_sCompID + "_" + std::to_string(i) + "_UI"; }());
    AddInput_ (mv_Ports[i].mv_sVoltage_IN); // Voltage in

    mv_Ports[i].mv_sVoltage_OUT = ([this, i] { return mc_sCompID + "_" + std::to_string(i) + "_UO"; }());
    AddOutput_(mv_Ports[i].mv_sVoltage_OUT); // Voltage source
  }
}
