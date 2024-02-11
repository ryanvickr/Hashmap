#ifndef HASHMAP_H
#define HASHMAP_H

#include <functional>
#include <utility>

namespace {
void LOG(std::string_view message) {
   std::cout << "[Info] " << message << std::endl;
}
}

template<typename K, typename V>
class HashMap {
 public:
   HashMap(int initial_size, std::function<int(const K&)> hash_function);

   // Inserts a value into the hashmap.
   void Insert(K key, V value);
   // Removes a value from the hashmap.
   void Remove(K& key);
   // Gets the value associated with the key, or returns
   // a null pointer.
   V* Get(const K& key);
   // Returns `true` if the key was found in the map, or 
   // false otherwise.
   bool Contains(K& key);
   // Returns the size of the hashmap.
   int Size();

 private:
   // The underlying array.
   std::vector<std::unique_ptr<std::pair<K, V>>> data_;
   // Hash function
   std::function<int(const K&)> hash_function_;

   int items = 0;
};

template<typename K, typename V>
HashMap<K, V>::HashMap(
   int initial_size, std::function<int(const K&)> hash_function)
   : data_(initial_size), hash_function_(hash_function) {}

template<typename K, typename V>
void HashMap<K, V>::Insert(K key, V value) {
   int index = hash_function_(key);
   // TODO: Validate that this index is within bounds first.
   std::unique_ptr<std::pair<K, V>>& pair = data_[index];

   // Check for a collision
   if (pair == nullptr) {
      // No collision, safe to insert.
      pair->first = key;
      pair->second = value;
      return;
   }

   // TODO: deal with collisions
}

template<typename K, typename V>
V* HashMap<K, V>::Get(const K& key) {
   int index = hash_function_(key);
   // TODO: Validate that this index is within bounds first.
   std::unique_ptr<std::pair<K, V>>& pair = data_[index];

   // If this key is empty, return nullptr.
   // TODO: Add an overload that allows a default value to be passed in.
   if (pair == nullptr) {
      return nullptr;
   }

   if (pair->first != key) {
      // We don't have a direct match, linear search.
      return nullptr;
   } else {
      return &pair->second;
   }
}

#endif  // HASHMAP_H
