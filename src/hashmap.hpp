#ifndef HASHMAP_H
#define HASHMAP_H

#include <functional>
#include <utility>

template<typename K, typename V>
class HashMap {
 public:
   HashMap(int initial_size, std::function<int(K)> hash_function);

   // Inserts a value into the hashmap.
   void Insert(K key, V value);
   // Removes a value from the hashmap.
   void Remove(K key);
   // Gets the value associated with the key, or returns
   // a null pointer.
   V& Get(K key);
   // Returns `true` if the key was found in the map, or 
   // false otherwise.
   bool Contains(K key);
   // Returns the size of the hashmap.
   int Size();
 private:
   // The underlying array.
   std::vector<std::pair<K, V>> data_;
   // Hash function
   std::function<int(K)> hash_function_;

   int items = 0;
    
};

template<typename K, typename V>
HashMap<K, V>::HashMap(
   int initial_size, std::function<int(K)> hash_function)
   : data_(initial_size), hash_function_(hash_function) {}

#endif  // HASHMAP_H
