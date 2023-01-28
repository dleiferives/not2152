
void quick_print(char * string, int len)
{
	for (int i=0; i < len; i++)
		putchar(string[i]);
}

void error_m(char * error_string, int line, char* file)
{
	printf("%s @ %i in %s\n",error_string,line,file);
	exit(1);
}

void token_error(char * error_string, int line, char *s)
{
	printf("%s -- %s @ %i\n",s,error_string,line);
	exit(1);
}
#define error(str) (error_m(str,__LINE__,__FILE__))
