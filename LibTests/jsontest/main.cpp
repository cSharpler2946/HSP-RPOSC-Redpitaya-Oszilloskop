#include <iostream>
#include <nlohmann/json.hpp>
#include <list>

using namespace std;
using namespace nlohmann;

/*
 * This program tests the conversion from c++ map to json objects and back.
 * Json Library used: https://github.com/nlohmann/json
 */
int main()
{
    map<string, string> newOptions = {{"data_edge", "rising"}, {"divider", "5"}};

    json j;
    j["type"] = "options";
    j["data"] = newOptions;

    cout << j << endl;

    cout << j["data"]["data_edge"] << endl;

    list<string> list = {"rising", "falling", "both"};
    j["testlist"] = list;

    cout << j << endl;

    //Testing building a data stream
    uint8_t inbuf[1024];
    srand(time(nullptr));
    for(int i = 0; i<1024; i++)
    {
        inbuf[i]=rand() % 255;
    }

    json d;
    d["type"] = "data_stream";
    d["data"] = inbuf;

    cout << d << endl;

    //Test building the options object
    json opt;
    opt["type"] = "options";
    json ele;
    ele["id"] = "data_edge";
    ele["def"] = "rising";
    ele["desc"] = "On which edge should be counted?";
    ele["values"] = list;

    opt["data"] += ele;

    ele["id"] = "divider";
    ele["def"] = "0";
    ele["desc"] = "Which divider?";
    ele["values"] = {"0", "1", "2", "3"};

    opt["data"] += ele;

    cout << ele["id"].dump() << endl;

    cout << opt << endl;

    //Test for channelMap
    string s = "{\"CH_A\":\"rx\",\"CH_B\":\"tx\"}";
    json cm = json::parse(s);

    for (auto& el : cm.items())
    {
        std::cout << "key: " << el.key() << ", value:" << el.value() << '\n';
    }



    return 0;
}