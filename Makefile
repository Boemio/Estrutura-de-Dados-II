all:
	gcc main.c lista.c -o exec -lm
run:
	./exec
debug:
	gcc -DDEBUG -Wall main.c lista.c -o exec

valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all --show-reachable=yes ./exec

clean:
	@rm *.o
