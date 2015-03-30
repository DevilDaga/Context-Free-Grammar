#include "cfg.h"
#include "helper.h"
#include <algorithm>

CFG::CFG ( const CFG& cfg )
{
	R = cfg.R;
	unordered_map<Variable*, Variable*> um_vars;
	for each ( auto v in cfg.V )
	{
		auto v_new = new Variable ( *v );
		um_vars[ v ] = v_new;
		V.push_back ( v_new );
	}
	for each ( auto v in V )
	{
		vector<vec_variable> new_productions;
		for each ( auto production in v->productions )
		{
			for ( size_t i = 0; i != production.size ( ); ++i )
				production[ i ] = um_vars[ production[ i ] ];
			new_productions.push_back ( production );
		}
		v->productions = new_productions;
	}
	S = um_vars[ cfg.S ];
}

CFG::CFG ( string parse )
{
	replaceAll ( parse, "EPSILON", string ( 1, EPSILON ) );
	auto lines = this->R = split ( parse, '\n' );
	size_t max_var_length = 0;
	unordered_map<string, Variable*> um_vars;
	for each ( auto line in lines )
	{
		auto index = line.find_first_of ( "-" );
		max_var_length = max ( max_var_length, index );
		string name = line.substr ( 0, index );
		um_vars[ name ] = new Variable ( name );
	}
	for each ( auto line in lines )
	{
		string trimmed = line.substr ( line.find_first_of ( ">" ) + 1 );
		auto productions = split ( trimmed, '|' );
		vector<vec_variable> cur_var_productions;
		for each ( auto production in productions )
		{
			vec_variable cur_production;
			size_t j = 0;
			for ( ; j < production.length ( ); ++j )
			{
				auto i = min ( max_var_length, production.length ( ) - j );
				for ( ; i != 0; --i )
				{
					string temp = production.substr ( j, i );
					auto it = um_vars.find ( temp );
					if ( it != um_vars.end ( ) )
					{
						cur_production.push_back ( it->second );
						j += i - 1;
						break;
					}
				}
				if ( !i )			// Terminal..
				{
					string original = production.substr ( j, 1 );
					string name = toUpper ( original );
					if ( name[ 0 ] == EPSILON )
						name = "EPSILON";
					if ( !isalpha ( name[ 0 ] ) )
					{
						name[ 0 ] = 'A';
						while ( um_vars.find ( name ) != um_vars.end ( ) )
							name[ 0 ]++;
					}
					Variable *term = nullptr;
					while ( um_vars.find ( name ) != um_vars.end ( ) )
					{
						if ( um_vars[ name ]->terminal == original )
						{
							term = um_vars[ name ];
							break;
						}
						name = increment ( name );
					}
					if ( !term )
						term = new Variable ( name, production[ j ] );
					um_vars[ name ] = term;
					cur_production.push_back ( term );
				}
			}
			cur_var_productions.push_back ( cur_production );
		}
		auto index = line.find_first_of ( "-" );
		string name = line.substr ( 0, index );
		um_vars[ name ]->addProductions ( cur_var_productions );
	}
	for each ( auto kv in um_vars )
		V.push_back ( kv.second );
	S = um_vars[ "S" ];
	remove_unreachable ( );
	remove_unit ( );
	remove_unproductive ( );
	remove_epsilon ( );
	sort_V ( );
}

void CFG::remove_unreachable ( )
{
	unordered_map<Variable*, bool> um_vars;
	for each ( auto v in V )
		um_vars[ v ] = false;
	vec_variable queue;
	queue.push_back ( S );
	um_vars[ S ] = true;
	for ( size_t i = 0; i != queue.size ( ); ++i )
	{
		auto v = queue[ i ];
		for each ( auto production in v->productions )
		{
			for each ( auto u in production )
			{
				if ( !um_vars[ u ] )
				{
					um_vars[ u ] = true;
					queue.push_back ( u );
				}
			}
		}
	}
	V.clear ( );
	for each ( auto kv in um_vars )
		if ( kv.second )
			V.push_back ( kv.first );
}

