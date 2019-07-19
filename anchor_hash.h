//
// Created by Janon Wang on 2019/7/17.
//

#ifndef ANCHORHASH_ANCHOR_HASH_H
#define ANCHORHASH_ANCHOR_HASH_H

#include <cstdint>
#include <vector>
#include <stack>
#include <map>

typedef uint32_t instance_idx_t;
typedef uint64_t key_t;

class AnchorHash final {
 public:
  AnchorHash(instance_idx_t n_instances, instance_idx_t n_used_instances);

  int addInstance(instance_idx_t instance_id);
  int removeInstance(instance_idx_t instance_id);
  instance_idx_t getInstance(key_t key);

 private:
  // map <bucket, instance>
  std::map<instance_idx_t, instance_idx_t> M_b2i;
  // map <instance, bucket>
  std::map<instance_idx_t, instance_idx_t> M_i2b;
  std::vector<instance_idx_t> A, K, W, L;
  std::stack<instance_idx_t> R;
  instance_idx_t N;
  const instance_idx_t max_instances;

  instance_idx_t addBucket();
  void removeBucket(instance_idx_t b);
  instance_idx_t getBucket(key_t key);

  // key is string and len is the length of string
  uint64_t MurmurHash64A ( const void * key, int len, uint64_t seed);

  // flea random number generator
  // http://burtleburtle.net/bob/rand/smallprng.html
  const uint32_t fleaSeed;
  const int  fleaRot1;
  const int fleaRot2;
  const int fleaInitRounds;
  struct FleaData {
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
  } fleaData;
  inline uint32_t rotateLeft32(uint32_t val, int k) {
    return (val << k) | (val >> (32 - k));
  }
  void fleaInit(uint64_t key);
  void fleaRound();
};

#endif //ANCHORHASH_ANCHOR_HASH_H
