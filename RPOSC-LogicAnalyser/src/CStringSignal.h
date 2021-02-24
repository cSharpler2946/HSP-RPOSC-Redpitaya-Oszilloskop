#pragma once

#include <CustomParameters.h>
#include <string>

//TODO: Class has to be tested!


class CStringSignal : public CCustomSignal<std::string>
{
public:
	CStringSignal(std::string _name, int _size, std::string _def_value)
		:CCustomSignal(_name, _size, _def_value){};

	CStringSignal(std::string _name, CBaseParameter::AccessMode _access_mode, int _size, std::string _def_value)
		:CCustomSignal(_name, _access_mode, _size, _def_value){};
};