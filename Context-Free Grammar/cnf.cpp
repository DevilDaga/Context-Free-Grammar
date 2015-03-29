#include "cnf.h"

CNF::CNF ( CFG cfg ) : CFG ( cfg )
{
	map<vec_variable, Variable*> map_replace;
	unordered_map<string, Variable*> um_vars;
	for each ( auto v in V )
		um_vars[ v->name ] = v;
	LOOP ( i, V.size ( ) )
	{
		auto v = V[ i ];
		LOOP ( j, v->productions.size ( ) )
		{
			auto production = v->productions[ j ];
			if ( production.size ( ) <= 2 )
				continue;
			auto updated = production;
			updated.erase ( updated.begin ( ) + 1, updated.end ( ) );
			string name = increment ( v->name );
			while ( um_vars.find ( name ) != um_vars.end ( ) )
				name = increment ( name );
			auto vPrime = new Variable ( name );
			um_vars[ name ] = vPrime;
			V.push_back ( vPrime );
			production.erase ( production.begin ( ) );
			vPrime->productions.push_back ( production );
			vPrime->build_str_productions ( );
			updated.push_back ( vPrime );
			v->productions[ j ] = updated;
		}
		v->build_str_productions ( );
	}
	sort_V ( );
	toString ( );
}

string CNF::toString ( )
{
	if ( my_string.length ( ) )
		return my_string;
	size_t longest = 0;
	for each ( auto v in V )
		longest = max ( longest, v->name.length ( ) );
	for each ( auto v in V )
	{
		my_string += v->name +
					string ( longest - v->name.length ( ), ' ' ) +
					" -> ";
		if ( v->isTerminal )
		{
			my_string += v->terminal + "\n";
			continue;
		}
		for each ( auto production in v->productions )
		{
			for each ( auto u in production )
				my_string += u->name;
			my_string.push_back ( '|' );
		}
		my_string.pop_back ( );
		my_string.push_back ( '\n' );
	}
	return my_string;
}
