char isops(char c)
{
	// + -
	switch(c)
	{
		case '+':
		case '-':
		case ';':
		case '=':
			return 1;
	}
	return 0;
}

int islet(char c)
{
	return ((c >= 'a') && (c <= 'z')) | ((c >= 'A') && (c <= 'Z'));
}

// tokenizing
int ops_eq(Token *tok, char *op) {
  return memcmp(tok->s, op, tok->len) == 0 && op[tok->len] == '\0';
}	

Token * ops_pass(Token *tok, char *op)
{
	if(!ops_eq(tok,op))
	{
		token_error("Invalid token",__LINE__,op);
	}
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


		// adding Identifiers
		// identifier = char ( char | num) *
		if(islet(*input_code))
		{
			printf("identifier\n\n");
			tokens = tokens->next = token_init(TOKEN_IDENT,input_code,input_code);
			char * name_start = input_code;
			while(islet(*input_code) | isdigit(*input_code))
				{ input_code++; }
			tokens->len = input_code - name_start;
			continue;
		}

		// throw error
		token_error("unknown token",__LINE__,input_code);
		exit(1);
	}

	tokens->next = token_init(TOKEN_EOF, input_code,input_code);
	return head;
}
