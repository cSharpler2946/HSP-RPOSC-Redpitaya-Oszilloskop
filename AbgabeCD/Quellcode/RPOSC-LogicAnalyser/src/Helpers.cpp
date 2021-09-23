#include "Helpers.hpp"

/** \brief Converst a json element to a string, stripping unneccessary ""
*/
std::string Helpers::JSONtoString(nlohmann::json j)
{
    if(j.is_string()) 
    {
        std::string ret = j.dump();
        ret.erase(0, 1);
        ret.erase(ret.length()-1, 1);
        return ret;
    }
    else
    {
        return j.dump();
    }
}