<!-- Sprachumschalter -->
<p align="right">
🌐 <a href="./README.md">English</a> | <a href="./README.de.md">Deutsch</a>
</p>

# ByteSequence

ByteSequence ist eine Single-Header-Lib für binäre Objekt-Serialisierung. ByteSequence implementiert Bytestreams, in die Objekte geschoben und aus denen Objekte entnommen werden können. 
Der Fokus liegt dabei auf der generischer Serialisierung (nutzt boost::pfr), der rekursiven Zerlegung dynamischer Container (std::vector, std::map, ...) und einer minimalistischen Syntax.

# 🧬 Anforderungen
- Sprachstandard : C++20
- Compiler : gcc/g++
- Plattform : Windows

Lib wurde bisher nicht anderweitig getestet

# 🚀 SetUp

- ByteSequence ins Projekt clonen
- Abhängigkeit Boost.PFR beschaffen oder über CMake `add_subdirectory` automatisch aufsetzen lassen und linken

```bash
git clone https://github.com/tr3dh/ByteSequence.git src

# In eigener CMakeLists.txt
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/ByteSequence")
target_link_libraries(... ... ByteSequence)
```
- Header einbinden

```cpp
#include "path/to/ByteSequence.hpp"
```

# 🔧 Features

- Bytestream interaktionen über `insert`, `extract`, `insertMultiple`, `extractMultiple`, `extractMultipleReversed`, `get<T>()`
- Lazy Syntax `operator +=` / `operator -=`
- Datei-I/O: `toFile`, `fromFile`
- String-I/O: `toString`, `fromString`
- XOR-Encoder : `encode(std::string)`, `decode(std::string)`

# 🏗️ Unterstützung

- Trivial Kopierbare Objekte (nicht dynamisch)
- Struct-Member
- Member vollständig öffentlicher Klassen
- `std::vector<T>`
- `std::map<Key, Val>`
- `std::unordered_map<Key, Val>`
- `std::multimap<Key, Val>`
- `std::pair<First, Second>`
- `std::string`
- `const char*`, `char*`, `char[N]`

# 🛠️ Verwendung

Über `insertMultiple` und `extractMultipleReversed` können Objekte in den Bytestream geschoben oder daraus entnommen werden. Das gilt für unterstützte (definierte Serialisierung) und trivial kopierbare Elemente. 

```cpp
ByteSequence bs;

int b, a = 42; double d, c = 3.14;

bs.insertMultiple(a, c);
bs.extractMultipleReverse(b, d);
```

Zudem können Struct-Member und Klassen-Member vollständig öffentlicher Klassen kopiert werden. Die Attribute werden dabei durch `boost::pfr` aufgeschüssselt und rekursiv eingeschoben. 

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

Kann ein Datentyp nicht über die rekursive Verschaltung von trivial kopierbaren oder unterstützten dynamischen Container serialisiert werden, kann eine typenspezifische Serialisierung geschrieben. Das funktioniert sowohl über externe als auch Memberfunktionen. Die Serialisierung von nicht unterstützten dynamischen Container ist ebenfalls möglich. Dazu können templatebasierte Serialisierungen genutzt werden.

```cpp
// Serialisierung über Memberfuntktionen
struct A {

    void toByteSequence(ByteSequence& seq) const {
        seq.insertMultiple(...);
    }

    void fromByteSequence(ByteSequence& seq) {
        seq.extractMultipleReversed(...);
    }
};

// Serialisierung über externe Funktionen
inline void toByteSequence(const A& a, ByteSequence& seq) {
    seq.insertMultiple(...);
}

inline void fromByteSequence(A& a, ByteSequence& seq) {
    seq.extractMultipleReversed(...);
}

// Serialisierung eines dynamischen Container am Beispiel std::vector
template<typename T>
inline void toByteSequence(const std::vector<T>& member, ByteSequence& seq) {

    for (auto it = member.rbegin(); it != member.rend(); ++it) {

        seq.insert(*it);
    }

    seq.insert(member.size());
}

template<typename T>
inline void fromByteSequence(std::vector<T>& member, ByteSequence& seq) {

    size_t size = seq.get<size_t>();

    member.resize(size);
    for (size_t i = 0; i < size; i++) {

        member[i] = seq.get<T>();
    }
}
```

Über die Datei- und String-I/O Funktionen können ByteCaches in Strings oder Files geschrieben und daraus entnommen werden.

```cpp
ByteSequence bs;
bs.insert(data);

bs.toFile("path/to/cache.bin");

ByteSequence cachedBS;
cachedBS.fromFile("path/to/cache.bin");

auto restoredData = cachedBS.get<...>();

```
# 📊 Limitierungen / Hinweise

- Rohes `memcpy` ist plattformabhängig, für plattformübergreifende Formate müssen unter Umständen eigene Wrapper implementiert werden
- `boost::pfr` funktioniert nur mit öffentlichen Membern und kann von speziellen Konstruktoren/Klassendeklarationen beeinflusst werden
- Komplexe Typen mit interner Allokation (z. B. sparse Matritzen) sollten explizit serialisiert werden

# 🤝 Danksagung

Mein besonderer Dank gilt meinem Betreuer Dr. Hendrik Geisler, der während der Entwicklung eine große Hilfe war.

Hendrik Geisler wurde während der Betreuungszeit von der Europäischen Union (ERC, Gen-TSM, project number 101124463) finanziert. Die geäußerten Ansichten und Meinungen sind jedoch ausschließlich die des Autors/der Autoren und spiegeln nicht unbedingt die der Europäischen Union oder der Exekutivagentur des Europäischen Forschungsrats wider. Weder die Europäische Union noch die Bewilligungsbehörde können für sie verantwortlich gemacht werden.

## 📚 Verwendete Bibliotheken
Bedanken möchte ich mich zudem bei den jeweiligen Entwicklern und Maintainern der im Rahmen des Projekts verwendeten Open-Source Bibliotheken.
Diese sind im Folgenden aufgeführt. Die zugehörigen Lizenztexte sind im Ordner [thirdPartyLicenses](/thirdPartyLicenses/) hinterlegt.

| Bibliothek        | Lizenz                          |
|-------------------|----------------------------------|
| [Boost.PFR](https://github.com/boostorg/pfr)        | Boost Software License 1.0       |
