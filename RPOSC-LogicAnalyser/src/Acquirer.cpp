using namespace std
#include "Aquirer.h"

private:
   list<int> supportedSampleRates;
   int choosenSampleRate;
   list<string> availableChannels;
   list<string> supportedDecimations;
   list<string> supportedGains;
   list<double> aquiredData;

class Acquirer{
public:
  // set the chossen parameters to private variables but make sure they are supported ones
  Acquirer(int sampleRate, list<string> channels, list<string> decimation, list<string> gains)
  {
    if(supportedSampleRates.Contains(sampleRate))
    {
      choosenSampleRate = sampleRate;
    }
  }

  void ChangeSampleRate(int rate)
  {
    choosenSampleRate = rate
  }

  void startAquire()
  {

  }

  list<double> getData()
  {
    return aquiredData;
  }
}

class AcqChosenOptions : Acquirer
{
private:
  Acquirer acquirer = new Acquirer()
  void setAcqOptions()
  {

  }
  override
}
