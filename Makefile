start:
	@(cc -std=c99 -Wall parsing.c mpc.c -ledit -lm -o parsing)
.PHONY: start
