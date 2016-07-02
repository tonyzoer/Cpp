#include "headers.h"
#include "math.h"

double got_num;
unsigned short base = 10;
char name_str[255];
TYPES curr_type;
char comments[300];

void comment_getter(char end_symbol)
{
	static int i = 0;
	do
	{
		cin.get(comments[i]);
	} while (comments[i++] != end_symbol&&i<299);
	cin.get(comments[i]);
	if (comments[i] != '/'&&comments[i - 1] != '\n') comment_getter(end_symbol);
	else cin.putback(comments[i]);
	comments[i] = '\n';
}

TYPES parser()
{
	char c;
	do
	{
		if (!cin.get(c)) return curr_type = END;
	} while (c != '\n' && isspace(c));

	switch (c)
	{
	case '*':	cin >> c;
		if (c == '*')
			return curr_type = POWER;
		else
		{
			cin.putback(c);
			return curr_type = MULTIPLY;
		}
	case '/':
	{
		cin >> c;
		if (c == '/')
			comment_getter('\n');
		else
			if (c == '*')
				comment_getter(c);
			else
			{
				cin.putback(c);
				return curr_type = DIV;
			}
		return curr_type = parser();
	}
	case ';':
	case '\n':
		return curr_type = PRINT;
	case '+':
	case '-':
	case '^':
	case '(':
	case ')':
	case '=':
		return curr_type = TYPES(c);
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '.': cin.putback(c);
		cin >> got_num;
		return curr_type = NUMBER;

	default: if (isalpha(c))
	{
		char* p = name_str;
		*p++ = c;
		while (cin.get(c) && isalnum(c)) *p++ = c;
		*p = 0;
		cin.putback(c);
		if (strcmp(name_str, "END") == 0)
			return curr_type = END;
		if (strcmp(name_str, "DEBUG") == 0)
		{
			silent = false;
			return curr_type = COMMAND;
		}
		if (strcmp(name_str, "SILENT") == 0)
		{
			silent = true;
			return curr_type = COMMAND;
		}
		if (strcmp(name_str, "HELP") == 0)
			cout << "END - terminate program." << endl << "DEBUG - debug mode." << endl << "SILENT - normal mode." << endl;
		return curr_type = NAME;
	}
			 error("bad token.");
			 return curr_type = PRINT;
	}

}

double mult()
{
	double left = powd();
	for (;;)
		switch (curr_type) {
		case MULTIPLY:
			parser();
			left *= powd();
			break;
		case DIV:
		{parser();
		int d = powd();
		if (d == 0)
		{
			error("Division by zero.");
			return left;
		}
		left /= d;
		break; }
		default: return left;
	}
}

double powd()
{
	double left = count();
	for (;;)
		switch (curr_type)
	{
		case POWER:
		{
			parser();
			left = pow(left, powd());
			break;
		}
		default: return left;
	}
}

double sum()
{
	double left = mult();
	for (;;)
		switch (curr_type)
	{
		case PLUS:
			parser();
			left += mult();
			break;
		case MINUS:
			parser();
			left -= mult();
			break;
		default: return left;
	}
}

char no_string;

double count()
{
	switch (curr_type) {
	case NUMBER:
		parser();
		return got_num;
	case MINUS:
		parser();
		return -count();
	case OPEN:
		{parser();
		int res = sum();
		if (curr_type != CLOSE) return error(") expected.");
		parser();
		return res; }
	case NAME:
	{
		if (parser() == RESULT) {
			name* n = insert(name_str);
			parser();
			n->value = sum();
			return n->value;
		}
		return look(name_str)->value;
		break; 
	}
	case COMMAND:
		return 1;
	case END: return 1;
	default:
		return error("Value expected.");
	}
	return error("Value expected.");
}
