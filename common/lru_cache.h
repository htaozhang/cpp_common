// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//
#ifndef __LRU_CACHE__
#define __LRU_CACHE__

#include <utility>
#include <list>
#include <unordered_map>

template<typename KeyT, typename ValueT>
class LRUCache {
public:
    struct CacheNode {
        CacheNode(const KeyT &k, const ValueT &v)
            : key(k), value(v), last_uptime(0) {
        }
        KeyT key;
        ValueT value;
        uint32_t last_uptime;
    };
    typedef typename std::list<CacheNode>::iterator list_iterator_t;
    typedef typename std::unordered_map<KeyT, list_iterator_t>::iterator map_iterator_t;

    LRUCache() : capacity_(1204) {

    }

    LRUCache(size_t capacity) : capacity_(capacity) {

    }

    void Put(const KeyT &key, const ValueT &value) {
        map_iterator_t iter = cache_map_.find(key);
        if (iter != cache_map_.end()) {
            cache_list_.erase(iter->second);
            cache_map_.erase(iter);
        }

        cache_list_.emplace_front(CacheNode{key, value});
        cache_map_[key] = cache_list_.begin();

        if (cache_list_.size() > capacity_) {
            cache_map_.erase(cache_list_.back().key);
            cache_list_.pop_back();
        }
    }

    int Get(const KeyT &key, ValueT &value) {
        map_iterator_t iter = cache_map_.find(key);
        if (iter == cache_map_.end()) {
            return -1;
        }

        cache_list_.splice(cache_list_.begin(), cache_list_, iter->second);
        value = iter->second->value;
        return 0;
    }

    void Del(const KeyT &key) {

    }

    size_t Size() {
        return cache_list_.size();
    }

private:
    size_t capacity_;
    std::list<CacheNode> cache_list_;
    std::unordered_map<KeyT, list_iterator_t> cache_map_;
};




#endif /* __LRU_CACHE__ */