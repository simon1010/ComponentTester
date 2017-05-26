#pragma once

#include <DSPatch.h>

typedef std::tuple<double, double> t_FlowData;

class IDispatchComponent : public DspComponent
{
protected:
  virtual const  char * mf_DerivedGetCompID() = 0;
  IDispatchComponent();
  static int sv_nCompNr;

private:
  const char * mf_getCompId()
  {
    return mf_DerivedGetCompID();
  }
};

int IDispatchComponent::sv_nCompNr = 0;

IDispatchComponent::IDispatchComponent()
{
  sv_nCompNr++;

  const char * lv_szCompUniqueId = mf_getCompId();
  char lv_StringID[6];
  strcat_s(&lv_StringID[0], 6, lv_szCompUniqueId);
  strcat_s(&lv_StringID[3], 6,"UI");
  AddInput_(lv_StringID);
  
  strcat_s(&lv_StringID[3], 6,"II");
  AddInput_(lv_StringID);

  strcat_s(&lv_StringID[3], 6,"UO");
  AddOutput_(lv_StringID);

  strcat_s(&lv_StringID[3], 6,"IO");
  AddOutput_(lv_StringID);
}
