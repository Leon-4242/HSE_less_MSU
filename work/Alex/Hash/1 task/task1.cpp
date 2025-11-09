#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <stdexcept>

template<typename K, typename V>
class HashMap {
private:
    struct Entry {
        K key;
        V value;

        Entry(const K& k, const V& v) : key(k), value(v) {}
    };

    using Bucket = std::list<Entry>;

    std::vector<Bucket> buckets;
    size_t size;
    size_t limit;
    double loadFactor;
    double multiplier;

    size_t hashCode(const K& key) const {
        return std::hash<K>{}(key) % buckets.size();
    }

    void rehashUp() {
        size_t newSize = static_cast<size_t>(buckets.size() * multiplier);
        if (newSize <= buckets.size()) {
            newSize = buckets.size() + 1; // Защита от переполнения или некорректного multiplier
        }
        std::vector<Bucket> newBuckets(newSize);

        for (const auto& bucket : buckets) {
            for (const auto& entry : bucket) {
                size_t newIndex = std::hash<K>{}(entry.key) % newSize;
                newBuckets[newIndex].push_back(entry);
            }
        }

        buckets = std::move(newBuckets);
    }

public:
    HashMap(size_t initialSize = 16, double loadFactor = 0.75, double multiplier = 2.0)
        : size(0), loadFactor(loadFactor), multiplier(multiplier) {
        if (loadFactor <= 0.0 || loadFactor > 1.0) {
            throw std::invalid_argument("Load factor must be in (0, 1]");
        }
        if (multiplier <= 1.0) {
            throw std::invalid_argument("Multiplier must be > 1");
        }
        limit = static_cast<size_t>(initialSize * loadFactor);
        buckets.resize(initialSize);
    }

    ~HashMap() = default;

    void put(const K& key, const V& value) {
        if (size >= limit) {
            rehashUp();
            limit = static_cast<size_t>(buckets.size() * loadFactor);
        }

        size_t index = hashCode(key);
        for (auto& entry : buckets[index]) {
            if (entry.key == key) {
                entry.value = value;
                return;
            }
        }

        buckets[index].emplace_back(key, value);
        size++;
    }

    bool remove(const K& key) {
        size_t index = hashCode(key);
        auto& bucket = buckets[index];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                size--;
                return true;
            }
        }

        return false;
    }

    V* get(const K& key) {
        size_t index = hashCode(key);
        auto& bucket = buckets[index];

        for (auto& entry : bucket) {
            if (entry.key == key) {
                return &entry.value;
            }
        }

        return nullptr;
    }

    bool contains(const K& key) const {
        size_t index = hashCode(key);
        const auto& bucket = buckets[index];

        for (const auto& entry : bucket) {
            if (entry.key == key) {
                return true;
            }
        }

        return false;
    }

    size_t getSize() const {
        return size;
    }

    void iterate(void (*func)(const K&, const V&, void*), void* data) const {
        for (const auto& bucket : buckets) {
            for (const auto& entry : bucket) {
                func(entry.key, entry.value, data);
            }
        }
    }
};

void printEntry(const std::string&, const int&, void*);

int main() {
    HashMap<std::string, int> map;

    map.put("one", 1);
    map.put("two", 2);
    map.put("three", 3);

    int* value = map.get("two");
    if (value) {
        std::cout << "Value for 'two': " << *value << std::endl;
    }

    map.remove("two");

    map.iterate(printEntry, nullptr);

    return 0;
}

void printEntry(const std::string& key, const int& value, void* data) {
    std::cout << "Key: " << key << ", Value: " << value << std::endl;
}

