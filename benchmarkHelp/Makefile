tinyBenchmark: tinyBenchmark.c benchmarkHelp.c
	gcc -o $@ $^ -lpthread -lchiredis -lhiredis
test: testBenchmarkHelp.c benchmarkHelp.c
	gcc -o $@ $^

.PHONY: clean

clean:
	-rm tinyBenchmark test
