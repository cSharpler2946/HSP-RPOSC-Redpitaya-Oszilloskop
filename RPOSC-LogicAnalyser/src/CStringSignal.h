#pragma once

#include <CustomParameters.h>
#include <string>

using namespace std;

//TODO: Class has to be tested!

class CStringSignal : public CCustomSignal<string>
{
public:
	CStringSignal(std::string _name, int _size, string _def_value)
		:CCustomSignal(_name, _size, _def_value){};

	CStringSignal(std::string _name, CBaseParameter::AccessMode _access_mode, int _size, string _def_value)
		:CCustomSignal(_name, _access_mode, _size, _def_value){};
};