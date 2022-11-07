#pragma once

#include <cstddef>
#include <cassert>
#include <optional>
#include <unordered_map>
#include <boost/intrusive/list.hpp>

#define LRU_CACHE_ENABLE_ASSERTIONS

#ifdef LRU_CACHE_ENABLE_ASSERTIONS
#define LRU_CACHE_ASSERT(expr) assert(expr)
#else 
#define LRU_CACHE_ASSERT(expr)
#endif // LRU_CACHE_ENABLE_ASSERTIONS

template<typename Key = int, typename Value = int>
class LRUCache {
    struct Node : boost::intrusive::list_base_hook<> {
        Value value;
        Node(const Value& value) : value(value) {}
    };
    using IntrusiveList = boost::intrusive::list<Node>;

    size_t capacity;
    std::unordered_map<Key, Node> data;
    boost::intrusive::list<Node> order;

    void makeFirst(Node& node) {
#ifdef LRU_CACHE_ENABLE_ASSERTIONS
        size_t prevSize = order.size();
#endif // LRU_CACHE_ENABLE_ASSERTIONS
        auto orderIt = order.iterator_to(node);
        LRU_CACHE_ASSERT(orderIt != order.end());
        
        order.erase(orderIt);
        order.push_front(*orderIt);
        LRU_CACHE_ASSERT(prevSize == order.size());
    }

    void eraseLast() {
        using MapValueType = typename std::unordered_map<Key, Node>::value_type;

        auto* mapValue = reinterpret_cast<MapValueType*>(reinterpret_cast<char*>(&order.back()) - 
            offsetof(MapValueType, second));
        order.pop_back();
        LRU_CACHE_ASSERT(data.find(mapValue->first) != data.end());
        data.erase(mapValue->first);
    }

    public:
    LRUCache(size_t capacity) : capacity(capacity) {
        LRU_CACHE_ASSERT(capacity > 0);
    }

    void put(const Key& key, const Value& value) {
        auto dataIt = data.find(key);
        if (dataIt != data.end()) {
            dataIt->second.value = value;
            makeFirst(dataIt->second);
        } else {
            if (order.size() == capacity) {
                eraseLast();
            }
            
            LRU_CACHE_ASSERT(data.find(key) == data.end());
            auto& node = (*data.insert({key, Node(value)}).first).second;
            order.push_front(node);
        }
        
        LRU_CACHE_ASSERT(static_cast<const Node&>(order.front()).value == value);
        LRU_CACHE_ASSERT(data.at(key).value == value);
    }

    const Value* get(const Key& key) {
        auto dataIt = data.find(key);
        if (dataIt == data.end()) {
            return nullptr;
        }

        makeFirst(dataIt->second);
        return &dataIt->second.value;
    }
};
