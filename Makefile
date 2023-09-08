TORCH_HOME=/home/kumpera/src/pytorch
LIB_LIST=-lm -lpthread -ltorch -lc10 -ltorch_cpu
SOURCES = main.cc \

machete: ${SOURCES}
	c++ -O2 ${LIB_LIST} -L${TORCH_HOME}/torch/lib -Wl,-rpath,${TORCH_HOME}/torch/lib -I${TORCH_HOME}/torch/include -Iinclude ${SOURCES} -o machete


ws_1: machete
	./machete 127.0.0.1 27456 1 0

ws_2: machete
	./machete 127.0.0.1 27456 2 0 &
	./machete 127.0.0.1 27456 2 1
