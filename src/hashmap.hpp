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
   HashMap(int initial_size);
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
   int GetIndex(const K& key);

   // Resizes and rehashes the underlying array.
   void Rehash();

   // The underlying array.
   std::vector<std::unique_ptr<std::pair<K, V>>> data_;
   // User-specified hash function
   std::function<int(const K&)> hash_function_;
   // Default hash function
   std::hash<K> hasher_;

   int num_items_ = 0;
};

template<typename K, typename V>
HashMap<K, V>::HashMap(
   int initial_size, std::function<int(const K&)> hash_function)
   : data_(initial_size), hash_function_(hash_function) {}

template<typename K, typename V>
HashMap<K, V>::HashMap(int initial_size): data_(initial_size) {}

template<typename K, typename V>
void HashMap<K, V>::Insert(const K& key, const V& value) {
   int index = GetIndex(key);
   std::unique_ptr<std::pair<K, V>>& pair = data_[index];

   // Check for a collision
   if (pair == nullptr) {
      // No collision, safe to insert.
      pair = std::make_unique<std::pair<K, V>>(key, value);
      num_items_++;
      return;
   } else if (pair->first == key) {
      // No collision, replace the existing value.
      pair->second = value;
   }

   // Deal with collisions.
   if ((num_items_ / (double)data_.size()) >= 0.7) {
      // If we're at or above 70% capacity, we need to rehash.
      
      // TODO: Implement the rehash function.
      // Rehash();
      // Insert(key, value);
   } else {
      // Perform a linear search for the next available slot.
      for (int i = index + 1; i < data_.size(); i++) {
         // Check to see whether the slot is empty.
         std::unique_ptr<std::pair<K, V>>& pair = data_[i];
         if (pair == nullptr) {
            // Found an empty slot.
            pair = std::make_unique<std::pair<K, V>>(key, value);
            num_items_++;
            return;
         }
      }

      // We've reached the end and not found an empty slot.
      // Try searching from the front.
      for (int i = 0; i < index; i++) {
         // Check to see whether the slot is empty.
         std::unique_ptr<std::pair<K, V>>& pair = data_[i];
         if (pair == nullptr) {
            // Found an empty slot.
            pair = std::make_unique<std::pair<K, V>>(key, value);
            num_items_++;
            return;
         }
      }

      // This shouldn't be possible, since a rehash should occur
      // before all slots are filled.
      // Rehash();
      // Insert(key, value);
   }
}

template<typename K, typename V>
void HashMap<K, V>::Remove(const K& key) {
   int index = GetIndex(key);
   std::unique_ptr<std::pair<K, V>>& pair = data_[index];

   // TODO: Check for a collision in this, Get, and Contains
   if (pair != nullptr) {
      pair = nullptr;
      num_items_--;
   }
}

template<typename K, typename V>
V* HashMap<K, V>::Get(const K& key) {
   int index = GetIndex(key);
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
   int index = GetIndex(key);
   std::unique_ptr<std::pair<K, V>>& pair = data_[index];
   return pair != nullptr;
}

template<typename K, typename V>
int HashMap<K, V>::Size() const {
   return num_items_;
}

template<typename K, typename V>
int HashMap<K, V>::GetIndex(const K& key) {
   // Use the user-provided hash function only if provided.
   if (hash_function_) {
      return hash_function_(key);
   }

   uint64_t hash = hasher_(key);
   std::string msg = "Hashcode=" + std::to_string(hash);
   LOG(msg);
   int index = hash % data_.size();
   msg = "Adjusted index=" + std::to_string(index);
   LOG(msg);
   return index;
}

#endif  // HASHMAP_H
