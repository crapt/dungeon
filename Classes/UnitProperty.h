#pragma once

#include <csvpp/csv.h>
#include <csvpp/csv_string.h>
#include <csvpp/base_object.h>

class UnitProperty
{
public:

	int				_uid;
	std::string		_name;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int) {
		ar & _uid;
		ar & _name;
	}
};

