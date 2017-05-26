#include <DSPatch.h>
#include "StepSignalGenerator.h"
#include "SignalLogger.h"
//#include "Ground.h"
#include "Resistor.h"
using namespace std;

int main()
{
  // 1. Create a DspCircuit where we can route our components
  // ========================================================
  DspCircuit circuit;

  // 2. Create instances of the components needed for our circuit
  // ============================================================
  CStepSignalGenerator lv_StepGen;
  CResistor lv_Resistor;
  //CGround lv_Ground;
  CSignalLogger lv_Logger;
    
  // 3. Add component instances to circuit
  // =====================================
  circuit.AddComponent(lv_StepGen, "StepGen");
  //circuit.AddComponent(lv_Resistor, "Resistor");
  //circuit.AddComponent(lv_Ground, "Ground");
  //circuit.AddComponent(lv_Logger,  "logger" );
    
  // 4. Wire up the components inside the circuit
  // ============================================
  circuit.ConnectOutToOut(lv_StepGen , "StGen1O", "G1O");
  //circuit.ConnectOutToIn(lv_Resistor, 0, lv_Logger  , 0);
  //circuit.ConnectOutToOut(lv_Resistor, "R1O", "G1O");

  // 5. Tick the circuit
  // ===================

  // Circuit tick method 1: Manual
  for (int i = 0; i < 20; i++)
  {
    circuit.Tick();
    circuit.Reset();
  }

  // Press any key to quit
  // getchar();

  // 6. Clean up
  // ===========
  DSPatch::Finalize();

  return 0;
}
