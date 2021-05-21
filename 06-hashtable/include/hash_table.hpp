#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <memory>

#define INITIAL_TABLE_SIZE 64

#include "hash_slot.hpp"
#include "hash_funcs.hpp"

template <typename K, typename V>
class HashTable {
public:
    HashTable(HashFunc *hash_func);
    ~HashTable();
    int get(const K &key, V &value);
    int put(const K &key, const V &value);
    int remove(const K &key);
    size_t get_table_size();
    size_t get_size();
    double get_load_factor();

protected:
    size_t table_size;
    
private:
    HashFunc *hash_func;
    size_t size;
    HashSlot<K, V> *table;

    // Should be overriden by the derived class
    virtual unsigned long get_next_pos(unsigned long pos,
                                       unsigned long step) = 0;
    unsigned long get_pos(const K key);
    void enlarge_table();
};

template <typename K, typename V>
HashTable<K, V>::HashTable(HashFunc *hash_func): table(), hash_func(hash_func),
                                                 size(0), table_size(INITIAL_TABLE_SIZE) {
    table = new HashSlot<K, V>[table_size];
}

template <typename K, typename V>
HashTable<K, V>::~HashTable() {
    delete[] table;
}

template <typename K, typename V>
void HashTable<K, V>::enlarge_table() {
    // TODO
    HashSlot<K, V> *new_table = new HashSlot<K, V>[table_size * 2];
    HashSlot<K, V> *old_table = table;
    int old_table_size = table_size;
    table_size *= 2;

    for (size_t i = 0; i < old_table_size; i++) {
        if (!table[i].is_removed() && !table[i].is_empty()) {
            new_table[get_pos(table[i].get_key())].set_key_value(table[i].get_key(), table[i].get_value());
        }
    }

    delete[] old_table;
    table = new_table;
}

template <typename K, typename V>
unsigned long HashTable<K, V>::get_pos(const K key) {
    // TODO
    return hash_func->hash(key) % table_size;
}

template <typename K, typename V>
int HashTable<K, V>::get(const K &key, V &value) {
    // TODO
    // unsigned long pos = get_pos(key);
    // unsigned long next_pos = pos;
    // int num_of_probe = 1;

    // for (num_of_probe; num_of_probe < table_size; num_of_probe++) {
    //     if (table[next_pos].is_empty()) {
    //         return -1;
    //     }
    //     // if (table[next_pos].is_empty() || table[next_pos].is_removed()) {
    //     //     return -1;
    //     // }
    //     if (table[next_pos].get_key() == key) {
    //         value = table[next_pos].get_value();
    //         return num_of_probe;
    //     }

    //     next_pos = get_next_pos(pos, num_of_probe);
    // }

    // return -1;

    int hash_idx = get_pos(key);
    int step = 1;
    
    // Find node with given key
    while (!table[hash_idx].is_removed() && !table[hash_idx].is_empty()) {
        if (step - 1 > table_size) {
            return -1;
        }
        if (table[hash_idx].get_key() == key) {
            value = table[hash_idx].get_value();
            return step;
        }
        hash_idx = get_next_pos(get_pos(key), step);
        step++;
    }

    return -1;
}

template <typename K, typename V>
int HashTable<K, V>::put(const K &key, const V &value) {
    // TODO
    // unsigned long pos = get_pos(key);
    // unsigned long next_pos = pos;
    // int num_of_probe = 1;

    // while (true) {
    //     if (table[next_pos].get_key() == key) num_of_probe = -1;
    //     if (!table[next_pos].is_empty() && !table[next_pos].is_removed()) {
    //         next_pos = get_next_pos(pos, num_of_probe);
    //         num_of_probe++;
    //     }
    //     else {
    //         table[next_pos].set_key_value(key, value);
    //         size++;
    //         break;
    //     }
    // }

    // if (get_load_factor() > 0.5) enlarge_table();

    // return num_of_probe;

    int hash_idx = get_pos(key);
    int step = 1;
    bool flag = false;

    // Find next free space
    while (!table[hash_idx].is_empty() && !table[hash_idx].is_removed() && table[hash_idx].get_key() != key) {
        hash_idx = get_next_pos(get_pos(key), step);
        step++;
    }

    if (table[hash_idx].is_empty() || table[hash_idx].is_removed()) {
        table_size++;
        table[hash_idx].set_key_value(key, value);
        flag = true;
    }

    if (get_load_factor() > 0.5) enlarge_table();

    return flag ? step : -1;
}

template <typename K, typename V>
int HashTable<K, V>::remove(const K &key) {
    // TODO        
    // unsigned long pos = get_pos(key);
    // unsigned long next_pos = pos % table_size;
    // int num_of_probe = 1;

    // while (!table[next_pos].is_removed()) {
    //     if (table[next_pos].get_key() == key) {
    //         table[next_pos].set_removed();
    //         size--;
    //         return num_of_probe;
    //     }
    //     next_pos = get_next_pos(pos, num_of_probe);
    //     num_of_probe++;
    // }

    // return -1;

    int hash_idx = get_pos(key);
    int step = 1;
    bool flag = false;

    while (!table[hash_idx].is_empty()) {
        // Node found
        if (table[hash_idx].get_key() == key) {
            table[hash_idx].set_removed();

            table_size--;
            flag = true;
        }
        hash_idx = get_next_pos(get_pos(key), step);
        step++;
    }

    return flag ? step : -1;
}

template <typename K, typename V>
size_t HashTable<K, V>::get_table_size() {
    return table_size;
}

template <typename K, typename V>
size_t HashTable<K, V>::get_size() {
    return size;
}

template <typename K, typename V>
double HashTable<K, V>::get_load_factor() {
    return (double)size/table_size;
}


template <typename K, typename V>
class LinearProbeHashTable: public HashTable<K, V> {
public:
    LinearProbeHashTable(HashFunc *hash_func): HashTable<K, V>(hash_func) {
    }
    
private:
    virtual unsigned long get_next_pos(unsigned long pos, unsigned long step) {
        // TODO
        return (pos + step) % HashTable<K, V>::table_size;
    }
};

template <typename K, typename V>
class QuadProbeHashTable: public HashTable<K, V> {
public:
    QuadProbeHashTable(HashFunc *hash_func): HashTable<K, V>(hash_func) {
    }
private:
    virtual unsigned long get_next_pos(unsigned long pos, unsigned long step) {
        // TODO
        return (pos + (step + step * step) / 2) % HashTable<K, V>::table_size;
        // return (size_t)(pos + 0.5 * step + 0.5 * step * step) % HashTable<K, V>::table_size;
    }
};

