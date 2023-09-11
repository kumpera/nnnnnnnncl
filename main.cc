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




void broadcastUniqueNCCLID(int rank, c10d::TCPStore &store, ncclUniqueId* ncclID) {
  std::string storeKey = "key_0";

  if (rank == 0) {
    auto vec = std::vector<uint8_t>(
        reinterpret_cast<uint8_t*>(ncclID),
        reinterpret_cast<uint8_t*>(ncclID) + NCCL_UNIQUE_ID_BYTES);
    store.set(storeKey, vec);
  } else {
    try {
      auto vec = store.get(storeKey);
      TORCH_CHECK(vec.size() == NCCL_UNIQUE_ID_BYTES);
      std::memcpy(ncclID, vec.data(), vec.size());
    } catch (const std::exception& e) {
      printf("RANK %d failed to get ncclUniqueId due to %s\n", rank, e.what());
    }
  }
}


int main(int argc, char **argv) {
  if (argc != 5) {
      printf("use machete <store-address> <store-port> <world_size> <rank>\n");
      return -1;
  }

  int world_size = std::atoi(argv[3]);
  int my_rank = std::atoi(argv[4]);

  c10d::TCPStoreOptions opts;
  opts.port = std::atoi(argv[2]);
  opts.numWorkers = world_size;
  opts.useLibUV = true; // BE BOLD
  opts.isServer = my_rank == 0;

  c10d::TCPStore store(argv[1], opts);

  ncclUniqueId ncclID;
  if (my_rank == 0) {
    C10D_NCCL_CHECK(ncclGetUniqueId(&ncclID));
    printf("rank 0 got an unique id\n");
  }

	broadcastUniqueNCCLID(my_rank, store, &ncclID);
  printf("rank %d got nccl unique id!\n", my_rank);

  ncclComm_t comm = nullptr;
  C10D_NCCL_CHECK(ncclCommInitRank(&comm, world_size, ncclID, my_rank));
  printf("rank %d gots the nccl %p\n", comm);

    printf("done rank:%d\n", my_rank);
    return 0;
}
