//
// Created by Simon B. Szabolcs on 5/25/17.
//

#ifndef DSPATCH_GROUND_H
#define DSPATCH_GROUND_H

#include "IDispatchComponent.h"
#include <iostream>

class CGround : public IDispatchComponent {
public:
  CGround() {
    mv_dfCurrent = NAN;
  }

protected:

  virtual void Process_(DspSignalBus &inputs, DspSignalBus &output) {
    double lv_dfCurrent;
    if(inputs.GetValue(mv_Ports[0].mv_sCurrent_IN, lv_dfCurrent))
    {
      mv_dfCurrent = lv_dfCurrent;
    }

    output.SetValue(mv_Ports[0].mv_sVoltage_OUT, 0.0);
    output.SetValue(mv_Ports[0].mv_sCurrent_OUT, mv_dfCurrent);

    std::cout << "Current flowing to ground : " << mv_dfCurrent << std::endl;
  }

private:
  double mv_dfCurrent;
};

#endif //DSPATCH_GROUND_H
