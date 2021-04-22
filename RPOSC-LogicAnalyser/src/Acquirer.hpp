#pragma once

using namespace std;
#include "ACQChoosenOptions.hpp"
#include <vector>
#include <list>
#include <string>
#include <unistd.h>
#include "rpApp.h"
#include <loguru.hpp>


class Acquirer {
public:
  bool startAcq();
  vector<double> getData(int channel);
  Acquirer(ACQChoosenOptions *choosenOptions);
  Acquirer(int sampleRate, int decimation, int pinState, ACQChoosenOptions *acqChoosenOption);
  Acquirer();
  void ChangeSampleRate(int rate);

private:
  bool acquisitionPending;
  bool acquisitionComplete;
  uint32_t previous_write_pointer;
};
