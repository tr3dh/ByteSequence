// ----------------------------------------------------------------------------------------------------------------
//
//    ██████╗ ██╗   ██╗████████╗███████╗    ███████╗███████╗ ██████╗ ██╗   ██╗███████╗███╗   ██╗ ██████╗███████╗
//    ██╔══██╗╚██╗ ██╔╝╚══██╔══╝██╔════╝    ██╔════╝██╔════╝██╔═══██╗██║   ██║██╔════╝████╗  ██║██╔════╝██╔════╝
//    ██████╔╝ ╚████╔╝    ██║   █████╗      ███████╗█████╗  ██║   ██║██║   ██║█████╗  ██╔██╗ ██║██║     █████╗  
//    ██╔══██╗  ╚██╔╝     ██║   ██╔══╝      ╚════██║██╔══╝  ██║▄▄ ██║██║   ██║██╔══╝  ██║╚██╗██║██║     ██╔══╝  
//    ██████╔╝   ██║      ██║   ███████╗    ███████║███████╗╚██████╔╝╚██████╔╝███████╗██║ ╚████║╚██████╗███████╗
//    ╚═════╝    ╚═╝      ╚═╝   ╚══════╝    ╚══════╝╚══════╝ ╚══▀▀═╝  ╚═════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝╚══════╝
// ----------------------------------------------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2025-2026 Tim Rother, Dr. Hendrik Geisler
// 
// All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// ---------------------------------------------------------------------------------
                                                                                                  
#pragma once

// Std Includes
#include <type_traits>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cassert>
#include <memory>
#include <cstring>

// External
#include <boost/pfr.hpp>

// Decls
namespace fs = std::filesystem;
typedef uint8_t Byte;

// Assertion Handler:
//
// connect a Funktion wich provides your case-specific Assertion Logging
// if no Funktion is connected the output will be provided via stdout
//
// For Example:
// 
// static void triggerAssertHandler(const char* message){
    
//     RETURNING_ASSERT(TRIGGER_ASSERT, message,);
// }
//
// G_BYTESEQ_ASSERT_HANDLER = triggerAssertHandler;

// Function Pointer as Handler
inline void (*G_BYTESEQ_ASSERT_HANDLER)(const char* message) = nullptr;

// Helper Makros
template <typename T, template <typename...> class Template>
struct IsInstanceOf : std::false_type {};

template <template <typename...> class Template, typename... Args>
struct IsInstanceOf<Template<Args...>, Template> : std::true_type {};

// The following templates check whether the functions `to` and `fromByteSequence`
// exist for the corresponding types or whether the types have these functions as attributes
// Accordingly, in template-based logic, `if constexpr` can be used to check whether these functions
// have been defined for the corresponding types, and if so, they can then be executed

// SFINAE (Substitution Failure Is Not An Error) is a template logic in C++
// that allows you to check whether a type has certain functions or attributes
// without the compiler throwing an error if they are not present.
// This means that if the substitution fails when attempting to use a function or a member,
// it is simply ignored and not treated as an error.
// Thus, in template logic using `if constexpr` or template specializations,
// you can easily check whether a function or a member exists,
// and only then execute the corresponding code.

// fwddecl
struct ByteSequence;

// Checks for Memberfunctions
template<typename, typename = std::void_t<>>
struct hasFromByteSequenceAttrib : std::false_type {};

template<typename T>
struct hasFromByteSequenceAttrib<T, std::void_t<
    decltype(std::declval<T&>().fromByteSequence(std::declval<ByteSequence&>()))
>> : std::true_type {};

// Checks for independent Functions
template<typename, typename = std::void_t<>>
struct hasToByteSequenceAttrib : std::false_type {};

template<typename T>
struct hasToByteSequenceAttrib<T, std::void_t<
    decltype(std::declval<const T&>().toByteSequence(std::declval<ByteSequence&>()))
>> : std::true_type {};

//
template<typename, typename = std::void_t<>>
struct hasFromByteSequence : std::false_type {};

