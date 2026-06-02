# C++ STL Containers and Algorithms - Short Notes

STL means Standard Template Library. It gives ready-made containers, iterators, algorithms, and utility classes. Containers store data, iterators move through data, and algorithms operate on data.

---

## Table of Contents

1. [STL Main Parts](#stl-main-parts)
2. [Container Types](#container-types)
3. [Iterators](#iterators)
4. [Sequence Containers](#sequence-containers)
5. [Associative Containers](#associative-containers)
6. [Unordered Containers](#unordered-containers)
7. [emplace_back vs push_back](#emplace_back-vs-push_back)
8. [STL Algorithms](#stl-algorithms)
9. [Editing Set/Map Keys with extract](#editing-setmap-keys-with-extract)
10. [Best Practices](#best-practices)

---

## STL Main Parts

The STL has four important ideas:

- Containers: store data, like `vector`, `list`, `set`, `map`.
- Iterators: behave like pointers and move through containers.
- Algorithms: functions like `sort`, `find`, `count`, `for_each`.
- Allocators: manage memory internally for containers.

Most containers use an allocator internally to manage memory.

---

## Container Types

STL containers are mainly divided into three groups.

| Type | Containers | Main Use |
|------|------------|----------|
| Sequence containers | `array`, `vector`, `deque`, `list`, `forward_list` | Store elements in a sequence |
| Associative containers | `set`, `multiset`, `map`, `multimap` | Sorted tree-based containers |
| Unordered containers | `unordered_set`, `unordered_multiset`, `unordered_map`, `unordered_multimap` | Hash-table based containers |

Common headers:

```cpp
#include <array>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
```

---

## Iterators

Iterators are used to access elements inside containers.

```cpp
auto begin = container.begin();
auto end = container.end();
```

`begin()` points to the first element.

`end()` points one position after the last element. It is not a valid element.

You can also use free functions:

```cpp
std::begin(container);
std::end(container);
```

Example:

```cpp
std::vector<int> nums {1, 2, 3};

for (auto it = nums.begin(); it != nums.end(); ++it) {
    std::cout << *it << "\n";
}
```

Range-based loop is easier:

```cpp
for (int value : nums) {
    std::cout << value << "\n";
}
```

---

## Sequence Containers

Sequence containers keep elements in a specific order.

### `std::array`

`std::array` is a fixed-size array.

```cpp
std::array<int, 5> arr {1, 2, 3, 4, 5};
```

Properties:

- Size is fixed at compile time.
- Supports iterators.
- Knows its own size.
- Provides random access.
- Can be passed to C-style functions using `.data()`.

```cpp
int* raw = arr.data();
```

### `std::vector`

`std::vector` is a dynamic array.

```cpp
std::vector<int> vec {1, 2, 3, 4, 5};
```

Properties:

- Grows automatically.
- Fast access by index.
- Fast insert/delete at the end.
- Slow insert/delete in the middle or beginning.

Common functions:

```cpp
vec.push_back(10);
vec.pop_back();
vec.insert(vec.begin() + 2, 99);
vec.erase(vec.begin() + 1);
```

Use vector as the default container when you are not sure which container to choose.

### `std::deque`

`std::deque` means double-ended queue.

```cpp
std::deque<int> dq {1, 2, 3};
```

Properties:

- Can grow automatically.
- Fast insert/delete at front and back.
- Provides random access.
- Slow insert/delete in the middle.

Common functions:

```cpp
dq.push_back(4);
dq.push_front(0);
dq.pop_back();
dq.pop_front();
```

### `std::list`

`std::list` is a doubly linked list.

```cpp
std::list<int> values {1, 2, 3};
```

Properties:

- No random access.
- Fast insert/delete anywhere if you already have the iterator.
- Each node stores links, so it uses more memory than vector.

Example:

```cpp
auto it = values.begin();
++it;

values.insert(it, 10);
values.erase(it);
```

### `std::forward_list`

`std::forward_list` is a singly linked list.

```cpp
std::forward_list<int> list {1, 2, 3};
```

Properties:

- One-way linked list.
- Smaller memory usage than `std::list`.
- No random access.
- No `size()` in older standards.
- No `push_back()`.

Common functions:

```cpp
list.push_front(0);
list.insert_after(list.begin(), 10);
list.erase_after(list.begin());
```

---

## Associative Containers

Associative containers are sorted tree-based containers. They store elements in sorted order and provide fast search.

### `std::set` and `std::multiset`

`std::set` stores unique values.

```cpp
std::set<int> s {18, 2, 4, 5, 3};
```

Properties:

- Values are sorted.
- Fast search.
- No random access.
- Values cannot be modified directly.
- Duplicate values are not allowed in `set`.
- Duplicate values are allowed in `multiset`.

Common functions:

```cpp
s.insert(9);
s.erase(2);
s.erase(s.begin());

auto it = s.find(9);

if (it != s.end()) {
    std::cout << "Found\n";
}
```

Sorting order:

```cpp
std::set<int, std::less<int>> lowToHigh;
std::set<int, std::greater<int>> highToLow;
```

`std::less` is the default.

For `multiset`, `equal_range()` gives the range of duplicate values:

```cpp
std::multiset<int> ms {1, 3, 3, 3, 5};

auto range = ms.equal_range(3);

for (auto it = range.first; it != range.second; ++it) {
    std::cout << *it << "\n";
}
```

### `std::map` and `std::multimap`

`std::map` stores key-value pairs.

```cpp
std::map<int, std::string> m {
    {1, "One"},
    {2, "Two"}
};
```

Properties:

- Sorted by key.
- Fast search by key.
- No random access.
- Key cannot be modified directly.
- Value can be modified.
- Duplicate keys are not allowed in `map`.
- Duplicate keys are allowed in `multimap`.

Common functions:

```cpp
m.insert(std::make_pair(3, "Three"));
m.insert(std::pair<int, std::string>(4, "Four"));

m[1] = "Dasun";     // changes value for key 1
m[5] = "Five";      // inserts new key-value pair

auto it = m.find(2);

if (it != m.end()) {
    std::cout << it->first << " " << it->second << "\n";
}
```

Important: `operator[]` can insert a new element if the key does not exist.

For duplicate keys, use `std::multimap`:

```cpp
std::multimap<int, std::string> mm {
    {6, "Dasun"},
    {6, "Madu"}
};

auto range = mm.equal_range(6);
```

---

## Unordered Containers

Unordered containers use hash tables.

```cpp
std::unordered_set<int> us {3, 1, 4};
std::unordered_map<int, std::string> um {
    {1, "One"},
    {2, "Two"}
};
```

Properties:

- Very fast average search, insert, and delete.
- Elements are not sorted.
- Uses buckets internally.
- Needs a hash function for keys.

Containers:

| Container | Meaning |
|-----------|---------|
| `unordered_set` | Unique values |
| `unordered_multiset` | Duplicate values allowed |
| `unordered_map` | Unique keys with values |
| `unordered_multimap` | Duplicate keys with values |

Example:

```cpp
auto it = um.find(2);

if (it != um.end()) {
    std::cout << it->second << "\n";
}
```

---

## `emplace_back` vs `push_back`

`push_back()` adds an already-created object.

```cpp
std::vector<std::string> names;

std::string name = "Dasun";
names.push_back(name);
names.push_back("Madu");
```

`emplace_back()` constructs the object directly inside the container.

```cpp
names.emplace_back("Umar");
```

For simple types like `int`, the difference is usually not important.

For objects, `emplace_back()` can avoid extra copies or moves.

---

## STL Algorithms

STL algorithms work with iterator ranges.

Most algorithms use this pattern:

```cpp
algorithm(container.begin(), container.end(), extra_arguments);
```

Include:

```cpp
#include <algorithm>
```

### `std::sort`

Sort a vector:

```cpp
std::vector<int> nums {4, 1, 5, 2};

std::sort(nums.begin(), nums.end());
```

Sort descending:

```cpp
std::sort(nums.begin(), nums.end(), std::greater<int>());
```

Sort with lambda:

```cpp
std::sort(nums.begin(), nums.end(), [](int a, int b) {
    return a > b;
});
```

Sort objects:

```cpp
struct Employee
{
    std::string name;
    int id;
};

std::vector<Employee> employees {
    {"Nimal", 3},
    {"Dasun", 1},
    {"Madu", 2}
};

std::sort(employees.begin(), employees.end(), [](const Employee& a, const Employee& b) {
    return a.id < b.id;
});
```

Note: `std::sort` needs random access iterators, so it works with `vector`, `array`, and `deque`. It does not work with `list`. For `std::list`, use `list.sort()`.

```cpp
std::list<int> values {3, 1, 2};
values.sort();
```

### `std::find`

Find a value:

```cpp
auto it = std::find(nums.begin(), nums.end(), 5);

if (it != nums.end()) {
    std::cout << "Found\n";
}
```

### `std::find_if`

Find using a condition:

```cpp
auto it = std::find_if(nums.begin(), nums.end(), [](int value) {
    return value > 10;
});
```

The lambda returns `true` when the wanted item is found.

### `std::count`

Count matching values:

```cpp
int count = std::count(nums.begin(), nums.end(), 3);
```

### `std::count_if`

Count using a condition:

```cpp
int evenCount = std::count_if(nums.begin(), nums.end(), [](int value) {
    return value % 2 == 0;
});
```

### `std::for_each`

Call a function or lambda for every element.

```cpp
std::for_each(nums.begin(), nums.end(), [](int value) {
    std::cout << value << "\n";
});
```

Modify elements:

```cpp
std::for_each(nums.begin(), nums.end(), [](int& value) {
    value *= 2;
});
```

### `std::copy`

Copy values from one container to another.

```cpp
std::vector<int> source {1, 2, 3};
std::vector<int> destination;

std::copy(source.begin(), source.end(), std::back_inserter(destination));
```

Needs:

```cpp
#include <iterator>
```

### `std::remove` and erase-remove idiom

`std::remove` does not actually erase elements from a vector. It moves unwanted values to the end and returns the new logical end.

Correct way:

```cpp
std::vector<int> nums {1, 2, 3, 2, 4};

nums.erase(
    std::remove(nums.begin(), nums.end(), 2),
    nums.end()
);
```

For a condition:

```cpp
nums.erase(
    std::remove_if(nums.begin(), nums.end(), [](int value) {
        return value % 2 == 0;
    }),
    nums.end()
);
```

### Common Algorithm List

| Algorithm | Use |
|-----------|-----|
| `sort` | Sort elements |
| `find` | Find exact value |
| `find_if` | Find using condition |
| `count` | Count exact value |
| `count_if` | Count using condition |
| `for_each` | Run callback for each element |
| `copy` | Copy elements |
| `remove` | Move unwanted values to end |
| `remove_if` | Move condition-matching values to end |
| `reverse` | Reverse order |
| `min_element` | Find smallest element |
| `max_element` | Find largest element |

---

## Editing Set/Map Keys with `extract`

In `set` and `map`, keys cannot be modified directly because that would break the sorted order.

Wrong:

```cpp
std::set<int> s {1, 2, 3};

auto it = s.find(2);
// *it = 20; // Error
```

C++17 added `extract()`, which removes a node, lets you edit it, and then insert it back.

For `set`:

```cpp
std::set<int> s {1, 2, 3};

auto node = s.extract(2);

if (!node.empty()) {
    node.value() = 20;
    s.insert(std::move(node));
}
```

For `map`:

```cpp
std::map<int, std::string> m {
    {1, "One"},
    {2, "Two"}
};

auto node = m.extract(2);

if (!node.empty()) {
    node.key() = 20;
    m.insert(std::move(node));
}
```

This is better than erase-and-insert when you want to preserve the stored node and avoid unnecessary allocation.

---

## Best Practices

- Use `std::vector` by default unless another container is clearly better.
- Use `std::array` when size is fixed at compile time.
- Use `std::deque` when you need fast front and back operations.
- Use `std::list` only when frequent middle insert/delete is important and you already have iterators.
- Use `std::set` or `std::map` when sorted order is needed.
- Use unordered containers when fast lookup matters and order does not matter.
- Use algorithms instead of writing manual loops when they make code clearer.
- Use lambdas for custom algorithm conditions.
- Remember that `end()` is one past the last element.
- Do not modify keys directly in `set` or `map`.

---

## Quick Summary

```cpp
std::vector<int> v {1, 2, 3};
v.push_back(4);
v.erase(v.begin());

std::set<int> s {3, 1, 2}; // sorted, unique
s.insert(4);

std::map<int, std::string> m;
m[1] = "One";

std::sort(v.begin(), v.end());
auto it = std::find(v.begin(), v.end(), 2);
int count = std::count(v.begin(), v.end(), 3);
```

Main idea:

- Containers store data.
- Iterators point to container elements.
- Algorithms work on iterator ranges.
- Sequence containers preserve order.
- Associative containers are sorted.
- Unordered containers use hashing.

