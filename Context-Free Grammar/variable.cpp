#include "variable.h"

Variable::Variable ( const Variable& var )
{
	this->name = var.name;
	this->terminal = var.terminal;
	this->isTerminal = var.isTerminal;
	this->str_productions = var.str_productions;
	this->productions = var.productions;
}

Variable::Variable ( char name, char terminal )
{
	this->name = string ( 1, name );
	this->terminal = string ( 1, terminal );
	this->isTerminal = true;
	str_productions = terminal;
}

Variable::Variable ( string name, char terminal )
{
	this->name = name;
	this->terminal = string ( 1, terminal );
	this->isTerminal = true;
	str_productions = terminal;
}

Variable::Variable ( string name )
{
	this->name = name;
	this->isTerminal = false;
	this->terminal = "\0";
	str_productions = terminal;
}

Variable::Variable ( char name )
{
	this->name = string ( 1, name );
	this->isTerminal = false;
	this->terminal = "\0";
	str_productions = terminal;
}

Variable::Variable ( string name, vector<vec_variable> productions )
{
	this->name = name;
	this->isTerminal = false;
	this->terminal = "\0";
	this->productions = productions;
	build_str_productions ( );
}

void Variable::addProductions ( vector<vec_variable> productions )
{
	this->productions = append ( this->productions, productions );
	build_str_productions ( );
}

void Variable::removeProductionsContaining ( Variable *v )
{
	LOOP ( i, productions.size ( ) )
	{
		if ( contains ( productions[ i ], v ) )
		{
			productions.erase ( productions.begin ( ) + i );
			--i;
		}
	}
	build_str_productions ( );
}

void Variable::removeEpsilon ( )
{
	LOOP ( i, productions.size ( ) )
	{
		if ( productions[ i ][ 0 ]->name == "EPSILON" )
		{
			productions.erase ( productions.begin ( ) + i );
			break;
		}
	}
	build_str_productions ( );
}

bool Variable::hasEpsilon ( )
{
	for each ( auto production in productions )
		if ( production[ 0 ]->name == "EPSILON" )
			return true;
	return false;
}

void Variable::build_str_productions ( )
{
	str_productions = "";
	if ( isTerminal )
	{
		str_productions = terminal;
		return;
	}
	for each ( auto production in productions )
	{
		for each ( auto var in production )
			str_productions.append ( var->isTerminal ?
			var->terminal :
			var->name );
		str_productions.push_back ( '|' );
	}
	if ( str_productions.length ( ) )
		str_productions.pop_back ( );
}

void Variable::consume ( Variable *var )
{
	vector<vec_variable> expanded = productions;
	set<vec_variable> updated ( productions.begin ( ), productions.end ( ) );
	LOOP ( i, expanded.size ( ) )
	{
		auto production = expanded[ i ];
		updated.insert ( production );
		if ( production[ 0 ] == var )
		{
			updated.erase ( production );
			production.erase ( production.begin ( ) );
			vec_variable backup = production;
			for each ( auto sub_prod in var->productions )
			{
				production = backup;
				production.insert ( production.begin ( ),
									sub_prod.begin ( ),
									sub_prod.end ( )
									);
				expanded.push_back ( production );
			}
			//--i;		// Left recursion will cause an infinite loop!
			/*
			 *
			 S->Abc|d
			 A->Ad|a
			 S->consume(A);
			 S->Adbc|abc|d
			 S->consume(A);
			 S->Addbc|adbc|abc|d
			 */
		}
	}
	productions.clear ( );
	productions.insert ( productions.begin ( ),
						 updated.begin ( ),
						 updated.end ( )
						 );
	build_str_productions ( );
}

void Variable::replaceAll ( vec_variable from, vec_variable to )
{
	vector<vec_variable> updated;
	for each ( auto production in productions )
	{
		while ( true )
		{
			auto it = search ( production.begin ( ),
							   production.end ( ),
							   from.begin ( ),
							   from.end ( )
							   );
			if ( it == production.end ( ) )
			{
				updated.push_back ( production );
				break;
			}
			it = production.erase ( it, it + from.size ( ) );
			production.insert ( it,
								to.begin ( ),
								to.end ( )
								);
		}
	}
	productions = updated;
	build_str_productions ( );
}
