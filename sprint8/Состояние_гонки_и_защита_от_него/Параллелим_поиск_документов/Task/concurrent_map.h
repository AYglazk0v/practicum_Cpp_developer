#pragma once

#include <map>
#include <vector>
#include <mutex>

using namespace std::string_literals;

template <typename Key, typename Value>
class ConcurrentMap {
    private:
        struct Bucket {
            std::mutex              mutex_;
            std::map<Key, Value>    map_;
        };
        
    public:
        static_assert(std::is_integral_v<Key>, "ConcurrentMap supports only integer keys"s);

        struct Access {
            std::lock_guard<std::mutex> lock;
            Value& ref_to_value;

            Access(const Key& key, Bucket& bucket)
                : lock(bucket.mutex_)
                , ref_to_value(bucket.map_[key]) {
            };
        };

        explicit ConcurrentMap(size_t bucket_count) : buckets_(bucket_count){}

        Access operator[](const Key& key) {
            auto& curr_bucket = buckets_[key % buckets_.size()];
            return { key, curr_bucket };
        }

        std::map<Key, Value> BuildOrdinaryMap() {
            std::map<Key, Value> ret_val;
            for (auto& curr_bucket : buckets_) {
                std::lock_guard locking(curr_bucket.mutex_);
                ret_val.insert(curr_bucket.map_.begin(), curr_bucket.map_.end());
            }
            return ret_val;
        }

        auto Erase(const Key& key) {
            std::lock_guard locking(buckets_[key % buckets_.size()].mutex_);
            return buckets_[key % buckets_.size()].map_.erase(key);
        }

    private:
        std::vector<Bucket> buckets_;
};