template<typename T>
struct hasFromByteSequence<T, std::void_t<
    decltype(fromByteSequence(std::declval<T&>(), std::declval<ByteSequence&>()))
>> : std::true_type {};

//
template<typename, typename = std::void_t<>>
struct hasToByteSequence : std::false_type {};

template<typename T>
struct hasToByteSequence<T, std::void_t<
    decltype(toByteSequence(std::declval<const T&>(), std::declval<ByteSequence&>()))
>> : std::true_type {};

// Macros that can be used to override the ByteSequence interaction with a specific structure
// In most specific cases, the functions fromByteSequence / toByteSequence or
// member.fromByteSequence / member.toByteSequence are more suitable 

// Calls to member attributes must be made using member.ATTRIB
#define OVERRIDE_BYTESEQUENZ_SERIALIZATION(STRUCTURE, INSERT_LOGIK, EXTRACT_LOGIK) \
    template<>\
    void ByteSequence::insert<STRUCTURE>(const STRUCTURE& member) {\
        INSERT_LOGIK;\
    }\
    \
    template<>\
    void ByteSequence::extract<STRUCTURE>(STRUCTURE& member) {\
        EXTRACT_LOGIK;\
    }

// Calls to member attributes must be made using member.ATTRIB
#define OVERRIDE_BYTESEQUENZ_SERIALIZATION_INLINE(STRUCTURE, INSERT_LOGIK, EXTRACT_LOGIK) \
    template<>\
    inline void ByteSequence::insert<STRUCTURE>(const STRUCTURE& member) {\
        INSERT_LOGIK;\
    }\
    \
    template<>\
    inline void ByteSequence::extract<STRUCTURE>(STRUCTURE& member) {\
        EXTRACT_LOGIK;\
    }

// # ByteSequence
// For the following byte sequence, `extract` and `insert` are called recursively until the extracted members have static sizes
// and are then inserted or removed via `memcopy`
// This allows any structure, e.g., `vector<vector<vector<vector<int>>>>`, to be inserted or extracted as long as the behavior is defined for `vector`
// Additionally, for custom classes, the processing of class members with dynamic attributes
// can be adapted to a sequential call sequence using custom insert/extract logic
// Standard types such as std::vector or std::map can be accessed
// Logic for these is already implemented
// This logic definition or overriding of the standard behavior can be set via the macro
// ```OVERRIDE_BYTESEQUENZ_SERIALIZATION(STRUCTURE, INSERT_LOGIC, EXTRACT_LOGIC)```
// Class attributes must be accessed via `member.`, and when calling Extract/Insert, the Extract must be performed in the reverse order of the Insert
//
//
// ```c++
// struct A{int b; char c;};
//
// OVERRIDE_BYTESEQUENZ_SERIALIZATION(A,
//    insertMultiple(member.b, member.c),
//    extractMultiple(member.c, member.b))
// ```
// 
// If the class is declared directly and private attributes need to be initialized as well,
// serialization can also be performed directly within the class declaration
//
// ```c++
// struct A{
// 
// public: // nur bei Klasse nötig
//     
//      void toByteSequence(ByteSequence& seq) const {
//          seq.insertMultiple(a,b);
//          seq.encode(...);
//      }
//
//      void fromByteSequence(ByteSequence& seq){
//          seq.extractMultiple(b,a);
//          seq.decode(...);
//      }
// 
// private:
//      int b; std::string c;
// };
// ```
// 
// Another option for serialization is to write external serialization functions for classes
// This is useful when conflicts arise in the insert/extract template overrides due to numerous typedefs
// In general, serialization functions are recommended for larger and more complex declarations
// 
// ```c++
// struct A{ ... };
//
//     
// void toByteSequence(const A& member, ByteSequence& seq) const {
//     seq.insertMultiple(...);
//     seq.encode(...);
// }
//
// void fromByteSequence(A& member, ByteSequence& seq){
//     seq.extractMultiple(...);
//     seq.decode(...);
// }
// ```
//
// For types that depend exclusively on standard types, their vectors, maps, or other already serialized types
// the function can handle serialization via pfr itself
// This allows most custom objects to be used without further declarations
// Exceptions are complex, self-allocating objects such as Symengine or Eigen matrices
// However, once serialized, they can simply be used as public attributes of custom classes, vectors, maps, etc.
// and automatically passed on in the background via PFR unpacking or handed over to the recursive call.
//
// For structs, PFR automatically detects their public fields, unpacks them
// and passes them directly to the desired function, in this case recursion or serialization.
// Note that PFR and automatic serialization work ONLY for public attributes
// and there may be issues with defined constructors. These cases must be handled via explicit serialization.
// All functions of the ByteSequence can be accessed
// e.g., via a class-internal encryption or similar
// When implementing serialization, it is essential to note that the insert order must be specified in reverse of
// the extract order. If the same order is to be specified for both insert and extract, the extractMultipleReversed function
// must be used
struct ByteSequence{

