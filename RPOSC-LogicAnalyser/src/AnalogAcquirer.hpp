#pragma once

using namespace std;
#include "ACQChoosenOptions.hpp"
#include <vector>
#include <list>
#include <string>
#include <unistd.h>
#include "rpApp.h"
#include <loguru.hpp>
#include "Acquirer.hpp"


class AnalogAcquirer: public Acquirer {
public:
  bool startAcq();
  vector<float> getData(int channel);
  AnalogAcquirer(ACQChoosenOptions *choosenOptions);
  AnalogAcquirer(int sampleRate, int decimation, int pinState, ACQChoosenOptions *acqChoosenOption);
  AnalogAcquirer();
  void ChangeSampleRate(int rate);

private:
  bool acquisitionPending;
  bool acquisitionComplete;
  uint32_t previous_write_pointer;
};
