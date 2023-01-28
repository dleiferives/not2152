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

int get_adress(Node * node)
{
	if(node->kind != NODE_VAR)
		error("supposed to be a variable");
	
	return node->var->offset + STACK_START_POINTER;
}


void pre_gen(Node *node)
{
	if(node->kind == NODE_LINE_EX)
	{
		tree_gen(node->ln);
		return;
	}

	error("unknown node");
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
		case NODE_ASSIGN:
			// get memory adress of ln
			printf("li	x5,	0x%X\n",get_adress(node->ln));
			// store value of rn to present register
			push();
			tree_gen(node->rn);
			// store register to memory adress of ln
			pop(); // pop the pointer to the memory adress off the stack!
			printf("sw	x5,	0(x6)\n");

			return;
		case NODE_VAR:
			// generate adress
			printf("li	x5,	0x%X	#%s\n",get_adress(node),node->var->name);
			// store adress's value to x5
			printf("lw	x5,	(x5)\n");
			return;
	}

	tree_gen(node->ln);
	push();
	//printf("										%ld -- %ld -- %i\n",node->rn,node->ln,node->kind);
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
	}

	error("invalid statment");
}

void generate_code(Program * prog)
{
	printf("li	x2,	0x%X\n",0xf000 + prog->offset);// initialize the stack pointer 
	//printf("addi	x2,	x2,	%i\n",prog->offset);

	Node * node_it = prog->tree;
	while(node_it)
	{
	//	printf("-------- %ld----- %ld-----",node_it,node_it->next);
	//	puts("loop_start");
		pre_gen(node_it);
	//	printf("-------- %ld----- %ld-----",node_it,node_it->next);
		node_it = node_it->next;
	}

}
