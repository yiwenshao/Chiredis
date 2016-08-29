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


LIBOBJ=connect.c crc16.c
LIBHEAD=connect.h

install:
	gcc -shared -fPIC -o libchiredis.so $(LIBOBJ)
	mkdir -p /usr/local/include/chiredis /usr/local/lib
	cp -a $(LIBHEAD) /usr/local/include/chiredis
	cp -a *.so /usr/local/lib/

.PHONY: clean
clean:
	rm *.o main *.so
