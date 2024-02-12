#ifndef HASHMAP_H
#define HASHMAP_H

#include <functional>
#include <iostream>
#include <utility>

namespace {

#define LOG(msg) \
    std::cout << __FILE__ << "(" << __LINE__ << ") INFO: " \
      << msg << std::endl;
#define LOG_ERR(msg) \
    std::cerr << __FILE__ << "(" << __LINE__ << ") ERROR: " \
      << msg << std::endl;

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
   // Helper function to get the hashed index of the specified key.
   int GetIndex(const K& key);

   // Helper function to find the next free slot by performing a
   // linear search in the underlying `data_` array. Used when a
   // collision occurs. Returns -1 if full.
   int GetNextAvailIndex(int start_index);

   // Helper function to perform a linear search of the underlying
   // `data_` array, to find a matching key value. Used when a
   // collision occurs. Returns -1 if there was no match.
   int LinearSearch(const K& key, int start_index);

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
      return;
   }

   // Deal with collisions.
   if ((num_items_ / (double)data_.size()) >= 0.7) {
      // If we're at or above 70% capacity, we need to rehash.
      
      // TODO: Implement the rehash function.
      // Rehash();
      // Insert(key, value);
      LOG("(Not implemented) At capacity. Rehashing.");
   } else {
      index = GetNextAvailIndex(/*start_index=*/index + 1);

      if (index == -1) {
         // This shouldn't be possible, since a rehash should occur
         // before all slots are filled.
         // Rehash();
         // Insert(key, value);
         LOG_ERR("Failed to insert. Hashmap is in invalid state.");
         return;
      }
      data_[index] = std::make_unique<std::pair<K, V>>(key, value);
      num_items_++;
   }
}

template<typename K, typename V>
void HashMap<K, V>::Remove(const K& key) {
   int index = GetIndex(key);
   std::unique_ptr<std::pair<K, V>>& pair = data_[index];

   // TODO: Check for a collision in this, Get, and Contains
   if (pair != nullptr) {
      // Check to make sure this isn't a collision.
      if (pair->first == key) {
         // No collision, safe to delete.
         pair = nullptr;
         num_items_--;
      } else {
         // We have a collision. Linearly search to find the slot.
         index = LinearSearch(key, /*start_index=*/index + 1);
         if (index != -1) {
            data_[index] = nullptr;
            num_items_--;
         }
      }
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
      // We had a collision, search linearly to find a match.
      index = LinearSearch(key, /*start_index=*/index + 1);
      if (index == -1) {
         LOG_ERR("Failed to get value, invalid index from collision.");
         return nullptr;
      }
      return &(data_[index]->second);
   } else {
      return &pair->second;
   }
}

template<typename K, typename V>
bool HashMap<K, V>::Contains(const K& key) {
   int index = GetIndex(key);
   std::unique_ptr<std::pair<K, V>>& pair = data_[index];

   if (pair != nullptr) {
      // Check for a collision.
      if (pair->first != key) {
         index = LinearSearch(key, /*start_index=*/index + 1);
         return index != -1;
      }
      return true;
   }

   return false;
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

template<typename K, typename V>
int HashMap<K, V>::GetNextAvailIndex(int start_index) {
   // Perform a linear search for the next available slot.
   for (int i = start_index; i < data_.size(); i++) {
      // Check to see whether the slot is empty.
      std::unique_ptr<std::pair<K, V>>& pair = data_[i];
      if (pair == nullptr) {
         // Found an empty slot.
         return i;
      }
   }

   // Reached the end without finding an open slot. Start
   // from the beginning and search.
   for (int i = 0; i < start_index; i++) {
      // Check to see whether the slot is empty.
      std::unique_ptr<std::pair<K, V>>& pair = data_[i];
      if (pair == nullptr) {
         // Found an empty slot.
         return i;
      }
   }

   // No slots were found (this should never happen).
   return -1;
}

template<typename K, typename V>
int HashMap<K,V>::LinearSearch(const K& key, int start_index) {
   // Perform a linear search to find a key match.
   for (int i = start_index; i < data_.size(); i++) {
      // Check to see whether the slot is a match.
      std::unique_ptr<std::pair<K, V>>& pair = data_[i];
      if (pair != nullptr && pair->first == key) {
         // Found a match.
         return i;
      }
   }

   // Reached the end without finding an open slot. Start
   // from the beginning and search.
   for (int i = 0; i < start_index; i++) {
      // Check to see whether the slot is a match.
      std::unique_ptr<std::pair<K, V>>& pair = data_[i];
      if (pair != nullptr && pair->first == key) {
         // Found a match.
         return i;
      }
   }

   // No match was found.
   return -1;
}

#endif  // HASHMAP_H
