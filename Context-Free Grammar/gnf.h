#pragma once
#include "cfg.h"

class GNF	:	public CFG
{

	string my_string;

public:

	GNF ( CFG );

	bool lemma1 ( );

	bool lemma2 ( );

	string toString ( );

	inline friend std::ostream& operator<<( std::ostream &os, GNF& gnf )
	{
		return os << gnf.toString ( );
	}
};