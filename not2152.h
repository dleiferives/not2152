#ifndef NOT2152_H
#define NOT2152_H

// tokenizer!
typedef enum{
	TOKEN_NUM,
	TOKEN_EOF,
	TOKEN_OPS
}Token_kind;

typedef struct Token Token;
struct Token{
	Token_kind kind;
	char * s; // the string that represents this token
	int len; // the length of the string
	int val; // if this is a number its value
	Token *next;
};

Token * gtokens;


// parser!
typedef enum
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

Node * node_init(Node_kind , Node * , Node * );
Node * value_node(Node_kind, Node*);
Node * value(Token ** , Token * );
Node * num(Token ** , Token * );
#endif
