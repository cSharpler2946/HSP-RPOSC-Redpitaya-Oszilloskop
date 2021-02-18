#include<string>

class SPContainer {
    public:
        virtual std::string toJsonString()=0;
        virtual void fromJsonString(std::string)=0;
        virtual void OnNew()=0;
        virtual void OnNewInternal()=0;
        virtual void Update()=0;
};