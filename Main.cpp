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

  for(int i = lv_StepGen.GetInputCount() - 1; i >= 0; --i) {
    std::cout << lv_StepGen.GetInputName(i) << std::endl;
    std::cout << lv_StepGen.GetOutputName(i) << std::endl;
  }

  circuit.AddComponent(lv_StepGen, "StepGen");
  circuit.AddComponent(lv_Logger,  "logger" );

  circuit.ConnectOutToIn(lv_StepGen, lv_StepGen.mc_sCurrent_OUT, lv_Logger, lv_Logger.mc_sCurrent_IN);
  circuit.ConnectOutToIn(lv_StepGen, lv_StepGen.mc_sVoltage_OUT, lv_Logger, lv_Logger.mc_sVoltage_IN);

  for (int i = 0; i < 20; i++)
  {
    circuit.Tick();
    circuit.Reset();
  }

  DSPatch::Finalize();
  return EXIT_SUCCESS;
}
