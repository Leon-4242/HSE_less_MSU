#include <iostream>
#include <vector>
#include <list>
#include <functional>

template<typename K, typename V>
class HashMap {
private:

	class Entry {
		public:
        K key;
        V value;
        bool isOccupied;
        bool isDeleted;

        Entry() : isOccupied(false), isDeleted(false) {}
        Entry(const K& k, const V& v) : key(k), value(v), isOccupied(true), isDeleted(false) {}
    };

    std::vector<Entry> table;
    size_t size;
    double loadFactor;
    double multiplier;

    size_t hashCode(const K& key) const {
        return std::hash<K>{}(key) % table.size();
    }

    void rehash() {
        size_t newCapacity = static_cast<size_t>(table.size() * multiplier);
        std::vector<Entry> newTable(newCapacity);

        for (const auto& entry : table) {
            if (entry.isOccupied && !entry.isDeleted) {
                size_t newIndex = std::hash<K>{}(entry.key) % newCapacity;
                while (newTable[newIndex].isOccupied) {
                    newIndex = (newIndex + 1) % newCapacity;
                }
                newTable[newIndex] = entry;
            }
        }

        table = std::move(newTable);
    }

    public:
    HashMap(size_t initialCapacity = 16, double loadFactor = 0.75, double multiplier = 2.0): size(0), loadFactor(loadFactor), multiplier(multiplier) {
        table.resize(initialCapacity);
    }

    bool put(const K& key, const V& value) {
        if (size >= loadFactor * table.size()) rehash();

        size_t index = hashCode(key);
        size_t startIndex = index;

        do {
            if (!table[index].isOccupied) {
                table[index] = Entry(key, value);
                size++;
                return true;
            }
            if (table[index].isOccupied && !table[index].isDeleted && table[index].key == key) {
                table[index].value = value;
                return true;
            }
            index = (index + 1) % table.size();
        } while (index != startIndex);

        return false;
    }

    V* get(const K& key) {
        size_t index = hashCode(key);
        size_t startIndex = index;

        do {
            if (table[index].isOccupied && !table[index].isDeleted && table[index].key == key) return &table[index].value;
            index = (index + 1) % table.size();
        } while (index != startIndex && table[index].isOccupied);

        return nullptr;
    }

    bool remove(const K& key) {
        size_t index = hashCode(key);
        size_t startIndex = index;

        do {
            if (table[index].isOccupied && !table[index].isDeleted && table[index].key == key) {
                table[index].isDeleted = true;
                size--;
                return true;
            }
            index = (index + 1) % table.size();
        } while (index != startIndex && table[index].isOccupied);

        return false;
    }

    bool contains(const K& key) const {
        size_t index = hashCode(key);
        size_t startIndex = index;

        do {
            if (table[index].isOccupied && !table[index].isDeleted && table[index].key == key) return true;
            index = (index + 1) % table.size();
        } while (index != startIndex && table[index].isOccupied);

        return false;
    }

    size_t getSize() const {
        return size;
	}
};

int main(void) {
	HashMap<std::string, int> map;
	map.put("a", 1);
	map.put("b", 2);
	return 0;
}
