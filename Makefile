obj=main.o connect.o crc16.o my_bench.o
main: $(obj) 
	gcc -o main $(obj) -l hiredis -lpthread
main.o: main.c connect.h
	gcc -c main.c
connect.o: connect.c connect.h
	gcc -c connect.c
crc16.o: crc16.c crc16.h
	gcc -c crc16.c
my_bench.o: my_bench.c my_bench.h
	gcc -c my_bench.c

.PHONY: install

install:
	gcc -shared -fPIC -o libchiredis.so connect.c crc16.c my_bench.c
	mkdir -p /usr/local/include/chiredis /usr/local/lib
	cp -a *.h /usr/local/include/chiredis
	cp -a *.so /usr/local/lib/

.PHONY: clean
clean:
	rm *.o main *.so
