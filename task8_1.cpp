#include <iostream>
#include <vector>


class HashFunction {
 public:
  HashFunction(size_t coefficient) : factor(coefficient) {}
  HashFunction() : factor(27644437) {} // простое число

  size_t operator()(const char* str) const;
  size_t operator()(const std::string& str) const;

 private:
  size_t factor;
};

size_t HashFunction::operator()(const char *str) const {
  size_t hash = 0;
  for (; *str != 0; str++) {
    hash = (hash + *str) * factor;
  }
  return hash;
}

size_t HashFunction::operator()(const std::string &str) const {
  size_t hash = 0;
  for (char ch: str) {
    hash = (hash + ch) * factor;
  }
  return hash;
}


class HashTable {
 public:
  HashTable(HashFunction hashFunction, float load_factor);
  HashTable(HashFunction hashFunction) : HashTable(hashFunction, 0.75) {}
  bool Add(const std::string& key);
  bool Delete(const std::string& key);
  bool Has(const std::string& key) const;

 private:
  float load_factor;
  const std::string DELETED = "DELETED";
  std::vector<std::string> keys;
  std::vector<bool> used;
  size_t size;
  size_t keys_count;
  HashFunction hashFunction;

  size_t Testing(size_t probe, size_t idx) const;
  void Rehash();
};

HashTable::HashTable(HashFunction hashFunction, float load_factor) : hashFunction(hashFunction), size(8), keys_count(0), load_factor(load_factor) {
  keys.resize(size);
  used.assign(size, false);
}

bool HashTable::Add(const std::string& key) {
  if (Has(key)) { // if element doesn't exist in table, insert it to nearest empty cell
    return false;
  }

  if (1. * (keys_count + 1) / size >= load_factor) {
    Rehash();
  }

  size_t probe = hashFunction(key);

  for (size_t i = 0; i < size; ++i) {
    probe = Testing(probe, i);
    if (!used[probe]) {
      break;
    }
  }

  used[probe] = true;
  keys[probe] = key;
  ++keys_count;
  return true;
}

bool HashTable::Delete(const std::string& key) {
  size_t probe = hashFunction(key);
  for (size_t i = 0; i < size; ++i) {
    probe = Testing(probe, i);
    if (used[probe] && keys[probe] == key) {
      used[probe] = false;
      keys[probe] = DELETED;
      --keys_count;
      return true;
    }
    if (!used[probe] && keys[probe] != DELETED) {
      return false;
    }
  }
  return false;
}

bool HashTable::Has(const std::string& key) const {
  size_t probe = hashFunction(key);
  for (int i = 0; i < size; ++i) {
    probe = Testing(probe, i);
    if (used[probe] && keys[probe] == key) { // "std::vector<bool> used" used in order to handle case when key == DELETED
      return true;
    }
    if (!used[probe] && keys[probe] != DELETED) {
      return false;
    }
  }
  return false;
}

size_t HashTable::Testing(size_t prev_probe, size_t i) const {
  return (prev_probe + i + 1) % size;
}

void HashTable::Rehash() {
  std::vector<std::string> old_keys(std::move(keys));
  std::vector<bool> old_used(std::move(used));

  keys_count = 0;
  size <<= 1;
  keys = std::vector<std::string>(size);
  used = std::vector<bool>(size, false);
  for (size_t i = 0; i < old_used.size(); ++i) {
    if (old_used[i]) {
      Add(old_keys[i]);
    }
  }
}


int main() {
  HashFunction hash;
  HashTable hashTable(hash);
  char operation;
  std::string str;
  while (std::cin >> operation >> str) {
    bool result;

    switch(operation) {
      case '+':
        result = hashTable.Add(str);
        break;
      case '-':
        result = hashTable.Delete(str);
        break;
      case '?':
        result = hashTable.Has(str);
        break;
    }

    if (result) {
      std::cout << "OK";
    } else {
      std::cout << "FAIL";
    }
    std::cout << std::endl;
  }
  return 0;
}