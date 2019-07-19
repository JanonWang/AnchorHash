//
// Created by Janon Wang on 2019/7/19.
//
#include <iostream>
#include "anchor_hash.h"

int test_flea() {
  const instance_idx_t n_instances = 10;
  const instance_idx_t n_used_instances = 3;

  AnchorHash anchorHash(n_instances, n_used_instances);

  uint64_t key = 100;
  anchorHash.fleaInit(key);
  anchorHash.fleaRound();

  return 0;
}

