#pragma once

using namespace std;
#include "ACQChoosenOptions.hpp"
#include <vector>
#include <list>
#include <string>
#include "rpApp.h"

class Acquirer {
public:
  const vector<string> supportedSampleRates;
  const vector<string> supportedDecimations;
  const vector<string> supportedPinState;
  void startAcq();
  vector<double> getData();
  Acquirer(ACQChoosenOptions *choosenOptions);
  Acquirer(int sampleRate, int decimation, int pinState, ACQChoosenOptions *acqChoosenOption);
  Acquirer();
  void ChangeSampleRate(int rate);

private:
  bool acquisitionPending;
  bool acquisitionComplete;
  uint32_t previous_write_pointer;
  //CBooleanParameter start_acquisition;
};
