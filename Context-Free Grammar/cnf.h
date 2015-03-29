#pragma once
#include "cfg.h"

class CNF	:	public CFG
{

	string my_string;

public:

	CNF ( CFG );

	string toString ( );

	inline friend std::ostream& operator<<( std::ostream &os, CNF& cnf )
	{
		return os << cnf.toString ( );
	}
};

