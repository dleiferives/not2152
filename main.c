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
	char * a = " a = 3+3-3+393993; 90+90;";
	Token *tokens = tokenizer(a);
	/*while(tokens->next != NULL)
	{
		printf("token %i - %i \n",tokens->next->val,tokens->next->kind);
		tokens->next = tokens->next->next;
	}*/

	Node * nodes = parser(tokens);
	generate_code(nodes);

	return 0;
}
