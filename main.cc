#include <atomic>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <randomx.h>

using randomx_dataset_ptr =
    std::unique_ptr<randomx_dataset, void (*)(randomx_dataset *)>;
using randomx_vm_ptr = std::unique_ptr<randomx_vm, void (*)(randomx_vm *)>;

namespace {

const char key[] = "RandomX example key";
const char input[] = "RandomX example input";

randomx_dataset_ptr init_dataset(randomx_flags flags, unsigned init_threads) {
  std::unique_ptr<randomx_cache, void (*)(randomx_cache *)> cache{
      randomx_alloc_cache(flags), randomx_release_cache};
  if (!cache) {
    throw std::runtime_error("Failed to allocate cache");
  }
  randomx_init_cache(cache.get(), key, sizeof(key));

  randomx_dataset_ptr dataset{randomx_alloc_dataset(flags),
                              randomx_release_dataset};
  if (!dataset) {
    throw std::runtime_error("Failed to allocate dataset");
  }

  auto dataset_item_count = randomx_dataset_item_count();
  if (init_threads == 1) {
    randomx_init_dataset(dataset.get(), cache.get(), 0, dataset_item_count);
  } else {
    std::vector<std::thread> threads;
    threads.reserve(init_threads);
    auto size = dataset_item_count / init_threads;
    auto last = dataset_item_count % init_threads;
    unsigned start = 0;
    for (unsigned i = 0; i < init_threads; i++) {
      auto this_size = size;
      if (i == init_threads - 1) {
        this_size += last;
      }
      threads.emplace_back(randomx_init_dataset, dataset.get(), cache.get(),
                           start, this_size);
      start += this_size;
    }
    for (auto &thread : threads) {
      thread.join();
    }
    threads.clear();
  }

  return dataset;
}

void do_hash(randomx_vm_ptr vm, std::atomic_uint &count) {
  char hash[RANDOMX_HASH_SIZE];
  char myinput[sizeof(input)];
  std::memcpy(myinput, input, sizeof(myinput));

  for (;;) {
    count++;
    myinput[0]++;
    randomx_calculate_hash(vm.get(), &input, sizeof(input), hash);
  }
}

} // namespace

int main(int argc, char **argv) {
  auto flags = randomx_get_flags();
  flags |= RANDOMX_FLAG_FULL_MEM;

  unsigned hashing_thread_count = 0;

  for (int i = 1; i < argc; i++) {
    std::string arg{argv[i]};
    if (arg == "-h" || arg == "--help") {
      std::cout << "Usage: " << argv[1] << " [-t <thread count>] [-H]"
                << std::endl;
      return 0;
    } else if (arg == "-t" || arg == "--threads") {
      if (i + 1 < argc) {
        hashing_thread_count = std::stoi(argv[++i]);
      } else {
        std::cerr << "--threads requires one argument" << std::endl;
        return 1;
      }
    } else if (arg == "-H" || arg == "--hugepages") {
      flags |= RANDOMX_FLAG_LARGE_PAGES;
    } else {
      std::cerr << "Unknown argument '" << arg << '\'' << std::endl;
      return 1;
    }
  }

  if (hashing_thread_count == 0) {
    hashing_thread_count = std::thread::hardware_concurrency() / 2;
    std::cerr << "Number of threads not provided. Defaulting to "
              << hashing_thread_count << std::endl;
  }

  auto start = std::chrono::high_resolution_clock::now();
  auto dataset = init_dataset(flags, std::thread::hardware_concurrency());
  auto now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> difference = now - start;
  std::cout << "Dataset initialised in " << difference.count() << 's'
            << std::endl;

  std::vector<std::thread> hashing_threads;
  hashing_threads.reserve(hashing_thread_count);

  std::atomic_uint count{0};

  for (unsigned i = 0; i < hashing_thread_count; i++) {
    randomx_vm_ptr vm{randomx_create_vm(flags, nullptr, dataset.get()),
                      randomx_destroy_vm};
    if (!vm) {
      throw std::runtime_error("Failed to create vm");
    }
    hashing_threads.emplace_back(do_hash, std::move(vm), std::ref(count));
  }

  start = std::chrono::high_resolution_clock::now();
  std::this_thread::sleep_for(std::chrono::seconds(2));

  for (;;) {
    now = std::chrono::high_resolution_clock::now();
    difference = now - start;

    unsigned hashes = count;
    start = now;
    count = 0;

    std::cout << hashes / difference.count() << " H/s" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(10));
  }

  return 0;
}
