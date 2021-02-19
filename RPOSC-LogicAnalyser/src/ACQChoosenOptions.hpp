using namespace std;
#include <vector>
#include <string>

class ACQChoosenOptions{
  public:
    ACQChoosenOptions();
    int TranslateDecimation(string decimation);
    int TranslateSampleRate(string sampleRate);
    int TranslatePinState(string pinState);
    int TranslateSampleCount(string count);
};
