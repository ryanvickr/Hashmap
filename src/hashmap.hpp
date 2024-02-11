#ifndef HASHMAP_H
#define HASHMAP_H

#include <iostream>
#include <functional>
#include <utility>

namespace {
void LOG(std::string_view message) {
   std::cout << "[Info] " << message << std::endl;
}
}  // namespace

template<typename K, typename V>
class HashMap {
 public:
   HashMap(int initial_size, std::function<int(const K&)> hash_function);

   // Inserts a value into the hashmap.
   void Insert(const K& key, const V& value);
   // Removes a value from the hashmap. Ignores keys that do not exist.
   void Remove(const K& key);
   // Gets the value associated with the key, or returns
   // a null pointer.
   V* Get(const K& key);
   // Returns `true` if the key was found in the map, or 
   // false otherwise.
   bool Contains(const K& key);
   // Returns the size of the hashmap.
   int Size() const;

 private:
   // The underlying array.
   std::vector<std::unique_ptr<std::pair<K, V>>> data_;
   // Hash function
   std::function<int(const K&)> hash_function_;

   int num_items_ = 0;
};

template<typename K, typename V>
HashMap<K, V>::HashMap(
   int initial_size, std::function<int(const K&)> hash_function)
   : data_(initial_size), hash_function_(hash_function) {}

template<typename K, typename V>
void HashMap<K, V>::Insert(const K& key, const V& value) {
   int index = hash_function_(key);
   // TODO: Validate that this index is within bounds first.
   std::unique_ptr<std::pair<K, V>>& pair = data_[index];

   // Check for a collision
   if (pair == nullptr) {
      // No collision, safe to insert.
      pair = std::make_unique<std::pair<K, V>>(key, value);
      // Increment the count of num_items_.
      num_items_++;
      return;
   } else if (pair->first == key) {
      // Replace an existing value.
      pair->second = value;
   }

   // TODO: deal with collisions
}

template<typename K, typename V>
void HashMap<K, V>::Remove(const K& key) {
   int index = hash_function_(key);
   // TODO: Validate that this index is within bounds first.
   std::unique_ptr<std::pair<K, V>>& pair = data_[index];

   if (pair != nullptr) {
      pair = nullptr;
      num_items_--;
   }
}

template<typename K, typename V>
V* HashMap<K, V>::Get(const K& key) {
   int index = hash_function_(key);
   // TODO: Validate that this index is within bounds first.
   std::unique_ptr<std::pair<K, V>>& pair = data_[index];

   // If this key is empty, return nullptr.
   // TODO: Add an overload that allows a default value to be passed in.
   if (pair == nullptr) {
      LOG("Couldn't find key");
      return nullptr;
   }

   if (pair->first != key) {
      // We don't have a direct match, linear search.
      LOG("Coudlnt' find match.");
      return nullptr;
   } else {
      return &pair->second;
   }
}

template<typename K, typename V>
bool HashMap<K, V>::Contains(const K& key) {
   int index = hash_function_(key);
   // TODO: Validate that this index is within bounds first.
   std::unique_ptr<std::pair<K, V>>& pair = data_[index];
   return pair != nullptr;
}

template<typename K, typename V>
int HashMap<K, V>::Size() const {
   return num_items_;
}

#endif  // HASHMAP_H
