#pragma once
#include <vector>
#include <string>
#include <ostream>
#include <set>
#include <unordered_map>
#include <map>
#include <algorithm>
#include "helper.h"

using namespace std;

const char
EPSILON = '\xEE';

class Variable;

typedef vector<Variable*> vec_variable;

class Variable
{
public:
	string name;
	bool isTerminal;
	string terminal;
	vector<vec_variable> productions;
	string str_productions;

	Variable ( const Variable& );

	Variable ( char, char );

	Variable ( string, char );

	Variable ( string );

	Variable ( char );

	Variable ( string, vector<vec_variable> );

	void addProductions ( vector<vec_variable> );

	void removeProductionsContaining ( Variable* );

	void removeEpsilon ( );

	bool hasEpsilon ( );

	void build_str_productions ( );

	void consume ( Variable* );

	void replaceAll ( vec_variable, vec_variable );
};