void CFG::remove_unit ( )
{
	unordered_map<Variable*, set<Variable*>> um_dependencies;
	for each ( auto v in V )
		um_dependencies[ v ].empty ( );
	for each ( auto v in V )
		for each ( auto production in v->productions )
			for each ( auto u in production )
				um_dependencies[ u ].insert ( v );
	vec_variable updated;
	for each ( auto v in V )
	{
		if ( v->productions.size ( ) == 1 )		// Should I check all productions?
		{
			if ( v->productions[ 0 ].size ( ) == 1 )
			{
				auto u = v->productions[ 0 ][ 0 ];
				if ( u == v )					// Looks stupid..  whatever
					break;
				vec_variable
					from = { v },
					to = { u };
				//for each ( auto w in um_dependencies[ v ] )
				//	w->replaceAll ( from, to );
				v->productions = u->productions;
			}
			else
				updated.push_back ( v );
		}
		else
			updated.push_back ( v );
	}
	V = updated;
}

void CFG::remove_unproductive ( )
{
	unordered_map<Variable*, set<Variable*>> um_dependencies;
	for each ( auto v in V )
		um_dependencies[ v ].empty ( );
	for each ( auto v in V )
		for each ( auto production in v->productions )
			for each ( auto u in production )
				um_dependencies[ u ].insert ( v );
	unordered_map<Variable*, bool> um_vars;
	for each ( auto v in V )
	{
		um_vars[ v ] = v->isTerminal;
		for each ( auto production in v->productions )
			um_vars[ v ] |= !contains ( production, v );
	}
	V.clear ( );
	for each ( auto kv in um_vars )
	{
		if ( kv.second )
			V.push_back ( kv.first );
		else
			for each ( auto v in um_dependencies[ kv.first ] )
				v->removeProductionsContaining ( kv.first );
	}
}

void CFG::remove_epsilon ( )
{
	unordered_map<Variable*, set<Variable*>> um_dependencies;
	Variable *var_eps = nullptr;
	size_t index_eps = 0;
	for each ( auto v in V )
	{
		um_dependencies[ v ].empty ( );
		if ( v->name == "EPSILON" )
			var_eps = v;
		if ( !var_eps )
			++index_eps;
	}
	for each ( auto v in V )
		for each ( auto production in v->productions )
			for each ( auto u in production )
				um_dependencies[ u ].insert ( v );
	for each ( auto v in um_dependencies[ var_eps ] )
	{
		v->removeEpsilon ( );
		if ( v == S )
			continue;
		for each ( auto depend in um_dependencies[ v ] )
		{
			vector<vec_variable> all_expanded;
			for each ( auto production in depend->productions )
			{
				vector<vec_variable> expanded;
				auto c = count ( production, v );
				if ( !c )
					continue;
				auto exp = 1ULL << c;
				auto bitmask = 0ULL;
				for ( ; bitmask != exp; ++bitmask )
				{
					vec_variable cur;
					size_t j = 0;
					LOOP ( i, production.size ( ) )
					{
						if ( production[ i ] != v )
							cur.push_back ( production[ i ] );
						else
						{
							if ( bitmask & ( 1ULL << j ) )
								cur.push_back ( v );
							++j;
						}
					}
					expanded.push_back ( cur );
				}
				all_expanded = append ( all_expanded, expanded );
			}
			depend->removeProductionsContaining ( v );
			depend->addProductions ( all_expanded );
		}
	}
	if ( var_eps )
		V.erase ( V.begin ( ) + index_eps );
}

void CFG::sort_V ( )
{
	vec_variable sorted;
	sorted.push_back ( S );
	set<Variable*> set_vars;
	set_vars.insert ( S );
	LOOP ( i, sorted.size ( ) )
	{
		auto v = sorted[ i ];
		for each ( auto production in v->productions )
		{
			for each ( auto u in production )
			{
				auto ret = set_vars.insert ( u );
				if ( ret.second )
					sorted.push_back ( u );
			}
		}
	}
	V = sorted;
}

string CFG::parse ( string str )
{
	string result;

	return result;
}

string CFG::toString ( )
{
	if ( my_string.length ( ) )
		return my_string;
	size_t longest = 0;
	for each ( auto v in V )
		longest = max ( longest, v->name.length ( ) );
	for each ( auto v in V )
		if ( !v->isTerminal )
			my_string += v->name +
						string ( longest - v->name.length ( ), ' ' ) +
						" -> " + v->str_productions + "\n";
	return my_string;
}