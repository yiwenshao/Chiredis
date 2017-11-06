COMMON_LIBS=-lchiredis -lhiredis -lpthread
ALL_TARGET=connect_and_disconnect example multi_nopipe PipelineCluster version


.PHONY: ALL

ALL: $(ALL_TARGET)

connect_and_disconnect: connect_and_disconnect.c
	gcc -g -o $@ $^ $(COMMON_LIBS)

example: example.c
	gcc -o $@ $^ $(COMMON_LIBS)

multi_nopipe: multi_nopipe.c
	gcc -o $@ $^ $(COMMON_LIBS)

PipelineCluster: PipelineCluster.c
	gcc -o $@ $^ $(COMMON_LIBS)
version: version.c
	gcc -o $@ $^ $(COMMON_LIBS)

.PHONY: clean
clean:
	-rm $(ALL_TARGET)
