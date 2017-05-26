#include "IDispatchComponent.h"
#include <iostream>
#include <tuple>

class CSignalLogger : public IDispatchComponent
{
public:
  CSignalLogger()
  {
  }

protected:

  virtual const char * mf_DerivedGetCompID() override
  {
    return "Lo1";
  }

  virtual void Process_(DspSignalBus& inputs, DspSignalBus&)
  {
    // create a local stack variable to hold input value
    //t_FlowData inputValue;

    // get the value from inputs bus and print it
    //inputs.GetValue(0, inputValue);
    //std::cout << "I: " << std::get<0>(inputValue) << " U: " << std::get<1>(inputValue) << std::endl;

  }
};
