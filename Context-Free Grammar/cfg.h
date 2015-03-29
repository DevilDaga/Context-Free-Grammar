#pragma once
#include "variable.h"

class CFG
{

	string my_string;

public:
	vector<string> R;
	Variable *S;
	vec_variable V;

	CFG ( const CFG& );

	CFG ( string );

	void remove_unreachable ( );

	void remove_unit ( );

	void remove_unproductive ( );

	void remove_epsilon ( );

	void sort_V ( );

	string toString ( );

	inline friend std::ostream& operator<<( std::ostream &os, CFG& cfg )
	{
		return os << cfg.toString ( );
	}
};