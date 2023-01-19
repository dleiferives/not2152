// 1/18/2022 dleiferives, simple stack programming language. trying to get "2 3 +" to compile
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



/*
void* array_add(void* ptr, int * elements, int * max_elements, unsigned long size)
{
	if(*elements+1 >= *max_elements)
	{
		// double size
		*max_elements*=2;
		void * new_ptr = realloc(ptr, max_elements);
		// throw error if wrong!
	}

	*elements+=1;

	
	
}
*/

typedef struct
{
	char * INT;// = "INT";
	char * ADD;// = "ADD";
}TokenType;


typedef struct
{
	char* 	  type;
	void* 	  data;
	int 	  line_number;
}Token;

void print_token(Token T)
{
	if(T.type == "INT")
	{
		printf("(%s,%li,%i)",T.type,(unsigned long)T.data,T.line_number);
	}else{
		printf("(%s,%c,%i)",T.type,(char)T.data,T.line_number);
	}
}
Token* get_tokens(char *);

Token init_Token(char* type, char* data, int line_number)
{
	Token result;
	result.type = type;
	result.data = data;
	result.line_number = line_number;
	return result;
}


TokenType init_TokenType(void)
{
	TokenType result;
	result.INT = "INT";
	result.ADD = "ADD";
	return result;
}

TokenType TokenTypes = {"INT","ADD"};


int is_int(char c)
{
	return (c >= '0' && c <= '9');
}

unsigned long string_to_int(char ** str)
{
	int result = 0;
	while(is_int(**str))
	{
		result *= 10;
		result += **str - '0'; 
		*str += sizeof(char);
	}
	return (unsigned long)result;
}

int main()
{
	TokenTypes = init_TokenType();
	char * string = "039845043jsldfkjsl";
	printf("hello world!\n%li\n%s\n",string_to_int(&string),string);
	puts(string);
	char * program = "2 3 +";
	Token * tokens = get_tokens(program);
	print_token(tokens[0]);
	print_token(tokens[1]);
	print_token(tokens[2]);

	return 0;
}

Token* get_tokens(char * program)
{
	Token* tokens = malloc(sizeof(Token) * 10);
	int num_tokens = 0;
	int max_tokens = 10;
	unsigned long size_token = sizeof(Token);

	int line_number = 0;
	// while program still has data to proccess 
	while (*program != 0)
	{

		// get rid of white spaces!
		while((*program == ' ') && (*program == '\n') && (*program == '\t'))
		{
			program++;
		}

		if (*program == 0)
		{
			break;
		}	

		// parsing into tokens!
		printf("%i- %c\n",line_number,*program);
		if (*program == '+') // addition
		{
			// add addition token to the result;
			tokens[num_tokens++] = init_Token(TokenTypes.ADD, (void*)'+', line_number);
			//printf("jlskjdflkdsjf%c\n\n\n",(char)tokens[num_tokens].data);
			puts("ADD");
			program++;
		}else if(is_int(*program)) {
			puts("INT");
			tokens[num_tokens++] = init_Token(TokenTypes.INT, (void*)string_to_int(&program), line_number);
		}
		if (*program == 0)
		{
			break;
		}

		program++;
		line_number++;	
	}
	return tokens;
}
