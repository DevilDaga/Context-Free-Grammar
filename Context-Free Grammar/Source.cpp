#include "gnf.h"
#include "cnf.h"
#include <iostream>

using namespace std;

int main ( )
{
	string toParse = "";
	//toParse = "S->ASBS|BSAS|AB|BA|C\nA->a|bAA\nB->b|aBB\nC->CA|CB|CS";
	//toParse = "S->A\nA->BbB|Cc\nB->b|c\nC->cc";
	//toParse = "S->ABCABACBACBD\nA->aaE\nB->bcE\nC->caE\nD->E\nE->eE|e";
	//toParse = "S->Ba\nB->Bb|EPSILON";
	//toParse = "S->x|y|z|S+S|S-S|S*S|S/S|(S)";
	if ( true )
	{
		cout << "Enter the CFG: ( -1 to stop )" << endl;
		string line = "";
		toParse = "";
		while ( true )
		{
			getline ( cin, line );
			if ( line == "-1" )
				break;
			toParse.append ( line );
			toParse.push_back ( '\n' );
		}
		toParse.pop_back ( );
	}
	CFG cfg ( toParse );
	GNF gnf ( cfg );
	CNF cnf ( cfg );
	cout << "CFG:\n" << cfg << endl
		<< "CNF:\n" << cnf << endl
		<< "GNF:\n" << gnf << endl;
	cin.ignore ( 2 );
	return 0;
}