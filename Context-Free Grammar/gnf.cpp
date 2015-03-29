#include "gnf.h"
#include "helper.h"

GNF::GNF ( CFG cfg ) : CFG ( cfg )
{
	my_string = "";
	while ( true )
	{
		if ( lemma2 ( ) )
			continue;
		if ( lemma1 ( ) )
			continue;
		break;
	}
	sort_V ( );
	toString ( );
}

bool GNF::lemma1 ( )
{
	bool result = false;
	set<Variable*> valid_vars ( V.begin ( ), V.end ( ) );
	for each ( auto v in V )
	{
		if ( valid_vars.find ( v ) == valid_vars.end ( ) )
			continue;
		LOOP ( j, v->productions.size ( ) )
		{
			auto production = v->productions[ j ];
			auto left_most = production.front ( );
			if ( !left_most->isTerminal &&
				 v != left_most &&
				 left_most != S )
			{
				result = true;
				v->consume ( left_most );
				valid_vars.erase ( left_most );
				--j;
			}
		}
	}
	remove_unreachable ( );
	remove_unproductive ( );
	return result;
}

bool GNF::lemma2 ( )
{
	bool result = false;
	unordered_map<string, Variable*> um_vars;
	for each ( auto v in V )
		um_vars[ v->name ] = v;
	LOOP ( i, V.size ( ) )
	{
		auto v = V[ i ];
		vector<vec_variable> alpha, beta;
		LOOP ( j, v->productions.size ( ) )
		{
			auto production = v->productions[ j ];
			auto left_most = production.front ( );
			( left_most == v ? alpha : beta ).push_back ( production );
		}
		if ( !alpha.size ( ) )
			continue;
		LOOP ( j, alpha.size ( ) )
			alpha[ j ].erase ( alpha[ j ].begin ( ) );
		result = true;
		string name = increment ( v->name );
		while ( um_vars.find ( name ) != um_vars.end ( ) )
			name = increment ( name );
		auto vPrime = new Variable ( name );
		um_vars[ name ] = vPrime;
		V.insert ( V.begin ( ) + i, vPrime );
		v->productions.clear ( );
		v->productions = beta;
		for each ( auto it_beta in beta )
		{
			auto temp = it_beta;
			temp.push_back ( vPrime );
			v->productions.push_back ( temp );
		}
		vPrime->productions = alpha;
		for each ( auto it_alpha in alpha )
		{
			auto temp = it_alpha;
			temp.push_back ( vPrime );
			vPrime->productions.push_back ( temp );
		}
		v->build_str_productions ( );
		vPrime->build_str_productions ( );
	}
	return result;
}

string GNF::toString ( )
{
	if ( my_string.length ( ) )
		return my_string;
	string result = "";
	size_t longest = 0;
	unordered_map<Variable*, bool> um_vars;
	for each ( auto v in V )
	{
		um_vars[ v ] = false;
		longest = max ( longest, v->name.length ( ) );
	}
	um_vars[ S ] = true;
	for each ( auto v in V )
	{
		if ( !um_vars[ v ] )
			continue;
		result += v->name +
			string ( longest - v->name.length ( ), ' ' ) +
			" -> ";
		if ( v->isTerminal )
			result.append ( v->terminal + " " );
		for each ( auto production in v->productions )
		{
			result.append ( production[ 0 ]->terminal );
			for ( size_t i = 1; i != production.size ( ); ++i )
			{
				result.append ( production[ i ]->name );
				um_vars[ production[ i ] ] = true;
			}
			result.push_back ( '|' );
		}
		result.pop_back ( );
		result.push_back ( '\n' );
	}
	my_string = result;
	return result;
}
