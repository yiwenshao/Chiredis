main: main.o connect.o
	gcc -o main main.o connect.o -l hiredis
main.o: main.c connect.h
	gcc -c main.c
connect.o: connect.c connect.h
	gcc -c connect.c
clean:
	rm *.o main
