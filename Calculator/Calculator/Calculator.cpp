#include "headers.h"



int error_count;
bool silent = true;
const int TBLSZ = 23;
name* table[TBLSZ];

void init()
{
	memset(table, 0, sizeof(table));
}

name* look(const char* p, int ins)
{
	int ii = 0;									//hash
	const char* pp = p;
	while (*pp) ii = ii << 1 ^ *pp++;
	if (ii < 0) ii = -ii;
	ii %= TBLSZ;

	for (name* n = table[ii]; n; n = n->next)		//search
		if (strcmp(p, n->string) == 0) return n;

	if (ins == 0) error("name not found.");

	name* nn = new name;
	nn->string = new char[strlen(p) + 1];
	//strcpy(nn->string, p);
	//strncpy(nn->string, p, sizeof(nn->string) - 1);
	//nn->string = p;
	nn->value = 0;
	nn->next = table[ii];
	table[ii] = nn;
	return nn;

}

inline name* insert(const char* s) { return look(s, 1); }

int error(const char* err)
{
	if (!silent)
		cerr << "Error: " << err << "Valid part of the expression will be counted." << '\n';
	error_count++;
	return 1;
}

int main(int argc, char * const argv[])
{
	init();
	insert("pi")->value = 3.14;
	insert("e")->value = 2.7;
	cout << '>';
	while (cin)
	{
		parser();
		if (curr_type == END) break;
		if (curr_type == PRINT || curr_type == COMMAND) continue;
		cout << sum() << '\n';
		cout << '>';
	}
	if (comments[0] != NULL)
		cout << "Comments:" << endl << comments;

	return error_count;
}


