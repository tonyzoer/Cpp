#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>

using namespace std;
enum TYPES {
	NUMBER,
	PLUS = '+',
	MINUS = '-',
	DIV = '/',
	POWER = '^',
	MULTIPLY = '*',
	RESULT = '=',
	PRINT = ';',
	END,
	OPEN = '(',
	CLOSE = ')',
	NAME,
	NOTHING,
	COMMAND
};

struct name{
	char* string;
	name* next;
	double value;
};

TYPES parser();
double count();
double sum();
double mult();
double powd();
int error(const char* err);
extern TYPES curr_type;
extern unsigned short base;
int buf();
void parseback(int p_what);
extern string res;
name* look(const char* p, int ins = 0);
extern char comments[300];
extern bool silent;
inline name* insert(const char* s);
