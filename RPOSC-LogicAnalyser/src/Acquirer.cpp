#include "Acquirer.hpp"

/* TODO: Hier ACQChoosenOptions object das im Konstruktor übergeben wird nehmen und entsprechen in startAcquire auf fpga image setzen
* Vor Starten die SetSrdMetadata starten, die von Florian geschrieben wurde..
* Dann das acquirieren starten und wenn fertig boolean mit true zurück geben, wenn irgendein Fehler, dann false
* Variablen entsprechend anpassen und public setzen (werden in UI angezeigt)
* Datentypen sind std::string und alles was mehrdimensional ist vectoren
* Defaultwerte für Konstruktorparameters.
* keine Werte wie sampleRate setzen, da die beim Start aus ChoosenOptions geholt werden.
* möglichen Werte aus header aussuchen und reinschreiben (z.B. supportedSampleRates)
*/

   //constant values which are given by the rp.h
   const vector<string> supportedSampleRates = {
     "125Msps",
     "15.625Msps",
     "1.953Msps",
     "122.070Ksps",
     "15.258Ksps",
     "1.907Ksps"};
   const vector<int> availableChannels = {"A", "B"};
   const vector<string> supportedDecimations = {
     "1", "8", "64", "1024", "8192", "65536"
   };
   const vector<string> supportedPinState = {"LOW", "HIGH"};

   // the choosen options and vectors to write the samples in
   vector<double> acquiredDataChannelA;
   vector<double> acquiredDataChannelB;
   ACQChoosenOptions *choosenOptions;

   //internal values to check if the acquisition is successfully finished
 private:
   bool acquisition_pending = false;
   bool acquisition_complete = false;

  // base constructor with default parameters
  Acquirer::Acquirer(int sampleRate = 1, int decimation = 8, int pinState = 1, ACQChoosenOptions *choosenOptions = new ACQChoosenOptions()){}
  Acquirer::Acquirer(ACQChoosenOptions *choosenOptions)
  {
    choosenOptions = choosenOptions;
  }

  // sets all the needed parameters and starts the acquisition
  bool startAcquire()
  {
    rp_AcqReset();
    rp_AcqSetDecimation(choosenOptions.decimation);
    rp_AcqSetTriggerDelay() // TODO: Calculate the needed time to get sampleCount with defined sampleRate
    rp_AcqSetGain(0,choosenOptions.pinState);
    rp_AcqStart();

    //TODO: Do for loop and calculate if the acquisition is pending and completed see main.cpp.bak

    // SetSrdMetadata();
    // take ACQChoosenOptions and write to fpga image.
    //
    // start acquisition for all channels and return bool if finished
  }

  vector<double> getData(int channel)
  {
    // get data from specified channel. The acquiredDataChannel vectors contain as much valued as defined in choosenOptions.sampleCount
    switch (channel) {
      case 0: return acquiredDataChannelA;
      case 1: return acquiredDataChannelB;
      default: return vector<double>();
    }
  }
