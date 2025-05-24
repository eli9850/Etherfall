#pragma once
#include <iostream>
#include <unordered_map>
#include <list>

namespace Etherfall::Utilities {
    
    template<typename T>
    using is_hashable = std::is_constructible<std::size_t, decltype(std::declval<std::hash<T>>()(std::declval<T>()))>;

    // LRUCache Template
    template<typename KeyType, typename ValueType>
    class LRUCache {
        static_assert(is_hashable<KeyType>::value, "KeyType must be hashable (valid std::hash<KeyType>).");

    public:
        explicit LRUCache(size_t capacity) : m_capacity(capacity) {}

        const ValueType& get(const KeyType& key) {
            auto it = m_cache.find(key);
            if (it == m_cache.end()) {
                throw std::out_of_range("Key not found in cache.");
            }

            m_usage.splice(m_usage.begin(), m_usage, it->second.second);
            return it->second.first;
        }

        void put(const KeyType& key, const ValueType& value) {
            auto it = m_cache.find(key);
            if (it != m_cache.end()) {
                it->second.first = value;
                m_usage.splice(m_usage.begin(), m_usage, it->second.second);
            }
            else {
                if (m_cache.size() >= m_capacity) {
                    const KeyType& lru_key = m_usage.back();
                    m_usage.pop_back();
                    m_cache.erase(lru_key);
                }
                m_usage.push_front(key);
                m_cache[key] = { value, m_usage.begin() };
            }
        }

        bool exists(const KeyType& key) const {
            return m_cache.find(key) != m_cache.end();
        }

        size_t size() const {
            return m_cache.size();
        }

    private:
        size_t m_capacity;
        std::list<KeyType> m_usage;
        std::unordered_map<KeyType, std::pair<ValueType, typename std::list<KeyType>::iterator>> m_cache;
    };
}
