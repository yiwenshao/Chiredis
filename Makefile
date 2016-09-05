obj=main.o connect.o crc16.o my_bench.o

OPTIMIZATION?=-O2
STD=-std=c99
OPT=$(OPTIMIZATION)
CHIREDISCC=gcc $(OPT)$(STD)
CHIREDISCC2=gcc $(STD)


main: $(obj) 
	$(CHIREDISCC) -o main $(obj) -l hiredis -lpthread
	@touch libchiredis.so
main.o: main.c connect.h
	$(CHIREDISCC2) -c main.c
connect.o: connect.c connect.h
	$(CHIREDISCC2) -c connect.c
crc16.o: crc16.c crc16.h
	$(CHIREDISCC2) -c crc16.c
my_bench.o: my_bench.c my_bench.h
	$(CHIREDISCC2) -c my_bench.c

.PHONY: install


LIBOBJ=connect.c crc16.c
LIBHEAD=connect.h

install:
	@$(CHIREDISCC2) -std=c99 -shared -fPIC -o libchiredis.so $(LIBOBJ)
	@mkdir -p /usr/local/include/chiredis /usr/local/lib
	@cp -a $(LIBHEAD) /usr/local/include/chiredis
	@cp -a *.so /usr/local/lib/

.PHONY: clean
clean:
	-rm *.o main *.so
