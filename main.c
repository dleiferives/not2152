#include <stdio.h>
#include <strings.h>
#include <ctype.h>
#include <stdlib.h>

#include "not2152.h"
// not2152 compiler.
// compiler -> front end back end
// front end -> scanner, parser, tokenizer?
// back end -> optimizer, code gen

// first int literals.

// helpers

char isops(char c)
{
	// + -
	switch(c)
	{
		case '+':
		case '-':
		case ';':
			return 1;
	}
	return 0;
}

// tokenizing
int ops_eq(Token *tok, char *op) {
  return memcmp(tok->s, op, tok->len) == 0 && op[tok->len] == '\0';
}	

Token * ops_pass(Token *tok, char *op)
{
	if(!ops_eq(tok,op))
	{printf("ERROR @ %i\n",__LINE__);}
	return tok->next;
}

//int o
	//return op->kind == TOKEN_OPS && (c == op->s[0]);
//}


Token *token_init(Token_kind kind, char * start, char * end)
{
	Token *new_token = calloc(1, sizeof(Token));
	new_token->kind = kind;
	new_token->s = start;
	new_token->len = end - start;
	new_token->next = NULL;
	return new_token;
}

Token * tokenizer(char * input_code)
{
	Token * head = calloc(1,sizeof(Token));
	Token * tokens = head;
	char * code_start = input_code;

	while(*input_code) // while input code is not done!
	{
		putchar(*input_code);
		if(isspace(*input_code))
			{ input_code++; continue;}

		if(isdigit(*input_code))
		{
			tokens->next = token_init(TOKEN_NUM, input_code, input_code);
			tokens = tokens->next;

			char * dig_start = input_code;
			int val = 0;
			while(isdigit(*input_code))
			{
				 val *= 10;
				 val += *input_code -'0';
				 input_code++;
			}
			tokens->len = input_code - dig_start;
			tokens->val = val;
			printf("digit:%i-%ld\n",tokens->val,tokens);
			continue;
		}

		// adding ops
		if(isops(*input_code))
		{
			printf("token ops, %i\n",TOKEN_OPS);
			tokens->next = token_init(TOKEN_OPS,input_code,input_code);
			tokens = tokens->next;
			tokens->len = isops(*input_code); // for multi char ops?

			input_code++;
			continue;
		}

		// throw error
		printf("unknown token %c at %i\n",input_code,code_start - input_code);
		puts("unknown token");
	}

	tokens->next = token_init(TOKEN_EOF, input_code,input_code);
	return head;
}

// parser!

/*typedef enum
{
	NODE_NEG,
	NODE_NUM,
	NODE_VALUE,
	NODE_EX_ADD,
	NODE_EX_SUB,
	NODE_HEAD
}Node_kind;

typedef struct Node Node;
struct Node{
	Node_kind kind; // the kind of node
	Node * ln; // left node 
	Node * rn; // right node
	int val; // for imm nums
};
*/

Node * node_init(Node_kind kind, Node * ln, Node * rn)
{
	Node * result = calloc(1, sizeof(Node));
	result->kind = kind;
	result->ln	 = ln;
	result->rn	 = rn;
	return result;
}

Node * node_ln_init(Node_kind kind, Node * ln)
{
	Node * result = calloc(1, sizeof(Node));
	result->kind = kind;
	result->ln	 = ln;
	return result;
}
	

Node * num_node(int val)
{
	Node * result = calloc(1,sizeof(Node));
	result->kind = NODE_NUM;
	result->val = val;
	return result;
}

Node * value_node(Node_kind kind, Node * ln)
{
	Node * new_node = calloc(1,sizeof(Node));
	new_node->kind = kind;
	new_node->ln = ln;
	return new_node;
}

Node * ex_node(Node_kind kind, Node * ln, Node * rn)
{
	Node * new_node = calloc(1,sizeof(Node));
	new_node->ln = ln;
	new_node->rn = rn;
	new_node->kind = kind;
	return new_node;
}


Node * line(Token ** top, Token * cur)
{
	printf("line %i\n",cur->kind);
	Node * node = node_ln_init(NODE_LINE_EX, ex(&cur,cur));
	*top = ops_pass(cur,";");
	return node;
}

