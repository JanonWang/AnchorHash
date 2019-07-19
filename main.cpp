#include <iostream>
#include <random>
#include <time.h>
#include "anchor_hash.h"

uint16_t rng(uint16_t seed);

int main() {
  const instance_idx_t n_instances = 10;
  const instance_idx_t n_used_instances = 3;
  std::vector<int> count_anchor(n_instances);
  std::vector<int> count_basic_hash(n_instances);
  AnchorHash anchorHash(n_instances, n_used_instances);

  uint64_t key;
  uint16_t seed = 500;
  int n_samples = 10000;
  for (int i = 0; i < n_samples; i++) {
    key = rng(seed);
    auto instance_id = anchorHash.getInstance(key);
    count_anchor[instance_id]++;

    auto basic_hash_id = key % n_used_instances;
    count_basic_hash[basic_hash_id]++;
  }

  anchorHash.addInstance(9);
  for (int i = 0; i < n_samples; i++) {
    key = rng(seed);
    auto instance_id = anchorHash.getInstance(key);
    count_anchor[instance_id]++;

    auto basic_hash_id = key % n_used_instances;
    count_basic_hash[basic_hash_id]++;
  }

  anchorHash.removeInstance(2);
  for (int i = 0; i < n_samples; i++) {
    key = rng(seed);
    auto instance_id = anchorHash.getInstance(key);
    count_anchor[instance_id]++;

    auto basic_hash_id = key % n_used_instances;
    count_basic_hash[basic_hash_id]++;
  }

  std::cout << "Verify the balancing of Anchor:" << std::endl;
  double over_subscription = 0;
  for (uint16_t i = 0; i < n_instances; i++) {
    if (count_anchor[i] > over_subscription) over_subscription = count_anchor[i];
    std::cout << count_anchor[i] << " ";
  }
  std::cout << std::endl;
//  double average = double(n_samples) / double(n_used_instances);
//  std::cout << std::endl << "The over subscription of Anchor is: "
//  << (over_subscription - average) / average << std::endl;

  std::cout << "Verify the balancing of basic hash:" << std::endl;
  over_subscription = 0;
  for (uint16_t i = 0; i < n_instances; i++) {
    if (count_basic_hash[i] > over_subscription) over_subscription = count_basic_hash[i];
    std::cout << count_basic_hash[i] << " ";
  }
//  std::cout << std::endl << "The over subscription of basic hash is: "
//  << (over_subscription - average) / average << std::endl;
//  std::cout << std::endl;
}

// create a random integer from 0 - 65535
uint16_t rng(uint16_t seed) {
  static uint16_t y = 0;
  if (seed != 0) y += (seed && 0x1FFF); // seeded with a different number
  y ^= y << 2;
  y ^= y >> 7;
  y ^= y << 7;
  return (y);
}
