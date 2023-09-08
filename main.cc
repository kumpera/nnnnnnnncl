#include <cstdio>
#include <string>

#include <torch/csrc/distributed/c10d/TCPStore.hpp>
#include <nccl.h>

#define C10D_NCCL_CHECK(cmd)                                   \
  do {                                                                        \
    ncclResult_t result = cmd;                                                \
    if (result != ncclSuccess) {                                              \
        printf("NCCL error in: %s:%d :: %d\n", __FILE__, __LINE__, result);     \
        exit(-2);                                                             \
    }                                                                         \
  } while (0)



int main(int argc, char **argv) {
    if (argc != 5) {
        printf("use machete <store-address> <store-port> <world_size> <rank>\n");
        return -1;
    }

    int my_rank = std::atoi(argv[4]);

    c10d::TCPStoreOptions opts;
    opts.port = std::atoi(argv[2]);
    opts.numWorkers = std::atoi(argv[3]);
    opts.useLibUV = true; // BE BOLD
    opts.isServer = my_rank == 0;

    c10d::TCPStore store(argv[1], opts);

    ncclUniqueId ncclID;
    C10D_NCCL_CHECK(ncclGetUniqueId(&ncclID));

	// broadcastUniqueNCCLID
	// ncclCommInitRank


    printf("done rank:%d\n", my_rank);
    return 0;
}
