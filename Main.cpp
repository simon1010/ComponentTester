#include <DSPatch.h>
#include "StepSignalGenerator.h"
#include "SignalLogger.h"
//#include "Ground.h"
#include "Resistor.h"
using namespace std;

int main()
{
  DspCircuit circuit;

  CStepSignalGenerator lv_StepGen;
  CSignalLogger lv_Logger;

  circuit.AddComponent(lv_StepGen, "StepGen");
  circuit.AddComponent(lv_Logger,  "logger" );

  circuit.ConnectOutToIn(lv_StepGen, lv_StepGen.mv_Ports[0].mv_sCurrent_OUT, lv_Logger, lv_Logger.mv_Ports[0].mv_sCurrent_IN);
  circuit.ConnectOutToIn(lv_StepGen, lv_StepGen.mv_Ports[0].mv_sVoltage_OUT, lv_Logger, lv_Logger.mv_Ports[0].mv_sVoltage_IN);

  for (int i = 0; i < 20; i++)
  {
    circuit.Tick();
    circuit.Reset();
  }

  DSPatch::Finalize();
  return EXIT_SUCCESS;
}
