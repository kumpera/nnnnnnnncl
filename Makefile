TORCH_HOME=/home/kumpera/src/pytorch
LIB_LIST=-lm -lpthread -ltorch -lc10 -ltorch_cpu
SOURCES = main.cc \
		fake_nccl/init.cc	\
		fake_nccl/bootstrap.cc	\
		fake_nccl/debug.cc	\
		fake_nccl/socket.cc	\
		fake_nccl/utils.cc	\
		fake_nccl/argcheck.cc	\
		fake_nccl/proxy.cc	\
		fake_nccl/param.cc	\
		fake_nccl/transport.cc	\
		fake_nccl/ipcsocket.cc	\
		fake_nccl/shmutils.cc	\
		fake_nccl/profiler.cc	\
		fake_nccl/coll_net.cc	\
		fake_nccl/p2p.cc	\
		fake_nccl/shm.cc	\
		fake_nccl/net.cc	\
		fake_nccl/topo.cc	\
		fake_nccl/search.cc	\
		fake_nccl/xml.cc	\
		fake_nccl/paths.cc	\
		fake_nccl/root-net.cc	\
		fake_nccl/net_ib.cc	\
		fake_nccl/net_socket.cc	\
		fake_nccl/ibvwrap.cc	\
		fake_nccl/ibvsymbols.cc	\
		fake_nccl/group.cc	\
		fake_nccl/connect.cc	\
		fake_nccl/enqueue.cc	\
		fake_nccl/trees.cc	\
		fake_nccl/rings.cc	\
		fake_nccl/tuning.cc	\





# TODO break this into multiple compilation units cuz it's going to get tedious really fast if the source list gets huge
machete: ${SOURCES}
	c++ -g ${LIB_LIST} -L${TORCH_HOME}/torch/lib -Wl,-rpath,${TORCH_HOME}/torch/lib -I${TORCH_HOME}/torch/include -Iinclude ${SOURCES} -o machete


ws_1: machete
	./machete 127.0.0.1 27456 1 0

ws_2: machete
	./machete 127.0.0.1 27456 2 0 &
	./machete 127.0.0.1 27456 2 1
