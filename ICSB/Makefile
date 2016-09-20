tinyBenchmark: tinyBenchmark.c benchmarkHelp.c
	gcc -o $@ $^ -lpthread -lchiredis -lhiredis

normal: normal.c benchmarkHelp.c
	gcc -o $@ $^ -lpthread -lchiredis -lhiredis

test: testBenchmarkHelp.c benchmarkHelp.c
	gcc -o $@ $^

token: token.c benchmarkHelp.c dbTest.c
	gcc -o $@ $^ -lpthread -lchiredis -lhiredis

target: $(source) benchmarkHelp.c dbTest.c
	gcc -o $@ $^ -lpthread $(depend)

.PHONY: clean

clean:
	-rm tinyBenchmark test
