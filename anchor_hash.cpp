//
// Created by Janon Wang on 2019/7/17.
//
#include <iostream>
#include "anchor_hash.h"
#define BIG_CONSTANT(x) (x##LLU)

AnchorHash::AnchorHash(instance_idx_t n_instances, instance_idx_t n_used_instances)
  : max_instances(n_instances), A(n_instances), K(n_instances), W(n_instances),
  L(n_instances), fleaSeed(0xf1ea5eed), fleaRot1(uint16_t(27)), fleaRot2(uint16_t(17)),
  fleaInitRounds(uint16_t(3)) {
  for (instance_idx_t i = 0; i < n_used_instances; i++) {
    M_b2i[i] = i;
    M_i2b[i] = i;
  }
  for (instance_idx_t i = 0; i < n_used_instances; i++) {
    K[i] = W[i] = L[i] = i;
  }
  for (instance_idx_t i = n_instances - instance_idx_t(1); i >= n_used_instances; i--) {
    A[i] = i;
    R.push(i);
  }
  N = n_used_instances;
}

instance_idx_t AnchorHash::getInstance(key_t key) {
  auto b = getBucket(key);
  return M_b2i[b];
}

int AnchorHash::addInstance(instance_idx_t instance_id) {
  if (instance_id >= max_instances) {
    return 1;
  }
  auto b = addBucket();
  M_b2i[b] = instance_id;
  M_i2b[instance_id] = b;
  return 0;
}

int AnchorHash::removeInstance(instance_idx_t instance_id) {
  if (instance_id >= max_instances) {
    return 1;
  }
  auto b = M_i2b[instance_id];
  M_i2b.erase(instance_id);
  M_b2i.erase(b);
  removeBucket(b);
  return 0;
}

instance_idx_t AnchorHash::getBucket(key_t key) {
  fleaInit(key);
  auto b = fleaData.d % max_instances;
  while (A[b] > 0) {
    fleaRound();
    auto h = fleaData.d % A[b];
    while (A[h] >= A[b]) {
      h = K[h];
    }
    b = h;
  }
  return b;
}

instance_idx_t AnchorHash::addBucket() {
  instance_idx_t b = R.top();
  R.pop();
  A[b] = 0;
  L[W[N]] = N;
  K[b] = b;
  W[L[b]] = K[b];
  N++;
  return b;
}

void AnchorHash::removeBucket(instance_idx_t b) {
  R.push(b);
  N--;
  A[b] = N;
  K[b] = W[N];
  W[L[b]] = K[b];
  L[W[N]] = L[b];
}

uint64_t AnchorHash::MurmurHash64A(const void *key, int len, uint64_t seed) {
  const uint64_t m = BIG_CONSTANT(0xc6a4a7935bd1e995);
  const int r = 47;

  uint64_t h = seed ^ (len * m);

  auto * data = (const uint64_t *)key;
  const uint64_t * end = data + (len/8);

  while(data != end)
  {
    uint64_t k = *data++;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;
  }

  auto * data2 = (const unsigned char*)data;

  switch(len & 7)
  {
    case 7: h ^= uint64_t(data2[6]) << 48;
    case 6: h ^= uint64_t(data2[5]) << 40;
    case 5: h ^= uint64_t(data2[4]) << 32;
    case 4: h ^= uint64_t(data2[3]) << 24;
    case 3: h ^= uint64_t(data2[2]) << 16;
    case 2: h ^= uint64_t(data2[1]) << 8;
    case 1: h ^= uint64_t(data2[0]);
            h *= m;
  };

  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}

void AnchorHash::fleaInit(uint64_t key) {
  auto seed = uint32_t((key >> 32) ^ key);
  fleaData.a = fleaSeed;
  fleaData.b = seed;
  fleaData.c = seed;
  fleaData.d = seed;

  for (int i = 0; i < fleaInitRounds; i++) {
    fleaRound();
  }
}

void AnchorHash::fleaRound() {
  auto e = fleaData.a - rotateLeft32(fleaData.b, fleaRot1);
  fleaData.a = fleaData.b ^ rotateLeft32(fleaData.c, fleaRot2);
  fleaData.b = fleaData.c + fleaData.d;
  fleaData.c = fleaData.d + e;
  fleaData.d = e + fleaData.a;
}