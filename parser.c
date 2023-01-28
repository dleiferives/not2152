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

Object *global;

Object *search_object(Object * o, char * name)
{
	while(o)
	{
		if(o){
			if(o->name){
				if(strcmp(name,o->name) == 0) {
					return o;
				}
			}
		}
		o = o->next;
	};
	return NULL;
}

Object * lvar_object(char * name, int name_len, int size)
{
	char * temp = calloc(name_len+1,sizeof(char));
	for(int i =0; i<name_len; i++)
		{temp[i] = name[i]; }	
	temp[name_len] = '\0';

	Object *result = search_object(global,temp);
	if(result != NULL)
		{return result;}
	result = calloc(1,sizeof(Object));
	result->name = temp;	
	result->size = size;
	result->offset = global->offset;
	global->offset += size;
	result->next = global->next;
	global->next = result;
	return result;
}




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

Node * var_node(char * name)
{
	Node * new_node = calloc(1,sizeof(Node));
	new_node->kind = NODE_VAR;
	new_node->name = name;
	return new_node;
}



Node * line(Token ** top, Token * cur)
{
	printf("line %i\n",cur->kind);
	Node * node = node_ln_init(NODE_LINE_EX, assign(&cur,cur));
	*top = ops_pass(cur,";");
	return node;
}

// assign = var '=' ex
Node * assign(Token ** top, Token * cur)
{
	Node * node = ex(&cur,cur);
	if(ops_eq(cur,"="))
	{
		node = node_init(NODE_ASSIGN, node,	assign(&cur,cur->next));
	}
	*top = cur;
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
	//printf("cur%i;kind%i\n",cur,cur->kind);
	if(cur->kind == TOKEN_NUM){
		Node *node = num_node(cur->val);
		//printf("\ntop:%ld,sub_test:%i\n",*top,*top - cur->next);
		printf("->%i\n",cur->val);
		*top = cur->next;
	return node;
	}

	if(cur->kind == TOKEN_IDENT)
	{
		puts("LKDJFS");
		Node * node = var_node(cur->s);
		Object *var = lvar_object(cur->s,cur->len,4);
		node->var = var;

		// create a local variable 
		// size of 4 
		// name is string?
		//
		*top = cur->next;
		return node;
	}

	Node * node = calloc(1,sizeof(Node));
	return node;
	
	// throw a fat ol error there
}

// line = ex ";"
// ex = value (+ ex | - ex)*
// value = (- | +) value | num
// num;


Program * parser(Token * tokens)
{
	Program * prog = calloc(1,sizeof(Program));
	Node * cur = prog->tree = calloc(1,sizeof(Node));

	int i =0;
	gtokens = tokens;
	while(tokens->next->kind != TOKEN_EOF)
	{
		//printf("%ld token head adress %i - %i\n",tokens,tokens->kind,tokens->val);
		printf("parser %i\n",i++);
		cur->next = line(&(gtokens->next),gtokens->next);	
		cur = cur->next;
	}
	
	// finding how big the stack is 
	int stack_size = 0;
	for(Object *cur = global; cur; cur = cur->next)
		{ stack_size += cur->size; }

	prog->offset = stack_size;
	prog->scope = global;	
	prog->tree = prog->tree->next;
	return prog;
}
