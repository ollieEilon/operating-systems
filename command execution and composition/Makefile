all: shell

shell:
	gcc -std=c99 -Wall -pedantic main.c scanner.c interpreter.c shell.c builtin.c command.c colours.c state.c -o shell
	# gcc -std=c99 -Wall -pedantic main.c scanner.c interpreter.c shell.c builtin.c command.c colours.c state.c -o shell -D EXT_PROMPT


clean:
	rm -f *~
	rm -f *.o
	rm -f shell

