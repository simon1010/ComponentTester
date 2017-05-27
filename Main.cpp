#include <DSPatch.h>
#include "StepSignalGenerator.h"
#include "SignalLogger.h"
#include "Ground.h"
#include "Resistor.h"
using namespace std;

template <class fromComponent, class toComponent>
void PortConnect(DspCircuit& circuit, const fromComponent& Comp_1, const int Comp_1_Port, const toComponent& Comp_2, const int Comp_2_Port)
{
  circuit.ConnectOutToIn(Comp_1, Comp_1.mv_Ports[Comp_1_Port].mv_sCurrent_OUT, Comp_2, Comp_2.mv_Ports[Comp_2_Port].mv_sCurrent_IN );
  circuit.ConnectOutToIn(Comp_1, Comp_1.mv_Ports[Comp_1_Port].mv_sVoltage_OUT, Comp_2, Comp_2.mv_Ports[Comp_2_Port].mv_sVoltage_IN );
  circuit.ConnectOutToIn(Comp_2, Comp_2.mv_Ports[Comp_2_Port].mv_sVoltage_OUT, Comp_1, Comp_1.mv_Ports[Comp_1_Port].mv_sVoltage_IN );
  circuit.ConnectOutToIn(Comp_2, Comp_2.mv_Ports[Comp_2_Port].mv_sCurrent_OUT, Comp_1, Comp_1.mv_Ports[Comp_1_Port].mv_sCurrent_IN );
}

int main()
{
  DspCircuit circuit;

  CStepSignalGenerator lv_StepGen;
//CSignalLogger        lv_Logger;
  CResistor            lv_Resistor;
  CGround              lv_Ground;

  circuit.AddComponent(lv_StepGen , "StepGen"  );
//circuit.AddComponent(lv_Logger  , "Logger"   );
  circuit.AddComponent(lv_Resistor, "Resistor" );
  circuit.AddComponent(lv_Ground  , "Ground"   );

  // food for thought:
  // circuit.connect(lv_StepGen, S_portNo, lv_Resistor, R_portNo);

  // P0
  //circuit.ConnectOutToIn(lv_StepGen , lv_StepGen.mv_Ports[0].mv_sCurrent_OUT, lv_Resistor, lv_Resistor.mv_Ports[0].mv_sCurrent_IN );
  //circuit.ConnectOutToIn(lv_StepGen , lv_StepGen.mv_Ports[0].mv_sVoltage_OUT, lv_Resistor, lv_Resistor.mv_Ports[0].mv_sVoltage_IN );
  //circuit.ConnectOutToIn(lv_Resistor, lv_Resistor.mv_Ports[0].mv_sVoltage_OUT, lv_StepGen, lv_StepGen.mv_Ports[0].mv_sVoltage_IN );
  //circuit.ConnectOutToIn(lv_Resistor, lv_Resistor.mv_Ports[0].mv_sCurrent_OUT, lv_StepGen, lv_StepGen.mv_Ports[0].mv_sCurrent_IN );

  PortConnect(circuit, lv_StepGen , 0, lv_Resistor, 0);
  PortConnect(circuit, lv_Resistor, 1, lv_Ground  , 0);

  for (int i = 0; i < 20; i++)
  {
    circuit.Tick();
    circuit.Reset();
  }

  DSPatch::Finalize();
  return EXIT_SUCCESS;
}
