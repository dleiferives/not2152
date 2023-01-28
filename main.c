#include <stdio.h>
#include <strings.h>
#include <ctype.h>
#include <stdlib.h>
#include "not2152.h"
// not2152 compiler.
//
// compiler -> front end back end
// front end -> scanner, parser, tokenizer?
// back end -> optimizer, code gen

// first int literals.

// helpers






int main(void)
{
	global = calloc(1,sizeof(Object));
	char * a = " a = 9; b = a; c = a + b;";
	Token *tokens = tokenizer(a);
	/*while(tokens->next != NULL)
	{
		printf("token %i - %i \n",tokens->next->val,tokens->next->kind);
		tokens->next = tokens->next->next;
	}*/

	Program * prog= parser(tokens);
	generate_code(prog);

	return 0;
}