// ex = value (+ ex | - ex)*
Node * ex(Token ** top, Token * cur)
{
	printf("ex %i\n",cur->kind);
	Node * node = value(&cur,cur);

	while(1){
		printf("ex_loop %i\n",cur->kind);
		if (ops_eq(cur, "+"))
		{
			printf("->e+ %i\n",cur->kind);
			node = ex_node(NODE_EX_ADD, node, ex(&cur,cur->next));
			continue;
		}

		if(ops_eq(cur,"-"))
		{
			printf("->e-\n");
			node = ex_node(NODE_EX_SUB,node,ex(&cur,cur->next));
			continue;
		}
		
		*top = cur;
		return node;
	}


}

// value = (- | +) value | num
Node * value(Token ** top, Token * cur)
{
	printf("->value %i\n",cur->kind);

	//( +
	if( ops_eq(cur, "+"))
		{printf("->+\n"); return value(top,cur->next);}

	// | - ) value
	if(ops_eq(cur, "-"))
			{ return value_node(NODE_NEG,value(top,cur->next));}

	// | num
	return num(top,cur);
}

// num;
Node * num(Token ** top, Token * cur)
{
	printf("->num %i\n",cur->kind);
	Node * node = calloc(1,sizeof(Node));
	//printf("cur%i;kind%i\n",cur,cur->kind);
	if(cur->kind == TOKEN_NUM){
		node = num_node(cur->val);
		//printf("\ntop:%ld,sub_test:%i\n",*top,*top - cur->next);
		printf("->%i\n",cur->val);
		*top = cur->next;
	return node;
	}

	return node;
	
	// throw a fat ol error there
}

// line = ex ";"
// ex = value (+ ex | - ex)*
// value = (- | +) value | num
// num;


Node * parser(Token * tokens)
{
	Node * head = node_init(NODE_HEAD,NULL,NULL);
	Node * cur = head;
	int i =0;
	gtokens = tokens;
	while(tokens->next->kind != TOKEN_EOF)
	{
		//printf("%ld token head adress %i - %i\n",tokens,tokens->kind,tokens->val);
		printf("parser %i\n",i++);
		cur->next = line(&(gtokens->next),gtokens->next);	
		cur = cur->next;
	}
	
	return head;
}




// input -
// char = ' ' repeat
// char = 0-9 its a NUM
// char = EOF, its end of file
// char = anything else -> error

// code generation!

void push(void)
{
	printf("sw	x5,	(x2) #push -> x5\n");
	puts(  "addi x2, x2, 4");
}

void pop(void)
{
	printf("lw	x6,	-4(x2)	#pop -> x6\n");
	puts(  "addi x2, x2, -4");
}


void pre_gen(Node *node)
{
	if(node->kind == NODE_LINE_EX)
	{
		tree_gen(node->ln);
		return;
	}

	printf("you should not end here??\n\n");
}

void tree_gen(Node * node)
{
	switch (node->kind)
	{
		case NODE_NEG:
			printf("li	x5, -%i\n",node->val);
			tree_gen(node->ln);
			return;
		case NODE_NUM:
			printf("li	x5,	%i\n",node->val);
			return;
	}

	tree_gen(node->ln);
	push();
	printf("										%ld -- %ld -- %i\n",node->rn,node->ln,node->kind);
	tree_gen(node->rn);
	pop();


	switch(node->kind)
	{
		case NODE_EX_ADD:
			printf("add	x5,	x5,	x6	#x5 <- x5 + x6\n");
			return;
		case NODE_EX_SUB:
			printf("sub x5,	x5,	x6	#x6 <- x5 + x6\n");
			return;
		default:
			printf("you did something wrong to end up here");
	}

	printf("you did something wrong to end up here");
}

void generate_code(Node * node)
{
	printf("li	x2,	0xF000\n");

	Node * node_it = node;
	while(node_it)
	{
		printf("-------- %ld----- %ld-----",node_it,node_it->next);
		puts("loop_start");
		pre_gen(node_it);
		printf("-------- %ld----- %ld-----",node_it,node_it->next);
		node_it = node_it->next;
	}

}


int main(void)
{
	char * a = " 3+3-3+393993; 90+90;";
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
