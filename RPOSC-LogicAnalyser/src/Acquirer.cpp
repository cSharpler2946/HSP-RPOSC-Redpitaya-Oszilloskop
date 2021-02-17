#include "Acquirer.h"
#include "ACQChoosenOptions.h"
#include <vector>
#include <list>
#include <string>

/* TODO: Hier ACQChoosenOptions object das im Konstruktor übergeben wird nehmen und entsprechen in startAcquire auf fpga image setzen
* Vor Starten die SetSrdMetadata starten, die von Florian geschrieben wurde..
* Dann das acquirieren starten und wenn fertig boolean mit true zurück geben, wenn irgendein Fehler, dann false
* Variablen entsprechend anpassen und public setzen (werden in UI angezeigt)
* Datentypen sind std::string und alles was mehrdimensional ist vectoren
* Defaultwerte für Konstruktorparameters.
* keine Werte wie sampleRate setzen, da die beim Start aus ChoosenOptions geholt werden.
* möglichen Werte aus header aussuchen und reinschreiben (z.B. supportedSampleRates)
*/

   list<int> supportedSampleRates = new list<int>(1000,5000,10000);
   int choosenSampleRate;
   std::list<int> availableChannels;
   std::list<string> supportedDecimations;
   std::list<string> supportedGains;
   std::list<double> aquiredData; //Todo: für channel 1 und 2

class Acquirer{
public:
  // set the chossen parameters to private variables but make sure they are supported ones
  Acquirer(int sampleRate = 15003, ACQChoosenOptions *choosenOptions, list<string> decimation, list<string> gains)
  {
    if(supportedSampleRates.Contains(sampleRate))
    {
      choosenSampleRate = sampleRate;
    }
  }

  bool startAcquire()
  {
    // take ACQChoosenOptions and write to fpga image.
    //
    // start acquisition for all channels and return bool if finished
  }

  vector<double> getData(int channel)
  {
    // get data from specified channel
    return aquiredData;
  }
}
