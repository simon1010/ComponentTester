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
  CGround():IDispatchComponent() {
    mv_dfCurrent = 0.0;
  }

protected:

  virtual void Process_(DspSignalBus &inputs, DspSignalBus &output) {
    // Update incoming current value, todo maybe not needed?
    t_FlowData inputValue;
    output.GetValue(0, inputValue);
    mv_dfCurrent = std::get<0>(inputValue);

    // the voltage supplied by ground is always 0
    t_FlowData outputValue = std::make_tuple(mv_dfCurrent, 0.0);
    output.SetValue(0, outputValue);

    std::cout << " I: " << mv_dfCurrent << std::endl;
  }

private:
  double mv_dfCurrent;
};


#endif //DSPATCH_GROUND_H
