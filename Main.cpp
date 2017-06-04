#include <DSPatch.h>
#include "StepSignalGenerator.h"
#include "SignalLogger.h"
#include "Ground.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "SweepGenerator.h"
#include <assert.h>

using namespace std;

template <class fromComponent, class toComponent>
void PortConnect(DspCircuit& circuit, const fromComponent& Comp_1, const int Comp_1_Port, const toComponent& Comp_2, const int Comp_2_Port)
{
  assert(circuit.ConnectOutToIn(Comp_1, Comp_1.mv_Ports[Comp_1_Port].mv_sCurrent_OUT, Comp_2, Comp_2.mv_Ports[Comp_2_Port].mv_sCurrent_IN ));
  assert(circuit.ConnectOutToIn(Comp_1, Comp_1.mv_Ports[Comp_1_Port].mv_sVoltage_OUT, Comp_2, Comp_2.mv_Ports[Comp_2_Port].mv_sVoltage_IN ));
  assert(circuit.ConnectOutToIn(Comp_2, Comp_2.mv_Ports[Comp_2_Port].mv_sVoltage_OUT, Comp_1, Comp_1.mv_Ports[Comp_1_Port].mv_sVoltage_IN ));
  assert(circuit.ConnectOutToIn(Comp_2, Comp_2.mv_Ports[Comp_2_Port].mv_sCurrent_OUT, Comp_1, Comp_1.mv_Ports[Comp_1_Port].mv_sCurrent_IN ));
}

int main()
{
  DspCircuit circuit;

  CStepSignalGenerator lv_StepGen;
  CResistor            lv_Resistor;
  CGround              lv_Ground;
  CCapacitor           lv_Capacitor;
  CSweepGenerator      lv_SweepGen;

  //circuit.AddComponent(lv_StepGen  , "StepGen"   );
  circuit.AddComponent(lv_Ground   , "Ground"    );
  circuit.AddComponent(lv_Resistor , "Resistor"  );
  circuit.AddComponent(lv_SweepGen , "Sweep"     );
  //circuit.AddComponent(lv_Capacitor, "Capacitor" );

  PortConnect(circuit, lv_SweepGen , 0, lv_Resistor  , 0);
  PortConnect(circuit, lv_Resistor , 1, lv_Ground    , 0);
  //PortConnect(circuit, lv_Capacitor, 1, lv_Ground    , 0);

  // Must keep time-step less than 25,000.00 ns in order to obtain correct real-time audio processing for the entire human audio-range

  for (int i = 0; i < 4000; i++)
  {
    // TODO: Pass the Time to the Process methods in order to have a RT time step
    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    circuit.Tick();
    //std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
    //const int lc_nTime_ns = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();
    //std::cout << "Time difference = " << lc_nTime_ns <<std::endl;
    circuit.Reset();
  }

  DSPatch::Finalize();
  return EXIT_SUCCESS;
}
