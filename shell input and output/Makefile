all: shell

shell:
	gcc -std=c99 -Wall -pedantic builtinCommands.c builtinMapper.c command.c commandQueue.c interpreter.c main.c parser.c pipeline.c redirections.c scanner.c state.c directories.c -o shell
	# gcc -std=c99 -Wall -pedantic builtinMapper.c builtinCommands.c command.c commandQueue.c interpreter.c main.c parser.c pipeline.c redirections.c scanner.c state.c directories.c colors.c directoryStack.c -o shell -D EXT_PROMPT


clean:
	rm -f *~
	rm -f *.o
	rm -f shell

