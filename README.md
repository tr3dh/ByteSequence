<!-- Sprachumschalter -->
<p align="right">
🌐 <a href="./README.md">English</a> | <a href="./README.de.md">Deutsch</a>
</p>

# ByteSequence

ByteSequence is a single-header library for binary object serialization. ByteSequence implements byte streams into which objects can be pushed and from which objects can be popped. 
The focus is on generic serialization (using boost::pfr), the recursive decomposition of dynamic containers (std::vector, std::map, ...), and a minimalist syntax.

# 🧬 Requirements
- Language standard: C++20
- Compiler: gcc/g++
- Platform: Windows

(Not tested on other platforms)

# 🚀 Setup

- Clone ByteSequence into the project
- Obtain the Boost.PFR dependency or have CMake automatically set it up using `add_subdirectory`

```bash
git clone https://github.com/tr3dh/ByteSequence.git src
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/ByteSequence")
```
- include Header

```cpp
#include "path/to/ByteSequence.hpp"
```

# 🔧 Features

- Bytestream interactions `insert`, `extract`, `insertMultiple`, `extractMultiple`, `extractMultipleReversed`, `get<T>()`
- Lazy Syntax `operator +=` / `operator -=`
- file-I/O: `toFile`, `fromFile`
- string-I/O: `toString`, `fromString`
- XOR-Encoder : `encode(std::string)`, `encode(std::string)`

# 🏗️ Support

- Trivial copyable objects (non-dynamic)
- Struct members
- Members of fully public classes
- `std::vector<T>`
- `std::map<Key, Val>`
- `std::unordered_map<Key, Val>`
- `std::multimap<Key, Val>`
- `std::pair<First, Second>`
- `std::string`
- `const char*`, `char*`, `char[N]`

# 🛠️ Usage

The `insertMultiple` and `extractMultipleReversed` methods can be used to insert objects into or extract them from the byte stream. This applies to supported (defined serialization) and trivially copyable elements.

```cpp
ByteSequence bs;

int b, a = 42; double d, c = 3.14;

bs.insertMultiple(a, c);
bs.extractMultiple(b, d);
```

In addition, struct members and class members of fully public classes can be copied. The attributes are parsed by `boost::pfr` and inserted recursively. 

```cpp
struct Whatever{ std::vector<std::vector<std::vector<int>>> m_intVecs; }

ByteSequence bs;
Whatever b, a; a.m_intVecs =  = {{{1,2,3}, {4,5,6}}};

bs += a;
bs -= b;
```
Über den XOR-Encoder können ByteSequences verschlüsselt und entschlüsselt werden.

```cpp
ByteSequence bs;
bs.insert(data);

std::string key = "...";
bs.encode(key);
bs.decode(key);
```

If a data type cannot be serialized using the recursive chaining of trivially copyable or supported dynamic containers, a type-specific serialization can be written. This works with both external and member functions. Serialization of unsupported dynamic containers is also possible. Template-based serializations can be used for this purpose.

```cpp
// Serialization via member functions
struct A {

    void toByteSequence(ByteSequence& seq) const {
        seq.insertMultiple(...);
    }

    void fromByteSequence(ByteSequence& seq) {
        seq.extractMultipleReversed(...);
    }
};

// Serialization via functions
inline void toByteSequence(const A& a, ByteSequence& seq) {
    seq.insertMultiple(...);
}

inline void fromByteSequence(A& a, ByteSequence& seq) {
    seq.extractMultipleReversed(...);
}

// Serialization of a dynamic container, example std::vector
template<typename T>
inline void toByteSequence(const std::vector<T>& member, ByteSequence& seq) {

    for (auto it = member.rbegin(); it != member.rend(); ++it) {

        seq.insert(*it);
    }

    seq.insert(member.size());
}
```

Using the file and string I/O functions, byte caches can be written to and read from strings or files.

```cpp
ByteSequence bs;
bs.insert(data);

bs.toFile("path/to/cache.bin");

ByteSequence cachedBS;
cachedBS.fromFile("path/to/cache.bin");

auto restored = loaded.get<MyType>();
```

# 📊 Limitations / Notes

- Raw `memcpy` is platform-dependent; for cross-platform formats, custom wrappers may need to be implemented
- `boost::pfr` only works with public members and may be affected by specific constructors or class declarations
- Complex types with internal allocation (e.g., sparse matrices) should be explicitly serialized

# 🤝 Many thanks

I would like to express my special thanks to my supervisor Dr. Hendrik Geisler, who was a great help during the development.

Hendrik Geisler was funded by the European Union (ERC, Gen-TSM, project number 101124463) during the supervision period. However, the views and opinions expressed are solely those of the author(s) and do not necessarily reflect those of the European Union or the European Research Council Executive Agency. Neither the European Union nor the funding authority can be held responsible for them.

## 📚 Libraries used
I would also like to thank the respective developers and maintainers of the open-source libraries used in the project.
These are listed below. The corresponding license texts are stored in the [thirdPartyLicenses](/thirdPartyLicenses/) folder.

| Library        | License                          |
|-------------------|----------------------------------|
| [Boost.PFR](https://github.com/boostorg/pfr)        | Boost Software License 1.0       |