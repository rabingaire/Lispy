/*
 * CopyRight @ Rabin Gaire 2018 rabingaire.com.np
 * Lipsy is a Lisp implementation in C programming Language
 * I have started to write this to learn how programming language
 * are implemented and to learn C language enough to be dangerous :)
 */
#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>

/* 
 * If user is on windows machine
 * windows machine does not need readline.h lib
 * So we create a fake function readline and add_history
 * to make our code protable across platforms
 */

#ifdef _WIN32
#include <string.h>

static char buffer[2048];

char* readline(char* prompt) {
	fputs(prompt, stdout);
	fgets(buffer, 2048, stdin);
	char* cpy = malloc(strlen(buffer)+1);
	strcopy(cpy, buffer);
	cpy[strlen(cpy)-1] = '\0';
	return cpy;
}

void add_history(char*, unused) {}

/* if user is on unix or linux then use these headers files */
#else
#include <editline/readline.h>
#endif

int main(int argc, char** argv) {
	mpc_parser_t* Number = mpc_new("number");
	mpc_parser_t* Operator = mpc_new("operator");
	mpc_parser_t* Expr     = mpc_new("expr");
	mpc_parser_t* Lispy    = mpc_new("lispy");

	mpca_lang(MPCA_LANG_DEFAULT,
  		"                                                           \
    			number   : /-?[0-9]+/ ;                             \
    			operator : '+' | '-' | '*' | '/' ;                  \
    			expr     : <number> | '(' <operator> <expr>+ ')' ;  \
    			lispy    : /^/ <operator> <expr>+ /$/ ;             \
  		",
  		Number, Operator, Expr, Lispy);

	puts("Lispy Version 0.1");
	puts("Press Ctrl+c to exit\n");

	while(1) {
		char* input = readline("lispy> ");
		add_history(input);
		mpc_result_t r;
		if(mpc_parse("<stdin>", input, Lispy, &r)) {
  			mpc_ast_print(r.output);
  			mpc_ast_delete(r.output);
		} else {
  			mpc_err_print(r.error);
  			mpc_err_delete(r.error);
		}
		free(input);
	}
	mpc_cleanup(4, Number, Operator, Expr, Lispy);
	return 0;
}
