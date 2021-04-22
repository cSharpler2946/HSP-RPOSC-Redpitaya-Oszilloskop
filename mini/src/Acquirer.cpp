#include "Acquirer.hpp"
//#include <sstream>
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
   /*const std::vector<string> supportedSampleRates = {
     "125Msps",
     "15.625Msps",
     "1.953Msps",
     "122.070Ksps",
     "15.258Ksps",
     "1.907Ksps"};*/
   const std::vector<string> availableChannels = {"A", "B"};
   const std::vector<string> supportedDecimations = {
     "1", "8", "64", "1024", "8192", "65536"
   };
   const std::vector<std::string> supportedPinState = {"LOW", "HIGH"};

   // the choosen options and vectors to write the samples in
   std::vector<float> acquiredDataChannelA;
   std::vector<float> acquiredDataChannelB;
   ACQChoosenOptions *choosenOptions;

   //internal values to check if the acquisition is successfully finished
   bool acquisitionPending = false;
   bool acquisitionComplete = false;
   uint32_t previousWritePointer = 0;
   //CBooleanParameter startAcquisition("START_ACQUISITION", CBaseParameter::AccessMode::RW, false, false);

  // base constructor with default parameters
  Acquirer::Acquirer(int sampleRate = 1, int decimation = 1, int pinState = 1, ACQChoosenOptions *choosenOptions = new ACQChoosenOptions()){}
  Acquirer::Acquirer(){}
  Acquirer::Acquirer(ACQChoosenOptions *_choosenOptions)
  {
    //choosenOptions = _choosenOptions;
  }

  // sets all the needed parameters and starts the acquisition
  /*bool Acquirer::startAcq()
  {

    /*LOOB BACK FROM OUTPUT of channel 1 - ONLY FOR TESTING
    // Delete if it works!!
    rp_GenReset();
    rp_GenFreq(RP_CH_1, 20000.0);
    rp_GenAmp(RP_CH_1, 1.0);
    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);
    rp_GenOutEnable(RP_CH_1);

    // needed variables
    uint32_t writePointer;
    rp_AcqGetWritePointer(&writePointer);

    // clear the vector
    acquiredDataChannelA.clear();
    acquiredDataChannelB.clear();
    // SetSrdMetadata();
    LOG_F(INFO, "cleared channel vectors");

    // do all the initialization stuff for the Acquisition
    rp_AcqReset();
    LOG_F(INFO, "reset acquiring");
    rp_AcqSetDecimation(rp_acq_decimation_t(choosenOptions->decimation));
    rp_AcqSetTriggerDelay(choosenOptions->decimation); // TODO: Calculate the needed time to get sampleCount with defined sampleRate
    rp_AcqSetGain(rp_channel_t(0),rp_pinState_t(choosenOptions->pinState));
    rp_AcqSetGain(rp_channel_t(1),rp_pinState_t(choosenOptions->pinState));
    LOG_F(INFO, "set data, start acquisition now!!");
    rp_AcqStart();
    usleep(100);   //TODO: replace this by proper method like timer

    // set the triggersrc to now so trigger is triggered now!!
    rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
    // wait for the buffer to be completely written
    while(!acquisitionComplete){
      if(previousWritePointer == writePointer)
      {
        acquisitionComplete = true;
      }
      previousWritePointer = writePointer;
    }

    LOG_F(INFO, "Write acquired data to arrays");
    // write the acquired data into the vectors
    // therefore create temp buffer array
    float buffA[choosenOptions->sampleCount];
    float buffB[choosenOptions->sampleCount];
    if(acquisitionComplete){
      acquisitionComplete = false;
      rp_AcqGetOldestDataV(rp_channel_t(0), &choosenOptions->sampleCount, buffA);
      rp_AcqGetOldestDataV(rp_channel_t(1), &choosenOptions->sampleCount, buffB);
      //write data into the vectors
      vector<float> a(buffA, buffA+sizeof buffA / sizeof buffA[0]);
      acquiredDataChannelA = a;
      vector<float> b(buffB, buffB+sizeof buffB / sizeof buffB[0]);
      acquiredDataChannelB = b;
    }

    LOG_F(INFO, "check arrays and return result");
    // check if vectors are filled with data
    if(!acquiredDataChannelA.empty() && !acquiredDataChannelB.empty())
    {
      return true;
    }
    else{
      return false;
    }
  }*/

  /*vector<double> Acquirer::getData(int channel)
  {
    // get data from specified channel. The acquiredDataChannel vectors contain as much valued as defined in choosenOptions.sampleCount
    vector<double> a(acquiredDataChannelA.begin(), acquiredDataChannelA.end());
    vector<double> b(acquiredDataChannelB.begin(), acquiredDataChannelB.end());
    switch (channel) {
      case 0: return a;
      case 1: return b;
      default: return vector<double>();
    }
  }*/
