/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LIBUNWINDSTACK_MEMORY_CACHE_H
#define _LIBUNWINDSTACK_MEMORY_CACHE_H

#include <stdint.h>

#include <memory>
#include <string>
#include <unordered_map>

#include <unwindstack/Memory.h>

namespace unwindstack {

class MemoryCache : public Memory {
 public:
  MemoryCache(Memory *memory) : impl_(memory) {}
  virtual ~MemoryCache() = default;

  size_t Read(uint64_t addr, void *dst, size_t size) override;
  long ReadTag(uint64_t addr) override { return impl_->ReadTag(addr); }

  void Clear() override { cache_.clear(); }

 private:
  constexpr static size_t kCacheBits = 12;
  constexpr static size_t kCacheMask = (1 << kCacheBits) - 1;
  constexpr static size_t kCacheSize = 1 << kCacheBits;
  std::unordered_map<uint64_t, uint8_t[kCacheSize]> cache_;

  std::unique_ptr<Memory> impl_;
};

}  // namespace unwindstack

#endif  // _LIBUNWINDSTACK_MEMORY_CACHE_H