    // Actual container for the data
    // This vector holds the byte sequence, and the entire struct contains only a few wrapper functions
    // that allow for conversion, insertion, extraction, and other operations
    std::vector<Byte> bytes = {};
    
    // Print the sequence; invisible characters are not displayed
    friend std::ostream& operator<<(std::ostream& os, const ByteSequence& sequence) {
        
        //
        os << "ByteSequence " << sequence.bytes.size() << " bytes [";

        //
        for(const auto& byte : sequence.bytes){

            os << byte;
        }

        //
        os << "]";
        return os;
    }

    // Size of the Sequence
    size_t size(){
        
        return bytes.size();
    }

    // Simple conversion via the constructor
    std::string toString(){

        return std::string(bytes.begin(), bytes.end());
    }

    // Simple conversion via the constructor
    void fromString(const std::string& str){

        bytes = std::vector<Byte>(str.begin(), str.end());
    }

    // Save in file
    void toFile(const std::string& file){

        // Create parent if it doesn't exist
        fs::create_directories(fs::path(file).parent_path());

        //
        std::ofstream outFile(file, std::ios::binary);

        //
        if(outFile){
            outFile << toString();
            outFile.close();
        }
    }

    // Load from file
    void fromFile(const std::string& file){

        std::ifstream inFile(file, std::ios::binary);

        if(inFile){

            std::ostringstream ss;
            ss << inFile.rdbuf();
            inFile.close();

            fromString(ss.str());
        }
    }

    // Performs an insert operation using memcopy to copy the entire contents of a static member into the byte vector
    template<typename T>
    void insertStaticMember(const T& member){

        // Sequence Größe cachen und erweiterung des byteVektors um benötigte Größe
        const size_t sequenceSize = bytes.size();
        bytes.resize(sequenceSize + sizeof(T));

        // memcopy aus source objekt in byte Vector
        std::memcpy(bytes.data() + sequenceSize, &member, sizeof(T));
    }

    // Extract, uses memcopy to copy the contents of the byte vector, up to the size of the static member, into the member and then
    // deletes them from the vector
    template<typename T>
    void extractStaticMember(T& member){

        // Check whether the sequence still contains enough bytes to fill the member
        if(G_BYTESEQ_ASSERT_HANDLER != nullptr && bytes.size() < sizeof(T)){

            (*G_BYTESEQ_ASSERT_HANDLER)("The byte sequence does not contain enough bytes for extraction");
            return;
        }
        else if(bytes.size() < sizeof(T)){

            std::cout << "The byte sequence does not contain enough bytes for extraction" << std::endl;
            return;
        }

        // Cache sequence size and copy byteVector to a referenced member
        const size_t sequenceSize = bytes.size() - sizeof(T);

        //
        std::memcpy(&member, bytes.data() + sequenceSize, sizeof(T));

        //
        bytes.resize(sequenceSize);
    }

