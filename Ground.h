//
// Created by Simon B. Szabolcs on 5/25/17.
//

#ifndef DSPATCH_GROUND_H
#define DSPATCH_GROUND_H

#include "IDispatchComponent.h"
#include <iostream>

typedef std::tuple<double, double> t_FlowData;

class CGround : public IDispatchComponent {
public:
  CGround() {
    mv_dfCurrent = 0.0;
  }

protected:

  virtual void Process_(DspSignalBus &inputs, DspSignalBus &output) {

  }

private:
  double mv_dfCurrent;
};


#endif //DSPATCH_GROUND_H
