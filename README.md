# Hashmap
A hashmap I implemented from scratch. I'm planning on adding a concurrent version in the end.
This uses template-programming and allows for any Key/Value to be provided. It uses a default
STL-based hash function, and optionally allows users to specify their own hashing function if
desired.

Has 5 core functions:
- Insert(key, value)
- Get(key)
- Remove(key)
- Contains(key)
- Size()