    // Recursive wrapper that recursively calls the respective nested insert function until
    // the final insert can be performed via a static member
    template<typename T>
    void insert(const T& member){

        if constexpr (hasToByteSequenceAttrib<T>::value) {

            // Call the member function and pass the byte sequence by reference
            member.toByteSequence(*this);
        }
        else if constexpr (hasToByteSequence<T>::value){

            toByteSequence(member, *this);
        }
        // Call the `insert` function for static members because the type is one of the standard sizes with a static memory size
        // or has a static memory size for other reasons
        else if constexpr(std::is_fundamental<T>::value ||
            std::is_trivially_copyable<T>::value && std::is_standard_layout<T>::value){
                                                            
            insertStaticMember(member);
        }
        else {

            //
            constexpr std::size_t nFields = boost::pfr::tuple_size_v<T>;
            auto memberAsTuple = boost::pfr::structure_to_tuple(member);

            // index für pfr feld Liste extrahieren
            [this, &memberAsTuple]<std::size_t... Is>(std::index_sequence<Is...>) {

                // The loop variable Is runs from front to back
                // >> reverse over
                (..., insert(std::get<nFields - 1 - Is>(memberAsTuple)));
            }(std::make_index_sequence<nFields>{});
        }
    }

    // Recursive wrapper that recursively calls the respective nested `extract` function until
    // the final `extract` can be performed via a static member 
    template<typename T>
    void extract(T& member){

        if constexpr (hasFromByteSequenceAttrib<T>::value) {

            //
            member.fromByteSequence(*this);
        }
        else if constexpr (hasFromByteSequence<T>::value){

            fromByteSequence(member, *this);
        }
        // Call the `extract` function for static members because the type is one of the standard sizes with a static memory size
        // or has a static memory size for other reasons
        else if constexpr(std::is_fundamental<T>::value ||
            std::is_trivially_copyable<T>::value && std::is_standard_layout<T>::value){
                                                            
            extractStaticMember(member);
        }
        else {

            boost::pfr::for_each_field(member, [this](auto& field) {
                extract(field);
            });
        }
    }

    // Wrapper that calls `insert` for a member list
    template<typename... Ts>
    void insertMultiple(const Ts&... members) {

        // Fold über Komma
        (insert(members), ...);
    }

    // Wrapper that calls `extract` for a member list
    template<typename... Ts>
    void extractMultiple(Ts&... members){

        // Fold über Komma
        (extract(members), ...);
    }

    // Wrapper that calls `extract` reversed for a member list
    template <typename... Ts>
    void extractMultipleReversed(Ts&... members) {
        
        auto args = std::forward_as_tuple(members...);
        constexpr size_t size = sizeof...(members);

        [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            (extract(std::get<size - 1 - Is>(args)), ...);
        }(std::make_index_sequence<sizeof...(members)>());
    }

    // # Shiffre Encoder
    // Encoding using a simple shiffre, i.e., token shifting
    // Since the higher-order bytes in the string conversion sometimes
    // represent character sequences rather than individual characters,
    // the resulting string is often larger than the string for the actual vector
    // >> inefficient for storage and not very secure
    void encode(const int& shiffreKey){

        for(uint8_t& byte : bytes){
            byte += shiffreKey;
        }
    }

    //
    void decode(const int& shiffreKey){

        encode(-shiffreKey);
    }

    // # XOR Encoder
    // Takes a byte sequence and XORs every nth character in the sequence
    // with the nth character in the key
    // The XOR (exclusive OR, operator^) is used for this operation
    // This is reversible
    // i.e., a * = a ^ b --> a = a * ^ b
    // Therefore, a byte stream encrypted with this encoding
    // can be decoded by calling the encoding function again
    // The only important thing is that the key remains the same 
    void encode(const uint8_t* key, size_t key_len) {
        for (size_t i = 0; i < bytes.size(); ++i) {
            bytes[i] ^= key[i % key_len];
        }
    }
    
    void decode(const uint8_t* key, size_t key_len) {
        encode(key, key_len);
    }

    // # String wrapper for XOR encoder
    // Takes a string and converts it into a byte sequence for the XOR encoder
    void encode(const std::string& key) {

        encode(reinterpret_cast<const uint8_t*>(key.data()), key.size());
    }

    // # String wrapper for XOR encoder
    // Takes a string and converts it into a byte sequence for the XOR encoder
    void decode(const std::string& key) {

        encode(key);
    }

    // Lazy syntax Funktionen

    //
    template<typename T>
    void operator +=(const T& member){

        insert(member);
    }

    //
    template<typename T>
    void operator -=(T& member){

        extract(member);
    }

    // Konstruiert member über angegeben Konstruktor Argumente und befüllt ihn dnn
    // braucht im Gegensatz zu extract keine Referenz
    // >> Objekt muss im Vorhinein nicht konstruiert werden sonder kann mit get initialisiert werden
    template<typename T, typename... Args>
    T get(Args&&... args){

        T member(std::forward<Args>(args)...);
        extract(member);

        return member;
    }
};

// ByteSequence already comes with a basic selection of implemented serialized dynamic containers

// Vektor
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

// map
template<typename Key, typename Val>
inline void toByteSequence(const std::map<Key, Val>& member, ByteSequence& seq) {

    for (auto it = member.rbegin(); it != member.rend(); ++it) {

        seq.insertMultiple(it->second, it->first);
    }

    seq.insert(member.size());
}

template<typename Key, typename Val>
inline void fromByteSequence(std::map<Key, Val>& member, ByteSequence& seq) {

    size_t size = seq.get<size_t>();

    for (size_t i = 0; i < size; i++) {

        Key key; Val val;
        
        seq.extractMultipleReversed(val, key);
        member.emplace(key, val);
    }
}

// unordered_map
template<typename Key, typename Val>
inline void toByteSequence(const std::unordered_map<Key, Val>& member, ByteSequence& seq) {

    for (const auto& [key, val] : member) {

        seq.insertMultiple(val, key);
    }

    seq.insert(member.size());
}

template<typename Key, typename Val>
inline void fromByteSequence(std::unordered_map<Key, Val>& member, ByteSequence& seq) {

    size_t size = seq.get<size_t>();

    for (size_t i = 0; i < size; i++) {

        Key key; Val val;

        seq.extractMultipleReversed(val, key);
        member.emplace(key, val);
    }
}

// multimap
template<typename Key, typename Val>
inline void toByteSequence(const std::multimap<Key, Val>& member, ByteSequence& seq) {

    for (auto it = member.rbegin(); it != member.rend(); ++it) {

        seq.insertMultiple(it->second, it->first);
    }

    seq.insert(member.size());
}

template<typename Key, typename Val>
inline void fromByteSequence(std::multimap<Key, Val>& member, ByteSequence& seq) {

    size_t size = seq.get<size_t>();

    for (size_t i = 0; i < size; i++) {

        Key key; Val val;

        seq.extractMultipleReversed(val, key);
        member.emplace(key, val);
    }
}

// Pair
template<typename First, typename Second>
inline void toByteSequence(const std::pair<First, Second>& member, ByteSequence& seq) {

    seq.insertMultiple(member.first, member.second);
}

template<typename First, typename Second>
inline void fromByteSequence(std::pair<First, Second>& member, ByteSequence& seq) {

    seq.extractMultipleReversed(member.first, member.second);
}

// string
inline void toByteSequence(const std::string& member, ByteSequence& seq) {

    for (auto it = member.rbegin(); it != member.rend(); ++it) {

        seq.insert(*it);
    }

    seq.insert(member.size());
}

inline void fromByteSequence(std::string& member, ByteSequence& seq) {

    size_t size = seq.get<size_t>();

    member.resize(size);
    for (size_t i = 0; i < size; i++) {

        member[i] = seq.get<char>();
    }
}

// const char*
inline void toByteSequence(const char* member, ByteSequence& seq) {

    seq.insert(std::string(member));
}

// char*
inline void toByteSequence(char* member, ByteSequence& seq) {

    seq.insert(std::string(member));
}

// char[]
template<size_t N>
inline void toByteSequence(const char (&member)[N], ByteSequence& seq) {

    seq.insert(std::string(member, N - 1));
}