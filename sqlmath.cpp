/*jslint-disable*/
/*
shRawLibFetch
{
    "fetchList": [
        {
            "comment": true,
            "url": "https://github.com/nodejs/node-addon-api/blob/3.2.1/LICENSE.md"
        },
        {
            "url": "https://github.com/nodejs/node-addon-api/blob/3.2.1/napi.h"
        },
        {
            "url": "https://github.com/nodejs/node-addon-api/blob/3.2.1/napi-inl.h"
        },
        {
            "comment": true,
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/LICENSE"
        },
        {
            "comment": true,
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/deps/common-sqlite.gypi"
        },
        {
            "comment": true,
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/deps/sqlite3.gyp"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/threading.h"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/macros.h"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/async.h"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/database.h"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/backup.h"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/statement.h"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/database.cc"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/backup.cc"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/statement.cc"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/node_sqlite3.cc"
        }
    ],
    "replaceList": [
        {
            "aa": "^#include ",
            "bb": "// $&",
            "flags": "gm"
        }
    ]
}
-# include "napi-inl.deprecated.h"
+// hack-sqlite
+// # include "napi-inl.deprecated.h"

-#define SRC_NAPI_H_
+// hack-sqlite
+#define SRC_NAPI_H_
+#include <algorithm>
+#include <assert.h>
+#include <cstdlib>
+#include <cstring>
+#include <functional>
+#include <initializer_list>
+#include <memory>
+#include <mutex>
+#include <node_api.h>
+#include <queue>
+#include <set>
+#include <sqlite3.h>
+#include <sstream>
+#include <stdint.h>
+#include <string.h>
+#include <string>
+#include <type_traits>
+#include <uv.h>
+#include <vector>

-#endif // SRC_NAPI_H_
+// hack-sqlite
+// #endif // SRC_NAPI_H_

-#endif // SRC_NAPI_INL_H_
+// hack-sqlite
+#endif // SRC_NAPI_INL_H_
+#endif // SRC_NAPI_H_
*/


/*
repo https://github.com/nodejs/node-addon-api/tree/3.2.1
committed 2021-05-28T18:09:02Z
*/


/*
file https://github.com/nodejs/node-addon-api/blob/3.2.1/LICENSE.md
*/
/*
The MIT License (MIT)
=====================

Copyright (c) 2017 Node.js API collaborators
-----------------------------------

*Node.js API collaborators listed at <https://github.com/nodejs/node-addon-api#collaborators>*

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


/*
file https://github.com/nodejs/node-addon-api/blob/3.2.1/napi.h
*/
#ifndef SRC_NAPI_H_
// hack-sqlite
#define SRC_NAPI_H_
#include <algorithm>
#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <memory>
#include <mutex>
#include <node_api.h>
#include <queue>
#include <set>
#include <sqlite3.h>
#include <sstream>
#include <stdint.h>
#include <string.h>
#include <string>
#include <type_traits>
#include <uv.h>
#include <vector>

// #include <node_api.h>
// #include <functional>
// #include <initializer_list>
// #include <memory>
// #include <mutex>
// #include <string>
// #include <vector>

// VS2015 RTM has bugs with constexpr, so require min of VS2015 Update 3 (known good version)
#if !defined(_MSC_VER) || _MSC_FULL_VER >= 190024210
#define NAPI_HAS_CONSTEXPR 1
#endif

// VS2013 does not support char16_t literal strings, so we'll work around it using wchar_t strings
// and casting them. This is safe as long as the character sizes are the same.
#if defined(_MSC_VER) && _MSC_VER <= 1800
static_assert(sizeof(char16_t) == sizeof(wchar_t), "Size mismatch between char16_t and wchar_t");
#define NAPI_WIDE_TEXT(x) reinterpret_cast<char16_t*>(L ## x)
#else
#define NAPI_WIDE_TEXT(x) u ## x
#endif

// If C++ exceptions are not explicitly enabled or disabled, enable them
// if exceptions were enabled in the compiler settings.
#if !defined(NAPI_CPP_EXCEPTIONS) && !defined(NAPI_DISABLE_CPP_EXCEPTIONS)
  #if defined(_CPPUNWIND) || defined (__EXCEPTIONS)
    #define NAPI_CPP_EXCEPTIONS
  #else
    #error Exception support not detected. \
      Define either NAPI_CPP_EXCEPTIONS or NAPI_DISABLE_CPP_EXCEPTIONS.
  #endif
#endif

#ifdef _NOEXCEPT
  #define NAPI_NOEXCEPT _NOEXCEPT
#else
  #define NAPI_NOEXCEPT noexcept
#endif

#ifdef NAPI_CPP_EXCEPTIONS

// When C++ exceptions are enabled, Errors are thrown directly. There is no need
// to return anything after the throw statements. The variadic parameter is an
// optional return value that is ignored.
// We need _VOID versions of the macros to avoid warnings resulting from
// leaving the NAPI_THROW_* `...` argument empty.

#define NAPI_THROW(e, ...)  throw e
#define NAPI_THROW_VOID(e)  throw e

#define NAPI_THROW_IF_FAILED(env, status, ...)           \
  if ((status) != napi_ok) throw Napi::Error::New(env);

#define NAPI_THROW_IF_FAILED_VOID(env, status)           \
  if ((status) != napi_ok) throw Napi::Error::New(env);

#else // NAPI_CPP_EXCEPTIONS

// When C++ exceptions are disabled, Errors are thrown as JavaScript exceptions,
// which are pending until the callback returns to JS.  The variadic parameter
// is an optional return value; usually it is an empty result.
// We need _VOID versions of the macros to avoid warnings resulting from
// leaving the NAPI_THROW_* `...` argument empty.

#define NAPI_THROW(e, ...)                               \
  do {                                                   \
    (e).ThrowAsJavaScriptException();                    \
    return __VA_ARGS__;                                  \
  } while (0)

#define NAPI_THROW_VOID(e)                               \
  do {                                                   \
    (e).ThrowAsJavaScriptException();                    \
    return;                                              \
  } while (0)

#define NAPI_THROW_IF_FAILED(env, status, ...)           \
  if ((status) != napi_ok) {                             \
    Napi::Error::New(env).ThrowAsJavaScriptException();  \
    return __VA_ARGS__;                                  \
  }

#define NAPI_THROW_IF_FAILED_VOID(env, status)           \
  if ((status) != napi_ok) {                             \
    Napi::Error::New(env).ThrowAsJavaScriptException();  \
    return;                                              \
  }

#endif // NAPI_CPP_EXCEPTIONS

# define NAPI_DISALLOW_ASSIGN(CLASS) void operator=(const CLASS&) = delete;
# define NAPI_DISALLOW_COPY(CLASS) CLASS(const CLASS&) = delete;

#define NAPI_DISALLOW_ASSIGN_COPY(CLASS)  \
    NAPI_DISALLOW_ASSIGN(CLASS)           \
    NAPI_DISALLOW_COPY(CLASS)

#define NAPI_FATAL_IF_FAILED(status, location, message)  \
  do {                                                   \
    if ((status) != napi_ok) {                           \
      Napi::Error::Fatal((location), (message));         \
    }                                                    \
  } while (0)

////////////////////////////////////////////////////////////////////////////////
/// Node-API C++ Wrapper Classes
///
/// These classes wrap the "Node-API" ABI-stable C APIs for Node.js, providing a
/// C++ object model and C++ exception-handling semantics with low overhead.
/// The wrappers are all header-only so that they do not affect the ABI.
////////////////////////////////////////////////////////////////////////////////
namespace Napi {

  // Forward declarations
  class Env;
  class Value;
  class Boolean;
  class Number;
#if NAPI_VERSION > 5
  class BigInt;
#endif  // NAPI_VERSION > 5
#if (NAPI_VERSION > 4)
  class Date;
#endif
  class String;
  class Object;
  class Array;
  class ArrayBuffer;
  class Function;
  class Error;
  class PropertyDescriptor;
  class CallbackInfo;
  class TypedArray;
  template <typename T> class TypedArrayOf;

  using Int8Array =
      TypedArrayOf<int8_t>;  ///< Typed-array of signed 8-bit integers
  using Uint8Array =
      TypedArrayOf<uint8_t>;  ///< Typed-array of unsigned 8-bit integers
  using Int16Array =
      TypedArrayOf<int16_t>;  ///< Typed-array of signed 16-bit integers
  using Uint16Array =
      TypedArrayOf<uint16_t>;  ///< Typed-array of unsigned 16-bit integers
  using Int32Array =
      TypedArrayOf<int32_t>;  ///< Typed-array of signed 32-bit integers
  using Uint32Array =
      TypedArrayOf<uint32_t>;  ///< Typed-array of unsigned 32-bit integers
  using Float32Array =
      TypedArrayOf<float>;  ///< Typed-array of 32-bit floating-point values
  using Float64Array =
      TypedArrayOf<double>;  ///< Typed-array of 64-bit floating-point values
#if NAPI_VERSION > 5
  using BigInt64Array =
      TypedArrayOf<int64_t>;  ///< Typed array of signed 64-bit integers
  using BigUint64Array =
      TypedArrayOf<uint64_t>;  ///< Typed array of unsigned 64-bit integers
#endif  // NAPI_VERSION > 5

  /// Defines the signature of a Node-API C++ module's registration callback
  /// (init) function.
  using ModuleRegisterCallback = Object (*)(Env env, Object exports);

  class MemoryManagement;

  /// Environment for Node-API values and operations.
  ///
  /// All Node-API values and operations must be associated with an environment.
  /// An environment instance is always provided to callback functions; that
  /// environment must then be used for any creation of Node-API values or other
  /// Node-API operations within the callback. (Many methods infer the
  /// environment from the `this` instance that the method is called on.)
  ///
  /// In the future, multiple environments per process may be supported,
  /// although current implementations only support one environment per process.
  ///
  /// In the V8 JavaScript engine, a Node-API environment approximately
  /// corresponds to an Isolate.
  class Env {
#if NAPI_VERSION > 5
  private:
    template <typename T> static void DefaultFini(Env, T* data);
    template <typename DataType, typename HintType>
    static void DefaultFiniWithHint(Env, DataType* data, HintType* hint);
#endif  // NAPI_VERSION > 5
  public:
    Env(napi_env env);

    operator napi_env() const;

    Object Global() const;
    Value Undefined() const;
    Value Null() const;

    bool IsExceptionPending() const;
    Error GetAndClearPendingException();

    Value RunScript(const char* utf8script);
    Value RunScript(const std::string& utf8script);
    Value RunScript(String script);

#if NAPI_VERSION > 5
    template <typename T> T* GetInstanceData();

    template <typename T> using Finalizer = void (*)(Env, T*);
    template <typename T, Finalizer<T> fini = Env::DefaultFini<T>>
    void SetInstanceData(T* data);

    template <typename DataType, typename HintType>
    using FinalizerWithHint = void (*)(Env, DataType*, HintType*);
    template <typename DataType,
              typename HintType,
              FinalizerWithHint<DataType, HintType> fini =
                Env::DefaultFiniWithHint<DataType, HintType>>
    void SetInstanceData(DataType* data, HintType* hint);
#endif  // NAPI_VERSION > 5

  private:
    napi_env _env;
  };

  /// A JavaScript value of unknown type.
  ///
  /// For type-specific operations, convert to one of the Value subclasses using a `To*` or `As()`
  /// method. The `To*` methods do type coercion; the `As()` method does not.
  ///
  ///     Napi::Value value = ...
  ///     if (!value.IsString()) throw Napi::TypeError::New(env, "Invalid arg...");
  ///     Napi::String str = value.As<Napi::String>(); // Cast to a string value
  ///
  ///     Napi::Value anotherValue = ...
  ///     bool isTruthy = anotherValue.ToBoolean(); // Coerce to a boolean value
  class Value {
  public:
    Value();                               ///< Creates a new _empty_ Value instance.
    Value(napi_env env,
          napi_value value);  ///< Wraps a Node-API value primitive.

    /// Creates a JS value from a C++ primitive.
    ///
    /// `value` may be any of:
    /// - bool
    /// - Any integer type
    /// - Any floating point type
    /// - const char* (encoded using UTF-8, null-terminated)
    /// - const char16_t* (encoded using UTF-16-LE, null-terminated)
    /// - std::string (encoded using UTF-8)
    /// - std::u16string
    /// - napi::Value
    /// - napi_value
    template <typename T>
    static Value From(napi_env env, const T& value);

    /// Converts to a Node-API value primitive.
    ///
    /// If the instance is _empty_, this returns `nullptr`.
    operator napi_value() const;

    /// Tests if this value strictly equals another value.
    bool operator ==(const Value& other) const;

    /// Tests if this value does not strictly equal another value.
    bool operator !=(const Value& other) const;

    /// Tests if this value strictly equals another value.
    bool StrictEquals(const Value& other) const;

    /// Gets the environment the value is associated with.
    Napi::Env Env() const;

    /// Checks if the value is empty (uninitialized).
    ///
    /// An empty value is invalid, and most attempts to perform an operation on an empty value
    /// will result in an exception. Note an empty value is distinct from JavaScript `null` or
    /// `undefined`, which are valid values.
    ///
    /// When C++ exceptions are disabled at compile time, a method with a `Value` return type may
    /// return an empty value to indicate a pending exception. So when not using C++ exceptions,
    /// callers should check whether the value is empty before attempting to use it.
    bool IsEmpty() const;

    napi_valuetype Type() const; ///< Gets the type of the value.

    bool IsUndefined() const;   ///< Tests if a value is an undefined JavaScript value.
    bool IsNull() const;        ///< Tests if a value is a null JavaScript value.
    bool IsBoolean() const;     ///< Tests if a value is a JavaScript boolean.
    bool IsNumber() const;      ///< Tests if a value is a JavaScript number.
#if NAPI_VERSION > 5
    bool IsBigInt() const;      ///< Tests if a value is a JavaScript bigint.
#endif  // NAPI_VERSION > 5
#if (NAPI_VERSION > 4)
    bool IsDate() const;        ///< Tests if a value is a JavaScript date.
#endif
    bool IsString() const;      ///< Tests if a value is a JavaScript string.
    bool IsSymbol() const;      ///< Tests if a value is a JavaScript symbol.
    bool IsArray() const;       ///< Tests if a value is a JavaScript array.
    bool IsArrayBuffer() const; ///< Tests if a value is a JavaScript array buffer.
    bool IsTypedArray() const;  ///< Tests if a value is a JavaScript typed array.
    bool IsObject() const;      ///< Tests if a value is a JavaScript object.
    bool IsFunction() const;    ///< Tests if a value is a JavaScript function.
    bool IsPromise() const;     ///< Tests if a value is a JavaScript promise.
    bool IsDataView() const;    ///< Tests if a value is a JavaScript data view.
    bool IsBuffer() const;      ///< Tests if a value is a Node buffer.
    bool IsExternal() const;    ///< Tests if a value is a pointer to external data.

    /// Casts to another type of `Napi::Value`, when the actual type is known or assumed.
    ///
    /// This conversion does NOT coerce the type. Calling any methods inappropriate for the actual
    /// value type will throw `Napi::Error`.
    template <typename T> T As() const;

    Boolean ToBoolean() const; ///< Coerces a value to a JavaScript boolean.
    Number ToNumber() const;   ///< Coerces a value to a JavaScript number.
    String ToString() const;   ///< Coerces a value to a JavaScript string.
    Object ToObject() const;   ///< Coerces a value to a JavaScript object.

  protected:
    /// !cond INTERNAL
    napi_env _env;
    napi_value _value;
    /// !endcond
  };

  /// A JavaScript boolean value.
  class Boolean : public Value {
  public:
   static Boolean New(napi_env env,  ///< Node-API environment
                      bool value     ///< Boolean value
   );

   Boolean();  ///< Creates a new _empty_ Boolean instance.
   Boolean(napi_env env,
           napi_value value);  ///< Wraps a Node-API value primitive.

   operator bool() const;  ///< Converts a Boolean value to a boolean primitive.
   bool Value() const;     ///< Converts a Boolean value to a boolean primitive.
  };

  /// A JavaScript number value.
  class Number : public Value {
  public:
   static Number New(napi_env env,  ///< Node-API environment
                     double value   ///< Number value
   );

   Number();  ///< Creates a new _empty_ Number instance.
   Number(napi_env env,
          napi_value value);  ///< Wraps a Node-API value primitive.

   operator int32_t()
       const;  ///< Converts a Number value to a 32-bit signed integer value.
   operator uint32_t()
       const;  ///< Converts a Number value to a 32-bit unsigned integer value.
   operator int64_t()
       const;  ///< Converts a Number value to a 64-bit signed integer value.
   operator float()
       const;  ///< Converts a Number value to a 32-bit floating-point value.
   operator double()
       const;  ///< Converts a Number value to a 64-bit floating-point value.

   int32_t Int32Value()
       const;  ///< Converts a Number value to a 32-bit signed integer value.
   uint32_t Uint32Value()
       const;  ///< Converts a Number value to a 32-bit unsigned integer value.
   int64_t Int64Value()
       const;  ///< Converts a Number value to a 64-bit signed integer value.
   float FloatValue()
       const;  ///< Converts a Number value to a 32-bit floating-point value.
   double DoubleValue()
       const;  ///< Converts a Number value to a 64-bit floating-point value.
  };

#if NAPI_VERSION > 5
  /// A JavaScript bigint value.
  class BigInt : public Value {
  public:
   static BigInt New(napi_env env,  ///< Node-API environment
                     int64_t value  ///< Number value
   );
   static BigInt New(napi_env env,   ///< Node-API environment
                     uint64_t value  ///< Number value
   );

   /// Creates a new BigInt object using a specified sign bit and a
   /// specified list of digits/words.
   /// The resulting number is calculated as:
   /// (-1)^sign_bit * (words[0] * (2^64)^0 + words[1] * (2^64)^1 + ...)
   static BigInt New(napi_env env,          ///< Node-API environment
                     int sign_bit,          ///< Sign bit. 1 if negative.
                     size_t word_count,     ///< Number of words in array
                     const uint64_t* words  ///< Array of words
   );

   BigInt();  ///< Creates a new _empty_ BigInt instance.
   BigInt(napi_env env,
          napi_value value);  ///< Wraps a Node-API value primitive.

   int64_t Int64Value(bool* lossless)
       const;  ///< Converts a BigInt value to a 64-bit signed integer value.
   uint64_t Uint64Value(bool* lossless)
       const;  ///< Converts a BigInt value to a 64-bit unsigned integer value.

   size_t WordCount() const;  ///< The number of 64-bit words needed to store
                              ///< the result of ToWords().

   /// Writes the contents of this BigInt to a specified memory location.
   /// `sign_bit` must be provided and will be set to 1 if this BigInt is
   /// negative.
   /// `*word_count` has to be initialized to the length of the `words` array.
   /// Upon return, it will be set to the actual number of words that would
   /// be needed to store this BigInt (i.e. the return value of `WordCount()`).
   void ToWords(int* sign_bit, size_t* word_count, uint64_t* words);
  };
#endif  // NAPI_VERSION > 5

#if (NAPI_VERSION > 4)
  /// A JavaScript date value.
  class Date : public Value {
  public:
    /// Creates a new Date value from a double primitive.
   static Date New(napi_env env,  ///< Node-API environment
                   double value   ///< Number value
   );

   Date();  ///< Creates a new _empty_ Date instance.
   Date(napi_env env, napi_value value);  ///< Wraps a Node-API value primitive.
   operator double() const;  ///< Converts a Date value to double primitive

   double ValueOf() const;  ///< Converts a Date value to a double primitive.
  };
  #endif

  /// A JavaScript string or symbol value (that can be used as a property name).
  class Name : public Value {
  public:
    Name();                               ///< Creates a new _empty_ Name instance.
    Name(napi_env env,
         napi_value value);  ///< Wraps a Node-API value primitive.
  };

  /// A JavaScript string value.
  class String : public Name {
  public:
    /// Creates a new String value from a UTF-8 encoded C++ string.
   static String New(napi_env env,             ///< Node-API environment
                     const std::string& value  ///< UTF-8 encoded C++ string
   );

   /// Creates a new String value from a UTF-16 encoded C++ string.
   static String New(napi_env env,                ///< Node-API environment
                     const std::u16string& value  ///< UTF-16 encoded C++ string
   );

   /// Creates a new String value from a UTF-8 encoded C string.
   static String New(
       napi_env env,      ///< Node-API environment
       const char* value  ///< UTF-8 encoded null-terminated C string
   );

   /// Creates a new String value from a UTF-16 encoded C string.
   static String New(
       napi_env env,          ///< Node-API environment
       const char16_t* value  ///< UTF-16 encoded null-terminated C string
   );

   /// Creates a new String value from a UTF-8 encoded C string with specified
   /// length.
   static String New(napi_env env,       ///< Node-API environment
                     const char* value,  ///< UTF-8 encoded C string (not
                                         ///< necessarily null-terminated)
                     size_t length       ///< length of the string in bytes
   );

   /// Creates a new String value from a UTF-16 encoded C string with specified
   /// length.
   static String New(
       napi_env env,           ///< Node-API environment
       const char16_t* value,  ///< UTF-16 encoded C string (not necessarily
                               ///< null-terminated)
       size_t length           ///< Length of the string in 2-byte code units
   );

   /// Creates a new String based on the original object's type.
   ///
   /// `value` may be any of:
   /// - const char* (encoded using UTF-8, null-terminated)
   /// - const char16_t* (encoded using UTF-16-LE, null-terminated)
   /// - std::string (encoded using UTF-8)
   /// - std::u16string
   template <typename T>
   static String From(napi_env env, const T& value);

   String();  ///< Creates a new _empty_ String instance.
   String(napi_env env,
          napi_value value);  ///< Wraps a Node-API value primitive.

   operator std::string()
       const;  ///< Converts a String value to a UTF-8 encoded C++ string.
   operator std::u16string()
       const;  ///< Converts a String value to a UTF-16 encoded C++ string.
   std::string Utf8Value()
       const;  ///< Converts a String value to a UTF-8 encoded C++ string.
   std::u16string Utf16Value()
       const;  ///< Converts a String value to a UTF-16 encoded C++ string.
  };

  /// A JavaScript symbol value.
  class Symbol : public Name {
  public:
    /// Creates a new Symbol value with an optional description.
   static Symbol New(
       napi_env env,  ///< Node-API environment
       const char* description =
           nullptr  ///< Optional UTF-8 encoded null-terminated C string
                    ///  describing the symbol
   );

   /// Creates a new Symbol value with a description.
   static Symbol New(
       napi_env env,  ///< Node-API environment
       const std::string&
           description  ///< UTF-8 encoded C++ string describing the symbol
   );

   /// Creates a new Symbol value with a description.
   static Symbol New(napi_env env,       ///< Node-API environment
                     String description  ///< String value describing the symbol
   );

   /// Creates a new Symbol value with a description.
   static Symbol New(
       napi_env env,           ///< Node-API environment
       napi_value description  ///< String value describing the symbol
   );

   /// Get a public Symbol (e.g. Symbol.iterator).
   static Symbol WellKnown(napi_env, const std::string& name);

   Symbol();  ///< Creates a new _empty_ Symbol instance.
   Symbol(napi_env env,
          napi_value value);  ///< Wraps a Node-API value primitive.
  };

  /// A JavaScript object value.
  class Object : public Value {
  public:
    /// Enables property and element assignments using indexing syntax.
    ///
    /// Example:
    ///
    ///     Napi::Value propertyValue = object1['A'];
    ///     object2['A'] = propertyValue;
    ///     Napi::Value elementValue = array[0];
    ///     array[1] = elementValue;
    template <typename Key>
    class PropertyLValue {
    public:
      /// Converts an L-value to a value.
      operator Value() const;

      /// Assigns a value to the property. The type of value can be
      /// anything supported by `Object::Set`.
      template <typename ValueType>
      PropertyLValue& operator =(ValueType value);

    private:
      PropertyLValue() = delete;
      PropertyLValue(Object object, Key key);
      napi_env _env;
      napi_value _object;
      Key _key;

      friend class Napi::Object;
    };

    /// Creates a new Object value.
    static Object New(napi_env env  ///< Node-API environment
    );

    Object();                               ///< Creates a new _empty_ Object instance.
    Object(napi_env env,
           napi_value value);  ///< Wraps a Node-API value primitive.

    /// Gets or sets a named property.
    PropertyLValue<std::string> operator [](
      const char* utf8name ///< UTF-8 encoded null-terminated property name
    );

    /// Gets or sets a named property.
    PropertyLValue<std::string> operator [](
      const std::string& utf8name ///< UTF-8 encoded property name
    );

    /// Gets or sets an indexed property or array element.
    PropertyLValue<uint32_t> operator [](
      uint32_t index /// Property / element index
    );

    /// Gets a named property.
    Value operator [](
      const char* utf8name ///< UTF-8 encoded null-terminated property name
    ) const;

    /// Gets a named property.
    Value operator [](
      const std::string& utf8name ///< UTF-8 encoded property name
    ) const;

    /// Gets an indexed property or array element.
    Value operator [](
      uint32_t index ///< Property / element index
    ) const;

    /// Checks whether a property is present.
    bool Has(
      napi_value key ///< Property key primitive
    ) const;

    /// Checks whether a property is present.
    bool Has(
      Value key ///< Property key
    ) const;

    /// Checks whether a named property is present.
    bool Has(
      const char* utf8name ///< UTF-8 encoded null-terminated property name
    ) const;

    /// Checks whether a named property is present.
    bool Has(
      const std::string& utf8name ///< UTF-8 encoded property name
    ) const;

    /// Checks whether a own property is present.
    bool HasOwnProperty(
      napi_value key ///< Property key primitive
    ) const;

    /// Checks whether a own property is present.
    bool HasOwnProperty(
      Value key ///< Property key
    ) const;

    /// Checks whether a own property is present.
    bool HasOwnProperty(
      const char* utf8name ///< UTF-8 encoded null-terminated property name
    ) const;

    /// Checks whether a own property is present.
    bool HasOwnProperty(
      const std::string& utf8name ///< UTF-8 encoded property name
    ) const;

    /// Gets a property.
    Value Get(
      napi_value key ///< Property key primitive
    ) const;

    /// Gets a property.
    Value Get(
      Value key ///< Property key
    ) const;

    /// Gets a named property.
    Value Get(
      const char* utf8name ///< UTF-8 encoded null-terminated property name
    ) const;

    /// Gets a named property.
    Value Get(
      const std::string& utf8name ///< UTF-8 encoded property name
    ) const;

    /// Sets a property.
    template <typename ValueType>
    bool Set(napi_value key,         ///< Property key primitive
             const ValueType& value  ///< Property value primitive
    );

    /// Sets a property.
    template <typename ValueType>
    bool Set(Value key,              ///< Property key
             const ValueType& value  ///< Property value
    );

    /// Sets a named property.
    template <typename ValueType>
    bool Set(
        const char* utf8name,  ///< UTF-8 encoded null-terminated property name
        const ValueType& value);

    /// Sets a named property.
    template <typename ValueType>
    bool Set(const std::string& utf8name,  ///< UTF-8 encoded property name
             const ValueType& value        ///< Property value primitive
    );

    /// Delete property.
    bool Delete(
      napi_value key ///< Property key primitive
    );

    /// Delete property.
    bool Delete(
      Value key ///< Property key
    );

    /// Delete property.
    bool Delete(
      const char* utf8name ///< UTF-8 encoded null-terminated property name
    );

    /// Delete property.
    bool Delete(
      const std::string& utf8name ///< UTF-8 encoded property name
    );

    /// Checks whether an indexed property is present.
    bool Has(
      uint32_t index ///< Property / element index
    ) const;

    /// Gets an indexed property or array element.
    Value Get(
      uint32_t index ///< Property / element index
    ) const;

    /// Sets an indexed property or array element.
    template <typename ValueType>
    bool Set(uint32_t index,         ///< Property / element index
             const ValueType& value  ///< Property value primitive
    );

    /// Deletes an indexed property or array element.
    bool Delete(
      uint32_t index ///< Property / element index
    );

    Array GetPropertyNames() const; ///< Get all property names

    /// Defines a property on the object.
    bool DefineProperty(
        const PropertyDescriptor&
            property  ///< Descriptor for the property to be defined
    );

    /// Defines properties on the object.
    bool DefineProperties(
        const std::initializer_list<PropertyDescriptor>& properties
        ///< List of descriptors for the properties to be defined
    );

    /// Defines properties on the object.
    bool DefineProperties(
        const std::vector<PropertyDescriptor>& properties
        ///< Vector of descriptors for the properties to be defined
    );

    /// Checks if an object is an instance created by a constructor function.
    ///
    /// This is equivalent to the JavaScript `instanceof` operator.
    bool InstanceOf(
      const Function& constructor ///< Constructor function
    ) const;

    template <typename Finalizer, typename T>
    inline void AddFinalizer(Finalizer finalizeCallback, T* data);

    template <typename Finalizer, typename T, typename Hint>
    inline void AddFinalizer(Finalizer finalizeCallback,
                             T* data,
                             Hint* finalizeHint);
#if NAPI_VERSION >= 8
    bool Freeze();
    bool Seal();
#endif  // NAPI_VERSION >= 8
  };

  template <typename T>
  class External : public Value {
  public:
    static External New(napi_env env, T* data);

    // Finalizer must implement `void operator()(Env env, T* data)`.
    template <typename Finalizer>
    static External New(napi_env env,
                        T* data,
                        Finalizer finalizeCallback);
    // Finalizer must implement `void operator()(Env env, T* data, Hint* hint)`.
    template <typename Finalizer, typename Hint>
    static External New(napi_env env,
                        T* data,
                        Finalizer finalizeCallback,
                        Hint* finalizeHint);

    External();
    External(napi_env env, napi_value value);

    T* Data() const;
  };

  class Array : public Object {
  public:
    static Array New(napi_env env);
    static Array New(napi_env env, size_t length);

    Array();
    Array(napi_env env, napi_value value);

    uint32_t Length() const;
  };

  /// A JavaScript array buffer value.
  class ArrayBuffer : public Object {
  public:
    /// Creates a new ArrayBuffer instance over a new automatically-allocated buffer.
   static ArrayBuffer New(
       napi_env env,      ///< Node-API environment
       size_t byteLength  ///< Length of the buffer to be allocated, in bytes
   );

   /// Creates a new ArrayBuffer instance, using an external buffer with
   /// specified byte length.
   static ArrayBuffer New(
       napi_env env,        ///< Node-API environment
       void* externalData,  ///< Pointer to the external buffer to be used by
                            ///< the array
       size_t byteLength    ///< Length of the external buffer to be used by the
                            ///< array, in bytes
   );

   /// Creates a new ArrayBuffer instance, using an external buffer with
   /// specified byte length.
   template <typename Finalizer>
   static ArrayBuffer New(
       napi_env env,        ///< Node-API environment
       void* externalData,  ///< Pointer to the external buffer to be used by
                            ///< the array
       size_t byteLength,   ///< Length of the external buffer to be used by the
                            ///< array,
                            ///  in bytes
       Finalizer finalizeCallback  ///< Function to be called when the array
                                   ///< buffer is destroyed;
                                   ///  must implement `void operator()(Env env,
                                   ///  void* externalData)`
   );

   /// Creates a new ArrayBuffer instance, using an external buffer with
   /// specified byte length.
   template <typename Finalizer, typename Hint>
   static ArrayBuffer New(
       napi_env env,        ///< Node-API environment
       void* externalData,  ///< Pointer to the external buffer to be used by
                            ///< the array
       size_t byteLength,   ///< Length of the external buffer to be used by the
                            ///< array,
                            ///  in bytes
       Finalizer finalizeCallback,  ///< Function to be called when the array
                                    ///< buffer is destroyed;
                                    ///  must implement `void operator()(Env
                                    ///  env, void* externalData, Hint* hint)`
       Hint* finalizeHint  ///< Hint (second parameter) to be passed to the
                           ///< finalize callback
   );

   ArrayBuffer();  ///< Creates a new _empty_ ArrayBuffer instance.
   ArrayBuffer(napi_env env,
               napi_value value);  ///< Wraps a Node-API value primitive.

   void* Data();         ///< Gets a pointer to the data buffer.
   size_t ByteLength();  ///< Gets the length of the array buffer in bytes.

#if NAPI_VERSION >= 7
    bool IsDetached() const;
    void Detach();
#endif  // NAPI_VERSION >= 7
  };

  /// A JavaScript typed-array value with unknown array type.
  ///
  /// For type-specific operations, cast to a `TypedArrayOf<T>` instance using the `As()`
  /// method:
  ///
  ///     Napi::TypedArray array = ...
  ///     if (t.TypedArrayType() == napi_int32_array) {
  ///         Napi::Int32Array int32Array = t.As<Napi::Int32Array>();
  ///     }
  class TypedArray : public Object {
  public:
    TypedArray();                               ///< Creates a new _empty_ TypedArray instance.
    TypedArray(napi_env env,
               napi_value value);  ///< Wraps a Node-API value primitive.

    napi_typedarray_type TypedArrayType() const; ///< Gets the type of this typed-array.
    Napi::ArrayBuffer ArrayBuffer() const;       ///< Gets the backing array buffer.

    uint8_t ElementSize() const;  ///< Gets the size in bytes of one element in the array.
    size_t ElementLength() const; ///< Gets the number of elements in the array.
    size_t ByteOffset() const;    ///< Gets the offset into the buffer where the array starts.
    size_t ByteLength() const;    ///< Gets the length of the array in bytes.

  protected:
    /// !cond INTERNAL
    napi_typedarray_type _type;
    size_t _length;

    TypedArray(napi_env env, napi_value value, napi_typedarray_type type, size_t length);

    static const napi_typedarray_type unknown_array_type = static_cast<napi_typedarray_type>(-1);

    template <typename T>
    static
#if defined(NAPI_HAS_CONSTEXPR)
    constexpr
#endif
    napi_typedarray_type TypedArrayTypeForPrimitiveType() {
      return std::is_same<T, int8_t>::value ? napi_int8_array
        : std::is_same<T, uint8_t>::value ? napi_uint8_array
        : std::is_same<T, int16_t>::value ? napi_int16_array
        : std::is_same<T, uint16_t>::value ? napi_uint16_array
        : std::is_same<T, int32_t>::value ? napi_int32_array
        : std::is_same<T, uint32_t>::value ? napi_uint32_array
        : std::is_same<T, float>::value ? napi_float32_array
        : std::is_same<T, double>::value ? napi_float64_array
#if NAPI_VERSION > 5
        : std::is_same<T, int64_t>::value ? napi_bigint64_array
        : std::is_same<T, uint64_t>::value ? napi_biguint64_array
#endif  // NAPI_VERSION > 5
        : unknown_array_type;
    }
    /// !endcond
  };

  /// A JavaScript typed-array value with known array type.
  ///
  /// Note while it is possible to create and access Uint8 "clamped" arrays using this class,
  /// the _clamping_ behavior is only applied in JavaScript.
  template <typename T>
  class TypedArrayOf : public TypedArray {
  public:
    /// Creates a new TypedArray instance over a new automatically-allocated array buffer.
    ///
    /// The array type parameter can normally be omitted (because it is inferred from the template
    /// parameter T), except when creating a "clamped" array:
    ///
    ///     Uint8Array::New(env, length, napi_uint8_clamped_array)
   static TypedArrayOf New(
       napi_env env,          ///< Node-API environment
       size_t elementLength,  ///< Length of the created array, as a number of
                              ///< elements
#if defined(NAPI_HAS_CONSTEXPR)
       napi_typedarray_type type =
           TypedArray::TypedArrayTypeForPrimitiveType<T>()
#else
       napi_typedarray_type type
#endif
       ///< Type of array, if different from the default array type for the
       ///< template parameter T.
   );

    /// Creates a new TypedArray instance over a provided array buffer.
    ///
    /// The array type parameter can normally be omitted (because it is inferred from the template
    /// parameter T), except when creating a "clamped" array:
    ///
    ///     Uint8Array::New(env, length, buffer, 0, napi_uint8_clamped_array)
   static TypedArrayOf New(
       napi_env env,          ///< Node-API environment
       size_t elementLength,  ///< Length of the created array, as a number of
                              ///< elements
       Napi::ArrayBuffer arrayBuffer,  ///< Backing array buffer instance to use
       size_t bufferOffset,  ///< Offset into the array buffer where the
                             ///< typed-array starts
#if defined(NAPI_HAS_CONSTEXPR)
       napi_typedarray_type type =
           TypedArray::TypedArrayTypeForPrimitiveType<T>()
#else
       napi_typedarray_type type
#endif
       ///< Type of array, if different from the default array type for the
       ///< template parameter T.
   );

    TypedArrayOf();                               ///< Creates a new _empty_ TypedArrayOf instance.
    TypedArrayOf(napi_env env,
                 napi_value value);  ///< Wraps a Node-API value primitive.

    T& operator [](size_t index);             ///< Gets or sets an element in the array.
    const T& operator [](size_t index) const; ///< Gets an element in the array.

    /// Gets a pointer to the array's backing buffer.
    ///
    /// This is not necessarily the same as the `ArrayBuffer::Data()` pointer, because the
    /// typed-array may have a non-zero `ByteOffset()` into the `ArrayBuffer`.
    T* Data();

    /// Gets a pointer to the array's backing buffer.
    ///
    /// This is not necessarily the same as the `ArrayBuffer::Data()` pointer, because the
    /// typed-array may have a non-zero `ByteOffset()` into the `ArrayBuffer`.
    const T* Data() const;

  private:
    T* _data;

    TypedArrayOf(napi_env env,
                 napi_value value,
                 napi_typedarray_type type,
                 size_t length,
                 T* data);
  };

  /// The DataView provides a low-level interface for reading/writing multiple
  /// number types in an ArrayBuffer irrespective of the platform's endianness.
  class DataView : public Object {
  public:
    static DataView New(napi_env env,
                        Napi::ArrayBuffer arrayBuffer);
    static DataView New(napi_env env,
                        Napi::ArrayBuffer arrayBuffer,
                        size_t byteOffset);
    static DataView New(napi_env env,
                        Napi::ArrayBuffer arrayBuffer,
                        size_t byteOffset,
                        size_t byteLength);

    DataView();                               ///< Creates a new _empty_ DataView instance.
    DataView(napi_env env,
             napi_value value);  ///< Wraps a Node-API value primitive.

    Napi::ArrayBuffer ArrayBuffer() const;    ///< Gets the backing array buffer.
    size_t ByteOffset() const;    ///< Gets the offset into the buffer where the array starts.
    size_t ByteLength() const;    ///< Gets the length of the array in bytes.

    void* Data() const;

    float GetFloat32(size_t byteOffset) const;
    double GetFloat64(size_t byteOffset) const;
    int8_t GetInt8(size_t byteOffset) const;
    int16_t GetInt16(size_t byteOffset) const;
    int32_t GetInt32(size_t byteOffset) const;
    uint8_t GetUint8(size_t byteOffset) const;
    uint16_t GetUint16(size_t byteOffset) const;
    uint32_t GetUint32(size_t byteOffset) const;

    void SetFloat32(size_t byteOffset, float value) const;
    void SetFloat64(size_t byteOffset, double value) const;
    void SetInt8(size_t byteOffset, int8_t value) const;
    void SetInt16(size_t byteOffset, int16_t value) const;
    void SetInt32(size_t byteOffset, int32_t value) const;
    void SetUint8(size_t byteOffset, uint8_t value) const;
    void SetUint16(size_t byteOffset, uint16_t value) const;
    void SetUint32(size_t byteOffset, uint32_t value) const;

  private:
    template <typename T>
    T ReadData(size_t byteOffset) const;

    template <typename T>
    void WriteData(size_t byteOffset, T value) const;

    void* _data;
    size_t _length;
  };

  class Function : public Object {
  public:
   using VoidCallback = void (*)(const CallbackInfo& info);
   using Callback = Value (*)(const CallbackInfo& info);

   template <VoidCallback cb>
   static Function New(napi_env env,
                       const char* utf8name = nullptr,
                       void* data = nullptr);

   template <Callback cb>
   static Function New(napi_env env,
                       const char* utf8name = nullptr,
                       void* data = nullptr);

   template <VoidCallback cb>
   static Function New(napi_env env,
                       const std::string& utf8name,
                       void* data = nullptr);

   template <Callback cb>
   static Function New(napi_env env,
                       const std::string& utf8name,
                       void* data = nullptr);

   /// Callable must implement operator() accepting a const CallbackInfo&
   /// and return either void or Value.
   template <typename Callable>
   static Function New(napi_env env,
                       Callable cb,
                       const char* utf8name = nullptr,
                       void* data = nullptr);
   /// Callable must implement operator() accepting a const CallbackInfo&
   /// and return either void or Value.
   template <typename Callable>
   static Function New(napi_env env,
                       Callable cb,
                       const std::string& utf8name,
                       void* data = nullptr);

   Function();
   Function(napi_env env, napi_value value);

   Value operator()(const std::initializer_list<napi_value>& args) const;

   Value Call(const std::initializer_list<napi_value>& args) const;
   Value Call(const std::vector<napi_value>& args) const;
   Value Call(size_t argc, const napi_value* args) const;
   Value Call(napi_value recv,
              const std::initializer_list<napi_value>& args) const;
   Value Call(napi_value recv, const std::vector<napi_value>& args) const;
   Value Call(napi_value recv, size_t argc, const napi_value* args) const;

   Value MakeCallback(napi_value recv,
                      const std::initializer_list<napi_value>& args,
                      napi_async_context context = nullptr) const;
   Value MakeCallback(napi_value recv,
                      const std::vector<napi_value>& args,
                      napi_async_context context = nullptr) const;
   Value MakeCallback(napi_value recv,
                      size_t argc,
                      const napi_value* args,
                      napi_async_context context = nullptr) const;

   Object New(const std::initializer_list<napi_value>& args) const;
   Object New(const std::vector<napi_value>& args) const;
   Object New(size_t argc, const napi_value* args) const;
  };

  class Promise : public Object {
  public:
    class Deferred {
    public:
      static Deferred New(napi_env env);
      Deferred(napi_env env);

      Napi::Promise Promise() const;
      Napi::Env Env() const;

      void Resolve(napi_value value) const;
      void Reject(napi_value value) const;

    private:
      napi_env _env;
      napi_deferred _deferred;
      napi_value _promise;
    };

    Promise(napi_env env, napi_value value);
  };

  template <typename T>
  class Buffer : public Uint8Array {
  public:
    static Buffer<T> New(napi_env env, size_t length);
    static Buffer<T> New(napi_env env, T* data, size_t length);

    // Finalizer must implement `void operator()(Env env, T* data)`.
    template <typename Finalizer>
    static Buffer<T> New(napi_env env, T* data,
                         size_t length,
                         Finalizer finalizeCallback);
    // Finalizer must implement `void operator()(Env env, T* data, Hint* hint)`.
    template <typename Finalizer, typename Hint>
    static Buffer<T> New(napi_env env, T* data,
                         size_t length,
                         Finalizer finalizeCallback,
                         Hint* finalizeHint);

    static Buffer<T> Copy(napi_env env, const T* data, size_t length);

    Buffer();
    Buffer(napi_env env, napi_value value);
    size_t Length() const;
    T* Data() const;

  private:
    mutable size_t _length;
    mutable T* _data;

    Buffer(napi_env env, napi_value value, size_t length, T* data);
    void EnsureInfo() const;
  };

  /// Holds a counted reference to a value; initially a weak reference unless otherwise specified,
  /// may be changed to/from a strong reference by adjusting the refcount.
  ///
  /// The referenced value is not immediately destroyed when the reference count is zero; it is
  /// merely then eligible for garbage-collection if there are no other references to the value.
  template <typename T>
  class Reference {
  public:
    static Reference<T> New(const T& value, uint32_t initialRefcount = 0);

    Reference();
    Reference(napi_env env, napi_ref ref);
    ~Reference();

    // A reference can be moved but cannot be copied.
    Reference(Reference<T>&& other);
    Reference<T>& operator =(Reference<T>&& other);
    NAPI_DISALLOW_ASSIGN(Reference<T>)

    operator napi_ref() const;
    bool operator ==(const Reference<T> &other) const;
    bool operator !=(const Reference<T> &other) const;

    Napi::Env Env() const;
    bool IsEmpty() const;

    // Note when getting the value of a Reference it is usually correct to do so
    // within a HandleScope so that the value handle gets cleaned up efficiently.
    T Value() const;

    uint32_t Ref();
    uint32_t Unref();
    void Reset();
    void Reset(const T& value, uint32_t refcount = 0);

    // Call this on a reference that is declared as static data, to prevent its
    // destructor from running at program shutdown time, which would attempt to
    // reset the reference when the environment is no longer valid. Avoid using
    // this if at all possible. If you do need to use static data, MAKE SURE to
    // warn your users that your addon is NOT threadsafe.
    void SuppressDestruct();

  protected:
    Reference(const Reference<T>&);

    /// !cond INTERNAL
    napi_env _env;
    napi_ref _ref;
    /// !endcond

  private:
    bool _suppressDestruct;
  };

  class ObjectReference: public Reference<Object> {
  public:
    ObjectReference();
    ObjectReference(napi_env env, napi_ref ref);

    // A reference can be moved but cannot be copied.
    ObjectReference(Reference<Object>&& other);
    ObjectReference& operator =(Reference<Object>&& other);
    ObjectReference(ObjectReference&& other);
    ObjectReference& operator =(ObjectReference&& other);
    NAPI_DISALLOW_ASSIGN(ObjectReference)

    Napi::Value Get(const char* utf8name) const;
    Napi::Value Get(const std::string& utf8name) const;
    bool Set(const char* utf8name, napi_value value);
    bool Set(const char* utf8name, Napi::Value value);
    bool Set(const char* utf8name, const char* utf8value);
    bool Set(const char* utf8name, bool boolValue);
    bool Set(const char* utf8name, double numberValue);
    bool Set(const std::string& utf8name, napi_value value);
    bool Set(const std::string& utf8name, Napi::Value value);
    bool Set(const std::string& utf8name, std::string& utf8value);
    bool Set(const std::string& utf8name, bool boolValue);
    bool Set(const std::string& utf8name, double numberValue);

    Napi::Value Get(uint32_t index) const;
    bool Set(uint32_t index, const napi_value value);
    bool Set(uint32_t index, const Napi::Value value);
    bool Set(uint32_t index, const char* utf8value);
    bool Set(uint32_t index, const std::string& utf8value);
    bool Set(uint32_t index, bool boolValue);
    bool Set(uint32_t index, double numberValue);

   protected:
    ObjectReference(const ObjectReference&);
  };

  class FunctionReference: public Reference<Function> {
  public:
    FunctionReference();
    FunctionReference(napi_env env, napi_ref ref);

    // A reference can be moved but cannot be copied.
    FunctionReference(Reference<Function>&& other);
    FunctionReference& operator =(Reference<Function>&& other);
    FunctionReference(FunctionReference&& other);
    FunctionReference& operator =(FunctionReference&& other);
    NAPI_DISALLOW_ASSIGN_COPY(FunctionReference)

    Napi::Value operator ()(const std::initializer_list<napi_value>& args) const;

    Napi::Value Call(const std::initializer_list<napi_value>& args) const;
    Napi::Value Call(const std::vector<napi_value>& args) const;
    Napi::Value Call(napi_value recv, const std::initializer_list<napi_value>& args) const;
    Napi::Value Call(napi_value recv, const std::vector<napi_value>& args) const;
    Napi::Value Call(napi_value recv, size_t argc, const napi_value* args) const;

    Napi::Value MakeCallback(napi_value recv,
                             const std::initializer_list<napi_value>& args,
                             napi_async_context context = nullptr) const;
    Napi::Value MakeCallback(napi_value recv,
                             const std::vector<napi_value>& args,
                             napi_async_context context = nullptr) const;
    Napi::Value MakeCallback(napi_value recv,
                             size_t argc,
                             const napi_value* args,
                             napi_async_context context = nullptr) const;

    Object New(const std::initializer_list<napi_value>& args) const;
    Object New(const std::vector<napi_value>& args) const;
  };

  // Shortcuts to creating a new reference with inferred type and refcount = 0.
  template <typename T> Reference<T> Weak(T value);
  ObjectReference Weak(Object value);
  FunctionReference Weak(Function value);

  // Shortcuts to creating a new reference with inferred type and refcount = 1.
  template <typename T> Reference<T> Persistent(T value);
  ObjectReference Persistent(Object value);
  FunctionReference Persistent(Function value);

  /// A persistent reference to a JavaScript error object. Use of this class
  /// depends somewhat on whether C++ exceptions are enabled at compile time.
  ///
  /// ### Handling Errors With C++ Exceptions
  ///
  /// If C++ exceptions are enabled, then the `Error` class extends
  /// `std::exception` and enables integrated error-handling for C++ exceptions
  /// and JavaScript exceptions.
  ///
  /// If a Node-API call fails without executing any JavaScript code (for
  /// example due to an invalid argument), then the Node-API wrapper
  /// automatically converts and throws the error as a C++ exception of type
  /// `Napi::Error`. Or if a JavaScript function called by C++ code via Node-API
  /// throws a JavaScript exception, then the Node-API wrapper automatically
  /// converts and throws it as a C++ exception of type `Napi::Error`.
  ///
  /// If a C++ exception of type `Napi::Error` escapes from a Node-API C++
  /// callback, then the Node-API wrapper automatically converts and throws it
  /// as a JavaScript exception. Therefore, catching a C++ exception of type
  /// `Napi::Error` prevents a JavaScript exception from being thrown.
  ///
  /// #### Example 1A - Throwing a C++ exception:
  ///
  ///     Napi::Env env = ...
  ///     throw Napi::Error::New(env, "Example exception");
  ///
  /// Following C++ statements will not be executed. The exception will bubble
  /// up as a C++ exception of type `Napi::Error`, until it is either caught
  /// while still in C++, or else automatically propataged as a JavaScript
  /// exception when the callback returns to JavaScript.
  ///
  /// #### Example 2A - Propagating a Node-API C++ exception:
  ///
  ///     Napi::Function jsFunctionThatThrows = someObj.As<Napi::Function>();
  ///     Napi::Value result = jsFunctionThatThrows({ arg1, arg2 });
  ///
  /// Following C++ statements will not be executed. The exception will bubble
  /// up as a C++ exception of type `Napi::Error`, until it is either caught
  /// while still in C++, or else automatically propagated as a JavaScript
  /// exception when the callback returns to JavaScript.
  ///
  /// #### Example 3A - Handling a Node-API C++ exception:
  ///
  ///     Napi::Function jsFunctionThatThrows = someObj.As<Napi::Function>();
  ///     Napi::Value result;
  ///     try {
  ///        result = jsFunctionThatThrows({ arg1, arg2 });
  ///     } catch (const Napi::Error& e) {
  ///       cerr << "Caught JavaScript exception: " + e.what();
  ///     }
  ///
  /// Since the exception was caught here, it will not be propagated as a
  /// JavaScript exception.
  ///
  /// ### Handling Errors Without C++ Exceptions
  ///
  /// If C++ exceptions are disabled (by defining `NAPI_DISABLE_CPP_EXCEPTIONS`)
  /// then this class does not extend `std::exception`, and APIs in the `Napi`
  /// namespace do not throw C++ exceptions when they fail. Instead, they raise
  /// _pending_ JavaScript exceptions and return _empty_ `Value`s. Calling code
  /// should check `Value::IsEmpty()` before attempting to use a returned value,
  /// and may use methods on the `Env` class to check for, get, and clear a
  /// pending JavaScript exception. If the pending exception is not cleared, it
  /// will be thrown when the native callback returns to JavaScript.
  ///
  /// #### Example 1B - Throwing a JS exception
  ///
  ///     Napi::Env env = ...
  ///     Napi::Error::New(env, "Example
  ///     exception").ThrowAsJavaScriptException(); return;
  ///
  /// After throwing a JS exception, the code should generally return
  /// immediately from the native callback, after performing any necessary
  /// cleanup.
  ///
  /// #### Example 2B - Propagating a Node-API JS exception:
  ///
  ///     Napi::Function jsFunctionThatThrows = someObj.As<Napi::Function>();
  ///     Napi::Value result = jsFunctionThatThrows({ arg1, arg2 });
  ///     if (result.IsEmpty()) return;
  ///
  /// An empty value result from a Node-API call indicates an error occurred,
  /// and a JavaScript exception is pending. To let the exception propagate, the
  /// code should generally return immediately from the native callback, after
  /// performing any necessary cleanup.
  ///
  /// #### Example 3B - Handling a Node-API JS exception:
  ///
  ///     Napi::Function jsFunctionThatThrows = someObj.As<Napi::Function>();
  ///     Napi::Value result = jsFunctionThatThrows({ arg1, arg2 });
  ///     if (result.IsEmpty()) {
  ///       Napi::Error e = env.GetAndClearPendingException();
  ///       cerr << "Caught JavaScript exception: " + e.Message();
  ///     }
  ///
  /// Since the exception was cleared here, it will not be propagated as a
  /// JavaScript exception after the native callback returns.
  class Error : public ObjectReference
#ifdef NAPI_CPP_EXCEPTIONS
    , public std::exception
#endif // NAPI_CPP_EXCEPTIONS
    {
  public:
    static Error New(napi_env env);
    static Error New(napi_env env, const char* message);
    static Error New(napi_env env, const std::string& message);

    static NAPI_NO_RETURN void Fatal(const char* location, const char* message);

    Error();
    Error(napi_env env, napi_value value);

    // An error can be moved or copied.
    Error(Error&& other);
    Error& operator =(Error&& other);
    Error(const Error&);
    Error& operator =(const Error&);

    const std::string& Message() const NAPI_NOEXCEPT;
    void ThrowAsJavaScriptException() const;

#ifdef NAPI_CPP_EXCEPTIONS
    const char* what() const NAPI_NOEXCEPT override;
#endif // NAPI_CPP_EXCEPTIONS

  protected:
    /// !cond INTERNAL
   using create_error_fn = napi_status (*)(napi_env envb,
                                           napi_value code,
                                           napi_value msg,
                                           napi_value* result);

   template <typename TError>
   static TError New(napi_env env,
                     const char* message,
                     size_t length,
                     create_error_fn create_error);
   /// !endcond

  private:
    mutable std::string _message;
  };

  class TypeError : public Error {
  public:
    static TypeError New(napi_env env, const char* message);
    static TypeError New(napi_env env, const std::string& message);

    TypeError();
    TypeError(napi_env env, napi_value value);
  };

  class RangeError : public Error {
  public:
    static RangeError New(napi_env env, const char* message);
    static RangeError New(napi_env env, const std::string& message);

    RangeError();
    RangeError(napi_env env, napi_value value);
  };

  class CallbackInfo {
  public:
    CallbackInfo(napi_env env, napi_callback_info info);
    ~CallbackInfo();

    // Disallow copying to prevent multiple free of _dynamicArgs
    NAPI_DISALLOW_ASSIGN_COPY(CallbackInfo)

    Napi::Env Env() const;
    Value NewTarget() const;
    bool IsConstructCall() const;
    size_t Length() const;
    const Value operator [](size_t index) const;
    Value This() const;
    void* Data() const;
    void SetData(void* data);

  private:
    const size_t _staticArgCount = 6;
    napi_env _env;
    napi_callback_info _info;
    napi_value _this;
    size_t _argc;
    napi_value* _argv;
    napi_value _staticArgs[6];
    napi_value* _dynamicArgs;
    void* _data;
  };

  class PropertyDescriptor {
  public:
   using GetterCallback = Napi::Value (*)(const Napi::CallbackInfo& info);
   using SetterCallback = void (*)(const Napi::CallbackInfo& info);

#ifndef NODE_ADDON_API_DISABLE_DEPRECATED
    template <typename Getter>
    static PropertyDescriptor Accessor(const char* utf8name,
                                       Getter getter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Getter>
    static PropertyDescriptor Accessor(const std::string& utf8name,
                                       Getter getter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Getter>
    static PropertyDescriptor Accessor(napi_value name,
                                       Getter getter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Getter>
    static PropertyDescriptor Accessor(Name name,
                                       Getter getter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Getter, typename Setter>
    static PropertyDescriptor Accessor(const char* utf8name,
                                       Getter getter,
                                       Setter setter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Getter, typename Setter>
    static PropertyDescriptor Accessor(const std::string& utf8name,
                                       Getter getter,
                                       Setter setter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Getter, typename Setter>
    static PropertyDescriptor Accessor(napi_value name,
                                       Getter getter,
                                       Setter setter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Getter, typename Setter>
    static PropertyDescriptor Accessor(Name name,
                                       Getter getter,
                                       Setter setter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Callable>
    static PropertyDescriptor Function(const char* utf8name,
                                       Callable cb,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Callable>
    static PropertyDescriptor Function(const std::string& utf8name,
                                       Callable cb,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Callable>
    static PropertyDescriptor Function(napi_value name,
                                       Callable cb,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Callable>
    static PropertyDescriptor Function(Name name,
                                       Callable cb,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
#endif // !NODE_ADDON_API_DISABLE_DEPRECATED

    template <GetterCallback Getter>
    static PropertyDescriptor Accessor(const char* utf8name,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);

    template <GetterCallback Getter>
    static PropertyDescriptor Accessor(const std::string& utf8name,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);

    template <GetterCallback Getter>
    static PropertyDescriptor Accessor(Name name,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);

    template <GetterCallback Getter, SetterCallback Setter>
    static PropertyDescriptor Accessor(const char* utf8name,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);

    template <GetterCallback Getter, SetterCallback Setter>
    static PropertyDescriptor Accessor(const std::string& utf8name,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);

    template <GetterCallback Getter, SetterCallback Setter>
    static PropertyDescriptor Accessor(Name name,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);

    template <typename Getter>
    static PropertyDescriptor Accessor(Napi::Env env,
                                       Napi::Object object,
                                       const char* utf8name,
                                       Getter getter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Getter>
    static PropertyDescriptor Accessor(Napi::Env env,
                                       Napi::Object object,
                                       const std::string& utf8name,
                                       Getter getter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Getter>
    static PropertyDescriptor Accessor(Napi::Env env,
                                       Napi::Object object,
                                       Name name,
                                       Getter getter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Getter, typename Setter>
    static PropertyDescriptor Accessor(Napi::Env env,
                                       Napi::Object object,
                                       const char* utf8name,
                                       Getter getter,
                                       Setter setter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Getter, typename Setter>
    static PropertyDescriptor Accessor(Napi::Env env,
                                       Napi::Object object,
                                       const std::string& utf8name,
                                       Getter getter,
                                       Setter setter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Getter, typename Setter>
    static PropertyDescriptor Accessor(Napi::Env env,
                                       Napi::Object object,
                                       Name name,
                                       Getter getter,
                                       Setter setter,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Callable>
    static PropertyDescriptor Function(Napi::Env env,
                                       Napi::Object object,
                                       const char* utf8name,
                                       Callable cb,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Callable>
    static PropertyDescriptor Function(Napi::Env env,
                                       Napi::Object object,
                                       const std::string& utf8name,
                                       Callable cb,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    template <typename Callable>
    static PropertyDescriptor Function(Napi::Env env,
                                       Napi::Object object,
                                       Name name,
                                       Callable cb,
                                       napi_property_attributes attributes = napi_default,
                                       void* data = nullptr);
    static PropertyDescriptor Value(const char* utf8name,
                                    napi_value value,
                                    napi_property_attributes attributes = napi_default);
    static PropertyDescriptor Value(const std::string& utf8name,
                                    napi_value value,
                                    napi_property_attributes attributes = napi_default);
    static PropertyDescriptor Value(napi_value name,
                                    napi_value value,
                                    napi_property_attributes attributes = napi_default);
    static PropertyDescriptor Value(Name name,
                                    Napi::Value value,
                                    napi_property_attributes attributes = napi_default);

    PropertyDescriptor(napi_property_descriptor desc);

    operator napi_property_descriptor&();
    operator const napi_property_descriptor&() const;

  private:
    napi_property_descriptor _desc;
  };

  /// Property descriptor for use with `ObjectWrap::DefineClass()`.
  ///
  /// This is different from the standalone `PropertyDescriptor` because it is specific to each
  /// `ObjectWrap<T>` subclass. This prevents using descriptors from a different class when
  /// defining a new class (preventing the callbacks from having incorrect `this` pointers).
  template <typename T>
  class ClassPropertyDescriptor {
  public:
    ClassPropertyDescriptor(napi_property_descriptor desc) : _desc(desc) {}

    operator napi_property_descriptor&() { return _desc; }
    operator const napi_property_descriptor&() const { return _desc; }

  private:
    napi_property_descriptor _desc;
  };

  template <typename T, typename TCallback>
  struct MethodCallbackData {
    TCallback callback;
    void* data;
  };

  template <typename T, typename TGetterCallback, typename TSetterCallback>
  struct AccessorCallbackData {
    TGetterCallback getterCallback;
    TSetterCallback setterCallback;
    void* data;
  };

  template <typename T>
  class InstanceWrap {
   public:
    using InstanceVoidMethodCallback = void (T::*)(const CallbackInfo& info);
    using InstanceMethodCallback = Napi::Value (T::*)(const CallbackInfo& info);
    using InstanceGetterCallback = Napi::Value (T::*)(const CallbackInfo& info);
    using InstanceSetterCallback = void (T::*)(const CallbackInfo& info,
                                               const Napi::Value& value);

    using PropertyDescriptor = ClassPropertyDescriptor<T>;

    static PropertyDescriptor InstanceMethod(const char* utf8name,
                                             InstanceVoidMethodCallback method,
                                             napi_property_attributes attributes = napi_default,
                                             void* data = nullptr);
    static PropertyDescriptor InstanceMethod(const char* utf8name,
                                             InstanceMethodCallback method,
                                             napi_property_attributes attributes = napi_default,
                                             void* data = nullptr);
    static PropertyDescriptor InstanceMethod(Symbol name,
                                             InstanceVoidMethodCallback method,
                                             napi_property_attributes attributes = napi_default,
                                             void* data = nullptr);
    static PropertyDescriptor InstanceMethod(Symbol name,
                                             InstanceMethodCallback method,
                                             napi_property_attributes attributes = napi_default,
                                             void* data = nullptr);
    template <InstanceVoidMethodCallback method>
    static PropertyDescriptor InstanceMethod(const char* utf8name,
                                             napi_property_attributes attributes = napi_default,
                                             void* data = nullptr);
    template <InstanceMethodCallback method>
    static PropertyDescriptor InstanceMethod(const char* utf8name,
                                             napi_property_attributes attributes = napi_default,
                                             void* data = nullptr);
    template <InstanceVoidMethodCallback method>
    static PropertyDescriptor InstanceMethod(Symbol name,
                                             napi_property_attributes attributes = napi_default,
                                             void* data = nullptr);
    template <InstanceMethodCallback method>
    static PropertyDescriptor InstanceMethod(Symbol name,
                                             napi_property_attributes attributes = napi_default,
                                             void* data = nullptr);
    static PropertyDescriptor InstanceAccessor(const char* utf8name,
                                               InstanceGetterCallback getter,
                                               InstanceSetterCallback setter,
                                               napi_property_attributes attributes = napi_default,
                                               void* data = nullptr);
    static PropertyDescriptor InstanceAccessor(Symbol name,
                                               InstanceGetterCallback getter,
                                               InstanceSetterCallback setter,
                                               napi_property_attributes attributes = napi_default,
                                               void* data = nullptr);
    template <InstanceGetterCallback getter, InstanceSetterCallback setter=nullptr>
    static PropertyDescriptor InstanceAccessor(const char* utf8name,
                                               napi_property_attributes attributes = napi_default,
                                               void* data = nullptr);
    template <InstanceGetterCallback getter, InstanceSetterCallback setter=nullptr>
    static PropertyDescriptor InstanceAccessor(Symbol name,
                                               napi_property_attributes attributes = napi_default,
                                               void* data = nullptr);
    static PropertyDescriptor InstanceValue(const char* utf8name,
                                            Napi::Value value,
                                            napi_property_attributes attributes = napi_default);
    static PropertyDescriptor InstanceValue(Symbol name,
                                            Napi::Value value,
                                            napi_property_attributes attributes = napi_default);

   protected:
    static void AttachPropData(napi_env env, napi_value value, const napi_property_descriptor* prop);

   private:
    using This = InstanceWrap<T>;

    using InstanceVoidMethodCallbackData =
        MethodCallbackData<T, InstanceVoidMethodCallback>;
    using InstanceMethodCallbackData =
        MethodCallbackData<T, InstanceMethodCallback>;
    using InstanceAccessorCallbackData =
        AccessorCallbackData<T, InstanceGetterCallback, InstanceSetterCallback>;

    static napi_value InstanceVoidMethodCallbackWrapper(napi_env env, napi_callback_info info);
    static napi_value InstanceMethodCallbackWrapper(napi_env env, napi_callback_info info);
    static napi_value InstanceGetterCallbackWrapper(napi_env env, napi_callback_info info);
    static napi_value InstanceSetterCallbackWrapper(napi_env env, napi_callback_info info);

    template <InstanceSetterCallback method>
    static napi_value WrappedMethod(napi_env env,
                                    napi_callback_info info) NAPI_NOEXCEPT;

    template <InstanceSetterCallback setter> struct SetterTag {};

    template <InstanceSetterCallback setter>
    static napi_callback WrapSetter(SetterTag<setter>) NAPI_NOEXCEPT {
      return &This::WrappedMethod<setter>;
    }
    static napi_callback WrapSetter(SetterTag<nullptr>) NAPI_NOEXCEPT {
      return nullptr;
    }
  };

  /// Base class to be extended by C++ classes exposed to JavaScript; each C++ class instance gets
  /// "wrapped" by a JavaScript object that is managed by this class.
  ///
  /// At initialization time, the `DefineClass()` method must be used to
  /// hook up the accessor and method callbacks. It takes a list of
  /// property descriptors, which can be constructed via the various
  /// static methods on the base class.
  ///
  /// #### Example:
  ///
  ///     class Example: public Napi::ObjectWrap<Example> {
  ///       public:
  ///         static void Initialize(Napi::Env& env, Napi::Object& target) {
  ///           Napi::Function constructor = DefineClass(env, "Example", {
  ///             InstanceAccessor<&Example::GetSomething, &Example::SetSomething>("value"),
  ///             InstanceMethod<&Example::DoSomething>("doSomething"),
  ///           });
  ///           target.Set("Example", constructor);
  ///         }
  ///
  ///         Example(const Napi::CallbackInfo& info); // Constructor
  ///         Napi::Value GetSomething(const Napi::CallbackInfo& info);
  ///         void SetSomething(const Napi::CallbackInfo& info, const Napi::Value& value);
  ///         Napi::Value DoSomething(const Napi::CallbackInfo& info);
  ///     }
  template <typename T>
  class ObjectWrap : public InstanceWrap<T>, public Reference<Object> {
  public:
    ObjectWrap(const CallbackInfo& callbackInfo);
    virtual ~ObjectWrap();

    static T* Unwrap(Object wrapper);

    // Methods exposed to JavaScript must conform to one of these callback signatures.
    using StaticVoidMethodCallback = void (*)(const CallbackInfo& info);
    using StaticMethodCallback = Napi::Value (*)(const CallbackInfo& info);
    using StaticGetterCallback = Napi::Value (*)(const CallbackInfo& info);
    using StaticSetterCallback = void (*)(const CallbackInfo& info,
                                          const Napi::Value& value);

    using PropertyDescriptor = ClassPropertyDescriptor<T>;

    static Function DefineClass(Napi::Env env,
                                const char* utf8name,
                                const std::initializer_list<PropertyDescriptor>& properties,
                                void* data = nullptr);
    static Function DefineClass(Napi::Env env,
                                const char* utf8name,
                                const std::vector<PropertyDescriptor>& properties,
                                void* data = nullptr);
    static PropertyDescriptor StaticMethod(const char* utf8name,
                                           StaticVoidMethodCallback method,
                                           napi_property_attributes attributes = napi_default,
                                           void* data = nullptr);
    static PropertyDescriptor StaticMethod(const char* utf8name,
                                           StaticMethodCallback method,
                                           napi_property_attributes attributes = napi_default,
                                           void* data = nullptr);
    static PropertyDescriptor StaticMethod(Symbol name,
                                           StaticVoidMethodCallback method,
                                           napi_property_attributes attributes = napi_default,
                                           void* data = nullptr);
    static PropertyDescriptor StaticMethod(Symbol name,
                                           StaticMethodCallback method,
                                           napi_property_attributes attributes = napi_default,
                                           void* data = nullptr);
    template <StaticVoidMethodCallback method>
    static PropertyDescriptor StaticMethod(const char* utf8name,
                                           napi_property_attributes attributes = napi_default,
                                           void* data = nullptr);
    template <StaticVoidMethodCallback method>
    static PropertyDescriptor StaticMethod(Symbol name,
                                           napi_property_attributes attributes = napi_default,
                                           void* data = nullptr);
    template <StaticMethodCallback method>
    static PropertyDescriptor StaticMethod(const char* utf8name,
                                           napi_property_attributes attributes = napi_default,
                                           void* data = nullptr);
    template <StaticMethodCallback method>
    static PropertyDescriptor StaticMethod(Symbol name,
                                           napi_property_attributes attributes = napi_default,
                                           void* data = nullptr);
    static PropertyDescriptor StaticAccessor(const char* utf8name,
                                             StaticGetterCallback getter,
                                             StaticSetterCallback setter,
                                             napi_property_attributes attributes = napi_default,
                                             void* data = nullptr);
    static PropertyDescriptor StaticAccessor(Symbol name,
                                             StaticGetterCallback getter,
                                             StaticSetterCallback setter,
                                             napi_property_attributes attributes = napi_default,
                                             void* data = nullptr);
    template <StaticGetterCallback getter, StaticSetterCallback setter=nullptr>
    static PropertyDescriptor StaticAccessor(const char* utf8name,
                                             napi_property_attributes attributes = napi_default,
                                             void* data = nullptr);
    template <StaticGetterCallback getter, StaticSetterCallback setter=nullptr>
    static PropertyDescriptor StaticAccessor(Symbol name,
                                             napi_property_attributes attributes = napi_default,
                                             void* data = nullptr);
    static PropertyDescriptor StaticValue(const char* utf8name,
                                          Napi::Value value,
                                          napi_property_attributes attributes = napi_default);
    static PropertyDescriptor StaticValue(Symbol name,
                                          Napi::Value value,
                                          napi_property_attributes attributes = napi_default);
    virtual void Finalize(Napi::Env env);

  private:
    using This = ObjectWrap<T>;

    static napi_value ConstructorCallbackWrapper(napi_env env, napi_callback_info info);
    static napi_value StaticVoidMethodCallbackWrapper(napi_env env, napi_callback_info info);
    static napi_value StaticMethodCallbackWrapper(napi_env env, napi_callback_info info);
    static napi_value StaticGetterCallbackWrapper(napi_env env, napi_callback_info info);
    static napi_value StaticSetterCallbackWrapper(napi_env env, napi_callback_info info);
    static void FinalizeCallback(napi_env env, void* data, void* hint);
    static Function DefineClass(Napi::Env env,
                                const char* utf8name,
                                const size_t props_count,
                                const napi_property_descriptor* props,
                                void* data = nullptr);

    using StaticVoidMethodCallbackData =
        MethodCallbackData<T, StaticVoidMethodCallback>;
    using StaticMethodCallbackData =
        MethodCallbackData<T, StaticMethodCallback>;

    using StaticAccessorCallbackData =
        AccessorCallbackData<T, StaticGetterCallback, StaticSetterCallback>;

    template <StaticSetterCallback method>
    static napi_value WrappedMethod(napi_env env,
                                    napi_callback_info info) NAPI_NOEXCEPT;

    template <StaticSetterCallback setter> struct StaticSetterTag {};

    template <StaticSetterCallback setter>
    static napi_callback WrapStaticSetter(StaticSetterTag<setter>)
        NAPI_NOEXCEPT {
      return &This::WrappedMethod<setter>;
    }
    static napi_callback WrapStaticSetter(StaticSetterTag<nullptr>)
        NAPI_NOEXCEPT {
      return nullptr;
    }

    bool _construction_failed = true;
  };

  class HandleScope {
  public:
    HandleScope(napi_env env, napi_handle_scope scope);
    explicit HandleScope(Napi::Env env);
    ~HandleScope();

    // Disallow copying to prevent double close of napi_handle_scope
    NAPI_DISALLOW_ASSIGN_COPY(HandleScope)

    operator napi_handle_scope() const;

    Napi::Env Env() const;

  private:
    napi_env _env;
    napi_handle_scope _scope;
  };

  class EscapableHandleScope {
  public:
    EscapableHandleScope(napi_env env, napi_escapable_handle_scope scope);
    explicit EscapableHandleScope(Napi::Env env);
    ~EscapableHandleScope();

    // Disallow copying to prevent double close of napi_escapable_handle_scope
    NAPI_DISALLOW_ASSIGN_COPY(EscapableHandleScope)

    operator napi_escapable_handle_scope() const;

    Napi::Env Env() const;
    Value Escape(napi_value escapee);

  private:
    napi_env _env;
    napi_escapable_handle_scope _scope;
  };

#if (NAPI_VERSION > 2)
  class CallbackScope {
  public:
    CallbackScope(napi_env env, napi_callback_scope scope);
    CallbackScope(napi_env env, napi_async_context context);
    virtual ~CallbackScope();

    // Disallow copying to prevent double close of napi_callback_scope
    NAPI_DISALLOW_ASSIGN_COPY(CallbackScope)

    operator napi_callback_scope() const;

    Napi::Env Env() const;

  private:
    napi_env _env;
    napi_callback_scope _scope;
  };
#endif

  class AsyncContext {
  public:
    explicit AsyncContext(napi_env env, const char* resource_name);
    explicit AsyncContext(napi_env env, const char* resource_name, const Object& resource);
    virtual ~AsyncContext();

    AsyncContext(AsyncContext&& other);
    AsyncContext& operator =(AsyncContext&& other);
    NAPI_DISALLOW_ASSIGN_COPY(AsyncContext)

    operator napi_async_context() const;

    Napi::Env Env() const;

  private:
    napi_env _env;
    napi_async_context _context;
  };

  class AsyncWorker {
  public:
    virtual ~AsyncWorker();

    // An async worker can be moved but cannot be copied.
    AsyncWorker(AsyncWorker&& other);
    AsyncWorker& operator =(AsyncWorker&& other);
    NAPI_DISALLOW_ASSIGN_COPY(AsyncWorker)

    operator napi_async_work() const;

    Napi::Env Env() const;

    void Queue();
    void Cancel();
    void SuppressDestruct();

    ObjectReference& Receiver();
    FunctionReference& Callback();

    virtual void OnExecute(Napi::Env env);
    virtual void OnWorkComplete(Napi::Env env,
                                napi_status status);

  protected:
    explicit AsyncWorker(const Function& callback);
    explicit AsyncWorker(const Function& callback,
                         const char* resource_name);
    explicit AsyncWorker(const Function& callback,
                         const char* resource_name,
                         const Object& resource);
    explicit AsyncWorker(const Object& receiver,
                         const Function& callback);
    explicit AsyncWorker(const Object& receiver,
                         const Function& callback,
                         const char* resource_name);
    explicit AsyncWorker(const Object& receiver,
                         const Function& callback,
                         const char* resource_name,
                         const Object& resource);

    explicit AsyncWorker(Napi::Env env);
    explicit AsyncWorker(Napi::Env env,
                         const char* resource_name);
    explicit AsyncWorker(Napi::Env env,
                         const char* resource_name,
                         const Object& resource);

    virtual void Execute() = 0;
    virtual void OnOK();
    virtual void OnError(const Error& e);
    virtual void Destroy();
    virtual std::vector<napi_value> GetResult(Napi::Env env);

    void SetError(const std::string& error);

  private:
    static inline void OnAsyncWorkExecute(napi_env env, void* asyncworker);
    static inline void OnAsyncWorkComplete(napi_env env,
                                           napi_status status,
                                           void* asyncworker);

    napi_env _env;
    napi_async_work _work;
    ObjectReference _receiver;
    FunctionReference _callback;
    std::string _error;
    bool _suppress_destruct;
  };

  #if (NAPI_VERSION > 3 && !defined(__wasm32__))
  class ThreadSafeFunction {
  public:
    // This API may only be called from the main thread.
    template <typename ResourceString>
    static ThreadSafeFunction New(napi_env env,
                                  const Function& callback,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount);

    // This API may only be called from the main thread.
    template <typename ResourceString, typename ContextType>
    static ThreadSafeFunction New(napi_env env,
                                  const Function& callback,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  ContextType* context);

    // This API may only be called from the main thread.
    template <typename ResourceString, typename Finalizer>
    static ThreadSafeFunction New(napi_env env,
                                  const Function& callback,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  Finalizer finalizeCallback);

    // This API may only be called from the main thread.
    template <typename ResourceString, typename Finalizer,
              typename FinalizerDataType>
    static ThreadSafeFunction New(napi_env env,
                                  const Function& callback,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  Finalizer finalizeCallback,
                                  FinalizerDataType* data);

    // This API may only be called from the main thread.
    template <typename ResourceString, typename ContextType, typename Finalizer>
    static ThreadSafeFunction New(napi_env env,
                                  const Function& callback,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  ContextType* context,
                                  Finalizer finalizeCallback);

    // This API may only be called from the main thread.
    template <typename ResourceString, typename ContextType,
              typename Finalizer, typename FinalizerDataType>
    static ThreadSafeFunction New(napi_env env,
                                  const Function& callback,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  ContextType* context,
                                  Finalizer finalizeCallback,
                                  FinalizerDataType* data);

    // This API may only be called from the main thread.
    template <typename ResourceString>
    static ThreadSafeFunction New(napi_env env,
                                  const Function& callback,
                                  const Object& resource,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount);

    // This API may only be called from the main thread.
    template <typename ResourceString, typename ContextType>
    static ThreadSafeFunction New(napi_env env,
                                  const Function& callback,
                                  const Object& resource,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  ContextType* context);

    // This API may only be called from the main thread.
    template <typename ResourceString, typename Finalizer>
    static ThreadSafeFunction New(napi_env env,
                                  const Function& callback,
                                  const Object& resource,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  Finalizer finalizeCallback);

    // This API may only be called from the main thread.
    template <typename ResourceString, typename Finalizer,
              typename FinalizerDataType>
    static ThreadSafeFunction New(napi_env env,
                                  const Function& callback,
                                  const Object& resource,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  Finalizer finalizeCallback,
                                  FinalizerDataType* data);

    // This API may only be called from the main thread.
    template <typename ResourceString, typename ContextType, typename Finalizer>
    static ThreadSafeFunction New(napi_env env,
                                  const Function& callback,
                                  const Object& resource,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  ContextType* context,
                                  Finalizer finalizeCallback);

    // This API may only be called from the main thread.
    template <typename ResourceString, typename ContextType,
              typename Finalizer, typename FinalizerDataType>
    static ThreadSafeFunction New(napi_env env,
                                  const Function& callback,
                                  const Object& resource,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  ContextType* context,
                                  Finalizer finalizeCallback,
                                  FinalizerDataType* data);

    ThreadSafeFunction();
    ThreadSafeFunction(napi_threadsafe_function tsFunctionValue);

    operator napi_threadsafe_function() const;

    // This API may be called from any thread.
    napi_status BlockingCall() const;

    // This API may be called from any thread.
    template <typename Callback>
    napi_status BlockingCall(Callback callback) const;

    // This API may be called from any thread.
    template <typename DataType, typename Callback>
    napi_status BlockingCall(DataType* data, Callback callback) const;

    // This API may be called from any thread.
    napi_status NonBlockingCall() const;

    // This API may be called from any thread.
    template <typename Callback>
    napi_status NonBlockingCall(Callback callback) const;

    // This API may be called from any thread.
    template <typename DataType, typename Callback>
    napi_status NonBlockingCall(DataType* data, Callback callback) const;

    // This API may only be called from the main thread.
    void Ref(napi_env env) const;

    // This API may only be called from the main thread.
    void Unref(napi_env env) const;

    // This API may be called from any thread.
    napi_status Acquire() const;

    // This API may be called from any thread.
    napi_status Release();

    // This API may be called from any thread.
    napi_status Abort();

    struct ConvertibleContext
    {
      template <class T>
      operator T*() { return static_cast<T*>(context); }
      void* context;
    };

    // This API may be called from any thread.
    ConvertibleContext GetContext() const;

  private:
    using CallbackWrapper = std::function<void(Napi::Env, Napi::Function)>;

    template <typename ResourceString, typename ContextType,
              typename Finalizer, typename FinalizerDataType>
    static ThreadSafeFunction New(napi_env env,
                                  const Function& callback,
                                  const Object& resource,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  ContextType* context,
                                  Finalizer finalizeCallback,
                                  FinalizerDataType* data,
                                  napi_finalize wrapper);

    napi_status CallInternal(CallbackWrapper* callbackWrapper,
                        napi_threadsafe_function_call_mode mode) const;

    static void CallJS(napi_env env,
                       napi_value jsCallback,
                       void* context,
                       void* data);

    napi_threadsafe_function _tsfn;
  };

  // A TypedThreadSafeFunction by default has no context (nullptr) and can
  // accept any type (void) to its CallJs.
  template <typename ContextType = std::nullptr_t,
            typename DataType = void,
            void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*) =
                nullptr>
  class TypedThreadSafeFunction {
   public:
    // This API may only be called from the main thread.
    // Helper function that returns nullptr if running Node-API 5+, otherwise a
    // non-empty, no-op Function. This provides the ability to specify at
    // compile-time a callback parameter to `New` that safely does no action
    // when targeting _any_ Node-API version.
#if NAPI_VERSION > 4
    static std::nullptr_t EmptyFunctionFactory(Napi::Env env);
#else
    static Napi::Function EmptyFunctionFactory(Napi::Env env);
#endif
    static Napi::Function FunctionOrEmpty(Napi::Env env,
                                          Napi::Function& callback);

#if NAPI_VERSION > 4
    // This API may only be called from the main thread.
    // Creates a new threadsafe function with:
    //   Callback [missing] Resource [missing] Finalizer [missing]
    template <typename ResourceString>
    static TypedThreadSafeFunction<ContextType, DataType, CallJs> New(
        napi_env env,
        ResourceString resourceName,
        size_t maxQueueSize,
        size_t initialThreadCount,
        ContextType* context = nullptr);

    // This API may only be called from the main thread.
    // Creates a new threadsafe function with:
    //   Callback [missing] Resource [passed] Finalizer [missing]
    template <typename ResourceString>
    static TypedThreadSafeFunction<ContextType, DataType, CallJs> New(
        napi_env env,
        const Object& resource,
        ResourceString resourceName,
        size_t maxQueueSize,
        size_t initialThreadCount,
        ContextType* context = nullptr);

    // This API may only be called from the main thread.
    // Creates a new threadsafe function with:
    //   Callback [missing] Resource [missing] Finalizer [passed]
    template <typename ResourceString,
              typename Finalizer,
              typename FinalizerDataType = void>
    static TypedThreadSafeFunction<ContextType, DataType, CallJs> New(
        napi_env env,
        ResourceString resourceName,
        size_t maxQueueSize,
        size_t initialThreadCount,
        ContextType* context,
        Finalizer finalizeCallback,
        FinalizerDataType* data = nullptr);

    // This API may only be called from the main thread.
    // Creates a new threadsafe function with:
    //   Callback [missing] Resource [passed] Finalizer [passed]
    template <typename ResourceString,
              typename Finalizer,
              typename FinalizerDataType = void>
    static TypedThreadSafeFunction<ContextType, DataType, CallJs> New(
        napi_env env,
        const Object& resource,
        ResourceString resourceName,
        size_t maxQueueSize,
        size_t initialThreadCount,
        ContextType* context,
        Finalizer finalizeCallback,
        FinalizerDataType* data = nullptr);
#endif

    // This API may only be called from the main thread.
    // Creates a new threadsafe function with:
    //   Callback [passed] Resource [missing] Finalizer [missing]
    template <typename ResourceString>
    static TypedThreadSafeFunction<ContextType, DataType, CallJs> New(
        napi_env env,
        const Function& callback,
        ResourceString resourceName,
        size_t maxQueueSize,
        size_t initialThreadCount,
        ContextType* context = nullptr);

    // This API may only be called from the main thread.
    // Creates a new threadsafe function with:
    //   Callback [passed] Resource [passed] Finalizer [missing]
    template <typename ResourceString>
    static TypedThreadSafeFunction<ContextType, DataType, CallJs> New(
        napi_env env,
        const Function& callback,
        const Object& resource,
        ResourceString resourceName,
        size_t maxQueueSize,
        size_t initialThreadCount,
        ContextType* context = nullptr);

    // This API may only be called from the main thread.
    // Creates a new threadsafe function with:
    //   Callback [passed] Resource [missing] Finalizer [passed]
    template <typename ResourceString,
              typename Finalizer,
              typename FinalizerDataType = void>
    static TypedThreadSafeFunction<ContextType, DataType, CallJs> New(
        napi_env env,
        const Function& callback,
        ResourceString resourceName,
        size_t maxQueueSize,
        size_t initialThreadCount,
        ContextType* context,
        Finalizer finalizeCallback,
        FinalizerDataType* data = nullptr);

    // This API may only be called from the main thread.
    // Creates a new threadsafe function with:
    //   Callback [passed] Resource [passed] Finalizer [passed]
    template <typename CallbackType,
              typename ResourceString,
              typename Finalizer,
              typename FinalizerDataType>
    static TypedThreadSafeFunction<ContextType, DataType, CallJs> New(
        napi_env env,
        CallbackType callback,
        const Object& resource,
        ResourceString resourceName,
        size_t maxQueueSize,
        size_t initialThreadCount,
        ContextType* context,
        Finalizer finalizeCallback,
        FinalizerDataType* data = nullptr);

    TypedThreadSafeFunction<ContextType, DataType, CallJs>();
    TypedThreadSafeFunction<ContextType, DataType, CallJs>(
        napi_threadsafe_function tsFunctionValue);

    operator napi_threadsafe_function() const;

    // This API may be called from any thread.
    napi_status BlockingCall(DataType* data = nullptr) const;

    // This API may be called from any thread.
    napi_status NonBlockingCall(DataType* data = nullptr) const;

    // This API may only be called from the main thread.
    void Ref(napi_env env) const;

    // This API may only be called from the main thread.
    void Unref(napi_env env) const;

    // This API may be called from any thread.
    napi_status Acquire() const;

    // This API may be called from any thread.
    napi_status Release();

    // This API may be called from any thread.
    napi_status Abort();

    // This API may be called from any thread.
    ContextType* GetContext() const;

   private:
    template <typename ResourceString,
              typename Finalizer,
              typename FinalizerDataType>
    static TypedThreadSafeFunction<ContextType, DataType, CallJs> New(
        napi_env env,
        const Function& callback,
        const Object& resource,
        ResourceString resourceName,
        size_t maxQueueSize,
        size_t initialThreadCount,
        ContextType* context,
        Finalizer finalizeCallback,
        FinalizerDataType* data,
        napi_finalize wrapper);

    static void CallJsInternal(napi_env env,
                               napi_value jsCallback,
                               void* context,
                               void* data);

   protected:
    napi_threadsafe_function _tsfn;
  };
  template <typename DataType>
  class AsyncProgressWorkerBase : public AsyncWorker {
    public:
     virtual void OnWorkProgress(DataType* data) = 0;
     class ThreadSafeData {
       public:
        ThreadSafeData(AsyncProgressWorkerBase* asyncprogressworker, DataType* data)
          : _asyncprogressworker(asyncprogressworker), _data(data) {}

        AsyncProgressWorkerBase* asyncprogressworker() { return _asyncprogressworker; };
        DataType* data() { return _data; };

       private:
        AsyncProgressWorkerBase* _asyncprogressworker;
        DataType* _data;
     };
     void OnWorkComplete(Napi::Env env, napi_status status) override;
    protected:
     explicit AsyncProgressWorkerBase(const Object& receiver,
                                      const Function& callback,
                                      const char* resource_name,
                                      const Object& resource,
                                      size_t queue_size = 1);
    virtual ~AsyncProgressWorkerBase();

// Optional callback of Napi::ThreadSafeFunction only available after NAPI_VERSION 4.
// Refs: https://github.com/nodejs/node/pull/27791
#if NAPI_VERSION > 4
     explicit AsyncProgressWorkerBase(Napi::Env env,
                                      const char* resource_name,
                                      const Object& resource,
                                      size_t queue_size = 1);
#endif

     static inline void OnAsyncWorkProgress(Napi::Env env,
                                            Napi::Function jsCallback,
                                            void* data);

     napi_status NonBlockingCall(DataType* data);

    private:
     ThreadSafeFunction _tsfn;
     bool _work_completed = false;
     napi_status _complete_status;
     static inline void OnThreadSafeFunctionFinalize(Napi::Env env, void* data, AsyncProgressWorkerBase* context);
  };

  template<class T>
  class AsyncProgressWorker : public AsyncProgressWorkerBase<void> {
    public:
     virtual ~AsyncProgressWorker();

     class ExecutionProgress {
        friend class AsyncProgressWorker;
       public:
        void Signal() const;
        void Send(const T* data, size_t count) const;
       private:
        explicit ExecutionProgress(AsyncProgressWorker* worker) : _worker(worker) {}
        AsyncProgressWorker* const _worker;
     };

     void OnWorkProgress(void*) override;

    protected:
     explicit AsyncProgressWorker(const Function& callback);
     explicit AsyncProgressWorker(const Function& callback,
                                  const char* resource_name);
     explicit AsyncProgressWorker(const Function& callback,
                                  const char* resource_name,
                                  const Object& resource);
     explicit AsyncProgressWorker(const Object& receiver,
                                  const Function& callback);
     explicit AsyncProgressWorker(const Object& receiver,
                                  const Function& callback,
                                  const char* resource_name);
     explicit AsyncProgressWorker(const Object& receiver,
                                  const Function& callback,
                                  const char* resource_name,
                                  const Object& resource);

// Optional callback of Napi::ThreadSafeFunction only available after NAPI_VERSION 4.
// Refs: https://github.com/nodejs/node/pull/27791
#if NAPI_VERSION > 4
     explicit AsyncProgressWorker(Napi::Env env);
     explicit AsyncProgressWorker(Napi::Env env,
                                  const char* resource_name);
     explicit AsyncProgressWorker(Napi::Env env,
                                  const char* resource_name,
                                  const Object& resource);
#endif
     virtual void Execute(const ExecutionProgress& progress) = 0;
     virtual void OnProgress(const T* data, size_t count) = 0;

    private:
     void Execute() override;
     void Signal() const;
     void SendProgress_(const T* data, size_t count);

     std::mutex _mutex;
     T* _asyncdata;
     size_t _asyncsize;
  };

  template<class T>
  class AsyncProgressQueueWorker : public AsyncProgressWorkerBase<std::pair<T*, size_t>> {
    public:
     virtual ~AsyncProgressQueueWorker() {};

     class ExecutionProgress {
        friend class AsyncProgressQueueWorker;
       public:
        void Signal() const;
        void Send(const T* data, size_t count) const;
       private:
        explicit ExecutionProgress(AsyncProgressQueueWorker* worker) : _worker(worker) {}
        AsyncProgressQueueWorker* const _worker;
     };

     void OnWorkComplete(Napi::Env env, napi_status status) override;
     void OnWorkProgress(std::pair<T*, size_t>*) override;

    protected:
     explicit AsyncProgressQueueWorker(const Function& callback);
     explicit AsyncProgressQueueWorker(const Function& callback,
                                       const char* resource_name);
     explicit AsyncProgressQueueWorker(const Function& callback,
                                       const char* resource_name,
                                       const Object& resource);
     explicit AsyncProgressQueueWorker(const Object& receiver,
                                       const Function& callback);
     explicit AsyncProgressQueueWorker(const Object& receiver,
                                       const Function& callback,
                                       const char* resource_name);
     explicit AsyncProgressQueueWorker(const Object& receiver,
                                       const Function& callback,
                                       const char* resource_name,
                                       const Object& resource);

// Optional callback of Napi::ThreadSafeFunction only available after NAPI_VERSION 4.
// Refs: https://github.com/nodejs/node/pull/27791
#if NAPI_VERSION > 4
     explicit AsyncProgressQueueWorker(Napi::Env env);
     explicit AsyncProgressQueueWorker(Napi::Env env,
                                       const char* resource_name);
     explicit AsyncProgressQueueWorker(Napi::Env env,
                                       const char* resource_name,
                                       const Object& resource);
#endif
     virtual void Execute(const ExecutionProgress& progress) = 0;
     virtual void OnProgress(const T* data, size_t count) = 0;

    private:
     void Execute() override;
     void Signal() const;
     void SendProgress_(const T* data, size_t count);
  };
  #endif  // NAPI_VERSION > 3 && !defined(__wasm32__)

  // Memory management.
  class MemoryManagement {
    public:
      static int64_t AdjustExternalMemory(Env env, int64_t change_in_bytes);
  };

  // Version management
  class VersionManagement {
    public:
      static uint32_t GetNapiVersion(Env env);
      static const napi_node_version* GetNodeVersion(Env env);
  };

#if NAPI_VERSION > 5
  template <typename T>
  class Addon : public InstanceWrap<T> {
   public:
    static inline Object Init(Env env, Object exports);
    static T* Unwrap(Object wrapper);

   protected:
    using AddonProp = ClassPropertyDescriptor<T>;
    void DefineAddon(Object exports,
                     const std::initializer_list<AddonProp>& props);
    Napi::Object DefineProperties(Object object,
                                 const std::initializer_list<AddonProp>& props);

   private:
    Object entry_point_;
  };
#endif  // NAPI_VERSION > 5
} // namespace Napi

// Inline implementations of all the above class methods are included here.
// #include "napi-inl.h"

// hack-sqlite
// #endif // SRC_NAPI_H_


/*
file https://github.com/nodejs/node-addon-api/blob/3.2.1/napi-inl.h
*/
#ifndef SRC_NAPI_INL_H_
#define SRC_NAPI_INL_H_

////////////////////////////////////////////////////////////////////////////////
// Node-API C++ Wrapper Classes
//
// Inline header-only implementations for "Node-API" ABI-stable C APIs for
// Node.js.
////////////////////////////////////////////////////////////////////////////////

// Note: Do not include this file directly! Include "napi.h" instead.

// #include <algorithm>
// #include <cstring>
// #include <mutex>
// #include <type_traits>

namespace Napi {

// Helpers to handle functions exposed from C++.
namespace details {

// Attach a data item to an object and delete it when the object gets
// garbage-collected.
// TODO: Replace this code with `napi_add_finalizer()` whenever it becomes
// available on all supported versions of Node.js.
template <typename FreeType>
static inline napi_status AttachData(napi_env env,
                                     napi_value obj,
                                     FreeType* data,
                                     napi_finalize finalizer = nullptr,
                                     void* hint = nullptr) {
  napi_status status;
  if (finalizer == nullptr) {
    finalizer = [](napi_env /*env*/, void* data, void* /*hint*/) {
      delete static_cast<FreeType*>(data);
    };
  }
#if (NAPI_VERSION < 5)
  napi_value symbol, external;
  status = napi_create_symbol(env, nullptr, &symbol);
  if (status == napi_ok) {
    status = napi_create_external(env,
                              data,
                              finalizer,
                              hint,
                              &external);
    if (status == napi_ok) {
      napi_property_descriptor desc = {
        nullptr,
        symbol,
        nullptr,
        nullptr,
        nullptr,
        external,
        napi_default,
        nullptr
      };
      status = napi_define_properties(env, obj, 1, &desc);
    }
  }
#else  // NAPI_VERSION >= 5
  status = napi_add_finalizer(env, obj, data, finalizer, hint, nullptr);
#endif
  return status;
}

// For use in JS to C++ callback wrappers to catch any Napi::Error exceptions
// and rethrow them as JavaScript exceptions before returning from the callback.
template <typename Callable>
inline napi_value WrapCallback(Callable callback) {
#ifdef NAPI_CPP_EXCEPTIONS
  try {
    return callback();
  } catch (const Error& e) {
    e.ThrowAsJavaScriptException();
    return nullptr;
  }
#else // NAPI_CPP_EXCEPTIONS
  // When C++ exceptions are disabled, errors are immediately thrown as JS
  // exceptions, so there is no need to catch and rethrow them here.
  return callback();
#endif // NAPI_CPP_EXCEPTIONS
}

// For use in JS to C++ void callback wrappers to catch any Napi::Error
// exceptions and rethrow them as JavaScript exceptions before returning from the
// callback.
template <typename Callable>
inline void WrapVoidCallback(Callable callback) {
#ifdef NAPI_CPP_EXCEPTIONS
  try {
    callback();
  } catch (const Error& e) {
    e.ThrowAsJavaScriptException();
  }
#else // NAPI_CPP_EXCEPTIONS
  // When C++ exceptions are disabled, errors are immediately thrown as JS
  // exceptions, so there is no need to catch and rethrow them here.
  callback();
#endif // NAPI_CPP_EXCEPTIONS
}

template <typename Callable, typename Return>
struct CallbackData {
  static inline
  napi_value Wrapper(napi_env env, napi_callback_info info) {
    return details::WrapCallback([&] {
      CallbackInfo callbackInfo(env, info);
      CallbackData* callbackData =
        static_cast<CallbackData*>(callbackInfo.Data());
      callbackInfo.SetData(callbackData->data);
      return callbackData->callback(callbackInfo);
    });
  }

  Callable callback;
  void* data;
};

template <typename Callable>
struct CallbackData<Callable, void> {
  static inline
  napi_value Wrapper(napi_env env, napi_callback_info info) {
    return details::WrapCallback([&] {
      CallbackInfo callbackInfo(env, info);
      CallbackData* callbackData =
        static_cast<CallbackData*>(callbackInfo.Data());
      callbackInfo.SetData(callbackData->data);
      callbackData->callback(callbackInfo);
      return nullptr;
    });
  }

  Callable callback;
  void* data;
};

template <void (*Callback)(const CallbackInfo& info)>
static napi_value
TemplatedVoidCallback(napi_env env, napi_callback_info info) NAPI_NOEXCEPT {
  return details::WrapCallback([&] {
    CallbackInfo cbInfo(env, info);
    Callback(cbInfo);
    return nullptr;
  });
}

template <Napi::Value (*Callback)(const CallbackInfo& info)>
static napi_value
TemplatedCallback(napi_env env, napi_callback_info info) NAPI_NOEXCEPT {
  return details::WrapCallback([&] {
    CallbackInfo cbInfo(env, info);
    return Callback(cbInfo);
  });
}

template <typename T,
          Napi::Value (T::*UnwrapCallback)(const CallbackInfo& info)>
static napi_value
TemplatedInstanceCallback(napi_env env, napi_callback_info info) NAPI_NOEXCEPT {
  return details::WrapCallback([&] {
    CallbackInfo cbInfo(env, info);
    T* instance = T::Unwrap(cbInfo.This().As<Object>());
    return (instance->*UnwrapCallback)(cbInfo);
  });
}

template <typename T, void (T::*UnwrapCallback)(const CallbackInfo& info)>
static napi_value
TemplatedInstanceVoidCallback(napi_env env,
                              napi_callback_info info) NAPI_NOEXCEPT {
  return details::WrapCallback([&] {
    CallbackInfo cbInfo(env, info);
    T* instance = T::Unwrap(cbInfo.This().As<Object>());
    (instance->*UnwrapCallback)(cbInfo);
    return nullptr;
  });
}

template <typename T, typename Finalizer, typename Hint = void>
struct FinalizeData {
  static inline void Wrapper(napi_env env,
                             void* data,
                             void* finalizeHint) NAPI_NOEXCEPT {
    WrapVoidCallback([&] {
      FinalizeData* finalizeData = static_cast<FinalizeData*>(finalizeHint);
      finalizeData->callback(Env(env), static_cast<T*>(data));
      delete finalizeData;
    });
  }

  static inline void WrapperWithHint(napi_env env,
                                     void* data,
                                     void* finalizeHint) NAPI_NOEXCEPT {
    WrapVoidCallback([&] {
      FinalizeData* finalizeData = static_cast<FinalizeData*>(finalizeHint);
      finalizeData->callback(Env(env), static_cast<T*>(data), finalizeData->hint);
      delete finalizeData;
    });
  }

  Finalizer callback;
  Hint* hint;
};

#if (NAPI_VERSION > 3 && !defined(__wasm32__))
template <typename ContextType=void,
          typename Finalizer=std::function<void(Env, void*, ContextType*)>,
          typename FinalizerDataType=void>
struct ThreadSafeFinalize {
  static inline
  void Wrapper(napi_env env, void* rawFinalizeData, void* /* rawContext */) {
    if (rawFinalizeData == nullptr)
      return;

    ThreadSafeFinalize* finalizeData =
        static_cast<ThreadSafeFinalize*>(rawFinalizeData);
    finalizeData->callback(Env(env));
    delete finalizeData;
  }

  static inline
  void FinalizeWrapperWithData(napi_env env,
                               void* rawFinalizeData,
                               void* /* rawContext */) {
    if (rawFinalizeData == nullptr)
      return;

    ThreadSafeFinalize* finalizeData =
        static_cast<ThreadSafeFinalize*>(rawFinalizeData);
    finalizeData->callback(Env(env), finalizeData->data);
    delete finalizeData;
  }

  static inline
  void FinalizeWrapperWithContext(napi_env env,
                                  void* rawFinalizeData,
                                  void* rawContext) {
    if (rawFinalizeData == nullptr)
      return;

    ThreadSafeFinalize* finalizeData =
        static_cast<ThreadSafeFinalize*>(rawFinalizeData);
    finalizeData->callback(Env(env), static_cast<ContextType*>(rawContext));
    delete finalizeData;
  }

  static inline
  void FinalizeFinalizeWrapperWithDataAndContext(napi_env env,
                                         void* rawFinalizeData,
                                         void* rawContext) {
    if (rawFinalizeData == nullptr)
      return;

    ThreadSafeFinalize* finalizeData =
        static_cast<ThreadSafeFinalize*>(rawFinalizeData);
    finalizeData->callback(Env(env), finalizeData->data,
        static_cast<ContextType*>(rawContext));
    delete finalizeData;
  }

  FinalizerDataType* data;
  Finalizer callback;
};

template <typename ContextType, typename DataType, typename CallJs, CallJs call>
typename std::enable_if<call != nullptr>::type static inline CallJsWrapper(
    napi_env env, napi_value jsCallback, void* context, void* data) {
  call(env,
       Function(env, jsCallback),
       static_cast<ContextType*>(context),
       static_cast<DataType*>(data));
}

template <typename ContextType, typename DataType, typename CallJs, CallJs call>
typename std::enable_if<call == nullptr>::type static inline CallJsWrapper(
    napi_env env, napi_value jsCallback, void* /*context*/, void* /*data*/) {
  if (jsCallback != nullptr) {
    Function(env, jsCallback).Call(0, nullptr);
  }
}

#if NAPI_VERSION > 4

template <typename CallbackType, typename TSFN>
napi_value DefaultCallbackWrapper(napi_env /*env*/, std::nullptr_t /*cb*/) {
  return nullptr;
}

template <typename CallbackType, typename TSFN>
napi_value DefaultCallbackWrapper(napi_env /*env*/, Napi::Function cb) {
  return cb;
}

#else
template <typename CallbackType, typename TSFN>
napi_value DefaultCallbackWrapper(napi_env env, Napi::Function cb) {
  if (cb.IsEmpty()) {
    return TSFN::EmptyFunctionFactory(env);
  }
  return cb;
}
#endif  // NAPI_VERSION > 4
#endif  // NAPI_VERSION > 3 && !defined(__wasm32__)

template <typename Getter, typename Setter>
struct AccessorCallbackData {
  static inline
  napi_value GetterWrapper(napi_env env, napi_callback_info info) {
    return details::WrapCallback([&] {
      CallbackInfo callbackInfo(env, info);
      AccessorCallbackData* callbackData =
        static_cast<AccessorCallbackData*>(callbackInfo.Data());
      callbackInfo.SetData(callbackData->data);
      return callbackData->getterCallback(callbackInfo);
    });
  }

  static inline
  napi_value SetterWrapper(napi_env env, napi_callback_info info) {
    return details::WrapCallback([&] {
      CallbackInfo callbackInfo(env, info);
      AccessorCallbackData* callbackData =
        static_cast<AccessorCallbackData*>(callbackInfo.Data());
      callbackInfo.SetData(callbackData->data);
      callbackData->setterCallback(callbackInfo);
      return nullptr;
    });
  }

  Getter getterCallback;
  Setter setterCallback;
  void* data;
};
}  // namespace details

#ifndef NODE_ADDON_API_DISABLE_DEPRECATED
// hack-sqlite
// # include "napi-inl.deprecated.h"
#endif // !NODE_ADDON_API_DISABLE_DEPRECATED

////////////////////////////////////////////////////////////////////////////////
// Module registration
////////////////////////////////////////////////////////////////////////////////

// Register an add-on based on an initializer function.
#define NODE_API_MODULE(modname, regfunc)                                      \
  static napi_value __napi_##regfunc(napi_env env, napi_value exports) {       \
    return Napi::RegisterModule(env, exports, regfunc);                        \
  }                                                                            \
  NAPI_MODULE(modname, __napi_##regfunc)

// Register an add-on based on a subclass of `Addon<T>` with a custom Node.js
// module name.
#define NODE_API_NAMED_ADDON(modname, classname)                 \
  static napi_value __napi_ ## classname(napi_env env,           \
                                         napi_value exports) {   \
    return Napi::RegisterModule(env, exports, &classname::Init); \
  }                                                              \
  NAPI_MODULE(modname, __napi_ ## classname)

// Register an add-on based on a subclass of `Addon<T>` with the Node.js module
// name given by node-gyp from the `target_name` in binding.gyp.
#define NODE_API_ADDON(classname) \
  NODE_API_NAMED_ADDON(NODE_GYP_MODULE_NAME, classname)

// Adapt the NAPI_MODULE registration function:
//  - Wrap the arguments in NAPI wrappers.
//  - Catch any NAPI errors and rethrow as JS exceptions.
inline napi_value RegisterModule(napi_env env,
                                 napi_value exports,
                                 ModuleRegisterCallback registerCallback) {
  return details::WrapCallback([&] {
    return napi_value(registerCallback(Napi::Env(env),
                                       Napi::Object(env, exports)));
  });
}

////////////////////////////////////////////////////////////////////////////////
// Env class
////////////////////////////////////////////////////////////////////////////////

inline Env::Env(napi_env env) : _env(env) {
}

inline Env::operator napi_env() const {
  return _env;
}

inline Object Env::Global() const {
  napi_value value;
  napi_status status = napi_get_global(*this, &value);
  NAPI_THROW_IF_FAILED(*this, status, Object());
  return Object(*this, value);
}

inline Value Env::Undefined() const {
  napi_value value;
  napi_status status = napi_get_undefined(*this, &value);
  NAPI_THROW_IF_FAILED(*this, status, Value());
  return Value(*this, value);
}

inline Value Env::Null() const {
  napi_value value;
  napi_status status = napi_get_null(*this, &value);
  NAPI_THROW_IF_FAILED(*this, status, Value());
  return Value(*this, value);
}

inline bool Env::IsExceptionPending() const {
  bool result;
  napi_status status = napi_is_exception_pending(_env, &result);
  if (status != napi_ok) result = false; // Checking for a pending exception shouldn't throw.
  return result;
}

inline Error Env::GetAndClearPendingException() {
  napi_value value;
  napi_status status = napi_get_and_clear_last_exception(_env, &value);
  if (status != napi_ok) {
    // Don't throw another exception when failing to get the exception!
    return Error();
  }
  return Error(_env, value);
}

inline Value Env::RunScript(const char* utf8script) {
  String script = String::New(_env, utf8script);
  return RunScript(script);
}

inline Value Env::RunScript(const std::string& utf8script) {
  return RunScript(utf8script.c_str());
}

inline Value Env::RunScript(String script) {
  napi_value result;
  napi_status status = napi_run_script(_env, script, &result);
  NAPI_THROW_IF_FAILED(_env, status, Undefined());
  return Value(_env, result);
}

#if NAPI_VERSION > 5
template <typename T, Env::Finalizer<T> fini>
inline void Env::SetInstanceData(T* data) {
  napi_status status =
    napi_set_instance_data(_env, data, [](napi_env env, void* data, void*) {
      fini(env, static_cast<T*>(data));
    }, nullptr);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

template <typename DataType,
          typename HintType,
          Napi::Env::FinalizerWithHint<DataType, HintType> fini>
inline void Env::SetInstanceData(DataType* data, HintType* hint) {
  napi_status status =
    napi_set_instance_data(_env, data,
      [](napi_env env, void* data, void* hint) {
        fini(env, static_cast<DataType*>(data), static_cast<HintType*>(hint));
      }, hint);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

template <typename T>
inline T* Env::GetInstanceData() {
  void* data = nullptr;

  napi_status status = napi_get_instance_data(_env, &data);
  NAPI_THROW_IF_FAILED(_env, status, nullptr);

  return static_cast<T*>(data);
}

template <typename T> void Env::DefaultFini(Env, T* data) {
  delete data;
}

template <typename DataType, typename HintType>
void Env::DefaultFiniWithHint(Env, DataType* data, HintType*) {
  delete data;
}
#endif  // NAPI_VERSION > 5

////////////////////////////////////////////////////////////////////////////////
// Value class
////////////////////////////////////////////////////////////////////////////////

inline Value::Value() : _env(nullptr), _value(nullptr) {
}

inline Value::Value(napi_env env, napi_value value) : _env(env), _value(value) {
}

inline Value::operator napi_value() const {
  return _value;
}

inline bool Value::operator ==(const Value& other) const {
  return StrictEquals(other);
}

inline bool Value::operator !=(const Value& other) const {
  return !this->operator ==(other);
}

inline bool Value::StrictEquals(const Value& other) const {
  bool result;
  napi_status status = napi_strict_equals(_env, *this, other, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline Napi::Env Value::Env() const {
  return Napi::Env(_env);
}

inline bool Value::IsEmpty() const {
  return _value == nullptr;
}

inline napi_valuetype Value::Type() const {
  if (IsEmpty()) {
    return napi_undefined;
  }

  napi_valuetype type;
  napi_status status = napi_typeof(_env, _value, &type);
  NAPI_THROW_IF_FAILED(_env, status, napi_undefined);
  return type;
}

inline bool Value::IsUndefined() const {
  return Type() == napi_undefined;
}

inline bool Value::IsNull() const {
  return Type() == napi_null;
}

inline bool Value::IsBoolean() const {
  return Type() == napi_boolean;
}

inline bool Value::IsNumber() const {
  return Type() == napi_number;
}

#if NAPI_VERSION > 5
inline bool Value::IsBigInt() const {
  return Type() == napi_bigint;
}
#endif  // NAPI_VERSION > 5

#if (NAPI_VERSION > 4)
inline bool Value::IsDate() const {
  if (IsEmpty()) {
    return false;
  }

  bool result;
  napi_status status = napi_is_date(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}
#endif

inline bool Value::IsString() const {
  return Type() == napi_string;
}

inline bool Value::IsSymbol() const {
  return Type() == napi_symbol;
}

inline bool Value::IsArray() const {
  if (IsEmpty()) {
    return false;
  }

  bool result;
  napi_status status = napi_is_array(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline bool Value::IsArrayBuffer() const {
  if (IsEmpty()) {
    return false;
  }

  bool result;
  napi_status status = napi_is_arraybuffer(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline bool Value::IsTypedArray() const {
  if (IsEmpty()) {
    return false;
  }

  bool result;
  napi_status status = napi_is_typedarray(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline bool Value::IsObject() const {
  return Type() == napi_object || IsFunction();
}

inline bool Value::IsFunction() const {
  return Type() == napi_function;
}

inline bool Value::IsPromise() const {
  if (IsEmpty()) {
    return false;
  }

  bool result;
  napi_status status = napi_is_promise(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline bool Value::IsDataView() const {
  if (IsEmpty()) {
    return false;
  }

  bool result;
  napi_status status = napi_is_dataview(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline bool Value::IsBuffer() const {
  if (IsEmpty()) {
    return false;
  }

  bool result;
  napi_status status = napi_is_buffer(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline bool Value::IsExternal() const {
  return Type() == napi_external;
}

template <typename T>
inline T Value::As() const {
  return T(_env, _value);
}

inline Boolean Value::ToBoolean() const {
  napi_value result;
  napi_status status = napi_coerce_to_bool(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, Boolean());
  return Boolean(_env, result);
}

inline Number Value::ToNumber() const {
  napi_value result;
  napi_status status = napi_coerce_to_number(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, Number());
  return Number(_env, result);
}

inline String Value::ToString() const {
  napi_value result;
  napi_status status = napi_coerce_to_string(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, String());
  return String(_env, result);
}

inline Object Value::ToObject() const {
  napi_value result;
  napi_status status = napi_coerce_to_object(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, Object());
  return Object(_env, result);
}

////////////////////////////////////////////////////////////////////////////////
// Boolean class
////////////////////////////////////////////////////////////////////////////////

inline Boolean Boolean::New(napi_env env, bool val) {
  napi_value value;
  napi_status status = napi_get_boolean(env, val, &value);
  NAPI_THROW_IF_FAILED(env, status, Boolean());
  return Boolean(env, value);
}

inline Boolean::Boolean() : Napi::Value() {
}

inline Boolean::Boolean(napi_env env, napi_value value) : Napi::Value(env, value) {
}

inline Boolean::operator bool() const {
  return Value();
}

inline bool Boolean::Value() const {
  bool result;
  napi_status status = napi_get_value_bool(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

////////////////////////////////////////////////////////////////////////////////
// Number class
////////////////////////////////////////////////////////////////////////////////

inline Number Number::New(napi_env env, double val) {
  napi_value value;
  napi_status status = napi_create_double(env, val, &value);
  NAPI_THROW_IF_FAILED(env, status, Number());
  return Number(env, value);
}

inline Number::Number() : Value() {
}

inline Number::Number(napi_env env, napi_value value) : Value(env, value) {
}

inline Number::operator int32_t() const {
  return Int32Value();
}

inline Number::operator uint32_t() const {
  return Uint32Value();
}

inline Number::operator int64_t() const {
  return Int64Value();
}

inline Number::operator float() const {
  return FloatValue();
}

inline Number::operator double() const {
  return DoubleValue();
}

inline int32_t Number::Int32Value() const {
  int32_t result;
  napi_status status = napi_get_value_int32(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, 0);
  return result;
}

inline uint32_t Number::Uint32Value() const {
  uint32_t result;
  napi_status status = napi_get_value_uint32(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, 0);
  return result;
}

inline int64_t Number::Int64Value() const {
  int64_t result;
  napi_status status = napi_get_value_int64(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, 0);
  return result;
}

inline float Number::FloatValue() const {
  return static_cast<float>(DoubleValue());
}

inline double Number::DoubleValue() const {
  double result;
  napi_status status = napi_get_value_double(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, 0);
  return result;
}

#if NAPI_VERSION > 5
////////////////////////////////////////////////////////////////////////////////
// BigInt Class
////////////////////////////////////////////////////////////////////////////////

inline BigInt BigInt::New(napi_env env, int64_t val) {
  napi_value value;
  napi_status status = napi_create_bigint_int64(env, val, &value);
  NAPI_THROW_IF_FAILED(env, status, BigInt());
  return BigInt(env, value);
}

inline BigInt BigInt::New(napi_env env, uint64_t val) {
  napi_value value;
  napi_status status = napi_create_bigint_uint64(env, val, &value);
  NAPI_THROW_IF_FAILED(env, status, BigInt());
  return BigInt(env, value);
}

inline BigInt BigInt::New(napi_env env, int sign_bit, size_t word_count, const uint64_t* words) {
  napi_value value;
  napi_status status = napi_create_bigint_words(env, sign_bit, word_count, words, &value);
  NAPI_THROW_IF_FAILED(env, status, BigInt());
  return BigInt(env, value);
}

inline BigInt::BigInt() : Value() {
}

inline BigInt::BigInt(napi_env env, napi_value value) : Value(env, value) {
}

inline int64_t BigInt::Int64Value(bool* lossless) const {
  int64_t result;
  napi_status status = napi_get_value_bigint_int64(
      _env, _value, &result, lossless);
  NAPI_THROW_IF_FAILED(_env, status, 0);
  return result;
}

inline uint64_t BigInt::Uint64Value(bool* lossless) const {
  uint64_t result;
  napi_status status = napi_get_value_bigint_uint64(
      _env, _value, &result, lossless);
  NAPI_THROW_IF_FAILED(_env, status, 0);
  return result;
}

inline size_t BigInt::WordCount() const {
  size_t word_count;
  napi_status status = napi_get_value_bigint_words(
      _env, _value, nullptr, &word_count, nullptr);
  NAPI_THROW_IF_FAILED(_env, status, 0);
  return word_count;
}

inline void BigInt::ToWords(int* sign_bit, size_t* word_count, uint64_t* words) {
  napi_status status = napi_get_value_bigint_words(
      _env, _value, sign_bit, word_count, words);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}
#endif  // NAPI_VERSION > 5

#if (NAPI_VERSION > 4)
////////////////////////////////////////////////////////////////////////////////
// Date Class
////////////////////////////////////////////////////////////////////////////////

inline Date Date::New(napi_env env, double val) {
  napi_value value;
  napi_status status = napi_create_date(env, val, &value);
  NAPI_THROW_IF_FAILED(env, status, Date());
  return Date(env, value);
}

inline Date::Date() : Value() {
}

inline Date::Date(napi_env env, napi_value value) : Value(env, value) {
}

inline Date::operator double() const {
  return ValueOf();
}

inline double Date::ValueOf() const {
  double result;
  napi_status status = napi_get_date_value(
      _env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, 0);
  return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// Name class
////////////////////////////////////////////////////////////////////////////////

inline Name::Name() : Value() {
}

inline Name::Name(napi_env env, napi_value value) : Value(env, value) {
}

////////////////////////////////////////////////////////////////////////////////
// String class
////////////////////////////////////////////////////////////////////////////////

inline String String::New(napi_env env, const std::string& val) {
  return String::New(env, val.c_str(), val.size());
}

inline String String::New(napi_env env, const std::u16string& val) {
  return String::New(env, val.c_str(), val.size());
}

inline String String::New(napi_env env, const char* val) {
  napi_value value;
  napi_status status = napi_create_string_utf8(env, val, std::strlen(val), &value);
  NAPI_THROW_IF_FAILED(env, status, String());
  return String(env, value);
}

inline String String::New(napi_env env, const char16_t* val) {
  napi_value value;
  napi_status status = napi_create_string_utf16(env, val, std::u16string(val).size(), &value);
  NAPI_THROW_IF_FAILED(env, status, String());
  return String(env, value);
}

inline String String::New(napi_env env, const char* val, size_t length) {
  napi_value value;
  napi_status status = napi_create_string_utf8(env, val, length, &value);
  NAPI_THROW_IF_FAILED(env, status, String());
  return String(env, value);
}

inline String String::New(napi_env env, const char16_t* val, size_t length) {
  napi_value value;
  napi_status status = napi_create_string_utf16(env, val, length, &value);
  NAPI_THROW_IF_FAILED(env, status, String());
  return String(env, value);
}

inline String::String() : Name() {
}

inline String::String(napi_env env, napi_value value) : Name(env, value) {
}

inline String::operator std::string() const {
  return Utf8Value();
}

inline String::operator std::u16string() const {
  return Utf16Value();
}

inline std::string String::Utf8Value() const {
  size_t length;
  napi_status status = napi_get_value_string_utf8(_env, _value, nullptr, 0, &length);
  NAPI_THROW_IF_FAILED(_env, status, "");

  std::string value;
  value.reserve(length + 1);
  value.resize(length);
  status = napi_get_value_string_utf8(_env, _value, &value[0], value.capacity(), nullptr);
  NAPI_THROW_IF_FAILED(_env, status, "");
  return value;
}

inline std::u16string String::Utf16Value() const {
  size_t length;
  napi_status status = napi_get_value_string_utf16(_env, _value, nullptr, 0, &length);
  NAPI_THROW_IF_FAILED(_env, status, NAPI_WIDE_TEXT(""));

  std::u16string value;
  value.reserve(length + 1);
  value.resize(length);
  status = napi_get_value_string_utf16(_env, _value, &value[0], value.capacity(), nullptr);
  NAPI_THROW_IF_FAILED(_env, status, NAPI_WIDE_TEXT(""));
  return value;
}

////////////////////////////////////////////////////////////////////////////////
// Symbol class
////////////////////////////////////////////////////////////////////////////////

inline Symbol Symbol::New(napi_env env, const char* description) {
  napi_value descriptionValue = description != nullptr ?
    String::New(env, description) : static_cast<napi_value>(nullptr);
  return Symbol::New(env, descriptionValue);
}

inline Symbol Symbol::New(napi_env env, const std::string& description) {
  napi_value descriptionValue = String::New(env, description);
  return Symbol::New(env, descriptionValue);
}

inline Symbol Symbol::New(napi_env env, String description) {
  napi_value descriptionValue = description;
  return Symbol::New(env, descriptionValue);
}

inline Symbol Symbol::New(napi_env env, napi_value description) {
  napi_value value;
  napi_status status = napi_create_symbol(env, description, &value);
  NAPI_THROW_IF_FAILED(env, status, Symbol());
  return Symbol(env, value);
}

inline Symbol Symbol::WellKnown(napi_env env, const std::string& name) {
  return Napi::Env(env).Global().Get("Symbol").As<Object>().Get(name).As<Symbol>();
}

inline Symbol::Symbol() : Name() {
}

inline Symbol::Symbol(napi_env env, napi_value value) : Name(env, value) {
}

////////////////////////////////////////////////////////////////////////////////
// Automagic value creation
////////////////////////////////////////////////////////////////////////////////

namespace details {
template <typename T>
struct vf_number {
  static Number From(napi_env env, T value) {
    return Number::New(env, static_cast<double>(value));
  }
};

template<>
struct vf_number<bool> {
  static Boolean From(napi_env env, bool value) {
    return Boolean::New(env, value);
  }
};

struct vf_utf8_charp {
  static String From(napi_env env, const char* value) {
    return String::New(env, value);
  }
};

struct vf_utf16_charp {
  static String From(napi_env env, const char16_t* value) {
    return String::New(env, value);
  }
};
struct vf_utf8_string {
  static String From(napi_env env, const std::string& value) {
    return String::New(env, value);
  }
};

struct vf_utf16_string {
  static String From(napi_env env, const std::u16string& value) {
    return String::New(env, value);
  }
};

template <typename T>
struct vf_fallback {
  static Value From(napi_env env, const T& value) {
    return Value(env, value);
  }
};

template <typename...> struct disjunction : std::false_type {};
template <typename B> struct disjunction<B> : B {};
template <typename B, typename... Bs>
struct disjunction<B, Bs...>
    : std::conditional<bool(B::value), B, disjunction<Bs...>>::type {};

template <typename T>
struct can_make_string
    : disjunction<typename std::is_convertible<T, const char *>::type,
                  typename std::is_convertible<T, const char16_t *>::type,
                  typename std::is_convertible<T, std::string>::type,
                  typename std::is_convertible<T, std::u16string>::type> {};
}

template <typename T>
Value Value::From(napi_env env, const T& value) {
  using Helper = typename std::conditional<
    std::is_integral<T>::value || std::is_floating_point<T>::value,
    details::vf_number<T>,
    typename std::conditional<
      details::can_make_string<T>::value,
      String,
      details::vf_fallback<T>
    >::type
  >::type;
  return Helper::From(env, value);
}

template <typename T>
String String::From(napi_env env, const T& value) {
  struct Dummy {};
  using Helper = typename std::conditional<
    std::is_convertible<T, const char*>::value,
    details::vf_utf8_charp,
    typename std::conditional<
      std::is_convertible<T, const char16_t*>::value,
      details::vf_utf16_charp,
      typename std::conditional<
        std::is_convertible<T, std::string>::value,
        details::vf_utf8_string,
        typename std::conditional<
          std::is_convertible<T, std::u16string>::value,
          details::vf_utf16_string,
          Dummy
        >::type
      >::type
    >::type
  >::type;
  return Helper::From(env, value);
}

////////////////////////////////////////////////////////////////////////////////
// Object class
////////////////////////////////////////////////////////////////////////////////

template <typename Key>
inline Object::PropertyLValue<Key>::operator Value() const {
  return Object(_env, _object).Get(_key);
}

template <typename Key> template <typename ValueType>
inline Object::PropertyLValue<Key>& Object::PropertyLValue<Key>::operator =(ValueType value) {
  Object(_env, _object).Set(_key, value);
  return *this;
}

template <typename Key>
inline Object::PropertyLValue<Key>::PropertyLValue(Object object, Key key)
  : _env(object.Env()), _object(object), _key(key) {}

inline Object Object::New(napi_env env) {
  napi_value value;
  napi_status status = napi_create_object(env, &value);
  NAPI_THROW_IF_FAILED(env, status, Object());
  return Object(env, value);
}

inline Object::Object() : Value() {
}

inline Object::Object(napi_env env, napi_value value) : Value(env, value) {
}

inline Object::PropertyLValue<std::string> Object::operator [](const char* utf8name) {
  return PropertyLValue<std::string>(*this, utf8name);
}

inline Object::PropertyLValue<std::string> Object::operator [](const std::string& utf8name) {
  return PropertyLValue<std::string>(*this, utf8name);
}

inline Object::PropertyLValue<uint32_t> Object::operator [](uint32_t index) {
  return PropertyLValue<uint32_t>(*this, index);
}

inline Value Object::operator [](const char* utf8name) const {
  return Get(utf8name);
}

inline Value Object::operator [](const std::string& utf8name) const {
  return Get(utf8name);
}

inline Value Object::operator [](uint32_t index) const {
  return Get(index);
}

inline bool Object::Has(napi_value key) const {
  bool result;
  napi_status status = napi_has_property(_env, _value, key, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline bool Object::Has(Value key) const {
  bool result;
  napi_status status = napi_has_property(_env, _value, key, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline bool Object::Has(const char* utf8name) const {
  bool result;
  napi_status status = napi_has_named_property(_env, _value, utf8name, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline bool Object::Has(const std::string& utf8name) const {
  return Has(utf8name.c_str());
}

inline bool Object::HasOwnProperty(napi_value key) const {
  bool result;
  napi_status status = napi_has_own_property(_env, _value, key, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline bool Object::HasOwnProperty(Value key) const {
  bool result;
  napi_status status = napi_has_own_property(_env, _value, key, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline bool Object::HasOwnProperty(const char* utf8name) const {
  napi_value key;
  napi_status status = napi_create_string_utf8(_env, utf8name, std::strlen(utf8name), &key);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return HasOwnProperty(key);
}

inline bool Object::HasOwnProperty(const std::string& utf8name) const {
  return HasOwnProperty(utf8name.c_str());
}

inline Value Object::Get(napi_value key) const {
  napi_value result;
  napi_status status = napi_get_property(_env, _value, key, &result);
  NAPI_THROW_IF_FAILED(_env, status, Value());
  return Value(_env, result);
}

inline Value Object::Get(Value key) const {
  napi_value result;
  napi_status status = napi_get_property(_env, _value, key, &result);
  NAPI_THROW_IF_FAILED(_env, status, Value());
  return Value(_env, result);
}

inline Value Object::Get(const char* utf8name) const {
  napi_value result;
  napi_status status = napi_get_named_property(_env, _value, utf8name, &result);
  NAPI_THROW_IF_FAILED(_env, status, Value());
  return Value(_env, result);
}

inline Value Object::Get(const std::string& utf8name) const {
  return Get(utf8name.c_str());
}

template <typename ValueType>
inline bool Object::Set(napi_value key, const ValueType& value) {
  napi_status status =
      napi_set_property(_env, _value, key, Value::From(_env, value));
  NAPI_THROW_IF_FAILED(_env, status, false);
  return true;
}

template <typename ValueType>
inline bool Object::Set(Value key, const ValueType& value) {
  napi_status status =
      napi_set_property(_env, _value, key, Value::From(_env, value));
  NAPI_THROW_IF_FAILED(_env, status, false);
  return true;
}

template <typename ValueType>
inline bool Object::Set(const char* utf8name, const ValueType& value) {
  napi_status status =
      napi_set_named_property(_env, _value, utf8name, Value::From(_env, value));
  NAPI_THROW_IF_FAILED(_env, status, false);
  return true;
}

template <typename ValueType>
inline bool Object::Set(const std::string& utf8name, const ValueType& value) {
  return Set(utf8name.c_str(), value);
}

inline bool Object::Delete(napi_value key) {
  bool result;
  napi_status status = napi_delete_property(_env, _value, key, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline bool Object::Delete(Value key) {
  bool result;
  napi_status status = napi_delete_property(_env, _value, key, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline bool Object::Delete(const char* utf8name) {
  return Delete(String::New(_env, utf8name));
}

inline bool Object::Delete(const std::string& utf8name) {
  return Delete(String::New(_env, utf8name));
}

inline bool Object::Has(uint32_t index) const {
  bool result;
  napi_status status = napi_has_element(_env, _value, index, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline Value Object::Get(uint32_t index) const {
  napi_value value;
  napi_status status = napi_get_element(_env, _value, index, &value);
  NAPI_THROW_IF_FAILED(_env, status, Value());
  return Value(_env, value);
}

template <typename ValueType>
inline bool Object::Set(uint32_t index, const ValueType& value) {
  napi_status status =
      napi_set_element(_env, _value, index, Value::From(_env, value));
  NAPI_THROW_IF_FAILED(_env, status, false);
  return true;
}

inline bool Object::Delete(uint32_t index) {
  bool result;
  napi_status status = napi_delete_element(_env, _value, index, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

inline Array Object::GetPropertyNames() const {
  napi_value result;
  napi_status status = napi_get_property_names(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, Array());
  return Array(_env, result);
}

inline bool Object::DefineProperty(const PropertyDescriptor& property) {
  napi_status status = napi_define_properties(_env, _value, 1,
    reinterpret_cast<const napi_property_descriptor*>(&property));
  NAPI_THROW_IF_FAILED(_env, status, false);
  return true;
}

inline bool Object::DefineProperties(
    const std::initializer_list<PropertyDescriptor>& properties) {
  napi_status status = napi_define_properties(_env, _value, properties.size(),
    reinterpret_cast<const napi_property_descriptor*>(properties.begin()));
  NAPI_THROW_IF_FAILED(_env, status, false);
  return true;
}

inline bool Object::DefineProperties(
    const std::vector<PropertyDescriptor>& properties) {
  napi_status status = napi_define_properties(_env, _value, properties.size(),
    reinterpret_cast<const napi_property_descriptor*>(properties.data()));
  NAPI_THROW_IF_FAILED(_env, status, false);
  return true;
}

inline bool Object::InstanceOf(const Function& constructor) const {
  bool result;
  napi_status status = napi_instanceof(_env, _value, constructor, &result);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return result;
}

template <typename Finalizer, typename T>
inline void Object::AddFinalizer(Finalizer finalizeCallback, T* data) {
  details::FinalizeData<T, Finalizer>* finalizeData =
      new details::FinalizeData<T, Finalizer>(
          {std::move(finalizeCallback), nullptr});
  napi_status status =
      details::AttachData(_env,
                          *this,
                          data,
                          details::FinalizeData<T, Finalizer>::Wrapper,
                          finalizeData);
  if (status != napi_ok) {
    delete finalizeData;
    NAPI_THROW_IF_FAILED_VOID(_env, status);
  }
}

template <typename Finalizer, typename T, typename Hint>
inline void Object::AddFinalizer(Finalizer finalizeCallback,
                                 T* data,
                                 Hint* finalizeHint) {
  details::FinalizeData<T, Finalizer, Hint>* finalizeData =
      new details::FinalizeData<T, Finalizer, Hint>(
          {std::move(finalizeCallback), finalizeHint});
  napi_status status =
      details::AttachData(_env,
                          *this,
                          data,
                          details::FinalizeData<T, Finalizer, Hint>::WrapperWithHint,
                          finalizeData);
  if (status != napi_ok) {
    delete finalizeData;
    NAPI_THROW_IF_FAILED_VOID(_env, status);
  }
}

#if NAPI_VERSION >= 8
inline bool Object::Freeze() {
  napi_status status = napi_object_freeze(_env, _value);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return true;
}

inline bool Object::Seal() {
  napi_status status = napi_object_seal(_env, _value);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return true;
}
#endif  // NAPI_VERSION >= 8

////////////////////////////////////////////////////////////////////////////////
// External class
////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline External<T> External<T>::New(napi_env env, T* data) {
  napi_value value;
  napi_status status = napi_create_external(env, data, nullptr, nullptr, &value);
  NAPI_THROW_IF_FAILED(env, status, External());
  return External(env, value);
}

template <typename T>
template <typename Finalizer>
inline External<T> External<T>::New(napi_env env,
                                    T* data,
                                    Finalizer finalizeCallback) {
  napi_value value;
  details::FinalizeData<T, Finalizer>* finalizeData =
      new details::FinalizeData<T, Finalizer>(
          {std::move(finalizeCallback), nullptr});
  napi_status status = napi_create_external(
    env,
    data,
    details::FinalizeData<T, Finalizer>::Wrapper,
    finalizeData,
    &value);
  if (status != napi_ok) {
    delete finalizeData;
    NAPI_THROW_IF_FAILED(env, status, External());
  }
  return External(env, value);
}

template <typename T>
template <typename Finalizer, typename Hint>
inline External<T> External<T>::New(napi_env env,
                                    T* data,
                                    Finalizer finalizeCallback,
                                    Hint* finalizeHint) {
  napi_value value;
  details::FinalizeData<T, Finalizer, Hint>* finalizeData =
      new details::FinalizeData<T, Finalizer, Hint>(
          {std::move(finalizeCallback), finalizeHint});
  napi_status status = napi_create_external(
    env,
    data,
    details::FinalizeData<T, Finalizer, Hint>::WrapperWithHint,
    finalizeData,
    &value);
  if (status != napi_ok) {
    delete finalizeData;
    NAPI_THROW_IF_FAILED(env, status, External());
  }
  return External(env, value);
}

template <typename T>
inline External<T>::External() : Value() {
}

template <typename T>
inline External<T>::External(napi_env env, napi_value value) : Value(env, value) {
}

template <typename T>
inline T* External<T>::Data() const {
  void* data;
  napi_status status = napi_get_value_external(_env, _value, &data);
  NAPI_THROW_IF_FAILED(_env, status, nullptr);
  return reinterpret_cast<T*>(data);
}

////////////////////////////////////////////////////////////////////////////////
// Array class
////////////////////////////////////////////////////////////////////////////////

inline Array Array::New(napi_env env) {
  napi_value value;
  napi_status status = napi_create_array(env, &value);
  NAPI_THROW_IF_FAILED(env, status, Array());
  return Array(env, value);
}

inline Array Array::New(napi_env env, size_t length) {
  napi_value value;
  napi_status status = napi_create_array_with_length(env, length, &value);
  NAPI_THROW_IF_FAILED(env, status, Array());
  return Array(env, value);
}

inline Array::Array() : Object() {
}

inline Array::Array(napi_env env, napi_value value) : Object(env, value) {
}

inline uint32_t Array::Length() const {
  uint32_t result;
  napi_status status = napi_get_array_length(_env, _value, &result);
  NAPI_THROW_IF_FAILED(_env, status, 0);
  return result;
}

////////////////////////////////////////////////////////////////////////////////
// ArrayBuffer class
////////////////////////////////////////////////////////////////////////////////

inline ArrayBuffer ArrayBuffer::New(napi_env env, size_t byteLength) {
  napi_value value;
  void* data;
  napi_status status = napi_create_arraybuffer(env, byteLength, &data, &value);
  NAPI_THROW_IF_FAILED(env, status, ArrayBuffer());

  return ArrayBuffer(env, value);
}

inline ArrayBuffer ArrayBuffer::New(napi_env env,
                                    void* externalData,
                                    size_t byteLength) {
  napi_value value;
  napi_status status = napi_create_external_arraybuffer(
    env, externalData, byteLength, nullptr, nullptr, &value);
  NAPI_THROW_IF_FAILED(env, status, ArrayBuffer());

  return ArrayBuffer(env, value);
}

template <typename Finalizer>
inline ArrayBuffer ArrayBuffer::New(napi_env env,
                                    void* externalData,
                                    size_t byteLength,
                                    Finalizer finalizeCallback) {
  napi_value value;
  details::FinalizeData<void, Finalizer>* finalizeData =
      new details::FinalizeData<void, Finalizer>(
          {std::move(finalizeCallback), nullptr});
  napi_status status = napi_create_external_arraybuffer(
    env,
    externalData,
    byteLength,
    details::FinalizeData<void, Finalizer>::Wrapper,
    finalizeData,
    &value);
  if (status != napi_ok) {
    delete finalizeData;
    NAPI_THROW_IF_FAILED(env, status, ArrayBuffer());
  }

  return ArrayBuffer(env, value);
}

template <typename Finalizer, typename Hint>
inline ArrayBuffer ArrayBuffer::New(napi_env env,
                                    void* externalData,
                                    size_t byteLength,
                                    Finalizer finalizeCallback,
                                    Hint* finalizeHint) {
  napi_value value;
  details::FinalizeData<void, Finalizer, Hint>* finalizeData =
      new details::FinalizeData<void, Finalizer, Hint>(
          {std::move(finalizeCallback), finalizeHint});
  napi_status status = napi_create_external_arraybuffer(
    env,
    externalData,
    byteLength,
    details::FinalizeData<void, Finalizer, Hint>::WrapperWithHint,
    finalizeData,
    &value);
  if (status != napi_ok) {
    delete finalizeData;
    NAPI_THROW_IF_FAILED(env, status, ArrayBuffer());
  }

  return ArrayBuffer(env, value);
}

inline ArrayBuffer::ArrayBuffer() : Object() {
}

inline ArrayBuffer::ArrayBuffer(napi_env env, napi_value value)
  : Object(env, value) {
}

inline void* ArrayBuffer::Data() {
  void* data;
  napi_status status = napi_get_arraybuffer_info(_env, _value, &data, nullptr);
  NAPI_THROW_IF_FAILED(_env, status, nullptr);
  return data;
}

inline size_t ArrayBuffer::ByteLength() {
  size_t length;
  napi_status status = napi_get_arraybuffer_info(_env, _value, nullptr, &length);
  NAPI_THROW_IF_FAILED(_env, status, 0);
  return length;
}

#if NAPI_VERSION >= 7
inline bool ArrayBuffer::IsDetached() const {
  bool detached;
  napi_status status = napi_is_detached_arraybuffer(_env, _value, &detached);
  NAPI_THROW_IF_FAILED(_env, status, false);
  return detached;
}

inline void ArrayBuffer::Detach() {
  napi_status status = napi_detach_arraybuffer(_env, _value);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}
#endif  // NAPI_VERSION >= 7

////////////////////////////////////////////////////////////////////////////////
// DataView class
////////////////////////////////////////////////////////////////////////////////
inline DataView DataView::New(napi_env env,
                              Napi::ArrayBuffer arrayBuffer) {
  return New(env, arrayBuffer, 0, arrayBuffer.ByteLength());
}

inline DataView DataView::New(napi_env env,
                              Napi::ArrayBuffer arrayBuffer,
                              size_t byteOffset) {
  if (byteOffset > arrayBuffer.ByteLength()) {
    NAPI_THROW(RangeError::New(env,
        "Start offset is outside the bounds of the buffer"),
        DataView());
  }
  return New(env, arrayBuffer, byteOffset,
      arrayBuffer.ByteLength() - byteOffset);
}

inline DataView DataView::New(napi_env env,
                              Napi::ArrayBuffer arrayBuffer,
                              size_t byteOffset,
                              size_t byteLength) {
  if (byteOffset + byteLength > arrayBuffer.ByteLength()) {
    NAPI_THROW(RangeError::New(env, "Invalid DataView length"),
               DataView());
  }
  napi_value value;
  napi_status status = napi_create_dataview(
    env, byteLength, arrayBuffer, byteOffset, &value);
  NAPI_THROW_IF_FAILED(env, status, DataView());
  return DataView(env, value);
}

inline DataView::DataView() : Object() {
}

inline DataView::DataView(napi_env env, napi_value value) : Object(env, value) {
  napi_status status = napi_get_dataview_info(
    _env,
    _value   /* dataView */,
    &_length /* byteLength */,
    &_data   /* data */,
    nullptr  /* arrayBuffer */,
    nullptr  /* byteOffset */);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

inline Napi::ArrayBuffer DataView::ArrayBuffer() const {
  napi_value arrayBuffer;
  napi_status status = napi_get_dataview_info(
    _env,
    _value       /* dataView */,
    nullptr      /* byteLength */,
    nullptr      /* data */,
    &arrayBuffer /* arrayBuffer */,
    nullptr      /* byteOffset */);
  NAPI_THROW_IF_FAILED(_env, status, Napi::ArrayBuffer());
  return Napi::ArrayBuffer(_env, arrayBuffer);
}

inline size_t DataView::ByteOffset() const {
  size_t byteOffset;
  napi_status status = napi_get_dataview_info(
    _env,
    _value      /* dataView */,
    nullptr     /* byteLength */,
    nullptr     /* data */,
    nullptr     /* arrayBuffer */,
    &byteOffset /* byteOffset */);
  NAPI_THROW_IF_FAILED(_env, status, 0);
  return byteOffset;
}

inline size_t DataView::ByteLength() const {
  return _length;
}

inline void* DataView::Data() const {
  return _data;
}

inline float DataView::GetFloat32(size_t byteOffset) const {
  return ReadData<float>(byteOffset);
}

inline double DataView::GetFloat64(size_t byteOffset) const {
  return ReadData<double>(byteOffset);
}

inline int8_t DataView::GetInt8(size_t byteOffset) const {
  return ReadData<int8_t>(byteOffset);
}

inline int16_t DataView::GetInt16(size_t byteOffset) const {
  return ReadData<int16_t>(byteOffset);
}

inline int32_t DataView::GetInt32(size_t byteOffset) const {
  return ReadData<int32_t>(byteOffset);
}

inline uint8_t DataView::GetUint8(size_t byteOffset) const {
  return ReadData<uint8_t>(byteOffset);
}

inline uint16_t DataView::GetUint16(size_t byteOffset) const {
  return ReadData<uint16_t>(byteOffset);
}

inline uint32_t DataView::GetUint32(size_t byteOffset) const {
  return ReadData<uint32_t>(byteOffset);
}

inline void DataView::SetFloat32(size_t byteOffset, float value) const {
  WriteData<float>(byteOffset, value);
}

inline void DataView::SetFloat64(size_t byteOffset, double value) const {
  WriteData<double>(byteOffset, value);
}

inline void DataView::SetInt8(size_t byteOffset, int8_t value) const {
  WriteData<int8_t>(byteOffset, value);
}

inline void DataView::SetInt16(size_t byteOffset, int16_t value) const {
  WriteData<int16_t>(byteOffset, value);
}

inline void DataView::SetInt32(size_t byteOffset, int32_t value) const {
  WriteData<int32_t>(byteOffset, value);
}

inline void DataView::SetUint8(size_t byteOffset, uint8_t value) const {
  WriteData<uint8_t>(byteOffset, value);
}

inline void DataView::SetUint16(size_t byteOffset, uint16_t value) const {
  WriteData<uint16_t>(byteOffset, value);
}

inline void DataView::SetUint32(size_t byteOffset, uint32_t value) const {
  WriteData<uint32_t>(byteOffset, value);
}

template <typename T>
inline T DataView::ReadData(size_t byteOffset) const {
  if (byteOffset + sizeof(T) > _length ||
      byteOffset + sizeof(T) < byteOffset) {  // overflow
    NAPI_THROW(RangeError::New(_env,
        "Offset is outside the bounds of the DataView"), 0);
  }

  return *reinterpret_cast<T*>(static_cast<uint8_t*>(_data) + byteOffset);
}

template <typename T>
inline void DataView::WriteData(size_t byteOffset, T value) const {
  if (byteOffset + sizeof(T) > _length ||
      byteOffset + sizeof(T) < byteOffset) {  // overflow
    NAPI_THROW_VOID(RangeError::New(_env,
        "Offset is outside the bounds of the DataView"));
  }

  *reinterpret_cast<T*>(static_cast<uint8_t*>(_data) + byteOffset) = value;
}

////////////////////////////////////////////////////////////////////////////////
// TypedArray class
////////////////////////////////////////////////////////////////////////////////

inline TypedArray::TypedArray()
  : Object(), _type(TypedArray::unknown_array_type), _length(0) {
}

inline TypedArray::TypedArray(napi_env env, napi_value value)
  : Object(env, value), _type(TypedArray::unknown_array_type), _length(0) {
}

inline TypedArray::TypedArray(napi_env env,
                              napi_value value,
                              napi_typedarray_type type,
                              size_t length)
  : Object(env, value), _type(type), _length(length) {
}

inline napi_typedarray_type TypedArray::TypedArrayType() const {
  if (_type == TypedArray::unknown_array_type) {
    napi_status status = napi_get_typedarray_info(_env, _value,
      &const_cast<TypedArray*>(this)->_type, &const_cast<TypedArray*>(this)->_length,
      nullptr, nullptr, nullptr);
    NAPI_THROW_IF_FAILED(_env, status, napi_int8_array);
  }

  return _type;
}

inline uint8_t TypedArray::ElementSize() const {
  switch (TypedArrayType()) {
    case napi_int8_array:
    case napi_uint8_array:
    case napi_uint8_clamped_array:
      return 1;
    case napi_int16_array:
    case napi_uint16_array:
      return 2;
    case napi_int32_array:
    case napi_uint32_array:
    case napi_float32_array:
      return 4;
    case napi_float64_array:
#if (NAPI_VERSION > 5)
    case napi_bigint64_array:
    case napi_biguint64_array:
#endif  // (NAPI_VERSION > 5)
      return 8;
    default:
      return 0;
  }
}

inline size_t TypedArray::ElementLength() const {
  if (_type == TypedArray::unknown_array_type) {
    napi_status status = napi_get_typedarray_info(_env, _value,
      &const_cast<TypedArray*>(this)->_type, &const_cast<TypedArray*>(this)->_length,
      nullptr, nullptr, nullptr);
    NAPI_THROW_IF_FAILED(_env, status, 0);
  }

  return _length;
}

inline size_t TypedArray::ByteOffset() const {
  size_t byteOffset;
  napi_status status = napi_get_typedarray_info(
    _env, _value, nullptr, nullptr, nullptr, nullptr, &byteOffset);
  NAPI_THROW_IF_FAILED(_env, status, 0);
  return byteOffset;
}

inline size_t TypedArray::ByteLength() const {
  return ElementSize() * ElementLength();
}

inline Napi::ArrayBuffer TypedArray::ArrayBuffer() const {
  napi_value arrayBuffer;
  napi_status status = napi_get_typedarray_info(
    _env, _value, nullptr, nullptr, nullptr, &arrayBuffer, nullptr);
  NAPI_THROW_IF_FAILED(_env, status, Napi::ArrayBuffer());
  return Napi::ArrayBuffer(_env, arrayBuffer);
}

////////////////////////////////////////////////////////////////////////////////
// TypedArrayOf<T> class
////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline TypedArrayOf<T> TypedArrayOf<T>::New(napi_env env,
                                            size_t elementLength,
                                            napi_typedarray_type type) {
  Napi::ArrayBuffer arrayBuffer = Napi::ArrayBuffer::New(env, elementLength * sizeof (T));
  return New(env, elementLength, arrayBuffer, 0, type);
}

template <typename T>
inline TypedArrayOf<T> TypedArrayOf<T>::New(napi_env env,
                                            size_t elementLength,
                                            Napi::ArrayBuffer arrayBuffer,
                                            size_t bufferOffset,
                                            napi_typedarray_type type) {
  napi_value value;
  napi_status status = napi_create_typedarray(
    env, type, elementLength, arrayBuffer, bufferOffset, &value);
  NAPI_THROW_IF_FAILED(env, status, TypedArrayOf<T>());

  return TypedArrayOf<T>(
    env, value, type, elementLength,
    reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(arrayBuffer.Data()) + bufferOffset));
}

template <typename T>
inline TypedArrayOf<T>::TypedArrayOf() : TypedArray(), _data(nullptr) {
}

template <typename T>
inline TypedArrayOf<T>::TypedArrayOf(napi_env env, napi_value value)
  : TypedArray(env, value), _data(nullptr) {
  napi_status status = napi_ok;
  if (value != nullptr) {
    status = napi_get_typedarray_info(
      _env, _value, &_type, &_length, reinterpret_cast<void**>(&_data), nullptr, nullptr);
  } else {
    _type = TypedArrayTypeForPrimitiveType<T>();
    _length = 0;
  }
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

template <typename T>
inline TypedArrayOf<T>::TypedArrayOf(napi_env env,
                                     napi_value value,
                                     napi_typedarray_type type,
                                     size_t length,
                                     T* data)
  : TypedArray(env, value, type, length), _data(data) {
  if (!(type == TypedArrayTypeForPrimitiveType<T>() ||
      (type == napi_uint8_clamped_array && std::is_same<T, uint8_t>::value))) {
    NAPI_THROW_VOID(TypeError::New(env, "Array type must match the template parameter. "
      "(Uint8 arrays may optionally have the \"clamped\" array type.)"));
  }
}

template <typename T>
inline T& TypedArrayOf<T>::operator [](size_t index) {
  return _data[index];
}

template <typename T>
inline const T& TypedArrayOf<T>::operator [](size_t index) const {
  return _data[index];
}

template <typename T>
inline T* TypedArrayOf<T>::Data() {
  return _data;
}

template <typename T>
inline const T* TypedArrayOf<T>::Data() const {
  return _data;
}

////////////////////////////////////////////////////////////////////////////////
// Function class
////////////////////////////////////////////////////////////////////////////////

template <typename CbData>
static inline napi_status
CreateFunction(napi_env env,
               const char* utf8name,
               napi_callback cb,
               CbData* data,
               napi_value* result) {
  napi_status status =
      napi_create_function(env, utf8name, NAPI_AUTO_LENGTH, cb, data, result);
  if (status == napi_ok) {
    status = Napi::details::AttachData(env, *result, data);
  }

  return status;
}

template <Function::VoidCallback cb>
inline Function Function::New(napi_env env, const char* utf8name, void* data) {
  napi_value result = nullptr;
  napi_status status = napi_create_function(env,
                                            utf8name,
                                            NAPI_AUTO_LENGTH,
                                            details::TemplatedVoidCallback<cb>,
                                            data,
                                            &result);
  NAPI_THROW_IF_FAILED(env, status, Function());
  return Function(env, result);
}

template <Function::Callback cb>
inline Function Function::New(napi_env env, const char* utf8name, void* data) {
  napi_value result = nullptr;
  napi_status status = napi_create_function(env,
                                            utf8name,
                                            NAPI_AUTO_LENGTH,
                                            details::TemplatedCallback<cb>,
                                            data,
                                            &result);
  NAPI_THROW_IF_FAILED(env, status, Function());
  return Function(env, result);
}

template <Function::VoidCallback cb>
inline Function Function::New(napi_env env,
                              const std::string& utf8name,
                              void* data) {
  return Function::New<cb>(env, utf8name.c_str(), data);
}

template <Function::Callback cb>
inline Function Function::New(napi_env env,
                              const std::string& utf8name,
                              void* data) {
  return Function::New<cb>(env, utf8name.c_str(), data);
}

template <typename Callable>
inline Function Function::New(napi_env env,
                              Callable cb,
                              const char* utf8name,
                              void* data) {
  using ReturnType = decltype(cb(CallbackInfo(nullptr, nullptr)));
  using CbData = details::CallbackData<Callable, ReturnType>;
  auto callbackData = new CbData({ cb, data });

  napi_value value;
  napi_status status = CreateFunction(env,
                                      utf8name,
                                      CbData::Wrapper,
                                      callbackData,
                                      &value);
  if (status != napi_ok) {
    delete callbackData;
    NAPI_THROW_IF_FAILED(env, status, Function());
  }

  return Function(env, value);
}

template <typename Callable>
inline Function Function::New(napi_env env,
                              Callable cb,
                              const std::string& utf8name,
                              void* data) {
  return New(env, cb, utf8name.c_str(), data);
}

inline Function::Function() : Object() {
}

inline Function::Function(napi_env env, napi_value value) : Object(env, value) {
}

inline Value Function::operator ()(const std::initializer_list<napi_value>& args) const {
  return Call(Env().Undefined(), args);
}

inline Value Function::Call(const std::initializer_list<napi_value>& args) const {
  return Call(Env().Undefined(), args);
}

inline Value Function::Call(const std::vector<napi_value>& args) const {
  return Call(Env().Undefined(), args);
}

inline Value Function::Call(size_t argc, const napi_value* args) const {
  return Call(Env().Undefined(), argc, args);
}

inline Value Function::Call(napi_value recv, const std::initializer_list<napi_value>& args) const {
  return Call(recv, args.size(), args.begin());
}

inline Value Function::Call(napi_value recv, const std::vector<napi_value>& args) const {
  return Call(recv, args.size(), args.data());
}

inline Value Function::Call(napi_value recv, size_t argc, const napi_value* args) const {
  napi_value result;
  napi_status status = napi_call_function(
    _env, recv, _value, argc, args, &result);
  NAPI_THROW_IF_FAILED(_env, status, Value());
  return Value(_env, result);
}

inline Value Function::MakeCallback(
    napi_value recv,
    const std::initializer_list<napi_value>& args,
    napi_async_context context) const {
  return MakeCallback(recv, args.size(), args.begin(), context);
}

inline Value Function::MakeCallback(
    napi_value recv,
    const std::vector<napi_value>& args,
    napi_async_context context) const {
  return MakeCallback(recv, args.size(), args.data(), context);
}

inline Value Function::MakeCallback(
    napi_value recv,
    size_t argc,
    const napi_value* args,
    napi_async_context context) const {
  napi_value result;
  napi_status status = napi_make_callback(
    _env, context, recv, _value, argc, args, &result);
  NAPI_THROW_IF_FAILED(_env, status, Value());
  return Value(_env, result);
}

inline Object Function::New(const std::initializer_list<napi_value>& args) const {
  return New(args.size(), args.begin());
}

inline Object Function::New(const std::vector<napi_value>& args) const {
  return New(args.size(), args.data());
}

inline Object Function::New(size_t argc, const napi_value* args) const {
  napi_value result;
  napi_status status = napi_new_instance(
    _env, _value, argc, args, &result);
  NAPI_THROW_IF_FAILED(_env, status, Object());
  return Object(_env, result);
}

////////////////////////////////////////////////////////////////////////////////
// Promise class
////////////////////////////////////////////////////////////////////////////////

inline Promise::Deferred Promise::Deferred::New(napi_env env) {
  return Promise::Deferred(env);
}

inline Promise::Deferred::Deferred(napi_env env) : _env(env) {
  napi_status status = napi_create_promise(_env, &_deferred, &_promise);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

inline Promise Promise::Deferred::Promise() const {
  return Napi::Promise(_env, _promise);
}

inline Napi::Env Promise::Deferred::Env() const {
  return Napi::Env(_env);
}

inline void Promise::Deferred::Resolve(napi_value value) const {
  napi_status status = napi_resolve_deferred(_env, _deferred, value);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

inline void Promise::Deferred::Reject(napi_value value) const {
  napi_status status = napi_reject_deferred(_env, _deferred, value);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

inline Promise::Promise(napi_env env, napi_value value) : Object(env, value) {
}

////////////////////////////////////////////////////////////////////////////////
// Buffer<T> class
////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline Buffer<T> Buffer<T>::New(napi_env env, size_t length) {
  napi_value value;
  void* data;
  napi_status status = napi_create_buffer(env, length * sizeof (T), &data, &value);
  NAPI_THROW_IF_FAILED(env, status, Buffer<T>());
  return Buffer(env, value, length, static_cast<T*>(data));
}

template <typename T>
inline Buffer<T> Buffer<T>::New(napi_env env, T* data, size_t length) {
  napi_value value;
  napi_status status = napi_create_external_buffer(
    env, length * sizeof (T), data, nullptr, nullptr, &value);
  NAPI_THROW_IF_FAILED(env, status, Buffer<T>());
  return Buffer(env, value, length, data);
}

template <typename T>
template <typename Finalizer>
inline Buffer<T> Buffer<T>::New(napi_env env,
                                T* data,
                                size_t length,
                                Finalizer finalizeCallback) {
  napi_value value;
  details::FinalizeData<T, Finalizer>* finalizeData =
      new details::FinalizeData<T, Finalizer>(
          {std::move(finalizeCallback), nullptr});
  napi_status status = napi_create_external_buffer(
    env,
    length * sizeof (T),
    data,
    details::FinalizeData<T, Finalizer>::Wrapper,
    finalizeData,
    &value);
  if (status != napi_ok) {
    delete finalizeData;
    NAPI_THROW_IF_FAILED(env, status, Buffer());
  }
  return Buffer(env, value, length, data);
}

template <typename T>
template <typename Finalizer, typename Hint>
inline Buffer<T> Buffer<T>::New(napi_env env,
                                T* data,
                                size_t length,
                                Finalizer finalizeCallback,
                                Hint* finalizeHint) {
  napi_value value;
  details::FinalizeData<T, Finalizer, Hint>* finalizeData =
      new details::FinalizeData<T, Finalizer, Hint>(
          {std::move(finalizeCallback), finalizeHint});
  napi_status status = napi_create_external_buffer(
    env,
    length * sizeof (T),
    data,
    details::FinalizeData<T, Finalizer, Hint>::WrapperWithHint,
    finalizeData,
    &value);
  if (status != napi_ok) {
    delete finalizeData;
    NAPI_THROW_IF_FAILED(env, status, Buffer());
  }
  return Buffer(env, value, length, data);
}

template <typename T>
inline Buffer<T> Buffer<T>::Copy(napi_env env, const T* data, size_t length) {
  napi_value value;
  napi_status status = napi_create_buffer_copy(
    env, length * sizeof (T), data, nullptr, &value);
  NAPI_THROW_IF_FAILED(env, status, Buffer<T>());
  return Buffer<T>(env, value);
}

template <typename T>
inline Buffer<T>::Buffer() : Uint8Array(), _length(0), _data(nullptr) {
}

template <typename T>
inline Buffer<T>::Buffer(napi_env env, napi_value value)
  : Uint8Array(env, value), _length(0), _data(nullptr) {
}

template <typename T>
inline Buffer<T>::Buffer(napi_env env, napi_value value, size_t length, T* data)
  : Uint8Array(env, value), _length(length), _data(data) {
}

template <typename T>
inline size_t Buffer<T>::Length() const {
  EnsureInfo();
  return _length;
}

template <typename T>
inline T* Buffer<T>::Data() const {
  EnsureInfo();
  return _data;
}

template <typename T>
inline void Buffer<T>::EnsureInfo() const {
  // The Buffer instance may have been constructed from a napi_value whose
  // length/data are not yet known. Fetch and cache these values just once,
  // since they can never change during the lifetime of the Buffer.
  if (_data == nullptr) {
    size_t byteLength;
    void* voidData;
    napi_status status = napi_get_buffer_info(_env, _value, &voidData, &byteLength);
    NAPI_THROW_IF_FAILED_VOID(_env, status);
    _length = byteLength / sizeof (T);
    _data = static_cast<T*>(voidData);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Error class
////////////////////////////////////////////////////////////////////////////////

inline Error Error::New(napi_env env) {
  napi_status status;
  napi_value error = nullptr;
  bool is_exception_pending;
  const napi_extended_error_info* info;

  // We must retrieve the last error info before doing anything else, because
  // doing anything else will replace the last error info.
  status = napi_get_last_error_info(env, &info);
  NAPI_FATAL_IF_FAILED(status, "Error::New", "napi_get_last_error_info");

  status = napi_is_exception_pending(env, &is_exception_pending);
  NAPI_FATAL_IF_FAILED(status, "Error::New", "napi_is_exception_pending");

  // A pending exception takes precedence over any internal error status.
  if (is_exception_pending) {
    status = napi_get_and_clear_last_exception(env, &error);
    NAPI_FATAL_IF_FAILED(status, "Error::New", "napi_get_and_clear_last_exception");
  }
  else {
    const char* error_message = info->error_message != nullptr ?
      info->error_message : "Error in native callback";

    napi_value message;
    status = napi_create_string_utf8(
      env,
      error_message,
      std::strlen(error_message),
      &message);
    NAPI_FATAL_IF_FAILED(status, "Error::New", "napi_create_string_utf8");

    switch (info->error_code) {
    case napi_object_expected:
    case napi_string_expected:
    case napi_boolean_expected:
    case napi_number_expected:
      status = napi_create_type_error(env, nullptr, message, &error);
      break;
    default:
      status = napi_create_error(env, nullptr,  message, &error);
      break;
    }
    NAPI_FATAL_IF_FAILED(status, "Error::New", "napi_create_error");
  }

  return Error(env, error);
}

inline Error Error::New(napi_env env, const char* message) {
  return Error::New<Error>(env, message, std::strlen(message), napi_create_error);
}

inline Error Error::New(napi_env env, const std::string& message) {
  return Error::New<Error>(env, message.c_str(), message.size(), napi_create_error);
}

inline NAPI_NO_RETURN void Error::Fatal(const char* location, const char* message) {
  napi_fatal_error(location, NAPI_AUTO_LENGTH, message, NAPI_AUTO_LENGTH);
}

inline Error::Error() : ObjectReference() {
}

inline Error::Error(napi_env env, napi_value value) : ObjectReference(env, nullptr) {
  if (value != nullptr) {
    napi_status status = napi_create_reference(env, value, 1, &_ref);

    // Avoid infinite recursion in the failure case.
    // Don't try to construct & throw another Error instance.
    NAPI_FATAL_IF_FAILED(status, "Error::Error", "napi_create_reference");
  }
}

inline Error::Error(Error&& other) : ObjectReference(std::move(other)) {
}

inline Error& Error::operator =(Error&& other) {
  static_cast<Reference<Object>*>(this)->operator=(std::move(other));
  return *this;
}

inline Error::Error(const Error& other) : ObjectReference(other) {
}

inline Error& Error::operator =(const Error& other) {
  Reset();

  _env = other.Env();
  HandleScope scope(_env);

  napi_value value = other.Value();
  if (value != nullptr) {
    napi_status status = napi_create_reference(_env, value, 1, &_ref);
    NAPI_THROW_IF_FAILED(_env, status, *this);
  }

  return *this;
}

inline const std::string& Error::Message() const NAPI_NOEXCEPT {
  if (_message.size() == 0 && _env != nullptr) {
#ifdef NAPI_CPP_EXCEPTIONS
    try {
      _message = Get("message").As<String>();
    }
    catch (...) {
      // Catch all errors here, to include e.g. a std::bad_alloc from
      // the std::string::operator=, because this method may not throw.
    }
#else // NAPI_CPP_EXCEPTIONS
    _message = Get("message").As<String>();
#endif // NAPI_CPP_EXCEPTIONS
  }
  return _message;
}

inline void Error::ThrowAsJavaScriptException() const {
  HandleScope scope(_env);
  if (!IsEmpty()) {

    // We intentionally don't use `NAPI_THROW_*` macros here to ensure
    // that there is no possible recursion as `ThrowAsJavaScriptException`
    // is part of `NAPI_THROW_*` macro definition for noexcept.

    napi_status status = napi_throw(_env, Value());

#ifdef NAPI_CPP_EXCEPTIONS
    if (status != napi_ok) {
      throw Error::New(_env);
    }
#else // NAPI_CPP_EXCEPTIONS
    NAPI_FATAL_IF_FAILED(status, "Error::ThrowAsJavaScriptException", "napi_throw");
#endif // NAPI_CPP_EXCEPTIONS
  }
}

#ifdef NAPI_CPP_EXCEPTIONS

inline const char* Error::what() const NAPI_NOEXCEPT {
  return Message().c_str();
}

#endif // NAPI_CPP_EXCEPTIONS

template <typename TError>
inline TError Error::New(napi_env env,
                         const char* message,
                         size_t length,
                         create_error_fn create_error) {
  napi_value str;
  napi_status status = napi_create_string_utf8(env, message, length, &str);
  NAPI_THROW_IF_FAILED(env, status, TError());

  napi_value error;
  status = create_error(env, nullptr, str, &error);
  NAPI_THROW_IF_FAILED(env, status, TError());

  return TError(env, error);
}

inline TypeError TypeError::New(napi_env env, const char* message) {
  return Error::New<TypeError>(env, message, std::strlen(message), napi_create_type_error);
}

inline TypeError TypeError::New(napi_env env, const std::string& message) {
  return Error::New<TypeError>(env, message.c_str(), message.size(), napi_create_type_error);
}

inline TypeError::TypeError() : Error() {
}

inline TypeError::TypeError(napi_env env, napi_value value) : Error(env, value) {
}

inline RangeError RangeError::New(napi_env env, const char* message) {
  return Error::New<RangeError>(env, message, std::strlen(message), napi_create_range_error);
}

inline RangeError RangeError::New(napi_env env, const std::string& message) {
  return Error::New<RangeError>(env, message.c_str(), message.size(), napi_create_range_error);
}

inline RangeError::RangeError() : Error() {
}

inline RangeError::RangeError(napi_env env, napi_value value) : Error(env, value) {
}

////////////////////////////////////////////////////////////////////////////////
// Reference<T> class
////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline Reference<T> Reference<T>::New(const T& value, uint32_t initialRefcount) {
  napi_env env = value.Env();
  napi_value val = value;

  if (val == nullptr) {
    return Reference<T>(env, nullptr);
  }

  napi_ref ref;
  napi_status status = napi_create_reference(env, value, initialRefcount, &ref);
  NAPI_THROW_IF_FAILED(env, status, Reference<T>());

  return Reference<T>(env, ref);
}


template <typename T>
inline Reference<T>::Reference() : _env(nullptr), _ref(nullptr), _suppressDestruct(false) {
}

template <typename T>
inline Reference<T>::Reference(napi_env env, napi_ref ref)
  : _env(env), _ref(ref), _suppressDestruct(false) {
}

template <typename T>
inline Reference<T>::~Reference() {
  if (_ref != nullptr) {
    if (!_suppressDestruct) {
      napi_delete_reference(_env, _ref);
    }

    _ref = nullptr;
  }
}

template <typename T>
inline Reference<T>::Reference(Reference<T>&& other)
  : _env(other._env), _ref(other._ref), _suppressDestruct(other._suppressDestruct) {
  other._env = nullptr;
  other._ref = nullptr;
  other._suppressDestruct = false;
}

template <typename T>
inline Reference<T>& Reference<T>::operator =(Reference<T>&& other) {
  Reset();
  _env = other._env;
  _ref = other._ref;
  _suppressDestruct = other._suppressDestruct;
  other._env = nullptr;
  other._ref = nullptr;
  other._suppressDestruct = false;
  return *this;
}

template <typename T>
inline Reference<T>::Reference(const Reference<T>& other)
  : _env(other._env), _ref(nullptr), _suppressDestruct(false) {
  HandleScope scope(_env);

  napi_value value = other.Value();
  if (value != nullptr) {
    // Copying is a limited scenario (currently only used for Error object) and always creates a
    // strong reference to the given value even if the incoming reference is weak.
    napi_status status = napi_create_reference(_env, value, 1, &_ref);
    NAPI_FATAL_IF_FAILED(status, "Reference<T>::Reference", "napi_create_reference");
  }
}

template <typename T>
inline Reference<T>::operator napi_ref() const {
  return _ref;
}

template <typename T>
inline bool Reference<T>::operator ==(const Reference<T> &other) const {
  HandleScope scope(_env);
  return this->Value().StrictEquals(other.Value());
}

template <typename T>
inline bool Reference<T>::operator !=(const Reference<T> &other) const {
  return !this->operator ==(other);
}

template <typename T>
inline Napi::Env Reference<T>::Env() const {
  return Napi::Env(_env);
}

template <typename T>
inline bool Reference<T>::IsEmpty() const {
  return _ref == nullptr;
}

template <typename T>
inline T Reference<T>::Value() const {
  if (_ref == nullptr) {
    return T(_env, nullptr);
  }

  napi_value value;
  napi_status status = napi_get_reference_value(_env, _ref, &value);
  NAPI_THROW_IF_FAILED(_env, status, T());
  return T(_env, value);
}

template <typename T>
inline uint32_t Reference<T>::Ref() {
  uint32_t result;
  napi_status status = napi_reference_ref(_env, _ref, &result);
  NAPI_THROW_IF_FAILED(_env, status, 1);
  return result;
}

template <typename T>
inline uint32_t Reference<T>::Unref() {
  uint32_t result;
  napi_status status = napi_reference_unref(_env, _ref, &result);
  NAPI_THROW_IF_FAILED(_env, status, 1);
  return result;
}

template <typename T>
inline void Reference<T>::Reset() {
  if (_ref != nullptr) {
    napi_status status = napi_delete_reference(_env, _ref);
    NAPI_THROW_IF_FAILED_VOID(_env, status);
    _ref = nullptr;
  }
}

template <typename T>
inline void Reference<T>::Reset(const T& value, uint32_t refcount) {
  Reset();
  _env = value.Env();

  napi_value val = value;
  if (val != nullptr) {
    napi_status status = napi_create_reference(_env, value, refcount, &_ref);
    NAPI_THROW_IF_FAILED_VOID(_env, status);
  }
}

template <typename T>
inline void Reference<T>::SuppressDestruct() {
  _suppressDestruct = true;
}

template <typename T>
inline Reference<T> Weak(T value) {
  return Reference<T>::New(value, 0);
}

inline ObjectReference Weak(Object value) {
  return Reference<Object>::New(value, 0);
}

inline FunctionReference Weak(Function value) {
  return Reference<Function>::New(value, 0);
}

template <typename T>
inline Reference<T> Persistent(T value) {
  return Reference<T>::New(value, 1);
}

inline ObjectReference Persistent(Object value) {
  return Reference<Object>::New(value, 1);
}

inline FunctionReference Persistent(Function value) {
  return Reference<Function>::New(value, 1);
}

////////////////////////////////////////////////////////////////////////////////
// ObjectReference class
////////////////////////////////////////////////////////////////////////////////

inline ObjectReference::ObjectReference(): Reference<Object>() {
}

inline ObjectReference::ObjectReference(napi_env env, napi_ref ref): Reference<Object>(env, ref) {
}

inline ObjectReference::ObjectReference(Reference<Object>&& other)
  : Reference<Object>(std::move(other)) {
}

inline ObjectReference& ObjectReference::operator =(Reference<Object>&& other) {
  static_cast<Reference<Object>*>(this)->operator=(std::move(other));
  return *this;
}

inline ObjectReference::ObjectReference(ObjectReference&& other)
  : Reference<Object>(std::move(other)) {
}

inline ObjectReference& ObjectReference::operator =(ObjectReference&& other) {
  static_cast<Reference<Object>*>(this)->operator=(std::move(other));
  return *this;
}

inline ObjectReference::ObjectReference(const ObjectReference& other)
  : Reference<Object>(other) {
}

inline Napi::Value ObjectReference::Get(const char* utf8name) const {
  EscapableHandleScope scope(_env);
  return scope.Escape(Value().Get(utf8name));
}

inline Napi::Value ObjectReference::Get(const std::string& utf8name) const {
  EscapableHandleScope scope(_env);
  return scope.Escape(Value().Get(utf8name));
}

inline bool ObjectReference::Set(const char* utf8name, napi_value value) {
  HandleScope scope(_env);
  return Value().Set(utf8name, value);
}

inline bool ObjectReference::Set(const char* utf8name, Napi::Value value) {
  HandleScope scope(_env);
  return Value().Set(utf8name, value);
}

inline bool ObjectReference::Set(const char* utf8name, const char* utf8value) {
  HandleScope scope(_env);
  return Value().Set(utf8name, utf8value);
}

inline bool ObjectReference::Set(const char* utf8name, bool boolValue) {
  HandleScope scope(_env);
  return Value().Set(utf8name, boolValue);
}

inline bool ObjectReference::Set(const char* utf8name, double numberValue) {
  HandleScope scope(_env);
  return Value().Set(utf8name, numberValue);
}

inline bool ObjectReference::Set(const std::string& utf8name,
                                 napi_value value) {
  HandleScope scope(_env);
  return Value().Set(utf8name, value);
}

inline bool ObjectReference::Set(const std::string& utf8name,
                                 Napi::Value value) {
  HandleScope scope(_env);
  return Value().Set(utf8name, value);
}

inline bool ObjectReference::Set(const std::string& utf8name,
                                 std::string& utf8value) {
  HandleScope scope(_env);
  return Value().Set(utf8name, utf8value);
}

inline bool ObjectReference::Set(const std::string& utf8name, bool boolValue) {
  HandleScope scope(_env);
  return Value().Set(utf8name, boolValue);
}

inline bool ObjectReference::Set(const std::string& utf8name,
                                 double numberValue) {
  HandleScope scope(_env);
  return Value().Set(utf8name, numberValue);
}

inline Napi::Value ObjectReference::Get(uint32_t index) const {
  EscapableHandleScope scope(_env);
  return scope.Escape(Value().Get(index));
}

inline bool ObjectReference::Set(uint32_t index, napi_value value) {
  HandleScope scope(_env);
  return Value().Set(index, value);
}

inline bool ObjectReference::Set(uint32_t index, Napi::Value value) {
  HandleScope scope(_env);
  return Value().Set(index, value);
}

inline bool ObjectReference::Set(uint32_t index, const char* utf8value) {
  HandleScope scope(_env);
  return Value().Set(index, utf8value);
}

inline bool ObjectReference::Set(uint32_t index, const std::string& utf8value) {
  HandleScope scope(_env);
  return Value().Set(index, utf8value);
}

inline bool ObjectReference::Set(uint32_t index, bool boolValue) {
  HandleScope scope(_env);
  return Value().Set(index, boolValue);
}

inline bool ObjectReference::Set(uint32_t index, double numberValue) {
  HandleScope scope(_env);
  return Value().Set(index, numberValue);
}

////////////////////////////////////////////////////////////////////////////////
// FunctionReference class
////////////////////////////////////////////////////////////////////////////////

inline FunctionReference::FunctionReference(): Reference<Function>() {
}

inline FunctionReference::FunctionReference(napi_env env, napi_ref ref)
  : Reference<Function>(env, ref) {
}

inline FunctionReference::FunctionReference(Reference<Function>&& other)
  : Reference<Function>(std::move(other)) {
}

inline FunctionReference& FunctionReference::operator =(Reference<Function>&& other) {
  static_cast<Reference<Function>*>(this)->operator=(std::move(other));
  return *this;
}

inline FunctionReference::FunctionReference(FunctionReference&& other)
  : Reference<Function>(std::move(other)) {
}

inline FunctionReference& FunctionReference::operator =(FunctionReference&& other) {
  static_cast<Reference<Function>*>(this)->operator=(std::move(other));
  return *this;
}

inline Napi::Value FunctionReference::operator ()(
    const std::initializer_list<napi_value>& args) const {
  EscapableHandleScope scope(_env);
  return scope.Escape(Value()(args));
}

inline Napi::Value FunctionReference::Call(const std::initializer_list<napi_value>& args) const {
  EscapableHandleScope scope(_env);
  Napi::Value result = Value().Call(args);
  if (scope.Env().IsExceptionPending()) {
    return Value();
  }
  return scope.Escape(result);
}

inline Napi::Value FunctionReference::Call(const std::vector<napi_value>& args) const {
  EscapableHandleScope scope(_env);
  Napi::Value result = Value().Call(args);
  if (scope.Env().IsExceptionPending()) {
    return Value();
  }
  return scope.Escape(result);
}

inline Napi::Value FunctionReference::Call(
    napi_value recv, const std::initializer_list<napi_value>& args) const {
  EscapableHandleScope scope(_env);
  Napi::Value result = Value().Call(recv, args);
  if (scope.Env().IsExceptionPending()) {
    return Value();
  }
  return scope.Escape(result);
}

inline Napi::Value FunctionReference::Call(
    napi_value recv, const std::vector<napi_value>& args) const {
  EscapableHandleScope scope(_env);
  Napi::Value result = Value().Call(recv, args);
  if (scope.Env().IsExceptionPending()) {
    return Value();
  }
  return scope.Escape(result);
}

inline Napi::Value FunctionReference::Call(
    napi_value recv, size_t argc, const napi_value* args) const {
  EscapableHandleScope scope(_env);
  Napi::Value result = Value().Call(recv, argc, args);
  if (scope.Env().IsExceptionPending()) {
    return Value();
  }
  return scope.Escape(result);
}

inline Napi::Value FunctionReference::MakeCallback(
    napi_value recv,
    const std::initializer_list<napi_value>& args,
    napi_async_context context) const {
  EscapableHandleScope scope(_env);
  Napi::Value result = Value().MakeCallback(recv, args, context);
  if (scope.Env().IsExceptionPending()) {
    return Value();
  }
  return scope.Escape(result);
}

inline Napi::Value FunctionReference::MakeCallback(
    napi_value recv,
    const std::vector<napi_value>& args,
    napi_async_context context) const {
  EscapableHandleScope scope(_env);
  Napi::Value result = Value().MakeCallback(recv, args, context);
  if (scope.Env().IsExceptionPending()) {
    return Value();
  }
  return scope.Escape(result);
}

inline Napi::Value FunctionReference::MakeCallback(
    napi_value recv,
    size_t argc,
    const napi_value* args,
    napi_async_context context) const {
  EscapableHandleScope scope(_env);
  Napi::Value result = Value().MakeCallback(recv, argc, args, context);
  if (scope.Env().IsExceptionPending()) {
    return Value();
  }
  return scope.Escape(result);
}

inline Object FunctionReference::New(const std::initializer_list<napi_value>& args) const {
  EscapableHandleScope scope(_env);
  return scope.Escape(Value().New(args)).As<Object>();
}

inline Object FunctionReference::New(const std::vector<napi_value>& args) const {
  EscapableHandleScope scope(_env);
  return scope.Escape(Value().New(args)).As<Object>();
}

////////////////////////////////////////////////////////////////////////////////
// CallbackInfo class
////////////////////////////////////////////////////////////////////////////////

inline CallbackInfo::CallbackInfo(napi_env env, napi_callback_info info)
    : _env(env), _info(info), _this(nullptr), _dynamicArgs(nullptr), _data(nullptr) {
  _argc = _staticArgCount;
  _argv = _staticArgs;
  napi_status status = napi_get_cb_info(env, info, &_argc, _argv, &_this, &_data);
  NAPI_THROW_IF_FAILED_VOID(_env, status);

  if (_argc > _staticArgCount) {
    // Use either a fixed-size array (on the stack) or a dynamically-allocated
    // array (on the heap) depending on the number of args.
    _dynamicArgs = new napi_value[_argc];
    _argv = _dynamicArgs;

    status = napi_get_cb_info(env, info, &_argc, _argv, nullptr, nullptr);
    NAPI_THROW_IF_FAILED_VOID(_env, status);
  }
}

inline CallbackInfo::~CallbackInfo() {
  if (_dynamicArgs != nullptr) {
    delete[] _dynamicArgs;
  }
}

inline Value CallbackInfo::NewTarget() const {
  napi_value newTarget;
  napi_status status = napi_get_new_target(_env, _info, &newTarget);
  NAPI_THROW_IF_FAILED(_env, status, Value());
  return Value(_env, newTarget);
}

inline bool CallbackInfo::IsConstructCall() const {
  return !NewTarget().IsEmpty();
}

inline Napi::Env CallbackInfo::Env() const {
  return Napi::Env(_env);
}

inline size_t CallbackInfo::Length() const {
  return _argc;
}

inline const Value CallbackInfo::operator [](size_t index) const {
  return index < _argc ? Value(_env, _argv[index]) : Env().Undefined();
}

inline Value CallbackInfo::This() const {
  if (_this == nullptr) {
    return Env().Undefined();
  }
  return Object(_env, _this);
}

inline void* CallbackInfo::Data() const {
  return _data;
}

inline void CallbackInfo::SetData(void* data) {
  _data = data;
}

////////////////////////////////////////////////////////////////////////////////
// PropertyDescriptor class
////////////////////////////////////////////////////////////////////////////////

template <typename PropertyDescriptor::GetterCallback Getter>
PropertyDescriptor
PropertyDescriptor::Accessor(const char* utf8name,
                             napi_property_attributes attributes,
                             void* data) {
  napi_property_descriptor desc = napi_property_descriptor();

  desc.utf8name = utf8name;
  desc.getter = details::TemplatedCallback<Getter>;
  desc.attributes = attributes;
  desc.data = data;

  return desc;
}

template <typename PropertyDescriptor::GetterCallback Getter>
PropertyDescriptor
PropertyDescriptor::Accessor(const std::string& utf8name,
                             napi_property_attributes attributes,
                             void* data) {
  return Accessor<Getter>(utf8name.c_str(), attributes, data);
}

template <typename PropertyDescriptor::GetterCallback Getter>
PropertyDescriptor
PropertyDescriptor::Accessor(Name name,
                             napi_property_attributes attributes,
                             void* data) {
  napi_property_descriptor desc = napi_property_descriptor();

  desc.name = name;
  desc.getter = details::TemplatedCallback<Getter>;
  desc.attributes = attributes;
  desc.data = data;

  return desc;
}

template <
typename PropertyDescriptor::GetterCallback Getter,
typename PropertyDescriptor::SetterCallback Setter>
PropertyDescriptor
PropertyDescriptor::Accessor(const char* utf8name,
                             napi_property_attributes attributes,
                             void* data) {

  napi_property_descriptor desc = napi_property_descriptor();

  desc.utf8name = utf8name;
  desc.getter = details::TemplatedCallback<Getter>;
  desc.setter = details::TemplatedVoidCallback<Setter>;
  desc.attributes = attributes;
  desc.data = data;

  return desc;
}

template <
typename PropertyDescriptor::GetterCallback Getter,
typename PropertyDescriptor::SetterCallback Setter>
PropertyDescriptor
PropertyDescriptor::Accessor(const std::string& utf8name,
                             napi_property_attributes attributes,
                             void* data) {
  return Accessor<Getter, Setter>(utf8name.c_str(), attributes, data);
}

template <
typename PropertyDescriptor::GetterCallback Getter,
typename PropertyDescriptor::SetterCallback Setter>
PropertyDescriptor
PropertyDescriptor::Accessor(Name name,
                             napi_property_attributes attributes,
                             void* data) {
  napi_property_descriptor desc = napi_property_descriptor();

  desc.name = name;
  desc.getter = details::TemplatedCallback<Getter>;
  desc.setter = details::TemplatedVoidCallback<Setter>;
  desc.attributes = attributes;
  desc.data = data;

  return desc;
}

template <typename Getter>
inline PropertyDescriptor
PropertyDescriptor::Accessor(Napi::Env env,
                             Napi::Object object,
                             const char* utf8name,
                             Getter getter,
                             napi_property_attributes attributes,
                             void* data) {
  using CbData = details::CallbackData<Getter, Napi::Value>;
  auto callbackData = new CbData({ getter, data });

  napi_status status = AttachData(env, object, callbackData);
  if (status != napi_ok) {
    delete callbackData;
    NAPI_THROW_IF_FAILED(env, status, napi_property_descriptor());
  }

  return PropertyDescriptor({
    utf8name,
    nullptr,
    nullptr,
    CbData::Wrapper,
    nullptr,
    nullptr,
    attributes,
    callbackData
  });
}

template <typename Getter>
inline PropertyDescriptor PropertyDescriptor::Accessor(Napi::Env env,
                                                       Napi::Object object,
                                                       const std::string& utf8name,
                                                       Getter getter,
                                                       napi_property_attributes attributes,
                                                       void* data) {
  return Accessor(env, object, utf8name.c_str(), getter, attributes, data);
}

template <typename Getter>
inline PropertyDescriptor PropertyDescriptor::Accessor(Napi::Env env,
                                                       Napi::Object object,
                                                       Name name,
                                                       Getter getter,
                                                       napi_property_attributes attributes,
                                                       void* data) {
  using CbData = details::CallbackData<Getter, Napi::Value>;
  auto callbackData = new CbData({ getter, data });

  napi_status status = AttachData(env, object, callbackData);
  if (status != napi_ok) {
    delete callbackData;
    NAPI_THROW_IF_FAILED(env, status, napi_property_descriptor());
  }

  return PropertyDescriptor({
    nullptr,
    name,
    nullptr,
    CbData::Wrapper,
    nullptr,
    nullptr,
    attributes,
    callbackData
  });
}

template <typename Getter, typename Setter>
inline PropertyDescriptor PropertyDescriptor::Accessor(Napi::Env env,
                                                       Napi::Object object,
                                                       const char* utf8name,
                                                       Getter getter,
                                                       Setter setter,
                                                       napi_property_attributes attributes,
                                                       void* data) {
  using CbData = details::AccessorCallbackData<Getter, Setter>;
  auto callbackData = new CbData({ getter, setter, data });

  napi_status status = AttachData(env, object, callbackData);
  if (status != napi_ok) {
    delete callbackData;
    NAPI_THROW_IF_FAILED(env, status, napi_property_descriptor());
  }

  return PropertyDescriptor({
    utf8name,
    nullptr,
    nullptr,
    CbData::GetterWrapper,
    CbData::SetterWrapper,
    nullptr,
    attributes,
    callbackData
  });
}

template <typename Getter, typename Setter>
inline PropertyDescriptor PropertyDescriptor::Accessor(Napi::Env env,
                                                       Napi::Object object,
                                                       const std::string& utf8name,
                                                       Getter getter,
                                                       Setter setter,
                                                       napi_property_attributes attributes,
                                                       void* data) {
  return Accessor(env, object, utf8name.c_str(), getter, setter, attributes, data);
}

template <typename Getter, typename Setter>
inline PropertyDescriptor PropertyDescriptor::Accessor(Napi::Env env,
                                                       Napi::Object object,
                                                       Name name,
                                                       Getter getter,
                                                       Setter setter,
                                                       napi_property_attributes attributes,
                                                       void* data) {
  using CbData = details::AccessorCallbackData<Getter, Setter>;
  auto callbackData = new CbData({ getter, setter, data });

  napi_status status = AttachData(env, object, callbackData);
  if (status != napi_ok) {
    delete callbackData;
    NAPI_THROW_IF_FAILED(env, status, napi_property_descriptor());
  }

  return PropertyDescriptor({
    nullptr,
    name,
    nullptr,
    CbData::GetterWrapper,
    CbData::SetterWrapper,
    nullptr,
    attributes,
    callbackData
  });
}

template <typename Callable>
inline PropertyDescriptor PropertyDescriptor::Function(Napi::Env env,
                                                       Napi::Object /*object*/,
                                                       const char* utf8name,
                                                       Callable cb,
                                                       napi_property_attributes attributes,
                                                       void* data) {
  return PropertyDescriptor({
    utf8name,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    Napi::Function::New(env, cb, utf8name, data),
    attributes,
    nullptr
  });
}

template <typename Callable>
inline PropertyDescriptor PropertyDescriptor::Function(Napi::Env env,
                                                       Napi::Object object,
                                                       const std::string& utf8name,
                                                       Callable cb,
                                                       napi_property_attributes attributes,
                                                       void* data) {
  return Function(env, object, utf8name.c_str(), cb, attributes, data);
}

template <typename Callable>
inline PropertyDescriptor PropertyDescriptor::Function(Napi::Env env,
                                                       Napi::Object /*object*/,
                                                       Name name,
                                                       Callable cb,
                                                       napi_property_attributes attributes,
                                                       void* data) {
  return PropertyDescriptor({
    nullptr,
    name,
    nullptr,
    nullptr,
    nullptr,
    Napi::Function::New(env, cb, nullptr, data),
    attributes,
    nullptr
  });
}

inline PropertyDescriptor PropertyDescriptor::Value(const char* utf8name,
                                                    napi_value value,
                                                    napi_property_attributes attributes) {
  return PropertyDescriptor({
    utf8name, nullptr, nullptr, nullptr, nullptr, value, attributes, nullptr
  });
}

inline PropertyDescriptor PropertyDescriptor::Value(const std::string& utf8name,
                                                    napi_value value,
                                                    napi_property_attributes attributes) {
  return Value(utf8name.c_str(), value, attributes);
}

inline PropertyDescriptor PropertyDescriptor::Value(napi_value name,
                                                    napi_value value,
                                                    napi_property_attributes attributes) {
  return PropertyDescriptor({
    nullptr, name, nullptr, nullptr, nullptr, value, attributes, nullptr
  });
}

inline PropertyDescriptor PropertyDescriptor::Value(Name name,
                                                    Napi::Value value,
                                                    napi_property_attributes attributes) {
  napi_value nameValue = name;
  napi_value valueValue = value;
  return PropertyDescriptor::Value(nameValue, valueValue, attributes);
}

inline PropertyDescriptor::PropertyDescriptor(napi_property_descriptor desc)
  : _desc(desc) {
}

inline PropertyDescriptor::operator napi_property_descriptor&() {
  return _desc;
}

inline PropertyDescriptor::operator const napi_property_descriptor&() const {
  return _desc;
}

////////////////////////////////////////////////////////////////////////////////
// InstanceWrap<T> class
////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline void InstanceWrap<T>::AttachPropData(napi_env env,
                                       napi_value value,
                                       const napi_property_descriptor* prop) {
  napi_status status;
  if (!(prop->attributes & napi_static)) {
    if (prop->method == T::InstanceVoidMethodCallbackWrapper) {
      status = Napi::details::AttachData(env,
                    value,
                    static_cast<InstanceVoidMethodCallbackData*>(prop->data));
      NAPI_THROW_IF_FAILED_VOID(env, status);
    } else if (prop->method == T::InstanceMethodCallbackWrapper) {
      status = Napi::details::AttachData(env,
                        value,
                        static_cast<InstanceMethodCallbackData*>(prop->data));
      NAPI_THROW_IF_FAILED_VOID(env, status);
    } else if (prop->getter == T::InstanceGetterCallbackWrapper ||
        prop->setter == T::InstanceSetterCallbackWrapper) {
      status = Napi::details::AttachData(env,
                          value,
                          static_cast<InstanceAccessorCallbackData*>(prop->data));
      NAPI_THROW_IF_FAILED_VOID(env, status);
    }
  }
}

template <typename T>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceMethod(
    const char* utf8name,
    InstanceVoidMethodCallback method,
    napi_property_attributes attributes,
    void* data) {
  InstanceVoidMethodCallbackData* callbackData =
    new InstanceVoidMethodCallbackData({ method, data});

  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.method = T::InstanceVoidMethodCallbackWrapper;
  desc.data = callbackData;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceMethod(
    const char* utf8name,
    InstanceMethodCallback method,
    napi_property_attributes attributes,
    void* data) {
  InstanceMethodCallbackData* callbackData = new InstanceMethodCallbackData({ method, data });

  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.method = T::InstanceMethodCallbackWrapper;
  desc.data = callbackData;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceMethod(
    Symbol name,
    InstanceVoidMethodCallback method,
    napi_property_attributes attributes,
    void* data) {
  InstanceVoidMethodCallbackData* callbackData =
    new InstanceVoidMethodCallbackData({ method, data});

  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.method = T::InstanceVoidMethodCallbackWrapper;
  desc.data = callbackData;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceMethod(
    Symbol name,
    InstanceMethodCallback method,
    napi_property_attributes attributes,
    void* data) {
  InstanceMethodCallbackData* callbackData = new InstanceMethodCallbackData({ method, data });

  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.method = T::InstanceMethodCallbackWrapper;
  desc.data = callbackData;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
template <typename InstanceWrap<T>::InstanceVoidMethodCallback method>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceMethod(
    const char* utf8name,
    napi_property_attributes attributes,
    void* data) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.method = details::TemplatedInstanceVoidCallback<T, method>;
  desc.data = data;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
template <typename InstanceWrap<T>::InstanceMethodCallback method>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceMethod(
    const char* utf8name,
    napi_property_attributes attributes,
    void* data) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.method = details::TemplatedInstanceCallback<T, method>;
  desc.data = data;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
template <typename InstanceWrap<T>::InstanceVoidMethodCallback method>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceMethod(
    Symbol name,
    napi_property_attributes attributes,
    void* data) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.method = details::TemplatedInstanceVoidCallback<T, method>;
  desc.data = data;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
template <typename InstanceWrap<T>::InstanceMethodCallback method>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceMethod(
    Symbol name,
    napi_property_attributes attributes,
    void* data) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.method = details::TemplatedInstanceCallback<T, method>;
  desc.data = data;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceAccessor(
    const char* utf8name,
    InstanceGetterCallback getter,
    InstanceSetterCallback setter,
    napi_property_attributes attributes,
    void* data) {
  InstanceAccessorCallbackData* callbackData =
    new InstanceAccessorCallbackData({ getter, setter, data });

  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.getter = getter != nullptr ? T::InstanceGetterCallbackWrapper : nullptr;
  desc.setter = setter != nullptr ? T::InstanceSetterCallbackWrapper : nullptr;
  desc.data = callbackData;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceAccessor(
    Symbol name,
    InstanceGetterCallback getter,
    InstanceSetterCallback setter,
    napi_property_attributes attributes,
    void* data) {
  InstanceAccessorCallbackData* callbackData =
    new InstanceAccessorCallbackData({ getter, setter, data });

  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.getter = getter != nullptr ? T::InstanceGetterCallbackWrapper : nullptr;
  desc.setter = setter != nullptr ? T::InstanceSetterCallbackWrapper : nullptr;
  desc.data = callbackData;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
template <typename InstanceWrap<T>::InstanceGetterCallback getter,
          typename InstanceWrap<T>::InstanceSetterCallback setter>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceAccessor(
    const char* utf8name,
    napi_property_attributes attributes,
    void* data) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.getter = details::TemplatedInstanceCallback<T, getter>;
  desc.setter = This::WrapSetter(This::SetterTag<setter>());
  desc.data = data;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
template <typename InstanceWrap<T>::InstanceGetterCallback getter,
          typename InstanceWrap<T>::InstanceSetterCallback setter>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceAccessor(
    Symbol name,
    napi_property_attributes attributes,
    void* data) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.getter = details::TemplatedInstanceCallback<T, getter>;
  desc.setter = This::WrapSetter(This::SetterTag<setter>());
  desc.data = data;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceValue(
    const char* utf8name,
    Napi::Value value,
    napi_property_attributes attributes) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.value = value;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> InstanceWrap<T>::InstanceValue(
    Symbol name,
    Napi::Value value,
    napi_property_attributes attributes) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.value = value;
  desc.attributes = attributes;
  return desc;
}

template <typename T>
inline napi_value InstanceWrap<T>::InstanceVoidMethodCallbackWrapper(
    napi_env env,
    napi_callback_info info) {
  return details::WrapCallback([&] {
    CallbackInfo callbackInfo(env, info);
    InstanceVoidMethodCallbackData* callbackData =
      reinterpret_cast<InstanceVoidMethodCallbackData*>(callbackInfo.Data());
    callbackInfo.SetData(callbackData->data);
    T* instance = T::Unwrap(callbackInfo.This().As<Object>());
    auto cb = callbackData->callback;
    (instance->*cb)(callbackInfo);
    return nullptr;
  });
}

template <typename T>
inline napi_value InstanceWrap<T>::InstanceMethodCallbackWrapper(
    napi_env env,
    napi_callback_info info) {
  return details::WrapCallback([&] {
    CallbackInfo callbackInfo(env, info);
    InstanceMethodCallbackData* callbackData =
      reinterpret_cast<InstanceMethodCallbackData*>(callbackInfo.Data());
    callbackInfo.SetData(callbackData->data);
    T* instance = T::Unwrap(callbackInfo.This().As<Object>());
    auto cb = callbackData->callback;
    return (instance->*cb)(callbackInfo);
  });
}

template <typename T>
inline napi_value InstanceWrap<T>::InstanceGetterCallbackWrapper(
    napi_env env,
    napi_callback_info info) {
  return details::WrapCallback([&] {
    CallbackInfo callbackInfo(env, info);
    InstanceAccessorCallbackData* callbackData =
      reinterpret_cast<InstanceAccessorCallbackData*>(callbackInfo.Data());
    callbackInfo.SetData(callbackData->data);
    T* instance = T::Unwrap(callbackInfo.This().As<Object>());
    auto cb = callbackData->getterCallback;
    return (instance->*cb)(callbackInfo);
  });
}

template <typename T>
inline napi_value InstanceWrap<T>::InstanceSetterCallbackWrapper(
    napi_env env,
    napi_callback_info info) {
  return details::WrapCallback([&] {
    CallbackInfo callbackInfo(env, info);
    InstanceAccessorCallbackData* callbackData =
      reinterpret_cast<InstanceAccessorCallbackData*>(callbackInfo.Data());
    callbackInfo.SetData(callbackData->data);
    T* instance = T::Unwrap(callbackInfo.This().As<Object>());
    auto cb = callbackData->setterCallback;
    (instance->*cb)(callbackInfo, callbackInfo[0]);
    return nullptr;
  });
}

template <typename T>
template <typename InstanceWrap<T>::InstanceSetterCallback method>
inline napi_value InstanceWrap<T>::WrappedMethod(
    napi_env env, napi_callback_info info) NAPI_NOEXCEPT {
  return details::WrapCallback([&] {
    const CallbackInfo cbInfo(env, info);
    T* instance = T::Unwrap(cbInfo.This().As<Object>());
    (instance->*method)(cbInfo, cbInfo[0]);
    return nullptr;
  });
}

////////////////////////////////////////////////////////////////////////////////
// ObjectWrap<T> class
////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline ObjectWrap<T>::ObjectWrap(const Napi::CallbackInfo& callbackInfo) {
  napi_env env = callbackInfo.Env();
  napi_value wrapper = callbackInfo.This();
  napi_status status;
  napi_ref ref;
  T* instance = static_cast<T*>(this);
  status = napi_wrap(env, wrapper, instance, FinalizeCallback, nullptr, &ref);
  NAPI_THROW_IF_FAILED_VOID(env, status);

  Reference<Object>* instanceRef = instance;
  *instanceRef = Reference<Object>(env, ref);
}

template <typename T>
inline ObjectWrap<T>::~ObjectWrap() {
  // If the JS object still exists at this point, remove the finalizer added
  // through `napi_wrap()`.
  if (!IsEmpty()) {
    Object object = Value();
    // It is not valid to call `napi_remove_wrap()` with an empty `object`.
    // This happens e.g. during garbage collection.
    if (!object.IsEmpty() && _construction_failed) {
      napi_remove_wrap(Env(), object, nullptr);
    }
  }
}

template<typename T>
inline T* ObjectWrap<T>::Unwrap(Object wrapper) {
  T* unwrapped;
  napi_status status = napi_unwrap(wrapper.Env(), wrapper, reinterpret_cast<void**>(&unwrapped));
  NAPI_THROW_IF_FAILED(wrapper.Env(), status, nullptr);
  return unwrapped;
}

template <typename T>
inline Function
ObjectWrap<T>::DefineClass(Napi::Env env,
                           const char* utf8name,
                           const size_t props_count,
                           const napi_property_descriptor* descriptors,
                           void* data) {
  napi_status status;
  std::vector<napi_property_descriptor> props(props_count);

  // We copy the descriptors to a local array because before defining the class
  // we must replace static method property descriptors with value property
  // descriptors such that the value is a function-valued `napi_value` created
  // with `CreateFunction()`.
  //
  // This replacement could be made for instance methods as well, but V8 aborts
  // if we do that, because it expects methods defined on the prototype template
  // to have `FunctionTemplate`s.
  for (size_t index = 0; index < props_count; index++) {
    props[index] = descriptors[index];
    napi_property_descriptor* prop = &props[index];
    if (prop->method == T::StaticMethodCallbackWrapper) {
      status = CreateFunction(env,
                             utf8name,
                             prop->method,
                             static_cast<StaticMethodCallbackData*>(prop->data),
               &(prop->value));
      NAPI_THROW_IF_FAILED(env, status, Function());
      prop->method = nullptr;
      prop->data = nullptr;
    } else if (prop->method == T::StaticVoidMethodCallbackWrapper) {
      status = CreateFunction(env,
                         utf8name,
                         prop->method,
                         static_cast<StaticVoidMethodCallbackData*>(prop->data),
                         &(prop->value));
      NAPI_THROW_IF_FAILED(env, status, Function());
      prop->method = nullptr;
      prop->data = nullptr;
    }
  }

  napi_value value;
  status = napi_define_class(env,
                             utf8name,
                             NAPI_AUTO_LENGTH,
                             T::ConstructorCallbackWrapper,
                             data,
                             props_count,
                             props.data(),
                             &value);
  NAPI_THROW_IF_FAILED(env, status, Function());

  // After defining the class we iterate once more over the property descriptors
  // and attach the data associated with accessors and instance methods to the
  // newly created JavaScript class.
  for (size_t idx = 0; idx < props_count; idx++) {
    const napi_property_descriptor* prop = &props[idx];

    if (prop->getter == T::StaticGetterCallbackWrapper ||
        prop->setter == T::StaticSetterCallbackWrapper) {
      status = Napi::details::AttachData(env,
                          value,
                          static_cast<StaticAccessorCallbackData*>(prop->data));
      NAPI_THROW_IF_FAILED(env, status, Function());
    } else {
      // InstanceWrap<T>::AttachPropData is responsible for attaching the data
      // of instance methods and accessors.
      T::AttachPropData(env, value, prop);
    }
  }

  return Function(env, value);
}

template <typename T>
inline Function ObjectWrap<T>::DefineClass(
    Napi::Env env,
    const char* utf8name,
    const std::initializer_list<ClassPropertyDescriptor<T>>& properties,
    void* data) {
  return DefineClass(env,
          utf8name,
          properties.size(),
          reinterpret_cast<const napi_property_descriptor*>(properties.begin()),
          data);
}

template <typename T>
inline Function ObjectWrap<T>::DefineClass(
    Napi::Env env,
    const char* utf8name,
    const std::vector<ClassPropertyDescriptor<T>>& properties,
    void* data) {
  return DefineClass(env,
           utf8name,
           properties.size(),
           reinterpret_cast<const napi_property_descriptor*>(properties.data()),
           data);
}

template <typename T>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticMethod(
    const char* utf8name,
    StaticVoidMethodCallback method,
    napi_property_attributes attributes,
    void* data) {
  StaticVoidMethodCallbackData* callbackData = new StaticVoidMethodCallbackData({ method, data });

  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.method = T::StaticVoidMethodCallbackWrapper;
  desc.data = callbackData;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticMethod(
    const char* utf8name,
    StaticMethodCallback method,
    napi_property_attributes attributes,
    void* data) {
  StaticMethodCallbackData* callbackData = new StaticMethodCallbackData({ method, data });

  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.method = T::StaticMethodCallbackWrapper;
  desc.data = callbackData;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticMethod(
    Symbol name,
    StaticVoidMethodCallback method,
    napi_property_attributes attributes,
    void* data) {
  StaticVoidMethodCallbackData* callbackData = new StaticVoidMethodCallbackData({ method, data });

  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.method = T::StaticVoidMethodCallbackWrapper;
  desc.data = callbackData;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticMethod(
    Symbol name,
    StaticMethodCallback method,
    napi_property_attributes attributes,
    void* data) {
  StaticMethodCallbackData* callbackData = new StaticMethodCallbackData({ method, data });

  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.method = T::StaticMethodCallbackWrapper;
  desc.data = callbackData;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
template <typename ObjectWrap<T>::StaticVoidMethodCallback method>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticMethod(
    const char* utf8name,
    napi_property_attributes attributes,
    void* data) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.method = details::TemplatedVoidCallback<method>;
  desc.data = data;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
template <typename ObjectWrap<T>::StaticVoidMethodCallback method>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticMethod(
    Symbol name,
    napi_property_attributes attributes,
    void* data) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.method = details::TemplatedVoidCallback<method>;
  desc.data = data;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
template <typename ObjectWrap<T>::StaticMethodCallback method>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticMethod(
    const char* utf8name,
    napi_property_attributes attributes,
    void* data) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.method = details::TemplatedCallback<method>;
  desc.data = data;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
template <typename ObjectWrap<T>::StaticMethodCallback method>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticMethod(
    Symbol name,
    napi_property_attributes attributes,
    void* data) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.method = details::TemplatedCallback<method>;
  desc.data = data;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticAccessor(
    const char* utf8name,
    StaticGetterCallback getter,
    StaticSetterCallback setter,
    napi_property_attributes attributes,
    void* data) {
  StaticAccessorCallbackData* callbackData =
    new StaticAccessorCallbackData({ getter, setter, data });

  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.getter = getter != nullptr ? T::StaticGetterCallbackWrapper : nullptr;
  desc.setter = setter != nullptr ? T::StaticSetterCallbackWrapper : nullptr;
  desc.data = callbackData;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticAccessor(
    Symbol name,
    StaticGetterCallback getter,
    StaticSetterCallback setter,
    napi_property_attributes attributes,
    void* data) {
  StaticAccessorCallbackData* callbackData =
    new StaticAccessorCallbackData({ getter, setter, data });

  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.getter = getter != nullptr ? T::StaticGetterCallbackWrapper : nullptr;
  desc.setter = setter != nullptr ? T::StaticSetterCallbackWrapper : nullptr;
  desc.data = callbackData;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
template <typename ObjectWrap<T>::StaticGetterCallback getter,
          typename ObjectWrap<T>::StaticSetterCallback setter>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticAccessor(
    const char* utf8name,
    napi_property_attributes attributes,
    void* data) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.getter = details::TemplatedCallback<getter>;
  desc.setter = This::WrapStaticSetter(This::StaticSetterTag<setter>());
  desc.data = data;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
template <typename ObjectWrap<T>::StaticGetterCallback getter,
          typename ObjectWrap<T>::StaticSetterCallback setter>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticAccessor(
    Symbol name,
    napi_property_attributes attributes,
    void* data) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.getter = details::TemplatedCallback<getter>;
  desc.setter = This::WrapStaticSetter(This::StaticSetterTag<setter>());
  desc.data = data;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticValue(const char* utf8name,
    Napi::Value value, napi_property_attributes attributes) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.utf8name = utf8name;
  desc.value = value;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
inline ClassPropertyDescriptor<T> ObjectWrap<T>::StaticValue(Symbol name,
    Napi::Value value, napi_property_attributes attributes) {
  napi_property_descriptor desc = napi_property_descriptor();
  desc.name = name;
  desc.value = value;
  desc.attributes = static_cast<napi_property_attributes>(attributes | napi_static);
  return desc;
}

template <typename T>
inline void ObjectWrap<T>::Finalize(Napi::Env /*env*/) {}

template <typename T>
inline napi_value ObjectWrap<T>::ConstructorCallbackWrapper(
    napi_env env,
    napi_callback_info info) {
  napi_value new_target;
  napi_status status = napi_get_new_target(env, info, &new_target);
  if (status != napi_ok) return nullptr;

  bool isConstructCall = (new_target != nullptr);
  if (!isConstructCall) {
    napi_throw_type_error(env, nullptr, "Class constructors cannot be invoked without 'new'");
    return nullptr;
  }

  napi_value wrapper = details::WrapCallback([&] {
    CallbackInfo callbackInfo(env, info);
    T* instance = new T(callbackInfo);
#ifdef NAPI_CPP_EXCEPTIONS
    instance->_construction_failed = false;
#else
    if (callbackInfo.Env().IsExceptionPending()) {
      // We need to clear the exception so that removing the wrap might work.
      Error e = callbackInfo.Env().GetAndClearPendingException();
      delete instance;
      e.ThrowAsJavaScriptException();
    } else {
      instance->_construction_failed = false;
    }
# endif  // NAPI_CPP_EXCEPTIONS
    return callbackInfo.This();
  });

  return wrapper;
}

template <typename T>
inline napi_value ObjectWrap<T>::StaticVoidMethodCallbackWrapper(
    napi_env env,
    napi_callback_info info) {
  return details::WrapCallback([&] {
    CallbackInfo callbackInfo(env, info);
    StaticVoidMethodCallbackData* callbackData =
      reinterpret_cast<StaticVoidMethodCallbackData*>(callbackInfo.Data());
    callbackInfo.SetData(callbackData->data);
    callbackData->callback(callbackInfo);
    return nullptr;
  });
}

template <typename T>
inline napi_value ObjectWrap<T>::StaticMethodCallbackWrapper(
    napi_env env,
    napi_callback_info info) {
  return details::WrapCallback([&] {
    CallbackInfo callbackInfo(env, info);
    StaticMethodCallbackData* callbackData =
      reinterpret_cast<StaticMethodCallbackData*>(callbackInfo.Data());
    callbackInfo.SetData(callbackData->data);
    return callbackData->callback(callbackInfo);
  });
}

template <typename T>
inline napi_value ObjectWrap<T>::StaticGetterCallbackWrapper(
    napi_env env,
    napi_callback_info info) {
  return details::WrapCallback([&] {
    CallbackInfo callbackInfo(env, info);
    StaticAccessorCallbackData* callbackData =
      reinterpret_cast<StaticAccessorCallbackData*>(callbackInfo.Data());
    callbackInfo.SetData(callbackData->data);
    return callbackData->getterCallback(callbackInfo);
  });
}

template <typename T>
inline napi_value ObjectWrap<T>::StaticSetterCallbackWrapper(
    napi_env env,
    napi_callback_info info) {
  return details::WrapCallback([&] {
    CallbackInfo callbackInfo(env, info);
    StaticAccessorCallbackData* callbackData =
      reinterpret_cast<StaticAccessorCallbackData*>(callbackInfo.Data());
    callbackInfo.SetData(callbackData->data);
    callbackData->setterCallback(callbackInfo, callbackInfo[0]);
    return nullptr;
  });
}

template <typename T>
inline void ObjectWrap<T>::FinalizeCallback(napi_env env, void* data, void* /*hint*/) {
  HandleScope scope(env);
  T* instance = static_cast<T*>(data);
  instance->Finalize(Napi::Env(env));
  delete instance;
}

template <typename T>
template <typename ObjectWrap<T>::StaticSetterCallback method>
inline napi_value ObjectWrap<T>::WrappedMethod(
    napi_env env, napi_callback_info info) NAPI_NOEXCEPT {
  return details::WrapCallback([&] {
    const CallbackInfo cbInfo(env, info);
    method(cbInfo, cbInfo[0]);
    return nullptr;
  });
}

////////////////////////////////////////////////////////////////////////////////
// HandleScope class
////////////////////////////////////////////////////////////////////////////////

inline HandleScope::HandleScope(napi_env env, napi_handle_scope scope)
    : _env(env), _scope(scope) {
}

inline HandleScope::HandleScope(Napi::Env env) : _env(env) {
  napi_status status = napi_open_handle_scope(_env, &_scope);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

inline HandleScope::~HandleScope() {
  napi_status status = napi_close_handle_scope(_env, _scope);
  NAPI_FATAL_IF_FAILED(status,
                       "HandleScope::~HandleScope",
                       "napi_close_handle_scope");
}

inline HandleScope::operator napi_handle_scope() const {
  return _scope;
}

inline Napi::Env HandleScope::Env() const {
  return Napi::Env(_env);
}

////////////////////////////////////////////////////////////////////////////////
// EscapableHandleScope class
////////////////////////////////////////////////////////////////////////////////

inline EscapableHandleScope::EscapableHandleScope(
  napi_env env, napi_escapable_handle_scope scope) : _env(env), _scope(scope) {
}

inline EscapableHandleScope::EscapableHandleScope(Napi::Env env) : _env(env) {
  napi_status status = napi_open_escapable_handle_scope(_env, &_scope);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

inline EscapableHandleScope::~EscapableHandleScope() {
  napi_status status = napi_close_escapable_handle_scope(_env, _scope);
  NAPI_FATAL_IF_FAILED(status,
                       "EscapableHandleScope::~EscapableHandleScope",
                       "napi_close_escapable_handle_scope");
}

inline EscapableHandleScope::operator napi_escapable_handle_scope() const {
  return _scope;
}

inline Napi::Env EscapableHandleScope::Env() const {
  return Napi::Env(_env);
}

inline Value EscapableHandleScope::Escape(napi_value escapee) {
  napi_value result;
  napi_status status = napi_escape_handle(_env, _scope, escapee, &result);
  NAPI_THROW_IF_FAILED(_env, status, Value());
  return Value(_env, result);
}


#if (NAPI_VERSION > 2)
////////////////////////////////////////////////////////////////////////////////
// CallbackScope class
////////////////////////////////////////////////////////////////////////////////

inline CallbackScope::CallbackScope(
  napi_env env, napi_callback_scope scope) : _env(env), _scope(scope) {
}

inline CallbackScope::CallbackScope(napi_env env, napi_async_context context)
    : _env(env) {
  napi_status status = napi_open_callback_scope(
      _env, Object::New(env), context, &_scope);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

inline CallbackScope::~CallbackScope() {
  napi_status status = napi_close_callback_scope(_env, _scope);
  NAPI_FATAL_IF_FAILED(status,
                       "CallbackScope::~CallbackScope",
                       "napi_close_callback_scope");
}

inline CallbackScope::operator napi_callback_scope() const {
  return _scope;
}

inline Napi::Env CallbackScope::Env() const {
  return Napi::Env(_env);
}
#endif

////////////////////////////////////////////////////////////////////////////////
// AsyncContext class
////////////////////////////////////////////////////////////////////////////////

inline AsyncContext::AsyncContext(napi_env env, const char* resource_name)
  : AsyncContext(env, resource_name, Object::New(env)) {
}

inline AsyncContext::AsyncContext(napi_env env,
                                  const char* resource_name,
                                  const Object& resource)
    : _env(env), _context(nullptr) {
  napi_value resource_id;
  napi_status status = napi_create_string_utf8(
      _env, resource_name, NAPI_AUTO_LENGTH, &resource_id);
  NAPI_THROW_IF_FAILED_VOID(_env, status);

  status = napi_async_init(_env, resource, resource_id, &_context);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

inline AsyncContext::~AsyncContext() {
  if (_context != nullptr) {
    napi_async_destroy(_env, _context);
    _context = nullptr;
  }
}

inline AsyncContext::AsyncContext(AsyncContext&& other) {
  _env = other._env;
  other._env = nullptr;
  _context = other._context;
  other._context = nullptr;
}

inline AsyncContext& AsyncContext::operator =(AsyncContext&& other) {
  _env = other._env;
  other._env = nullptr;
  _context = other._context;
  other._context = nullptr;
  return *this;
}

inline AsyncContext::operator napi_async_context() const {
  return _context;
}

inline Napi::Env AsyncContext::Env() const {
  return Napi::Env(_env);
}

////////////////////////////////////////////////////////////////////////////////
// AsyncWorker class
////////////////////////////////////////////////////////////////////////////////

inline AsyncWorker::AsyncWorker(const Function& callback)
  : AsyncWorker(callback, "generic") {
}

inline AsyncWorker::AsyncWorker(const Function& callback,
                                const char* resource_name)
  : AsyncWorker(callback, resource_name, Object::New(callback.Env())) {
}

inline AsyncWorker::AsyncWorker(const Function& callback,
                                const char* resource_name,
                                const Object& resource)
  : AsyncWorker(Object::New(callback.Env()),
                callback,
                resource_name,
                resource) {
}

inline AsyncWorker::AsyncWorker(const Object& receiver,
                                const Function& callback)
  : AsyncWorker(receiver, callback, "generic") {
}

inline AsyncWorker::AsyncWorker(const Object& receiver,
                                const Function& callback,
                                const char* resource_name)
  : AsyncWorker(receiver,
                callback,
                resource_name,
                Object::New(callback.Env())) {
}

inline AsyncWorker::AsyncWorker(const Object& receiver,
                                const Function& callback,
                                const char* resource_name,
                                const Object& resource)
  : _env(callback.Env()),
    _receiver(Napi::Persistent(receiver)),
    _callback(Napi::Persistent(callback)),
    _suppress_destruct(false) {
  napi_value resource_id;
  napi_status status = napi_create_string_latin1(
      _env, resource_name, NAPI_AUTO_LENGTH, &resource_id);
  NAPI_THROW_IF_FAILED_VOID(_env, status);

  status = napi_create_async_work(_env, resource, resource_id, OnAsyncWorkExecute,
                                  OnAsyncWorkComplete, this, &_work);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

inline AsyncWorker::AsyncWorker(Napi::Env env)
  : AsyncWorker(env, "generic") {
}

inline AsyncWorker::AsyncWorker(Napi::Env env,
                                const char* resource_name)
  : AsyncWorker(env, resource_name, Object::New(env)) {
}

inline AsyncWorker::AsyncWorker(Napi::Env env,
                                const char* resource_name,
                                const Object& resource)
  : _env(env),
    _receiver(),
    _callback(),
    _suppress_destruct(false) {
  napi_value resource_id;
  napi_status status = napi_create_string_latin1(
      _env, resource_name, NAPI_AUTO_LENGTH, &resource_id);
  NAPI_THROW_IF_FAILED_VOID(_env, status);

  status = napi_create_async_work(_env, resource, resource_id, OnAsyncWorkExecute,
                                  OnAsyncWorkComplete, this, &_work);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

inline AsyncWorker::~AsyncWorker() {
  if (_work != nullptr) {
    napi_delete_async_work(_env, _work);
    _work = nullptr;
  }
}

inline void AsyncWorker::Destroy() {
  delete this;
}

inline AsyncWorker::AsyncWorker(AsyncWorker&& other) {
  _env = other._env;
  other._env = nullptr;
  _work = other._work;
  other._work = nullptr;
  _receiver = std::move(other._receiver);
  _callback = std::move(other._callback);
  _error = std::move(other._error);
  _suppress_destruct = other._suppress_destruct;
}

inline AsyncWorker& AsyncWorker::operator =(AsyncWorker&& other) {
  _env = other._env;
  other._env = nullptr;
  _work = other._work;
  other._work = nullptr;
  _receiver = std::move(other._receiver);
  _callback = std::move(other._callback);
  _error = std::move(other._error);
  _suppress_destruct = other._suppress_destruct;
  return *this;
}

inline AsyncWorker::operator napi_async_work() const {
  return _work;
}

inline Napi::Env AsyncWorker::Env() const {
  return Napi::Env(_env);
}

inline void AsyncWorker::Queue() {
  napi_status status = napi_queue_async_work(_env, _work);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

inline void AsyncWorker::Cancel() {
  napi_status status = napi_cancel_async_work(_env, _work);
  NAPI_THROW_IF_FAILED_VOID(_env, status);
}

inline ObjectReference& AsyncWorker::Receiver() {
  return _receiver;
}

inline FunctionReference& AsyncWorker::Callback() {
  return _callback;
}

inline void AsyncWorker::SuppressDestruct() {
  _suppress_destruct = true;
}

inline void AsyncWorker::OnOK() {
  if (!_callback.IsEmpty()) {
    _callback.Call(_receiver.Value(), GetResult(_callback.Env()));
  }
}

inline void AsyncWorker::OnError(const Error& e) {
  if (!_callback.IsEmpty()) {
    _callback.Call(_receiver.Value(), std::initializer_list<napi_value>{ e.Value() });
  }
}

inline void AsyncWorker::SetError(const std::string& error) {
  _error = error;
}

inline std::vector<napi_value> AsyncWorker::GetResult(Napi::Env /*env*/) {
  return {};
}
// The OnAsyncWorkExecute method receives an napi_env argument. However, do NOT
// use it within this method, as it does not run on the JavaScript thread and
// must not run any method that would cause JavaScript to run. In practice,
// this means that almost any use of napi_env will be incorrect.
inline void AsyncWorker::OnAsyncWorkExecute(napi_env env, void* asyncworker) {
  AsyncWorker* self = static_cast<AsyncWorker*>(asyncworker);
  self->OnExecute(env);
}
// The OnExecute method receives an napi_env argument. However, do NOT
// use it within this method, as it does not run on the JavaScript thread and
// must not run any method that would cause JavaScript to run. In practice,
// this means that almost any use of napi_env will be incorrect.
inline void AsyncWorker::OnExecute(Napi::Env /*DO_NOT_USE*/) {
#ifdef NAPI_CPP_EXCEPTIONS
  try {
    Execute();
  } catch (const std::exception& e) {
    SetError(e.what());
  }
#else // NAPI_CPP_EXCEPTIONS
  Execute();
#endif // NAPI_CPP_EXCEPTIONS
}

inline void AsyncWorker::OnAsyncWorkComplete(napi_env env,
                                             napi_status status,
                                             void* asyncworker) {
  AsyncWorker* self = static_cast<AsyncWorker*>(asyncworker);
  self->OnWorkComplete(env, status);
}
inline void AsyncWorker::OnWorkComplete(Napi::Env /*env*/, napi_status status) {
  if (status != napi_cancelled) {
    HandleScope scope(_env);
    details::WrapCallback([&] {
      if (_error.size() == 0) {
        OnOK();
      }
      else {
        OnError(Error::New(_env, _error));
      }
      return nullptr;
    });
  }
  if (!_suppress_destruct) {
    Destroy();
  }
}

#if (NAPI_VERSION > 3 && !defined(__wasm32__))
////////////////////////////////////////////////////////////////////////////////
// TypedThreadSafeFunction<ContextType,DataType,CallJs> class
////////////////////////////////////////////////////////////////////////////////

// Starting with NAPI 5, the JavaScript function `func` parameter of
// `napi_create_threadsafe_function` is optional.
#if NAPI_VERSION > 4
// static, with Callback [missing] Resource [missing] Finalizer [missing]
template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
template <typename ResourceString>
inline TypedThreadSafeFunction<ContextType, DataType, CallJs>
TypedThreadSafeFunction<ContextType, DataType, CallJs>::New(
    napi_env env,
    ResourceString resourceName,
    size_t maxQueueSize,
    size_t initialThreadCount,
    ContextType* context) {
  TypedThreadSafeFunction<ContextType, DataType, CallJs> tsfn;

  napi_status status =
      napi_create_threadsafe_function(env,
                                      nullptr,
                                      nullptr,
                                      String::From(env, resourceName),
                                      maxQueueSize,
                                      initialThreadCount,
                                      nullptr,
                                      nullptr,
                                      context,
                                      CallJsInternal,
                                      &tsfn._tsfn);
  if (status != napi_ok) {
    NAPI_THROW_IF_FAILED(
        env, status, TypedThreadSafeFunction<ContextType, DataType, CallJs>());
  }

  return tsfn;
}

// static, with Callback [missing] Resource [passed] Finalizer [missing]
template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
template <typename ResourceString>
inline TypedThreadSafeFunction<ContextType, DataType, CallJs>
TypedThreadSafeFunction<ContextType, DataType, CallJs>::New(
    napi_env env,
    const Object& resource,
    ResourceString resourceName,
    size_t maxQueueSize,
    size_t initialThreadCount,
    ContextType* context) {
  TypedThreadSafeFunction<ContextType, DataType, CallJs> tsfn;

  napi_status status =
      napi_create_threadsafe_function(env,
                                      nullptr,
                                      resource,
                                      String::From(env, resourceName),
                                      maxQueueSize,
                                      initialThreadCount,
                                      nullptr,
                                      nullptr,
                                      context,
                                      CallJsInternal,
                                      &tsfn._tsfn);
  if (status != napi_ok) {
    NAPI_THROW_IF_FAILED(
        env, status, TypedThreadSafeFunction<ContextType, DataType, CallJs>());
  }

  return tsfn;
}

// static, with Callback [missing] Resource [missing] Finalizer [passed]
template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
template <typename ResourceString,
          typename Finalizer,
          typename FinalizerDataType>
inline TypedThreadSafeFunction<ContextType, DataType, CallJs>
TypedThreadSafeFunction<ContextType, DataType, CallJs>::New(
    napi_env env,
    ResourceString resourceName,
    size_t maxQueueSize,
    size_t initialThreadCount,
    ContextType* context,
    Finalizer finalizeCallback,
    FinalizerDataType* data) {
  TypedThreadSafeFunction<ContextType, DataType, CallJs> tsfn;

  auto* finalizeData = new details::
      ThreadSafeFinalize<ContextType, Finalizer, FinalizerDataType>(
          {data, finalizeCallback});
  napi_status status = napi_create_threadsafe_function(
      env,
      nullptr,
      nullptr,
      String::From(env, resourceName),
      maxQueueSize,
      initialThreadCount,
      finalizeData,
      details::ThreadSafeFinalize<ContextType, Finalizer, FinalizerDataType>::
          FinalizeFinalizeWrapperWithDataAndContext,
      context,
      CallJsInternal,
      &tsfn._tsfn);
  if (status != napi_ok) {
    delete finalizeData;
    NAPI_THROW_IF_FAILED(
        env, status, TypedThreadSafeFunction<ContextType, DataType, CallJs>());
  }

  return tsfn;
}

// static, with Callback [missing] Resource [passed] Finalizer [passed]
template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
template <typename ResourceString,
          typename Finalizer,
          typename FinalizerDataType>
inline TypedThreadSafeFunction<ContextType, DataType, CallJs>
TypedThreadSafeFunction<ContextType, DataType, CallJs>::New(
    napi_env env,
    const Object& resource,
    ResourceString resourceName,
    size_t maxQueueSize,
    size_t initialThreadCount,
    ContextType* context,
    Finalizer finalizeCallback,
    FinalizerDataType* data) {
  TypedThreadSafeFunction<ContextType, DataType, CallJs> tsfn;

  auto* finalizeData = new details::
      ThreadSafeFinalize<ContextType, Finalizer, FinalizerDataType>(
          {data, finalizeCallback});
  napi_status status = napi_create_threadsafe_function(
      env,
      nullptr,
      resource,
      String::From(env, resourceName),
      maxQueueSize,
      initialThreadCount,
      finalizeData,
      details::ThreadSafeFinalize<ContextType, Finalizer, FinalizerDataType>::
          FinalizeFinalizeWrapperWithDataAndContext,
      context,
      CallJsInternal,
      &tsfn._tsfn);
  if (status != napi_ok) {
    delete finalizeData;
    NAPI_THROW_IF_FAILED(
        env, status, TypedThreadSafeFunction<ContextType, DataType, CallJs>());
  }

  return tsfn;
}
#endif

// static, with Callback [passed] Resource [missing] Finalizer [missing]
template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
template <typename ResourceString>
inline TypedThreadSafeFunction<ContextType, DataType, CallJs>
TypedThreadSafeFunction<ContextType, DataType, CallJs>::New(
    napi_env env,
    const Function& callback,
    ResourceString resourceName,
    size_t maxQueueSize,
    size_t initialThreadCount,
    ContextType* context) {
  TypedThreadSafeFunction<ContextType, DataType, CallJs> tsfn;

  napi_status status =
      napi_create_threadsafe_function(env,
                                      callback,
                                      nullptr,
                                      String::From(env, resourceName),
                                      maxQueueSize,
                                      initialThreadCount,
                                      nullptr,
                                      nullptr,
                                      context,
                                      CallJsInternal,
                                      &tsfn._tsfn);
  if (status != napi_ok) {
    NAPI_THROW_IF_FAILED(
        env, status, TypedThreadSafeFunction<ContextType, DataType, CallJs>());
  }

  return tsfn;
}

// static, with Callback [passed] Resource [passed] Finalizer [missing]
template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
template <typename ResourceString>
inline TypedThreadSafeFunction<ContextType, DataType, CallJs>
TypedThreadSafeFunction<ContextType, DataType, CallJs>::New(
    napi_env env,
    const Function& callback,
    const Object& resource,
    ResourceString resourceName,
    size_t maxQueueSize,
    size_t initialThreadCount,
    ContextType* context) {
  TypedThreadSafeFunction<ContextType, DataType, CallJs> tsfn;

  napi_status status =
      napi_create_threadsafe_function(env,
                                      callback,
                                      resource,
                                      String::From(env, resourceName),
                                      maxQueueSize,
                                      initialThreadCount,
                                      nullptr,
                                      nullptr,
                                      context,
                                      CallJsInternal,
                                      &tsfn._tsfn);
  if (status != napi_ok) {
    NAPI_THROW_IF_FAILED(
        env, status, TypedThreadSafeFunction<ContextType, DataType, CallJs>());
  }

  return tsfn;
}

// static, with Callback [passed] Resource [missing] Finalizer [passed]
template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
template <typename ResourceString,
          typename Finalizer,
          typename FinalizerDataType>
inline TypedThreadSafeFunction<ContextType, DataType, CallJs>
TypedThreadSafeFunction<ContextType, DataType, CallJs>::New(
    napi_env env,
    const Function& callback,
    ResourceString resourceName,
    size_t maxQueueSize,
    size_t initialThreadCount,
    ContextType* context,
    Finalizer finalizeCallback,
    FinalizerDataType* data) {
  TypedThreadSafeFunction<ContextType, DataType, CallJs> tsfn;

  auto* finalizeData = new details::
      ThreadSafeFinalize<ContextType, Finalizer, FinalizerDataType>(
          {data, finalizeCallback});
  napi_status status = napi_create_threadsafe_function(
      env,
      callback,
      nullptr,
      String::From(env, resourceName),
      maxQueueSize,
      initialThreadCount,
      finalizeData,
      details::ThreadSafeFinalize<ContextType, Finalizer, FinalizerDataType>::
          FinalizeFinalizeWrapperWithDataAndContext,
      context,
      CallJsInternal,
      &tsfn._tsfn);
  if (status != napi_ok) {
    delete finalizeData;
    NAPI_THROW_IF_FAILED(
        env, status, TypedThreadSafeFunction<ContextType, DataType, CallJs>());
  }

  return tsfn;
}

// static, with: Callback [passed] Resource [passed] Finalizer [passed]
template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
template <typename CallbackType,
          typename ResourceString,
          typename Finalizer,
          typename FinalizerDataType>
inline TypedThreadSafeFunction<ContextType, DataType, CallJs>
TypedThreadSafeFunction<ContextType, DataType, CallJs>::New(
    napi_env env,
    CallbackType callback,
    const Object& resource,
    ResourceString resourceName,
    size_t maxQueueSize,
    size_t initialThreadCount,
    ContextType* context,
    Finalizer finalizeCallback,
    FinalizerDataType* data) {
  TypedThreadSafeFunction<ContextType, DataType, CallJs> tsfn;

  auto* finalizeData = new details::
      ThreadSafeFinalize<ContextType, Finalizer, FinalizerDataType>(
          {data, finalizeCallback});
  napi_status status = napi_create_threadsafe_function(
      env,
      details::DefaultCallbackWrapper<
          CallbackType,
          TypedThreadSafeFunction<ContextType, DataType, CallJs>>(env,
                                                                  callback),
      resource,
      String::From(env, resourceName),
      maxQueueSize,
      initialThreadCount,
      finalizeData,
      details::ThreadSafeFinalize<ContextType, Finalizer, FinalizerDataType>::
          FinalizeFinalizeWrapperWithDataAndContext,
      context,
      CallJsInternal,
      &tsfn._tsfn);
  if (status != napi_ok) {
    delete finalizeData;
    NAPI_THROW_IF_FAILED(
        env, status, TypedThreadSafeFunction<ContextType, DataType, CallJs>());
  }

  return tsfn;
}

template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
inline TypedThreadSafeFunction<ContextType, DataType, CallJs>::
    TypedThreadSafeFunction()
    : _tsfn() {}

template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
inline TypedThreadSafeFunction<ContextType, DataType, CallJs>::
    TypedThreadSafeFunction(napi_threadsafe_function tsfn)
    : _tsfn(tsfn) {}

template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
inline TypedThreadSafeFunction<ContextType, DataType, CallJs>::
operator napi_threadsafe_function() const {
  return _tsfn;
}

template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
inline napi_status
TypedThreadSafeFunction<ContextType, DataType, CallJs>::BlockingCall(
    DataType* data) const {
  return napi_call_threadsafe_function(_tsfn, data, napi_tsfn_blocking);
}

template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
inline napi_status
TypedThreadSafeFunction<ContextType, DataType, CallJs>::NonBlockingCall(
    DataType* data) const {
  return napi_call_threadsafe_function(_tsfn, data, napi_tsfn_nonblocking);
}

template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
inline void TypedThreadSafeFunction<ContextType, DataType, CallJs>::Ref(
    napi_env env) const {
  if (_tsfn != nullptr) {
    napi_status status = napi_ref_threadsafe_function(env, _tsfn);
    NAPI_THROW_IF_FAILED_VOID(env, status);
  }
}

template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
inline void TypedThreadSafeFunction<ContextType, DataType, CallJs>::Unref(
    napi_env env) const {
  if (_tsfn != nullptr) {
    napi_status status = napi_unref_threadsafe_function(env, _tsfn);
    NAPI_THROW_IF_FAILED_VOID(env, status);
  }
}

template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
inline napi_status
TypedThreadSafeFunction<ContextType, DataType, CallJs>::Acquire() const {
  return napi_acquire_threadsafe_function(_tsfn);
}

template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
inline napi_status
TypedThreadSafeFunction<ContextType, DataType, CallJs>::Release() {
  return napi_release_threadsafe_function(_tsfn, napi_tsfn_release);
}

template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
inline napi_status
TypedThreadSafeFunction<ContextType, DataType, CallJs>::Abort() {
  return napi_release_threadsafe_function(_tsfn, napi_tsfn_abort);
}

template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
inline ContextType*
TypedThreadSafeFunction<ContextType, DataType, CallJs>::GetContext() const {
  void* context;
  napi_status status = napi_get_threadsafe_function_context(_tsfn, &context);
  NAPI_FATAL_IF_FAILED(status,
                       "TypedThreadSafeFunction::GetContext",
                       "napi_get_threadsafe_function_context");
  return static_cast<ContextType*>(context);
}

// static
template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
void TypedThreadSafeFunction<ContextType, DataType, CallJs>::CallJsInternal(
    napi_env env, napi_value jsCallback, void* context, void* data) {
  details::CallJsWrapper<ContextType, DataType, decltype(CallJs), CallJs>(
      env, jsCallback, context, data);
}

#if NAPI_VERSION == 4
// static
template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
Napi::Function
TypedThreadSafeFunction<ContextType, DataType, CallJs>::EmptyFunctionFactory(
    Napi::Env env) {
  return Napi::Function::New(env, [](const CallbackInfo& cb) {});
}

// static
template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
Napi::Function
TypedThreadSafeFunction<ContextType, DataType, CallJs>::FunctionOrEmpty(
    Napi::Env env, Napi::Function& callback) {
  if (callback.IsEmpty()) {
    return EmptyFunctionFactory(env);
  }
  return callback;
}

#else
// static
template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
std::nullptr_t
TypedThreadSafeFunction<ContextType, DataType, CallJs>::EmptyFunctionFactory(
    Napi::Env /*env*/) {
  return nullptr;
}

// static
template <typename ContextType,
          typename DataType,
          void (*CallJs)(Napi::Env, Napi::Function, ContextType*, DataType*)>
Napi::Function
TypedThreadSafeFunction<ContextType, DataType, CallJs>::FunctionOrEmpty(
    Napi::Env /*env*/, Napi::Function& callback) {
  return callback;
}

#endif

////////////////////////////////////////////////////////////////////////////////
// ThreadSafeFunction class
////////////////////////////////////////////////////////////////////////////////

// static
template <typename ResourceString>
inline ThreadSafeFunction ThreadSafeFunction::New(napi_env env,
                                  const Function& callback,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount) {
  return New(env, callback, Object(), resourceName, maxQueueSize,
             initialThreadCount);
}

// static
template <typename ResourceString, typename ContextType>
inline ThreadSafeFunction ThreadSafeFunction::New(napi_env env,
                                  const Function& callback,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  ContextType* context) {
  return New(env, callback, Object(), resourceName, maxQueueSize,
             initialThreadCount, context);
}

// static
template <typename ResourceString, typename Finalizer>
inline ThreadSafeFunction ThreadSafeFunction::New(napi_env env,
                                  const Function& callback,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  Finalizer finalizeCallback) {
  return New(env, callback, Object(), resourceName, maxQueueSize,
             initialThreadCount, finalizeCallback);
}

// static
template <typename ResourceString, typename Finalizer,
          typename FinalizerDataType>
inline ThreadSafeFunction ThreadSafeFunction::New(napi_env env,
                                  const Function& callback,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  Finalizer finalizeCallback,
                                  FinalizerDataType* data) {
  return New(env, callback, Object(), resourceName, maxQueueSize,
             initialThreadCount, finalizeCallback, data);
}

// static
template <typename ResourceString, typename ContextType, typename Finalizer>
inline ThreadSafeFunction ThreadSafeFunction::New(napi_env env,
                                  const Function& callback,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  ContextType* context,
                                  Finalizer finalizeCallback) {
  return New(env, callback, Object(), resourceName, maxQueueSize,
             initialThreadCount, context, finalizeCallback);
}

// static
template <typename ResourceString, typename ContextType,
          typename Finalizer, typename FinalizerDataType>
inline ThreadSafeFunction ThreadSafeFunction::New(napi_env env,
                                                  const Function& callback,
                                                  ResourceString resourceName,
                                                  size_t maxQueueSize,
                                                  size_t initialThreadCount,
                                                  ContextType* context,
                                                  Finalizer finalizeCallback,
                                                  FinalizerDataType* data) {
  return New(env, callback, Object(), resourceName, maxQueueSize,
             initialThreadCount, context, finalizeCallback, data);
}

// static
template <typename ResourceString>
inline ThreadSafeFunction ThreadSafeFunction::New(napi_env env,
                                  const Function& callback,
                                  const Object& resource,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount) {
  return New(env, callback, resource, resourceName, maxQueueSize,
             initialThreadCount, static_cast<void*>(nullptr) /* context */);
}

// static
template <typename ResourceString, typename ContextType>
inline ThreadSafeFunction ThreadSafeFunction::New(napi_env env,
                                  const Function& callback,
                                  const Object& resource,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  ContextType* context) {
  return New(env, callback, resource, resourceName, maxQueueSize,
             initialThreadCount, context,
             [](Env, ContextType*) {} /* empty finalizer */);
}

// static
template <typename ResourceString, typename Finalizer>
inline ThreadSafeFunction ThreadSafeFunction::New(napi_env env,
                                  const Function& callback,
                                  const Object& resource,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  Finalizer finalizeCallback) {
  return New(env, callback, resource, resourceName, maxQueueSize,
             initialThreadCount, static_cast<void*>(nullptr) /* context */,
             finalizeCallback, static_cast<void*>(nullptr) /* data */,
             details::ThreadSafeFinalize<void, Finalizer>::Wrapper);
}

// static
template <typename ResourceString, typename Finalizer,
          typename FinalizerDataType>
inline ThreadSafeFunction ThreadSafeFunction::New(napi_env env,
                                  const Function& callback,
                                  const Object& resource,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  Finalizer finalizeCallback,
                                  FinalizerDataType* data) {
  return New(env, callback, resource, resourceName, maxQueueSize,
             initialThreadCount, static_cast<void*>(nullptr) /* context */,
             finalizeCallback, data,
             details::ThreadSafeFinalize<
                 void, Finalizer, FinalizerDataType>::FinalizeWrapperWithData);
}

// static
template <typename ResourceString, typename ContextType, typename Finalizer>
inline ThreadSafeFunction ThreadSafeFunction::New(napi_env env,
                                  const Function& callback,
                                  const Object& resource,
                                  ResourceString resourceName,
                                  size_t maxQueueSize,
                                  size_t initialThreadCount,
                                  ContextType* context,
                                  Finalizer finalizeCallback) {
  return New(env, callback, resource, resourceName, maxQueueSize,
             initialThreadCount, context, finalizeCallback,
             static_cast<void*>(nullptr) /* data */,
             details::ThreadSafeFinalize<
                 ContextType, Finalizer>::FinalizeWrapperWithContext);
}

// static
template <typename ResourceString, typename ContextType,
          typename Finalizer, typename FinalizerDataType>
inline ThreadSafeFunction ThreadSafeFunction::New(napi_env env,
                                                  const Function& callback,
                                                  const Object& resource,
                                                  ResourceString resourceName,
                                                  size_t maxQueueSize,
                                                  size_t initialThreadCount,
                                                  ContextType* context,
                                                  Finalizer finalizeCallback,
                                                  FinalizerDataType* data) {
  return New(env, callback, resource, resourceName, maxQueueSize,
             initialThreadCount, context, finalizeCallback, data,
             details::ThreadSafeFinalize<ContextType, Finalizer,
                 FinalizerDataType>::FinalizeFinalizeWrapperWithDataAndContext);
}

inline ThreadSafeFunction::ThreadSafeFunction()
  : _tsfn() {
}

inline ThreadSafeFunction::ThreadSafeFunction(
    napi_threadsafe_function tsfn)
  : _tsfn(tsfn) {
}

inline ThreadSafeFunction::operator napi_threadsafe_function() const {
  return _tsfn;
}

inline napi_status ThreadSafeFunction::BlockingCall() const {
  return CallInternal(nullptr, napi_tsfn_blocking);
}

template <>
inline napi_status ThreadSafeFunction::BlockingCall(
    void* data) const {
  return napi_call_threadsafe_function(_tsfn, data, napi_tsfn_blocking);
}

template <typename Callback>
inline napi_status ThreadSafeFunction::BlockingCall(
    Callback callback) const {
  return CallInternal(new CallbackWrapper(callback), napi_tsfn_blocking);
}

template <typename DataType, typename Callback>
inline napi_status ThreadSafeFunction::BlockingCall(
    DataType* data, Callback callback) const {
  auto wrapper = [data, callback](Env env, Function jsCallback) {
    callback(env, jsCallback, data);
  };
  return CallInternal(new CallbackWrapper(wrapper), napi_tsfn_blocking);
}

inline napi_status ThreadSafeFunction::NonBlockingCall() const {
  return CallInternal(nullptr, napi_tsfn_nonblocking);
}

template <>
inline napi_status ThreadSafeFunction::NonBlockingCall(
    void* data) const {
  return napi_call_threadsafe_function(_tsfn, data, napi_tsfn_nonblocking);
}

template <typename Callback>
inline napi_status ThreadSafeFunction::NonBlockingCall(
    Callback callback) const {
  return CallInternal(new CallbackWrapper(callback), napi_tsfn_nonblocking);
}

template <typename DataType, typename Callback>
inline napi_status ThreadSafeFunction::NonBlockingCall(
    DataType* data, Callback callback) const {
  auto wrapper = [data, callback](Env env, Function jsCallback) {
    callback(env, jsCallback, data);
  };
  return CallInternal(new CallbackWrapper(wrapper), napi_tsfn_nonblocking);
}

inline void ThreadSafeFunction::Ref(napi_env env) const {
  if (_tsfn != nullptr) {
    napi_status status = napi_ref_threadsafe_function(env, _tsfn);
    NAPI_THROW_IF_FAILED_VOID(env, status);
  }
}

inline void ThreadSafeFunction::Unref(napi_env env) const {
  if (_tsfn != nullptr) {
    napi_status status = napi_unref_threadsafe_function(env, _tsfn);
    NAPI_THROW_IF_FAILED_VOID(env, status);
  }
}

inline napi_status ThreadSafeFunction::Acquire() const {
  return napi_acquire_threadsafe_function(_tsfn);
}

inline napi_status ThreadSafeFunction::Release() {
  return napi_release_threadsafe_function(_tsfn, napi_tsfn_release);
}

inline napi_status ThreadSafeFunction::Abort() {
  return napi_release_threadsafe_function(_tsfn, napi_tsfn_abort);
}

inline ThreadSafeFunction::ConvertibleContext
ThreadSafeFunction::GetContext() const {
  void* context;
  napi_status status = napi_get_threadsafe_function_context(_tsfn, &context);
  NAPI_FATAL_IF_FAILED(status, "ThreadSafeFunction::GetContext", "napi_get_threadsafe_function_context");
  return ConvertibleContext({ context });
}

// static
template <typename ResourceString, typename ContextType,
          typename Finalizer, typename FinalizerDataType>
inline ThreadSafeFunction ThreadSafeFunction::New(napi_env env,
                                                  const Function& callback,
                                                  const Object& resource,
                                                  ResourceString resourceName,
                                                  size_t maxQueueSize,
                                                  size_t initialThreadCount,
                                                  ContextType* context,
                                                  Finalizer finalizeCallback,
                                                  FinalizerDataType* data,
                                                  napi_finalize wrapper) {
  static_assert(details::can_make_string<ResourceString>::value
      || std::is_convertible<ResourceString, napi_value>::value,
      "Resource name should be convertible to the string type");

  ThreadSafeFunction tsfn;
  auto* finalizeData = new details::ThreadSafeFinalize<ContextType, Finalizer,
      FinalizerDataType>({ data, finalizeCallback });
  napi_status status = napi_create_threadsafe_function(env, callback, resource,
      Value::From(env, resourceName), maxQueueSize, initialThreadCount,
      finalizeData, wrapper, context, CallJS, &tsfn._tsfn);
  if (status != napi_ok) {
    delete finalizeData;
    NAPI_THROW_IF_FAILED(env, status, ThreadSafeFunction());
  }

  return tsfn;
}

inline napi_status ThreadSafeFunction::CallInternal(
    CallbackWrapper* callbackWrapper,
    napi_threadsafe_function_call_mode mode) const {
  napi_status status = napi_call_threadsafe_function(
      _tsfn, callbackWrapper, mode);
  if (status != napi_ok && callbackWrapper != nullptr) {
    delete callbackWrapper;
  }

  return status;
}

// static
inline void ThreadSafeFunction::CallJS(napi_env env,
                                       napi_value jsCallback,
                                       void* /* context */,
                                       void* data) {
  if (env == nullptr && jsCallback == nullptr) {
    return;
  }

  if (data != nullptr) {
    auto* callbackWrapper = static_cast<CallbackWrapper*>(data);
    (*callbackWrapper)(env, Function(env, jsCallback));
    delete callbackWrapper;
  } else if (jsCallback != nullptr) {
    Function(env, jsCallback).Call({});
  }
}

////////////////////////////////////////////////////////////////////////////////
// Async Progress Worker Base class
////////////////////////////////////////////////////////////////////////////////
template <typename DataType>
inline AsyncProgressWorkerBase<DataType>::AsyncProgressWorkerBase(const Object& receiver,
                                                                  const Function& callback,
                                                                  const char* resource_name,
                                                                  const Object& resource,
                                                                  size_t queue_size)
  : AsyncWorker(receiver, callback, resource_name, resource) {
  // Fill all possible arguments to work around ambiguous ThreadSafeFunction::New signatures.
  _tsfn = ThreadSafeFunction::New(callback.Env(),
                                  callback,
                                  resource,
                                  resource_name,
                                  queue_size,
                                  /** initialThreadCount */ 1,
                                  /** context */ this,
                                  OnThreadSafeFunctionFinalize,
                                  /** finalizeData */ this);
}

#if NAPI_VERSION > 4
template <typename DataType>
inline AsyncProgressWorkerBase<DataType>::AsyncProgressWorkerBase(Napi::Env env,
                                                                  const char* resource_name,
                                                                  const Object& resource,
                                                                  size_t queue_size)
  : AsyncWorker(env, resource_name, resource) {
  // TODO: Once the changes to make the callback optional for threadsafe
  // functions are available on all versions we can remove the dummy Function here.
  Function callback;
  // Fill all possible arguments to work around ambiguous ThreadSafeFunction::New signatures.
  _tsfn = ThreadSafeFunction::New(env,
                                  callback,
                                  resource,
                                  resource_name,
                                  queue_size,
                                  /** initialThreadCount */ 1,
                                  /** context */ this,
                                  OnThreadSafeFunctionFinalize,
                                  /** finalizeData */ this);
}
#endif

template<typename DataType>
inline AsyncProgressWorkerBase<DataType>::~AsyncProgressWorkerBase() {
  // Abort pending tsfn call.
  // Don't send progress events after we've already completed.
  // It's ok to call ThreadSafeFunction::Abort and ThreadSafeFunction::Release duplicated.
  _tsfn.Abort();
}

template <typename DataType>
inline void AsyncProgressWorkerBase<DataType>::OnAsyncWorkProgress(Napi::Env /* env */,
                                Napi::Function /* jsCallback */,
                                void* data) {
  ThreadSafeData* tsd = static_cast<ThreadSafeData*>(data);
  tsd->asyncprogressworker()->OnWorkProgress(tsd->data());
  delete tsd;
}

template <typename DataType>
inline napi_status AsyncProgressWorkerBase<DataType>::NonBlockingCall(DataType* data) {
  auto tsd = new AsyncProgressWorkerBase::ThreadSafeData(this, data);
  return _tsfn.NonBlockingCall(tsd, OnAsyncWorkProgress);
}

template <typename DataType>
inline void AsyncProgressWorkerBase<DataType>::OnWorkComplete(Napi::Env /* env */, napi_status status) {
  _work_completed = true;
  _complete_status = status;
  _tsfn.Release();
}

template <typename DataType>
inline void AsyncProgressWorkerBase<DataType>::OnThreadSafeFunctionFinalize(Napi::Env env, void* /* data */, AsyncProgressWorkerBase* context) {
  if (context->_work_completed) {
    context->AsyncWorker::OnWorkComplete(env, context->_complete_status);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Async Progress Worker class
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline AsyncProgressWorker<T>::AsyncProgressWorker(const Function& callback)
  : AsyncProgressWorker(callback, "generic") {
}

template<class T>
inline AsyncProgressWorker<T>::AsyncProgressWorker(const Function& callback,
                                const char* resource_name)
  : AsyncProgressWorker(callback, resource_name, Object::New(callback.Env())) {
}

template<class T>
inline AsyncProgressWorker<T>::AsyncProgressWorker(const Function& callback,
                                const char* resource_name,
                                const Object& resource)
  : AsyncProgressWorker(Object::New(callback.Env()),
                callback,
                resource_name,
                resource) {
}

template<class T>
inline AsyncProgressWorker<T>::AsyncProgressWorker(const Object& receiver,
                                                   const Function& callback)
  : AsyncProgressWorker(receiver, callback, "generic") {
}

template<class T>
inline AsyncProgressWorker<T>::AsyncProgressWorker(const Object& receiver,
                                                   const Function& callback,
                                                   const char* resource_name)
  : AsyncProgressWorker(receiver,
                callback,
                resource_name,
                Object::New(callback.Env())) {
}

template<class T>
inline AsyncProgressWorker<T>::AsyncProgressWorker(const Object& receiver,
                                                   const Function& callback,
                                                   const char* resource_name,
                                                   const Object& resource)
  : AsyncProgressWorkerBase(receiver, callback, resource_name, resource),
    _asyncdata(nullptr),
    _asyncsize(0) {
}

#if NAPI_VERSION > 4
template<class T>
inline AsyncProgressWorker<T>::AsyncProgressWorker(Napi::Env env)
  : AsyncProgressWorker(env, "generic") {
}

template<class T>
inline AsyncProgressWorker<T>::AsyncProgressWorker(Napi::Env env,
                                                   const char* resource_name)
  : AsyncProgressWorker(env, resource_name, Object::New(env)) {
}

template<class T>
inline AsyncProgressWorker<T>::AsyncProgressWorker(Napi::Env env,
                                                   const char* resource_name,
                                                   const Object& resource)
  : AsyncProgressWorkerBase(env, resource_name, resource),
    _asyncdata(nullptr),
    _asyncsize(0) {
}
#endif

template<class T>
inline AsyncProgressWorker<T>::~AsyncProgressWorker() {
  {
    std::lock_guard<std::mutex> lock(this->_mutex);
    _asyncdata = nullptr;
    _asyncsize = 0;
  }
}

template<class T>
inline void AsyncProgressWorker<T>::Execute() {
  ExecutionProgress progress(this);
  Execute(progress);
}

template<class T>
inline void AsyncProgressWorker<T>::OnWorkProgress(void*) {
  T* data;
  size_t size;
  {
    std::lock_guard<std::mutex> lock(this->_mutex);
    data = this->_asyncdata;
    size = this->_asyncsize;
    this->_asyncdata = nullptr;
    this->_asyncsize = 0;
  }

  /**
   * The callback of ThreadSafeFunction is not been invoked immediately on the
   * callback of uv_async_t (uv io poll), rather the callback of TSFN is
   * invoked on the right next uv idle callback. There are chances that during
   * the deferring the signal of uv_async_t is been sent again, i.e. potential
   * not coalesced two calls of the TSFN callback.
   */
  if (data == nullptr) {
    return;
  }

  this->OnProgress(data, size);
  delete[] data;
}

template<class T>
inline void AsyncProgressWorker<T>::SendProgress_(const T* data, size_t count) {
    T* new_data = new T[count];
    std::copy(data, data + count, new_data);

    T* old_data;
    {
      std::lock_guard<std::mutex> lock(this->_mutex);
      old_data = _asyncdata;
      _asyncdata = new_data;
      _asyncsize = count;
    }
    this->NonBlockingCall(nullptr);

    delete[] old_data;
}

template<class T>
inline void AsyncProgressWorker<T>::Signal() const {
  this->NonBlockingCall(static_cast<T*>(nullptr));
}

template<class T>
inline void AsyncProgressWorker<T>::ExecutionProgress::Signal() const {
  _worker->Signal();
}

template<class T>
inline void AsyncProgressWorker<T>::ExecutionProgress::Send(const T* data, size_t count) const {
  _worker->SendProgress_(data, count);
}

////////////////////////////////////////////////////////////////////////////////
// Async Progress Queue Worker class
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline AsyncProgressQueueWorker<T>::AsyncProgressQueueWorker(const Function& callback)
  : AsyncProgressQueueWorker(callback, "generic") {
}

template<class T>
inline AsyncProgressQueueWorker<T>::AsyncProgressQueueWorker(const Function& callback,
                                                             const char* resource_name)
  : AsyncProgressQueueWorker(callback, resource_name, Object::New(callback.Env())) {
}

template<class T>
inline AsyncProgressQueueWorker<T>::AsyncProgressQueueWorker(const Function& callback,
                                                             const char* resource_name,
                                                             const Object& resource)
  : AsyncProgressQueueWorker(Object::New(callback.Env()),
                             callback,
                             resource_name,
                             resource) {
}

template<class T>
inline AsyncProgressQueueWorker<T>::AsyncProgressQueueWorker(const Object& receiver,
                                                             const Function& callback)
  : AsyncProgressQueueWorker(receiver, callback, "generic") {
}

template<class T>
inline AsyncProgressQueueWorker<T>::AsyncProgressQueueWorker(const Object& receiver,
                                                             const Function& callback,
                                                             const char* resource_name)
  : AsyncProgressQueueWorker(receiver,
                             callback,
                             resource_name,
                             Object::New(callback.Env())) {
}

template<class T>
inline AsyncProgressQueueWorker<T>::AsyncProgressQueueWorker(const Object& receiver,
                                                             const Function& callback,
                                                             const char* resource_name,
                                                             const Object& resource)
  : AsyncProgressWorkerBase<std::pair<T*, size_t>>(receiver, callback, resource_name, resource, /** unlimited queue size */0) {
}

#if NAPI_VERSION > 4
template<class T>
inline AsyncProgressQueueWorker<T>::AsyncProgressQueueWorker(Napi::Env env)
  : AsyncProgressQueueWorker(env, "generic") {
}

template<class T>
inline AsyncProgressQueueWorker<T>::AsyncProgressQueueWorker(Napi::Env env,
                                const char* resource_name)
  : AsyncProgressQueueWorker(env, resource_name, Object::New(env)) {
}

template<class T>
inline AsyncProgressQueueWorker<T>::AsyncProgressQueueWorker(Napi::Env env,
                                                             const char* resource_name,
                                                             const Object& resource)
  : AsyncProgressWorkerBase<std::pair<T*, size_t>>(env, resource_name, resource, /** unlimited queue size */0) {
}
#endif

template<class T>
inline void AsyncProgressQueueWorker<T>::Execute() {
  ExecutionProgress progress(this);
  Execute(progress);
}

template<class T>
inline void AsyncProgressQueueWorker<T>::OnWorkProgress(std::pair<T*, size_t>* datapair) {
  if (datapair == nullptr) {
    return;
  }

  T *data = datapair->first;
  size_t size = datapair->second;

  this->OnProgress(data, size);
  delete datapair;
  delete[] data;
}

template<class T>
inline void AsyncProgressQueueWorker<T>::SendProgress_(const T* data, size_t count) {
    T* new_data = new T[count];
    std::copy(data, data + count, new_data);

    auto pair = new std::pair<T*, size_t>(new_data, count);
    this->NonBlockingCall(pair);
}

template<class T>
inline void AsyncProgressQueueWorker<T>::Signal() const {
  this->NonBlockingCall(nullptr);
}

template<class T>
inline void AsyncProgressQueueWorker<T>::OnWorkComplete(Napi::Env env, napi_status status) {
  // Draining queued items in TSFN.
  AsyncProgressWorkerBase<std::pair<T*, size_t>>::OnWorkComplete(env, status);
}

template<class T>
inline void AsyncProgressQueueWorker<T>::ExecutionProgress::Signal() const {
  _worker->Signal();
}

template<class T>
inline void AsyncProgressQueueWorker<T>::ExecutionProgress::Send(const T* data, size_t count) const {
  _worker->SendProgress_(data, count);
}
#endif  // NAPI_VERSION > 3 && !defined(__wasm32__)

////////////////////////////////////////////////////////////////////////////////
// Memory Management class
////////////////////////////////////////////////////////////////////////////////

inline int64_t MemoryManagement::AdjustExternalMemory(Env env, int64_t change_in_bytes) {
  int64_t result;
  napi_status status = napi_adjust_external_memory(env, change_in_bytes, &result);
  NAPI_THROW_IF_FAILED(env, status, 0);
  return result;
}

////////////////////////////////////////////////////////////////////////////////
// Version Management class
////////////////////////////////////////////////////////////////////////////////

inline uint32_t VersionManagement::GetNapiVersion(Env env) {
  uint32_t result;
  napi_status status = napi_get_version(env, &result);
  NAPI_THROW_IF_FAILED(env, status, 0);
  return result;
}

inline const napi_node_version* VersionManagement::GetNodeVersion(Env env) {
  const napi_node_version* result;
  napi_status status = napi_get_node_version(env, &result);
  NAPI_THROW_IF_FAILED(env, status, 0);
  return result;
}

#if NAPI_VERSION > 5
////////////////////////////////////////////////////////////////////////////////
// Addon<T> class
////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline Object Addon<T>::Init(Env env, Object exports) {
  T* addon = new T(env, exports);
  env.SetInstanceData(addon);
  return addon->entry_point_;
}

template <typename T>
inline T* Addon<T>::Unwrap(Object wrapper) {
  return wrapper.Env().GetInstanceData<T>();
}

template <typename T>
inline void
Addon<T>::DefineAddon(Object exports,
                      const std::initializer_list<AddonProp>& props) {
  DefineProperties(exports, props);
  entry_point_ = exports;
}

template <typename T>
inline Napi::Object
Addon<T>::DefineProperties(Object object,
                           const std::initializer_list<AddonProp>& props) {
  const napi_property_descriptor* properties =
    reinterpret_cast<const napi_property_descriptor*>(props.begin());
  size_t size = props.size();
  napi_status status = napi_define_properties(object.Env(),
                                              object,
                                              size,
                                              properties);
  NAPI_THROW_IF_FAILED(object.Env(), status, object);
  for (size_t idx = 0; idx < size; idx++)
    T::AttachPropData(object.Env(), object, &properties[idx]);
  return object;
}
#endif  // NAPI_VERSION > 5
} // namespace Napi

// hack-sqlite
#endif // SRC_NAPI_INL_H_
#endif // SRC_NAPI_H_


/*
repo https://github.com/mapbox/node-sqlite3/tree/v5.0.2
committed 2021-02-15T14:42:52Z
*/


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/LICENSE
*/
/*
Copyright (c) MapBox
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.
- Neither the name "MapBox" nor the names of its contributors may be
  used to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/deps/common-sqlite.gypi
*/
/*
{
  'variables': {
      'sqlite_version%':'3340000',
      "toolset%":'',
  },
  'target_defaults': {
    'default_configuration': 'Release',
    'conditions': [
      [ 'toolset!=""', {
        'msbuild_toolset':'<(toolset)'
      }]
    ],
    'configurations': {
      'Debug': {
        'defines!': [
          'NDEBUG'
        ],
        'cflags_cc!': [
          '-O3',
          '-Os',
          '-DNDEBUG'
        ],
        'xcode_settings': {
          'OTHER_CPLUSPLUSFLAGS!': [
            '-O3',
            '-Os',
            '-DDEBUG'
          ],
          'GCC_OPTIMIZATION_LEVEL': '0',
          'GCC_GENERATE_DEBUGGING_SYMBOLS': 'YES'
        },
        'msvs_settings': {
          'VCCLCompilerTool': {
            'ExceptionHandling': 1, # /EHsc
          }
        }
      },
      'Release': {
        'defines': [
          'NDEBUG'
        ],
        'xcode_settings': {
          'OTHER_CPLUSPLUSFLAGS!': [
            '-Os',
            '-O2'
          ],
          'GCC_OPTIMIZATION_LEVEL': '3',
          'GCC_GENERATE_DEBUGGING_SYMBOLS': 'NO',
          'DEAD_CODE_STRIPPING': 'YES',
          'GCC_INLINES_ARE_PRIVATE_EXTERN': 'YES'
        },
        'msvs_settings': {
          'VCCLCompilerTool': {
            'ExceptionHandling': 1, # /EHsc
          }
        }
      }
    }
  }
}
*/


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/deps/sqlite3.gyp
*/
/*
{
  'includes': [ 'common-sqlite.gypi' ],

  'variables': {
    'sqlite_magic%': '',
  },

  'target_defaults': {
    'default_configuration': 'Release',
    'cflags':[
      '-std=c99'
    ],
    'configurations': {
      'Debug': {
        'defines': [ 'DEBUG', '_DEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 1, # static debug
          },
        },
      },
      'Release': {
        'defines': [ 'NDEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 0, # static release
          },
        },
      }
    },
    'msvs_settings': {
      'VCCLCompilerTool': {
      },
      'VCLibrarianTool': {
      },
      'VCLinkerTool': {
        'GenerateDebugInformation': 'true',
      },
    },
    'conditions': [
      ['OS == "win"', {
        'defines': [
          'WIN32'
        ],
      }]
    ],
  },

  'targets': [
    {
      'target_name': 'action_before_build',
      'type': 'none',
      'hard_dependency': 1,
      'actions': [
        {
          'action_name': 'unpack_sqlite_dep',
          'inputs': [
            './sqlite-autoconf-<@(sqlite_version).tar.gz'
          ],
          'outputs': [
            '<(SHARED_INTERMEDIATE_DIR)/sqlite-autoconf-<@(sqlite_version)/sqlite3.c'
          ],
          'action': ['<!(node -p "process.env.npm_config_python || \\"python\\"")','./extract.py','./sqlite-autoconf-<@(sqlite_version).tar.gz','<(SHARED_INTERMEDIATE_DIR)']
        }
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(SHARED_INTERMEDIATE_DIR)/sqlite-autoconf-<@(sqlite_version)/',
        ]
      },
    },
    {
      'target_name': 'sqlite3',
      'type': 'static_library',
      'include_dirs': [ '<(SHARED_INTERMEDIATE_DIR)/sqlite-autoconf-<@(sqlite_version)/' ],
      'dependencies': [
        'action_before_build'
      ],
      'sources': [
        '<(SHARED_INTERMEDIATE_DIR)/sqlite-autoconf-<@(sqlite_version)/sqlite3.c'
      ],
      'direct_dependent_settings': {
        'include_dirs': [ '<(SHARED_INTERMEDIATE_DIR)/sqlite-autoconf-<@(sqlite_version)/' ],
        'defines': [
          'SQLITE_THREADSAFE=1',
          'HAVE_USLEEP=1',
          'SQLITE_ENABLE_FTS3',
          'SQLITE_ENABLE_FTS4',
          'SQLITE_ENABLE_FTS5',
          'SQLITE_ENABLE_JSON1',
          'SQLITE_ENABLE_RTREE',
          'SQLITE_ENABLE_DBSTAT_VTAB=1'
        ],
      },
      'cflags_cc': [
          '-Wno-unused-value'
      ],
      'defines': [
        '_REENTRANT=1',
        'SQLITE_THREADSAFE=1',
        'HAVE_USLEEP=1',
        'SQLITE_ENABLE_FTS3',
        'SQLITE_ENABLE_FTS4',
        'SQLITE_ENABLE_FTS5',
        'SQLITE_ENABLE_JSON1',
        'SQLITE_ENABLE_RTREE',
        'SQLITE_ENABLE_DBSTAT_VTAB=1'
      ],
      'export_dependent_settings': [
        'action_before_build',
      ],
      'conditions': [
        ["sqlite_magic != ''", {
            'defines': [
              'SQLITE_FILE_HEADER="<(sqlite_magic)"'
            ]
        }]
      ],
    }
  ]
}
*/


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/threading.h
*/
#ifndef NODE_SQLITE3_SRC_THREADING_H
#define NODE_SQLITE3_SRC_THREADING_H


#ifdef _WIN32

// #include <windows.h>

    #define NODE_SQLITE3_MUTEX_t HANDLE mutex;

    #define NODE_SQLITE3_MUTEX_INIT mutex = CreateMutex(NULL, FALSE, NULL);

    #define NODE_SQLITE3_MUTEX_LOCK(m) WaitForSingleObject(*m, INFINITE);

    #define NODE_SQLITE3_MUTEX_UNLOCK(m) ReleaseMutex(*m);

    #define NODE_SQLITE3_MUTEX_DESTROY CloseHandle(mutex);

#elif defined(NODE_SQLITE3_BOOST_THREADING)

// #include <boost/thread/mutex.hpp>

    #define NODE_SQLITE3_MUTEX_t boost::mutex mutex;

    #define NODE_SQLITE3_MUTEX_INIT

    #define NODE_SQLITE3_MUTEX_LOCK(m) (*m).lock();

    #define NODE_SQLITE3_MUTEX_UNLOCK(m) (*m).unlock();

    #define NODE_SQLITE3_MUTEX_DESTROY mutex.unlock();

#else

    #define NODE_SQLITE3_MUTEX_t pthread_mutex_t mutex;

    #define NODE_SQLITE3_MUTEX_INIT pthread_mutex_init(&mutex,NULL);

    #define NODE_SQLITE3_MUTEX_LOCK(m) pthread_mutex_lock(m);

    #define NODE_SQLITE3_MUTEX_UNLOCK(m) pthread_mutex_unlock(m);

    #define NODE_SQLITE3_MUTEX_DESTROY pthread_mutex_destroy(&mutex);

#endif


#endif // NODE_SQLITE3_SRC_THREADING_H


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/macros.h
*/
#ifndef NODE_SQLITE3_SRC_MACROS_H
#define NODE_SQLITE3_SRC_MACROS_H

const char* sqlite_code_string(int code);
const char* sqlite_authorizer_string(int type);
// #include <vector>

// TODO: better way to work around StringConcat?
// #include <napi.h>
inline Napi::String StringConcat(Napi::Value str1, Napi::Value str2) {
  return Napi::String::New(str1.Env(), str1.As<Napi::String>().Utf8Value() +
                    str2.As<Napi::String>().Utf8Value() );
}

// A Napi substitute IsInt32()
inline bool OtherIsInt(Napi::Number source) {
    double orig_val = source.DoubleValue();
    double int_val = (double)source.Int32Value();
    if (orig_val == int_val) {
        return true;
    } else {
        return false;
    }
}

#define REQUIRE_ARGUMENTS(n)                                                   \
    if (info.Length() < (n)) {                                                 \
        Napi::TypeError::New(env, "Expected " #n "arguments").ThrowAsJavaScriptException(); \
        return env.Null(); \
    }


#define REQUIRE_ARGUMENT_EXTERNAL(i, var)                                      \
    if (info.Length() <= (i) || !info[i].IsExternal()) {                       \
        Napi::TypeError::New(env, "Argument " #i " invalid").ThrowAsJavaScriptException(); \
        return env.Null(); \
    }                                                                          \
    Napi::External var = info[i].As<Napi::External>();


#define REQUIRE_ARGUMENT_FUNCTION(i, var)                                      \
    if (info.Length() <= (i) || !info[i].IsFunction()) {                        \
        Napi::TypeError::New(env, "Argument " #i " must be a function").ThrowAsJavaScriptException(); \
        return env.Null(); \
    }                                                                          \
    Napi::Function var = info[i].As<Napi::Function>();


#define REQUIRE_ARGUMENT_STRING(i, var)                                        \
    if (info.Length() <= (i) || !info[i].IsString()) {                         \
        Napi::TypeError::New(env, "Argument " #i " must be a string").ThrowAsJavaScriptException(); \
        return env.Null(); \
    }                                                                          \
    std::string var = info[i].As<Napi::String>();

#define REQUIRE_ARGUMENT_INTEGER(i, var)                                        \
    if (info.Length() <= (i) || !info[i].IsNumber()) {                        \
        Napi::TypeError::New(env, "Argument " #i " must be an integer").ThrowAsJavaScriptException(); \
        return env.Null();        \
    }                                                                          \
    int var(info[i].As<Napi::Number>().Int32Value());

#define OPTIONAL_ARGUMENT_FUNCTION(i, var)                                     \
    Napi::Function var;                                                        \
    if (info.Length() > i && !info[i].IsUndefined()) {                         \
        if (!info[i].IsFunction()) {                                           \
            Napi::TypeError::New(env, "Argument " #i " must be a function").ThrowAsJavaScriptException(); \
            return env.Null(); \
        }                                                                      \
        var = info[i].As<Napi::Function>();                                    \
    }


#define OPTIONAL_ARGUMENT_INTEGER(i, var, default)                             \
    int var;                                                                   \
    if (info.Length() <= (i)) {                                                \
        var = (default);                                                       \
    }                                                                          \
    else if (info[i].IsNumber()) {                                             \
        if (OtherIsInt(info[i].As<Number>())) {                                \
            var = info[i].As<Napi::Number>().Int32Value();                     \
        }                                                                      \
    }                                                                          \
    else {                                                                     \
        Napi::TypeError::New(env, "Argument " #i " must be an integer").ThrowAsJavaScriptException(); \
        return env.Null(); \
    }


#define DEFINE_CONSTANT_INTEGER(target, constant, name)                        \
    Napi::PropertyDescriptor::Value(#name, Napi::Number::New(env, constant),   \
        static_cast<napi_property_attributes>(napi_enumerable | napi_configurable)),

#define DEFINE_CONSTANT_STRING(target, constant, name)                         \
    Napi::PropertyDescriptor::Value(#name, Napi::String::New(env, constant),   \
        static_cast<napi_property_attributes>(napi_enumerable | napi_configurable)),

#define EXCEPTION(msg, errno, name)                                            \
    Napi::Value name = Napi::Error::New(env,                                   \
        StringConcat(                                                          \
            StringConcat(                                                      \
                Napi::String::New(env, sqlite_code_string(errno)),             \
                Napi::String::New(env, ": ")                                   \
            ),                                                                 \
            (msg)                                                              \
        ).Utf8Value()                                                          \
    ).Value();                                                                 \
    Napi::Object name ##_obj = name.As<Napi::Object>();                        \
    (name ##_obj).Set( Napi::String::New(env, "errno"), Napi::Number::New(env, errno)); \
    (name ##_obj).Set( Napi::String::New(env, "code"),                         \
        Napi::String::New(env, sqlite_code_string(errno)));


#define EMIT_EVENT(obj, argc, argv)                                            \
    TRY_CATCH_CALL((obj),                                                      \
        (obj).Get("emit").As<Napi::Function>(),\
        argc, argv                                                             \
    );

// The Mac OS compiler complains when argv is NULL unless we
// first assign it to a locally defined variable.
#define TRY_CATCH_CALL(context, callback, argc, argv, ...)                     \
    Napi::Value* passed_argv = argv;\
    std::vector<napi_value> args;\
    if ((argc != 0) && (passed_argv != NULL)) {\
      args.assign(passed_argv, passed_argv + argc);\
    }\
    Napi::Value res = (callback).MakeCallback(Napi::Value(context), args);     \
    if (res.IsEmpty()) return __VA_ARGS__;

#define WORK_DEFINITION(name)                                                  \
    Napi::Value name(const Napi::CallbackInfo& info);                          \
    static void Work_Begin##name(Baton* baton);                                \
    static void Work_##name(napi_env env, void* data);                         \
    static void Work_After##name(napi_env env, napi_status status, void* data);

#define STATEMENT_BEGIN(type)                                                  \
    assert(baton);                                                             \
    assert(baton->stmt);                                                       \
    assert(!baton->stmt->locked);                                              \
    assert(!baton->stmt->finalized);                                           \
    assert(baton->stmt->prepared);                                             \
    baton->stmt->locked = true;                                                \
    baton->stmt->db->pending++;                                                \
    Napi::Env env = baton->stmt->Env();                                        \
    int status = napi_create_async_work(                                       \
        env, NULL, Napi::String::New(env, "sqlite3.Statement."#type),          \
        Work_##type, Work_After##type, baton, &baton->request                  \
    );                                                                         \
    assert(status == 0);                                                       \
    napi_queue_async_work(env, baton->request);

#define STATEMENT_INIT(type)                                                   \
    type* baton = static_cast<type*>(data);                                    \
    Statement* stmt = baton->stmt;

#define STATEMENT_MUTEX(name) \
    if (!stmt->db->_handle) { \
        stmt->status = SQLITE_MISUSE; \
        stmt->message = "Database handle is closed"; \
        return; \
    } \
    sqlite3_mutex* name = sqlite3_db_mutex(stmt->db->_handle);

#define STATEMENT_END()                                                        \
    assert(stmt->locked);                                                      \
    assert(stmt->db->pending);                                                 \
    stmt->locked = false;                                                      \
    stmt->db->pending--;                                                       \
    stmt->Process();                                                           \
    stmt->db->Process();

#define BACKUP_BEGIN(type)                                                     \
    assert(baton);                                                             \
    assert(baton->backup);                                                     \
    assert(!baton->backup->locked);                                            \
    assert(!baton->backup->finished);                                          \
    assert(baton->backup->inited);                                             \
    baton->backup->locked = true;                                              \
    baton->backup->db->pending++;                                              \
    Napi::Env env = baton->backup->Env();                                      \
    int status = napi_create_async_work(                                       \
        env, NULL, Napi::String::New(env, "sqlite3.Backup."#type),             \
        Work_##type, Work_After##type, baton, &baton->request                  \
    );                                                                         \
    assert(status == 0);                                                       \
    napi_queue_async_work(env, baton->request);

#define BACKUP_INIT(type)                                                      \
    type* baton = static_cast<type*>(data);                                    \
    Backup* backup = baton->backup;

#define BACKUP_END()                                                           \
    assert(backup->locked);                                                    \
    assert(backup->db->pending);                                               \
    backup->locked = false;                                                    \
    backup->db->pending--;                                                     \
    backup->Process();                                                         \
    backup->db->Process();

#define DELETE_FIELD(field)                                                    \
    if (field != NULL) {                                                       \
        switch ((field)->type) {                                               \
            case SQLITE_INTEGER: delete (Values::Integer*)(field); break;      \
            case SQLITE_FLOAT:   delete (Values::Float*)(field); break;        \
            case SQLITE_TEXT:    delete (Values::Text*)(field); break;         \
            case SQLITE_BLOB:    delete (Values::Blob*)(field); break;         \
            case SQLITE_NULL:    delete (Values::Null*)(field); break;         \
        }                                                                      \
    }

#endif


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/async.h
*/
#ifndef NODE_SQLITE3_SRC_ASYNC_H
#define NODE_SQLITE3_SRC_ASYNC_H

// #include <napi.h>
// #include <uv.h>

// #include "threading.h"

#if defined(NODE_SQLITE3_BOOST_THREADING)
// #include <boost/thread/mutex.hpp>
#endif


// Generic uv_async handler.
template <class Item, class Parent> class Async {
    typedef void (*Callback)(Parent* parent, Item* item);

protected:
    uv_async_t watcher;
    NODE_SQLITE3_MUTEX_t
    std::vector<Item*> data;
    Callback callback;
public:
    Parent* parent;

public:
    Async(Parent* parent_, Callback cb_)
        : callback(cb_), parent(parent_) {
        watcher.data = this;
        NODE_SQLITE3_MUTEX_INIT
        uv_loop_t *loop;
        napi_get_uv_event_loop(parent_->Env(), &loop);
        uv_async_init(loop, &watcher, reinterpret_cast<uv_async_cb>(listener));
    }

    static void listener(uv_async_t* handle) {
        Async* async = static_cast<Async*>(handle->data);
        std::vector<Item*> rows;
        NODE_SQLITE3_MUTEX_LOCK(&async->mutex)
        rows.swap(async->data);
        NODE_SQLITE3_MUTEX_UNLOCK(&async->mutex)
        for (unsigned int i = 0, size = rows.size(); i < size; i++) {
            async->callback(async->parent, rows[i]);
        }
    }

    static void close(uv_handle_t* handle) {
        assert(handle != NULL);
        assert(handle->data != NULL);
        Async* async = static_cast<Async*>(handle->data);
        delete async;
    }

    void finish() {
        // Need to call the listener again to ensure all items have been
        // processed. Is this a bug in uv_async? Feels like uv_close
        // should handle that.
        listener(&watcher);
        uv_close((uv_handle_t*)&watcher, close);
    }

    void add(Item* item) {
        NODE_SQLITE3_MUTEX_LOCK(&mutex);
        data.push_back(item);
        NODE_SQLITE3_MUTEX_UNLOCK(&mutex)
    }

    void send() {
        uv_async_send(&watcher);
    }

    void send(Item* item) {
        add(item);
        send();
    }

    ~Async() {
        NODE_SQLITE3_MUTEX_DESTROY
    }
};

#endif


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/database.h
*/
#ifndef NODE_SQLITE3_SRC_DATABASE_H
#define NODE_SQLITE3_SRC_DATABASE_H


// #include <assert.h>
// #include <string>
// #include <queue>

// #include <sqlite3.h>
// #include <napi.h>

// #include "async.h"

using namespace Napi;

namespace node_sqlite3 {

class Database;


class Database : public Napi::ObjectWrap<Database> {
public:
#if NAPI_VERSION < 6
    static Napi::FunctionReference constructor;
#endif
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    static inline bool HasInstance(Napi::Value val) {
        Napi::Env env = val.Env();
        Napi::HandleScope scope(env);
        if (!val.IsObject()) return false;
        Napi::Object obj = val.As<Napi::Object>();
#if NAPI_VERSION < 6
        return obj.InstanceOf(constructor.Value());
#else
        Napi::FunctionReference* constructor =
            env.GetInstanceData<Napi::FunctionReference>();
        return obj.InstanceOf(constructor->Value());
#endif
    }

    struct Baton {
        napi_async_work request = NULL;
        Database* db;
        Napi::FunctionReference callback;
        int status;
        std::string message;

        Baton(Database* db_, Napi::Function cb_) :
                db(db_), status(SQLITE_OK) {
            db->Ref();
            if (!cb_.IsUndefined() && cb_.IsFunction()) {
                callback.Reset(cb_, 1);
            }
        }
        virtual ~Baton() {
            if (request) napi_delete_async_work(db->Env(), request);
            db->Unref();
            callback.Reset();
        }
    };

    struct OpenBaton : Baton {
        std::string filename;
        int mode;
        OpenBaton(Database* db_, Napi::Function cb_, const char* filename_, int mode_) :
            Baton(db_, cb_), filename(filename_), mode(mode_) {}
    };

    struct ExecBaton : Baton {
        std::string sql;
        ExecBaton(Database* db_, Napi::Function cb_, const char* sql_) :
            Baton(db_, cb_), sql(sql_) {}
    };

    struct LoadExtensionBaton : Baton {
        std::string filename;
        LoadExtensionBaton(Database* db_, Napi::Function cb_, const char* filename_) :
            Baton(db_, cb_), filename(filename_) {}
    };

    typedef void (*Work_Callback)(Baton* baton);

    struct Call {
        Call(Work_Callback cb_, Baton* baton_, bool exclusive_ = false) :
            callback(cb_), exclusive(exclusive_), baton(baton_) {};
        Work_Callback callback;
        bool exclusive;
        Baton* baton;
    };

    struct ProfileInfo {
        std::string sql;
        sqlite3_int64 nsecs;
    };

    struct UpdateInfo {
        int type;
        std::string database;
        std::string table;
        sqlite3_int64 rowid;
    };

    bool IsOpen() { return open; }
    bool IsLocked() { return locked; }

    typedef Async<std::string, Database> AsyncTrace;
    typedef Async<ProfileInfo, Database> AsyncProfile;
    typedef Async<UpdateInfo, Database> AsyncUpdate;

    friend class Statement;
    friend class Backup;

    void init() {
        _handle = NULL;
        open = false;
        closing = false;
        locked = false;
        pending = 0;
        serialize = false;
        debug_trace = NULL;
        debug_profile = NULL;
        update_event = NULL;
    }

    Database(const Napi::CallbackInfo& info);

    ~Database() {
        RemoveCallbacks();
        sqlite3_close(_handle);
        _handle = NULL;
        open = false;
    }

protected:
    static void Work_BeginOpen(Baton* baton);
    static void Work_Open(napi_env env, void* data);
    static void Work_AfterOpen(napi_env env, napi_status status, void* data);

    Napi::Value OpenGetter(const Napi::CallbackInfo& info);

    void Schedule(Work_Callback callback, Baton* baton, bool exclusive = false);
    void Process();

    Napi::Value Exec(const Napi::CallbackInfo& info);
    static void Work_BeginExec(Baton* baton);
    static void Work_Exec(napi_env env, void* data);
    static void Work_AfterExec(napi_env env, napi_status status, void* data);

    Napi::Value Wait(const Napi::CallbackInfo& info);
    static void Work_Wait(Baton* baton);

    Napi::Value Close(const Napi::CallbackInfo& info);
    static void Work_BeginClose(Baton* baton);
    static void Work_Close(napi_env env, void* data);
    static void Work_AfterClose(napi_env env, napi_status status, void* data);

    Napi::Value LoadExtension(const Napi::CallbackInfo& info);
    static void Work_BeginLoadExtension(Baton* baton);
    static void Work_LoadExtension(napi_env env, void* data);
    static void Work_AfterLoadExtension(napi_env env, napi_status status, void* data);

    Napi::Value Serialize(const Napi::CallbackInfo& info);
    Napi::Value Parallelize(const Napi::CallbackInfo& info);

    Napi::Value Configure(const Napi::CallbackInfo& info);

    Napi::Value Interrupt(const Napi::CallbackInfo& info);

    static void SetBusyTimeout(Baton* baton);

    static void RegisterTraceCallback(Baton* baton);
    static void TraceCallback(void* db, const char* sql);
    static void TraceCallback(Database* db, std::string* sql);

    static void RegisterProfileCallback(Baton* baton);
    static void ProfileCallback(void* db, const char* sql, sqlite3_uint64 nsecs);
    static void ProfileCallback(Database* db, ProfileInfo* info);

    static void RegisterUpdateCallback(Baton* baton);
    static void UpdateCallback(void* db, int type, const char* database, const char* table, sqlite3_int64 rowid);
    static void UpdateCallback(Database* db, UpdateInfo* info);

    void RemoveCallbacks();

protected:
    sqlite3* _handle;

    bool open;
    bool closing;
    bool locked;
    unsigned int pending;

    bool serialize;

    std::queue<Call*> queue;

    AsyncTrace* debug_trace;
    AsyncProfile* debug_profile;
    AsyncUpdate* update_event;
};
}

#endif


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/backup.h
*/
#ifndef NODE_SQLITE3_SRC_BACKUP_H
#define NODE_SQLITE3_SRC_BACKUP_H

// #include "database.h"

// #include <string>
// #include <queue>
// #include <set>

// #include <sqlite3.h>
// #include <napi.h>

using namespace Napi;

namespace node_sqlite3 {

/**
 *
 * A class for managing an sqlite3_backup object.  For consistency
 * with other node-sqlite3 classes, it maintains an internal queue
 * of calls.
 *
 * Intended usage from node:
 *
 *   var db = new sqlite3.Database('live.db');
 *   var backup = db.backup('backup.db');
 *   ...
 *   // in event loop, move backup forward when we have time.
 *   if (backup.idle) { backup.step(NPAGES); }
 *   if (backup.completed) { ... success ... }
 *   if (backup.failed)    { ... sadness ... }
 *   // do other work in event loop - fine to modify live.db
 *   ...
 *
 * Here is how sqlite's backup api is exposed:
 *
 *   - `sqlite3_backup_init`: This is implemented as
 *     `db.backup(filename, [callback])` or
 *     `db.backup(filename, destDbName, sourceDbName, filenameIsDest, [callback])`.
 *   - `sqlite3_backup_step`: `backup.step(pages, [callback])`.
 *   - `sqlite3_backup_finish`: `backup.finish([callback])`.
 *   - `sqlite3_backup_remaining`: `backup.remaining`.
 *   - `sqlite3_backup_pagecount`: `backup.pageCount`.
 *
 * There are the following read-only properties:
 *
 *   - `backup.completed` is set to `true` when the backup
 *     succeeeds.
 *   - `backup.failed` is set to `true` when the backup
 *     has a fatal error.
 *   - `backup.idle` is set to `true` when no operation
 *     is currently in progress or queued for the backup.
 *   - `backup.remaining` is an integer with the remaining
 *     number of pages after the last call to `backup.step`
 *     (-1 if `step` not yet called).
 *   - `backup.pageCount` is an integer with the total number
 *     of pages measured during the last call to `backup.step`
 *     (-1 if `step` not yet called).
 *
 * There is the following writable property:
 *
 *   - `backup.retryErrors`: an array of sqlite3 error codes
 *     that are treated as non-fatal - meaning, if they occur,
 *     backup.failed is not set, and the backup may continue.
 *     By default, this is `[sqlite3.BUSY, sqlite3.LOCKED]`.
 *
 * The `db.backup(filename, [callback])` shorthand is sufficient
 * for making a backup of a database opened by node-sqlite3.  If
 * using attached or temporary databases, or moving data in the
 * opposite direction, the more complete (but daunting)
 * `db.backup(filename, destDbName, sourceDbName, filenameIsDest, [callback])`
 * signature is provided.
 *
 * A backup will finish automatically when it succeeds or a fatal
 * error occurs, meaning it is not necessary to call `db.finish()`.
 * By default, SQLITE_LOCKED and SQLITE_BUSY errors are not
 * treated as failures, and the backup will continue if they
 * occur.  The set of errors that are tolerated can be controlled
 * by setting `backup.retryErrors`. To disable automatic
 * finishing and stick strictly to sqlite's raw api, set
 * `backup.retryErrors` to `[]`.  In that case, it is necessary
 * to call `backup.finish()`.
 *
 * In the same way as node-sqlite3 databases and statements,
 * backup methods can be called safely without callbacks, due
 * to an internal call queue.  So for example this naive code
 * will correctly back up a db, if there are no errors:
 *
 *   var backup = db.backup('backup.db');
 *   backup.step(-1);
 *   backup.finish();
 *
 */
class Backup : public Napi::ObjectWrap<Backup> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    struct Baton {
        napi_async_work request = NULL;
        Backup* backup;
        Napi::FunctionReference callback;

        Baton(Backup* backup_, Napi::Function cb_) : backup(backup_) {
            backup->Ref();
            callback.Reset(cb_, 1);
        }
        virtual ~Baton() {
            if (request) napi_delete_async_work(backup->Env(), request);
            backup->Unref();
            callback.Reset();
        }
    };

    struct InitializeBaton : Database::Baton {
        Backup* backup;
        std::string filename;
        std::string sourceName;
        std::string destName;
        bool filenameIsDest;
        InitializeBaton(Database* db_, Napi::Function cb_, Backup* backup_) :
            Baton(db_, cb_), backup(backup_), filenameIsDest(true) {
            backup->Ref();
        }
        virtual ~InitializeBaton() {
            backup->Unref();
            if (!db->IsOpen() && db->IsLocked()) {
                // The database handle was closed before the backup could be opened.
                backup->FinishAll();
            }
        }
    };

    struct StepBaton : Baton {
        int pages;
        std::set<int> retryErrorsSet;
        StepBaton(Backup* backup_, Napi::Function cb_, int pages_) :
            Baton(backup_, cb_), pages(pages_) {}
    };

    typedef void (*Work_Callback)(Baton* baton);

    struct Call {
        Call(Work_Callback cb_, Baton* baton_) : callback(cb_), baton(baton_) {};
        Work_Callback callback;
        Baton* baton;
    };

    void init(Database* db_) {
        db = db_;
        _handle = NULL;
        _otherDb = NULL;
        _destDb = NULL;
        inited = false;
        locked = true;
        completed = false;
        failed = false;
        remaining = -1;
        pageCount = -1;
        finished = false;
        db->Ref();
    }

    Backup(const Napi::CallbackInfo& info);

    ~Backup() {
        if (!finished) {
            FinishAll();
        }
        retryErrors.Reset();
    }

    WORK_DEFINITION(Step);
    WORK_DEFINITION(Finish);
    Napi::Value IdleGetter(const Napi::CallbackInfo& info);
    Napi::Value CompletedGetter(const Napi::CallbackInfo& info);
    Napi::Value FailedGetter(const Napi::CallbackInfo& info);
    Napi::Value PageCountGetter(const Napi::CallbackInfo& info);
    Napi::Value RemainingGetter(const Napi::CallbackInfo& info);
    Napi::Value FatalErrorGetter(const Napi::CallbackInfo& info);
    Napi::Value RetryErrorGetter(const Napi::CallbackInfo& info);

    void FatalErrorSetter(const Napi::CallbackInfo& info, const Napi::Value& value);
    void RetryErrorSetter(const Napi::CallbackInfo& info, const Napi::Value& value);

protected:
    static void Work_BeginInitialize(Database::Baton* baton);
    static void Work_Initialize(napi_env env, void* data);
    static void Work_AfterInitialize(napi_env env, napi_status status, void* data);

    void Schedule(Work_Callback callback, Baton* baton);
    void Process();
    void CleanQueue();
    template <class T> static void Error(T* baton);

    void FinishAll();
    void FinishSqlite();
    void GetRetryErrors(std::set<int>& retryErrorsSet);

    Database* db;

    sqlite3_backup* _handle;
    sqlite3* _otherDb;
    sqlite3* _destDb;
    int status;
    std::string message;

    bool inited;
    bool locked;
    bool completed;
    bool failed;
    int remaining;
    int pageCount;
    bool finished;
    std::queue<Call*> queue;

    Napi::Reference<Array> retryErrors;
};
}

#endif


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/statement.h
*/
#ifndef NODE_SQLITE3_SRC_STATEMENT_H
#define NODE_SQLITE3_SRC_STATEMENT_H

// #include <cstdlib>
// #include <cstring>
// #include <string>
// #include <queue>
// #include <vector>

// #include <sqlite3.h>
// #include <napi.h>
// #include <uv.h>

// #include "database.h"
// #include "threading.h"

using namespace Napi;

namespace node_sqlite3 {

namespace Values {
    struct Field {
        inline Field(unsigned short _index, unsigned short _type = SQLITE_NULL) :
            type(_type), index(_index) {}
        inline Field(const char* _name, unsigned short _type = SQLITE_NULL) :
            type(_type), index(0), name(_name) {}

        unsigned short type;
        unsigned short index;
        std::string name;
    };

    struct Integer : Field {
        template <class T> inline Integer(T _name, int64_t val) :
            Field(_name, SQLITE_INTEGER), value(val) {}
        int64_t value;
    };

    struct Float : Field {
        template <class T> inline Float(T _name, double val) :
            Field(_name, SQLITE_FLOAT), value(val) {}
        double value;
    };

    struct Text : Field {
        template <class T> inline Text(T _name, size_t len, const char* val) :
            Field(_name, SQLITE_TEXT), value(val, len) {}
        std::string value;
    };

    struct Blob : Field {
        template <class T> inline Blob(T _name, size_t len, const void* val) :
                Field(_name, SQLITE_BLOB), length(len) {
            value = (char*)malloc(len);
            memcpy(value, val, len);
        }
        inline ~Blob() {
            free(value);
        }
        int length;
        char* value;
    };

    typedef Field Null;
}

typedef std::vector<Values::Field*> Row;
typedef std::vector<Row*> Rows;
typedef Row Parameters;


class Statement : public Napi::ObjectWrap<Statement> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Value New(const Napi::CallbackInfo& info);

    struct Baton {
        napi_async_work request = NULL;
        Statement* stmt;
        Napi::FunctionReference callback;
        Parameters parameters;

        Baton(Statement* stmt_, Napi::Function cb_) : stmt(stmt_) {
            stmt->Ref();
            callback.Reset(cb_, 1);
        }
        virtual ~Baton() {
            for (unsigned int i = 0; i < parameters.size(); i++) {
                Values::Field* field = parameters[i];
                DELETE_FIELD(field);
            }
            if (request) napi_delete_async_work(stmt->Env(), request);
            stmt->Unref();
            callback.Reset();
        }
    };

    struct RowBaton : Baton {
        RowBaton(Statement* stmt_, Napi::Function cb_) :
            Baton(stmt_, cb_) {}
        Row row;
    };

    struct RunBaton : Baton {
        RunBaton(Statement* stmt_, Napi::Function cb_) :
            Baton(stmt_, cb_), inserted_id(0), changes(0) {}
        sqlite3_int64 inserted_id;
        int changes;
    };

    struct RowsBaton : Baton {
        RowsBaton(Statement* stmt_, Napi::Function cb_) :
            Baton(stmt_, cb_) {}
        Rows rows;
    };

    struct Async;

    struct EachBaton : Baton {
        Napi::FunctionReference completed;
        Async* async; // Isn't deleted when the baton is deleted.

        EachBaton(Statement* stmt_, Napi::Function cb_) :
            Baton(stmt_, cb_) {}
        virtual ~EachBaton() {
            completed.Reset();
        }
    };

    struct PrepareBaton : Database::Baton {
        Statement* stmt;
        std::string sql;
        PrepareBaton(Database* db_, Napi::Function cb_, Statement* stmt_) :
            Baton(db_, cb_), stmt(stmt_) {
            stmt->Ref();
        }
        virtual ~PrepareBaton() {
            stmt->Unref();
            if (!db->IsOpen() && db->IsLocked()) {
                // The database handle was closed before the statement could be
                // prepared.
                stmt->Finalize_();
            }
        }
    };

    typedef void (*Work_Callback)(Baton* baton);

    struct Call {
        Call(Work_Callback cb_, Baton* baton_) : callback(cb_), baton(baton_) {};
        Work_Callback callback;
        Baton* baton;
    };

    struct Async {
        uv_async_t watcher;
        Statement* stmt;
        Rows data;
        NODE_SQLITE3_MUTEX_t;
        bool completed;
        int retrieved;

        // Store the callbacks here because we don't have
        // access to the baton in the async callback.
        Napi::FunctionReference item_cb;
        Napi::FunctionReference completed_cb;

        Async(Statement* st, uv_async_cb async_cb) :
                stmt(st), completed(false), retrieved(0) {
            watcher.data = this;
            NODE_SQLITE3_MUTEX_INIT
            stmt->Ref();
            uv_loop_t *loop;
            napi_get_uv_event_loop(stmt->Env(), &loop);
            uv_async_init(loop, &watcher, async_cb);
        }

        ~Async() {
            stmt->Unref();
            item_cb.Reset();
            completed_cb.Reset();
            NODE_SQLITE3_MUTEX_DESTROY
        }
    };

    void init(Database* db_) {
        db = db_;
        _handle = NULL;
        status = SQLITE_OK;
        prepared = false;
        locked = true;
        finalized = false;
        db->Ref();
    }

    Statement(const Napi::CallbackInfo& info);

    ~Statement() {
        if (!finalized) Finalize_();
    }

    WORK_DEFINITION(Bind);
    WORK_DEFINITION(Get);
    WORK_DEFINITION(Run);
    WORK_DEFINITION(All);
    WORK_DEFINITION(Each);
    WORK_DEFINITION(Reset);

    Napi::Value Finalize_(const Napi::CallbackInfo& info);

protected:
    static void Work_BeginPrepare(Database::Baton* baton);
    static void Work_Prepare(napi_env env, void* data);
    static void Work_AfterPrepare(napi_env env, napi_status status, void* data);

    static void AsyncEach(uv_async_t* handle);
    static void CloseCallback(uv_handle_t* handle);

    static void Finalize_(Baton* baton);
    void Finalize_();

    template <class T> inline Values::Field* BindParameter(const Napi::Value source, T pos);
    template <class T> T* Bind(const Napi::CallbackInfo& info, int start = 0, int end = -1);
    bool Bind(const Parameters &parameters);

    static void GetRow(Row* row, sqlite3_stmt* stmt);
    static Napi::Value RowToJS(Napi::Env env, Row* row);
    void Schedule(Work_Callback callback, Baton* baton);
    void Process();
    void CleanQueue();
    template <class T> static void Error(T* baton);

protected:
    Database* db;

    sqlite3_stmt* _handle;
    int status;
    std::string message;

    bool prepared;
    bool locked;
    bool finalized;
    std::queue<Call*> queue;
};
}

#endif


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/database.cc
*/
// #include <string.h>
// #include <napi.h>

// #include "macros.h"
// #include "database.h"
// #include "statement.h"

using namespace node_sqlite3;

#if NAPI_VERSION < 6
Napi::FunctionReference Database::constructor;
#endif

Napi::Object Database::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function t = DefineClass(env, "Database", {
        InstanceMethod("close", &Database::Close),
        InstanceMethod("exec", &Database::Exec),
        InstanceMethod("wait", &Database::Wait),
        InstanceMethod("loadExtension", &Database::LoadExtension),
        InstanceMethod("serialize", &Database::Serialize),
        InstanceMethod("parallelize", &Database::Parallelize),
        InstanceMethod("configure", &Database::Configure),
        InstanceMethod("interrupt", &Database::Interrupt),
        InstanceAccessor("open", &Database::OpenGetter, nullptr)
    });

#if NAPI_VERSION < 6
    constructor = Napi::Persistent(t);
    constructor.SuppressDestruct();
#else
    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(t);
    env.SetInstanceData<Napi::FunctionReference>(constructor);
#endif

    exports.Set("Database", t);
    return exports;
}

void Database::Process() {
    Napi::Env env = this->Env();
    Napi::HandleScope scope(env);

    if (!open && locked && !queue.empty()) {
        EXCEPTION(Napi::String::New(env, "Database handle is closed"), SQLITE_MISUSE, exception);
        Napi::Value argv[] = { exception };
        bool called = false;

        // Call all callbacks with the error object.
        while (!queue.empty()) {
            std::unique_ptr<Call> call(queue.front());
            queue.pop();
            std::unique_ptr<Baton> baton(call->baton);
            Napi::Function cb = baton->callback.Value();
            if (!cb.IsUndefined() && cb.IsFunction()) {
                TRY_CATCH_CALL(this->Value(), cb, 1, argv);
                called = true;
            }
        }

        // When we couldn't call a callback function, emit an error on the
        // Database object.
        if (!called) {
            Napi::Value info[] = { Napi::String::New(env, "error"), exception };
            EMIT_EVENT(Value(), 2, info);
        }
        return;
    }

    while (open && (!locked || pending == 0) && !queue.empty()) {
        Call *c = queue.front();

        if (c->exclusive && pending > 0) {
            break;
        }

        queue.pop();
        std::unique_ptr<Call> call(c);
        locked = call->exclusive;
        call->callback(call->baton);

        if (locked) break;
    }
}

void Database::Schedule(Work_Callback callback, Baton* baton, bool exclusive) {
    Napi::Env env = this->Env();
    Napi::HandleScope scope(env);

    if (!open && locked) {
        EXCEPTION(Napi::String::New(env, "Database is closed"), SQLITE_MISUSE, exception);
        Napi::Function cb = baton->callback.Value();
        // We don't call the actual callback, so we have to make sure that
        // the baton gets destroyed.
        delete baton;
        if (!cb.IsUndefined() && cb.IsFunction()) {
            Napi::Value argv[] = { exception };
            TRY_CATCH_CALL(Value(), cb, 1, argv);
        }
        else {
            Napi::Value argv[] = { Napi::String::New(env, "error"), exception };
            EMIT_EVENT(Value(), 2, argv);
        }
        return;
    }

    if (!open || ((locked || exclusive || serialize) && pending > 0)) {
        queue.push(new Call(callback, baton, exclusive || serialize));
    }
    else {
        locked = exclusive;
        callback(baton);
    }
}

Database::Database(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Database>(info) {
    init();
    Napi::Env env = info.Env();

    if (info.Length() <= 0 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return;
    }
    std::string filename = info[0].As<Napi::String>();

    unsigned int pos = 1;

    int mode;
    if (info.Length() >= pos && info[pos].IsNumber() && OtherIsInt(info[pos].As<Napi::Number>())) {
        mode = info[pos++].As<Napi::Number>().Int32Value();
    }
    else {
        mode = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX;
    }

    Napi::Function callback;
    if (info.Length() >= pos && info[pos].IsFunction()) {
        callback = info[pos++].As<Napi::Function>();
    }

    info.This().As<Napi::Object>().DefineProperty(Napi::PropertyDescriptor::Value("filename", info[0].As<Napi::String>(), napi_default));
    info.This().As<Napi::Object>().DefineProperty(Napi::PropertyDescriptor::Value("mode", Napi::Number::New(env, mode), napi_default));

    // Start opening the database.
    OpenBaton* baton = new OpenBaton(this, callback, filename.c_str(), mode);
    Work_BeginOpen(baton);
}

void Database::Work_BeginOpen(Baton* baton) {
    Napi::Env env = baton->db->Env();
    int status = napi_create_async_work(
        env, NULL, Napi::String::New(env, "sqlite3.Database.Open"),
        Work_Open, Work_AfterOpen, baton, &baton->request
    );
    assert(status == 0);
    napi_queue_async_work(env, baton->request);
}

void Database::Work_Open(napi_env e, void* data) {
    OpenBaton* baton = static_cast<OpenBaton*>(data);
    Database* db = baton->db;

    baton->status = sqlite3_open_v2(
        baton->filename.c_str(),
        &db->_handle,
        baton->mode,
        NULL
    );

    if (baton->status != SQLITE_OK) {
        baton->message = std::string(sqlite3_errmsg(db->_handle));
        sqlite3_close(db->_handle);
        db->_handle = NULL;
    }
    else {
        // Set default database handle values.
        sqlite3_busy_timeout(db->_handle, 1000);
    }
}

void Database::Work_AfterOpen(napi_env e, napi_status status, void* data) {
    std::unique_ptr<OpenBaton> baton(static_cast<OpenBaton*>(data));

    Database* db = baton->db;

    Napi::Env env = db->Env();
    Napi::HandleScope scope(env);

    Napi::Value argv[1];
    if (baton->status != SQLITE_OK) {
        EXCEPTION(Napi::String::New(env, baton->message.c_str()), baton->status, exception);
        argv[0] = exception;
    }
    else {
        db->open = true;
        argv[0] = env.Null();
    }

    Napi::Function cb = baton->callback.Value();

    if (!cb.IsUndefined() && cb.IsFunction()) {
        TRY_CATCH_CALL(db->Value(), cb, 1, argv);
    }
    else if (!db->open) {
        Napi::Value info[] = { Napi::String::New(env, "error"), argv[0] };
        EMIT_EVENT(db->Value(), 2, info);
    }

    if (db->open) {
        Napi::Value info[] = { Napi::String::New(env, "open") };
        EMIT_EVENT(db->Value(), 1, info);
        db->Process();
    }
}

Napi::Value Database::OpenGetter(const Napi::CallbackInfo& info) {
    Napi::Env env = this->Env();
    Database* db = this;
    return Napi::Boolean::New(env, db->open);
}

Napi::Value Database::Close(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Database* db = this;
    OPTIONAL_ARGUMENT_FUNCTION(0, callback);

    Baton* baton = new Baton(db, callback);
    db->Schedule(Work_BeginClose, baton, true);

    return info.This();
}

void Database::Work_BeginClose(Baton* baton) {
    assert(baton->db->locked);
    assert(baton->db->open);
    assert(baton->db->_handle);
    assert(baton->db->pending == 0);

    baton->db->pending++;
    baton->db->RemoveCallbacks();
    baton->db->closing = true;

    Napi::Env env = baton->db->Env();

    int status = napi_create_async_work(
        env, NULL, Napi::String::New(env, "sqlite3.Database.Close"),
        Work_Close, Work_AfterClose, baton, &baton->request
    );
    assert(status == 0);
    napi_queue_async_work(env, baton->request);
}

void Database::Work_Close(napi_env e, void* data) {
    Baton* baton = static_cast<Baton*>(data);
    Database* db = baton->db;

    baton->status = sqlite3_close(db->_handle);

    if (baton->status != SQLITE_OK) {
        baton->message = std::string(sqlite3_errmsg(db->_handle));
    }
    else {
        db->_handle = NULL;
    }
}

void Database::Work_AfterClose(napi_env e, napi_status status, void* data) {
    std::unique_ptr<Baton> baton(static_cast<Baton*>(data));

    Database* db = baton->db;

    Napi::Env env = db->Env();
    Napi::HandleScope scope(env);

    db->pending--;
    db->closing = false;

    Napi::Value argv[1];
    if (baton->status != SQLITE_OK) {
        EXCEPTION(Napi::String::New(env, baton->message.c_str()), baton->status, exception);
        argv[0] = exception;
    }
    else {
        db->open = false;
        // Leave db->locked to indicate that this db object has reached
        // the end of its life.
        argv[0] = env.Null();
    }

    Napi::Function cb = baton->callback.Value();

    // Fire callbacks.
    if (!cb.IsUndefined() && cb.IsFunction()) {
        TRY_CATCH_CALL(db->Value(), cb, 1, argv);
    }
    else if (db->open) {
        Napi::Value info[] = { Napi::String::New(env, "error"), argv[0] };
        EMIT_EVENT(db->Value(), 2, info);
    }

    if (!db->open) {
        Napi::Value info[] = { Napi::String::New(env, "close"), argv[0] };
        EMIT_EVENT(db->Value(), 1, info);
        db->Process();
    }
}

Napi::Value Database::Serialize(const Napi::CallbackInfo& info) {
    Napi::Env env = this->Env();
    Database* db = this;
    OPTIONAL_ARGUMENT_FUNCTION(0, callback);

    bool before = db->serialize;
    db->serialize = true;

    if (!callback.IsEmpty() && callback.IsFunction()) {
        TRY_CATCH_CALL(info.This(), callback, 0, NULL, info.This());
        db->serialize = before;
    }

    db->Process();

    return info.This();
}

Napi::Value Database::Parallelize(const Napi::CallbackInfo& info) {
    Napi::Env env = this->Env();
    Database* db = this;
    OPTIONAL_ARGUMENT_FUNCTION(0, callback);

    bool before = db->serialize;
    db->serialize = false;

    if (!callback.IsEmpty() && callback.IsFunction()) {
        TRY_CATCH_CALL(info.This(), callback, 0, NULL, info.This());
        db->serialize = before;
    }

    db->Process();

    return info.This();
}

Napi::Value Database::Configure(const Napi::CallbackInfo& info) {
    Napi::Env env = this->Env();
    Database* db = this;

    REQUIRE_ARGUMENTS(2);

    if (info[0].StrictEquals( Napi::String::New(env, "trace"))) {
        Napi::Function handle;
        Baton* baton = new Baton(db, handle);
        db->Schedule(RegisterTraceCallback, baton);
    }
    else if (info[0].StrictEquals( Napi::String::New(env, "profile"))) {
        Napi::Function handle;
        Baton* baton = new Baton(db, handle);
        db->Schedule(RegisterProfileCallback, baton);
    }
    else if (info[0].StrictEquals( Napi::String::New(env, "busyTimeout"))) {
        if (!info[1].IsNumber()) {
            Napi::TypeError::New(env, "Value must be an integer").ThrowAsJavaScriptException();
            return env.Null();
        }
        Napi::Function handle;
        Baton* baton = new Baton(db, handle);
        baton->status = info[1].As<Napi::Number>().Int32Value();
        db->Schedule(SetBusyTimeout, baton);
    }
    else {
        Napi::TypeError::New(env, (StringConcat(
#if V8_MAJOR_VERSION > 6
            info.GetIsolate(),
#endif
            info[0].As<Napi::String>(),
            Napi::String::New(env, " is not a valid configuration option")
        )).Utf8Value().c_str()).ThrowAsJavaScriptException();
        return env.Null();
    }

    db->Process();

    return info.This();
}

Napi::Value Database::Interrupt(const Napi::CallbackInfo& info) {
    Napi::Env env = this->Env();
    Database* db = this;

    if (!db->open) {
        Napi::Error::New(env, "Database is not open").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (db->closing) {
        Napi::Error::New(env, "Database is closing").ThrowAsJavaScriptException();
        return env.Null();
    }

    sqlite3_interrupt(db->_handle);
    return info.This();
}

void Database::SetBusyTimeout(Baton* b) {
    std::unique_ptr<Baton> baton(b);

    assert(baton->db->open);
    assert(baton->db->_handle);

    // Abuse the status field for passing the timeout.
    sqlite3_busy_timeout(baton->db->_handle, baton->status);
}

void Database::RegisterTraceCallback(Baton* b) {
    std::unique_ptr<Baton> baton(b);
    assert(baton->db->open);
    assert(baton->db->_handle);
    Database* db = baton->db;

    if (db->debug_trace == NULL) {
        // Add it.
        db->debug_trace = new AsyncTrace(db, TraceCallback);
        sqlite3_trace(db->_handle, TraceCallback, db);
    }
    else {
        // Remove it.
        sqlite3_trace(db->_handle, NULL, NULL);
        db->debug_trace->finish();
        db->debug_trace = NULL;
    }
}

void Database::TraceCallback(void* db, const char* sql) {
    // Note: This function is called in the thread pool.
    // Note: Some queries, such as "EXPLAIN" queries, are not sent through this.
    static_cast<Database*>(db)->debug_trace->send(new std::string(sql));
}

void Database::TraceCallback(Database* db, std::string* s) {
    std::unique_ptr<std::string> sql(s);
    // Note: This function is called in the main V8 thread.
    Napi::Env env = db->Env();
    Napi::HandleScope scope(env);

    Napi::Value argv[] = {
        Napi::String::New(env, "trace"),
        Napi::String::New(env, sql->c_str())
    };
    EMIT_EVENT(db->Value(), 2, argv);
}

void Database::RegisterProfileCallback(Baton* b) {
    std::unique_ptr<Baton> baton(b);
    assert(baton->db->open);
    assert(baton->db->_handle);
    Database* db = baton->db;

    if (db->debug_profile == NULL) {
        // Add it.
        db->debug_profile = new AsyncProfile(db, ProfileCallback);
        sqlite3_profile(db->_handle, ProfileCallback, db);
    }
    else {
        // Remove it.
        sqlite3_profile(db->_handle, NULL, NULL);
        db->debug_profile->finish();
        db->debug_profile = NULL;
    }
}

void Database::ProfileCallback(void* db, const char* sql, sqlite3_uint64 nsecs) {
    // Note: This function is called in the thread pool.
    // Note: Some queries, such as "EXPLAIN" queries, are not sent through this.
    ProfileInfo* info = new ProfileInfo();
    info->sql = std::string(sql);
    info->nsecs = nsecs;
    static_cast<Database*>(db)->debug_profile->send(info);
}

void Database::ProfileCallback(Database *db, ProfileInfo* i) {
    std::unique_ptr<ProfileInfo> info(i);
    Napi::Env env = db->Env();
    Napi::HandleScope scope(env);

    Napi::Value argv[] = {
        Napi::String::New(env, "profile"),
        Napi::String::New(env, info->sql.c_str()),
        Napi::Number::New(env, (double)info->nsecs / 1000000.0)
    };
    EMIT_EVENT(db->Value(), 3, argv);
}

void Database::RegisterUpdateCallback(Baton* b) {
    std::unique_ptr<Baton> baton(b);
    assert(baton->db->open);
    assert(baton->db->_handle);
    Database* db = baton->db;

    if (db->update_event == NULL) {
        // Add it.
        db->update_event = new AsyncUpdate(db, UpdateCallback);
        sqlite3_update_hook(db->_handle, UpdateCallback, db);
    }
    else {
        // Remove it.
        sqlite3_update_hook(db->_handle, NULL, NULL);
        db->update_event->finish();
        db->update_event = NULL;
    }
}

void Database::UpdateCallback(void* db, int type, const char* database,
        const char* table, sqlite3_int64 rowid) {
    // Note: This function is called in the thread pool.
    // Note: Some queries, such as "EXPLAIN" queries, are not sent through this.
    UpdateInfo* info = new UpdateInfo();
    info->type = type;
    info->database = std::string(database);
    info->table = std::string(table);
    info->rowid = rowid;
    static_cast<Database*>(db)->update_event->send(info);
}

void Database::UpdateCallback(Database *db, UpdateInfo* i) {
    std::unique_ptr<UpdateInfo> info(i);
    Napi::Env env = db->Env();
    Napi::HandleScope scope(env);

    Napi::Value argv[] = {
        Napi::String::New(env, sqlite_authorizer_string(info->type)),
        Napi::String::New(env, info->database.c_str()),
        Napi::String::New(env, info->table.c_str()),
        Napi::Number::New(env, info->rowid),
    };
    EMIT_EVENT(db->Value(), 4, argv);
}

Napi::Value Database::Exec(const Napi::CallbackInfo& info) {
    Napi::Env env = this->Env();
    Database* db = this;

    REQUIRE_ARGUMENT_STRING(0, sql);
    OPTIONAL_ARGUMENT_FUNCTION(1, callback);

    Baton* baton = new ExecBaton(db, callback, sql.c_str());
    db->Schedule(Work_BeginExec, baton, true);

    return info.This();
}

void Database::Work_BeginExec(Baton* baton) {
    assert(baton->db->locked);
    assert(baton->db->open);
    assert(baton->db->_handle);
    assert(baton->db->pending == 0);
    baton->db->pending++;
    Napi::Env env = baton->db->Env();
    int status = napi_create_async_work(
        env, NULL, Napi::String::New(env, "sqlite3.Database.Exec"),
        Work_Exec, Work_AfterExec, baton, &baton->request
    );
    assert(status == 0);
    napi_queue_async_work(env, baton->request);
}

void Database::Work_Exec(napi_env e, void* data) {
    ExecBaton* baton = static_cast<ExecBaton*>(data);

    char* message = NULL;
    baton->status = sqlite3_exec(
        baton->db->_handle,
        baton->sql.c_str(),
        NULL,
        NULL,
        &message
    );

    if (baton->status != SQLITE_OK && message != NULL) {
        baton->message = std::string(message);
        sqlite3_free(message);
    }
}

void Database::Work_AfterExec(napi_env e, napi_status status, void* data) {
    std::unique_ptr<ExecBaton> baton(static_cast<ExecBaton*>(data));

    Database* db = baton->db;
    db->pending--;

    Napi::Env env = db->Env();
    Napi::HandleScope scope(env);

    Napi::Function cb = baton->callback.Value();

    if (baton->status != SQLITE_OK) {
        EXCEPTION(Napi::String::New(env, baton->message.c_str()), baton->status, exception);

        if (!cb.IsUndefined() && cb.IsFunction()) {
            Napi::Value argv[] = { exception };
            TRY_CATCH_CALL(db->Value(), cb, 1, argv);
        }
        else {
            Napi::Value info[] = { Napi::String::New(env, "error"), exception };
            EMIT_EVENT(db->Value(), 2, info);
        }
    }
    else if (!cb.IsUndefined() && cb.IsFunction()) {
        Napi::Value argv[] = { env.Null() };
        TRY_CATCH_CALL(db->Value(), cb, 1, argv);
    }

    db->Process();
}

Napi::Value Database::Wait(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Database* db = this;

    OPTIONAL_ARGUMENT_FUNCTION(0, callback);

    Baton* baton = new Baton(db, callback);
    db->Schedule(Work_Wait, baton, true);

    return info.This();
}

void Database::Work_Wait(Baton* b) {
    std::unique_ptr<Baton> baton(b);

    Napi::Env env = baton->db->Env();
    Napi::HandleScope scope(env);

    assert(baton->db->locked);
    assert(baton->db->open);
    assert(baton->db->_handle);
    assert(baton->db->pending == 0);

    Napi::Function cb = baton->callback.Value();
    if (!cb.IsUndefined() && cb.IsFunction()) {
        Napi::Value argv[] = { env.Null() };
        TRY_CATCH_CALL(baton->db->Value(), cb, 1, argv);
    }

    baton->db->Process();
}

Napi::Value Database::LoadExtension(const Napi::CallbackInfo& info) {
    Napi::Env env = this->Env();
    Database* db = this;

    REQUIRE_ARGUMENT_STRING(0, filename);
    OPTIONAL_ARGUMENT_FUNCTION(1, callback);

    Baton* baton = new LoadExtensionBaton(db, callback, filename.c_str());
    db->Schedule(Work_BeginLoadExtension, baton, true);

    return info.This();
}

void Database::Work_BeginLoadExtension(Baton* baton) {
    assert(baton->db->locked);
    assert(baton->db->open);
    assert(baton->db->_handle);
    assert(baton->db->pending == 0);
    baton->db->pending++;
    Napi::Env env = baton->db->Env();
    int status = napi_create_async_work(
        env, NULL, Napi::String::New(env, "sqlite3.Database.LoadExtension"),
        Work_LoadExtension, Work_AfterLoadExtension, baton, &baton->request
    );
    assert(status == 0);
    napi_queue_async_work(env, baton->request);
}

void Database::Work_LoadExtension(napi_env e, void* data) {
    LoadExtensionBaton* baton = static_cast<LoadExtensionBaton*>(data);

    sqlite3_enable_load_extension(baton->db->_handle, 1);

    char* message = NULL;
    baton->status = sqlite3_load_extension(
        baton->db->_handle,
        baton->filename.c_str(),
        0,
        &message
    );

    sqlite3_enable_load_extension(baton->db->_handle, 0);

    if (baton->status != SQLITE_OK && message != NULL) {
        baton->message = std::string(message);
        sqlite3_free(message);
    }
}

void Database::Work_AfterLoadExtension(napi_env e, napi_status status, void* data) {
    std::unique_ptr<LoadExtensionBaton> baton(static_cast<LoadExtensionBaton*>(data));

    Database* db = baton->db;
    db->pending--;

    Napi::Env env = db->Env();
    Napi::HandleScope scope(env);

    Napi::Function cb = baton->callback.Value();

    if (baton->status != SQLITE_OK) {
        EXCEPTION(Napi::String::New(env, baton->message.c_str()), baton->status, exception);

        if (!cb.IsUndefined() && cb.IsFunction()) {
            Napi::Value argv[] = { exception };
            TRY_CATCH_CALL(db->Value(), cb, 1, argv);
        }
        else {
            Napi::Value info[] = { Napi::String::New(env, "error"), exception };
            EMIT_EVENT(db->Value(), 2, info);
        }
    }
    else if (!cb.IsUndefined() && cb.IsFunction()) {
        Napi::Value argv[] = { env.Null() };
        TRY_CATCH_CALL(db->Value(), cb, 1, argv);
    }

    db->Process();
}

void Database::RemoveCallbacks() {
    if (debug_trace) {
        debug_trace->finish();
        debug_trace = NULL;
    }
    if (debug_profile) {
        debug_profile->finish();
        debug_profile = NULL;
    }
}


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/backup.cc
*/
// #include <string.h>
// #include <napi.h>

// #include "macros.h"
// #include "database.h"
// #include "backup.h"

using namespace node_sqlite3;

Napi::Object Backup::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function t = DefineClass(env, "Backup", {
        InstanceMethod("step", &Backup::Step),
        InstanceMethod("finish", &Backup::Finish),
        InstanceAccessor("idle", &Backup::IdleGetter, nullptr),
        InstanceAccessor("completed", &Backup::CompletedGetter, nullptr),
        InstanceAccessor("failed", &Backup::FailedGetter, nullptr),
        InstanceAccessor("remaining", &Backup::RemainingGetter, nullptr),
        InstanceAccessor("pageCount", &Backup::PageCountGetter, nullptr),
        InstanceAccessor("retryErrors", &Backup::RetryErrorGetter, &Backup::RetryErrorSetter),
    });

    exports.Set("Backup", t);
    return exports;
}

void Backup::Process() {
    if (finished && !queue.empty()) {
        return CleanQueue();
    }

    while (inited && !locked && !queue.empty()) {
        std::unique_ptr<Call> call(queue.front());
        queue.pop();

        call->callback(call->baton);
    }
}

void Backup::Schedule(Work_Callback callback, Baton* baton) {
    if (finished) {
        queue.push(new Call(callback, baton));
        CleanQueue();
    }
    else if (!inited || locked || !queue.empty()) {
        queue.push(new Call(callback, baton));
    }
    else {
        callback(baton);
    }
}

template <class T> void Backup::Error(T* baton) {
    Napi::Env env = baton->backup->Env();
    Napi::HandleScope scope(env);

    Backup* backup = baton->backup;
    // Fail hard on logic errors.
    assert(backup->status != 0);
    EXCEPTION(Napi::String::New(env, backup->message), backup->status, exception);

    Napi::Function cb = baton->callback.Value();

    if (!cb.IsEmpty() && cb.IsFunction()) {
        Napi::Value argv[] = { exception };
        TRY_CATCH_CALL(backup->Value(), cb, 1, argv);
    }
    else {
        Napi::Value argv[] = { Napi::String::New(env, "error"), exception };
        EMIT_EVENT(backup->Value(), 2, argv);
    }
}

void Backup::CleanQueue() {
    Napi::Env env = this->Env();
    Napi::HandleScope scope(env);

    if (inited && !queue.empty()) {
        // This backup has already been initialized and is now finished.
        // Fire error for all remaining items in the queue.
        EXCEPTION(Napi::String::New(env, "Backup is already finished"), SQLITE_MISUSE, exception);
        Napi::Value argv[] = { exception };
        bool called = false;

        // Clear out the queue so that this object can get GC'ed.
        while (!queue.empty()) {
            std::unique_ptr<Call> call(queue.front());
            queue.pop();

            std::unique_ptr<Baton> baton(call->baton);
            Napi::Function cb = baton->callback.Value();

            if (inited && !cb.IsEmpty() &&
                cb.IsFunction()) {
                TRY_CATCH_CALL(Value(), cb, 1, argv);
                called = true;
            }
        }

        // When we couldn't call a callback function, emit an error on the
        // Backup object.
        if (!called) {
            Napi::Value info[] = { Napi::String::New(env, "error"), exception };
            EMIT_EVENT(Value(), 2, info);
        }
    }
    else while (!queue.empty()) {
        // Just delete all items in the queue; we already fired an event when
        // initializing the backup failed.
        std::unique_ptr<Call> call(queue.front());
        queue.pop();

        // We don't call the actual callback, so we have to make sure that
        // the baton gets destroyed.
        delete call->baton;
    }
}

Backup::Backup(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Backup>(info) {
    Napi::Env env = info.Env();
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Use the new operator to create new Backup objects").ThrowAsJavaScriptException();
        return;
    }

    int length = info.Length();

    if (length <= 0 || !Database::HasInstance(info[0])) {
        Napi::TypeError::New(env, "Database object expected").ThrowAsJavaScriptException();
        return;
    }
    else if (length <= 1 || !info[1].IsString()) {
        Napi::TypeError::New(env, "Filename expected").ThrowAsJavaScriptException();
        return;
    }
    else if (length <= 2 || !info[2].IsString()) {
        Napi::TypeError::New(env, "Source database name expected").ThrowAsJavaScriptException();
        return;
    }
    else if (length <= 3 || !info[3].IsString()) {
        Napi::TypeError::New(env, "Destination database name expected").ThrowAsJavaScriptException();
        return;
    }
    else if (length <= 4 || !info[4].IsBoolean()) {
        Napi::TypeError::New(env, "Direction flag expected").ThrowAsJavaScriptException();
        return;
    }
    else if (length > 5 && !info[5].IsUndefined() && !info[5].IsFunction()) {
        Napi::TypeError::New(env, "Callback expected").ThrowAsJavaScriptException();
        return;
    }

    Database* db = Napi::ObjectWrap<Database>::Unwrap(info[0].As<Napi::Object>());
    Napi::String filename = info[1].As<Napi::String>();
    Napi::String sourceName = info[2].As<Napi::String>();
    Napi::String destName = info[3].As<Napi::String>();
    Napi::Boolean filenameIsDest = info[4].As<Napi::Boolean>();

    info.This().As<Napi::Object>().DefineProperty(Napi::PropertyDescriptor::Value("filename", filename));
    info.This().As<Napi::Object>().DefineProperty(Napi::PropertyDescriptor::Value("sourceName", sourceName));
    info.This().As<Napi::Object>().DefineProperty(Napi::PropertyDescriptor::Value("destName", destName));
    info.This().As<Napi::Object>().DefineProperty(Napi::PropertyDescriptor::Value("filenameIsDest", filenameIsDest));

    init(db);

    InitializeBaton* baton = new InitializeBaton(db, info[5].As<Napi::Function>(), this);
    baton->filename = filename.Utf8Value();
    baton->sourceName = sourceName.Utf8Value();
    baton->destName = destName.Utf8Value();
    baton->filenameIsDest = filenameIsDest.Value();
    db->Schedule(Work_BeginInitialize, baton);
}

void Backup::Work_BeginInitialize(Database::Baton* baton) {
    assert(baton->db->open);
    baton->db->pending++;
    Napi::Env env = baton->db->Env();
    int status = napi_create_async_work(
        env, NULL, Napi::String::New(env, "sqlite3.Backup.Initialize"),
        Work_Initialize, Work_AfterInitialize, baton, &baton->request
    );
    assert(status == 0);
    napi_queue_async_work(env, baton->request);
}

void Backup::Work_Initialize(napi_env e, void* data) {
    BACKUP_INIT(InitializeBaton);

    // In case stepping fails, we use a mutex to make sure we get the associated
    // error message.
    sqlite3_mutex* mtx = sqlite3_db_mutex(baton->db->_handle);
    sqlite3_mutex_enter(mtx);

    backup->status = sqlite3_open(baton->filename.c_str(), &backup->_otherDb);

    if (backup->status == SQLITE_OK) {
        backup->_handle = sqlite3_backup_init(
            baton->filenameIsDest ? backup->_otherDb : backup->db->_handle,
            baton->destName.c_str(),
            baton->filenameIsDest ? backup->db->_handle : backup->_otherDb,
            baton->sourceName.c_str());
    }
    backup->_destDb = baton->filenameIsDest ? backup->_otherDb : backup->db->_handle;

    if (backup->status != SQLITE_OK) {
        backup->message = std::string(sqlite3_errmsg(backup->_destDb));
        sqlite3_close(backup->_otherDb);
        backup->_otherDb = NULL;
        backup->_destDb = NULL;
    }

    sqlite3_mutex_leave(mtx);
}

void Backup::Work_AfterInitialize(napi_env e, napi_status status, void* data) {
    std::unique_ptr<InitializeBaton> baton(static_cast<InitializeBaton*>(data));
    Backup* backup = baton->backup;

    Napi::Env env = backup->Env();
    Napi::HandleScope scope(env);

    if (backup->status != SQLITE_OK) {
        Error(baton.get());
        backup->FinishAll();
    }
    else {
        backup->inited = true;
        Napi::Function cb = baton->callback.Value();
        if (!cb.IsEmpty() && cb.IsFunction()) {
            Napi::Value argv[] = { env.Null() };
            TRY_CATCH_CALL(backup->Value(), cb, 1, argv);
        }
    }
    BACKUP_END();
}

Napi::Value Backup::Step(const Napi::CallbackInfo& info) {
    Backup* backup = this;
    Napi::Env env = backup->Env();

    REQUIRE_ARGUMENT_INTEGER(0, pages);
    OPTIONAL_ARGUMENT_FUNCTION(1, callback);

    StepBaton* baton = new StepBaton(backup, callback, pages);
    backup->GetRetryErrors(baton->retryErrorsSet);
    backup->Schedule(Work_BeginStep, baton);
    return info.This();
}

void Backup::Work_BeginStep(Baton* baton) {
    BACKUP_BEGIN(Step);
}

void Backup::Work_Step(napi_env e, void* data) {
    BACKUP_INIT(StepBaton);
    if (backup->_handle) {
        backup->status = sqlite3_backup_step(backup->_handle, baton->pages);
        backup->remaining = sqlite3_backup_remaining(backup->_handle);
        backup->pageCount = sqlite3_backup_pagecount(backup->_handle);
    }
    if (backup->status != SQLITE_OK) {
        // Text of message is a little awkward to get, since the error is not associated
        // with a db connection.
#if SQLITE_VERSION_NUMBER >= 3007015
        // sqlite3_errstr is a relatively new method
        backup->message = std::string(sqlite3_errstr(backup->status));
#else
        backup->message = "Sqlite error";
#endif
        if (baton->retryErrorsSet.size() > 0) {
            if (baton->retryErrorsSet.find(backup->status) == baton->retryErrorsSet.end()) {
                backup->FinishSqlite();
            }
        }
    }
}

void Backup::Work_AfterStep(napi_env e, napi_status status, void* data) {
    std::unique_ptr<StepBaton> baton(static_cast<StepBaton*>(data));
    Backup* backup = baton->backup;

    Napi::Env env = backup->Env();
    Napi::HandleScope scope(env);

    if (backup->status == SQLITE_DONE) {
        backup->completed = true;
    } else if (!backup->_handle) {
        backup->failed = true;
    }

    if (backup->status != SQLITE_OK && backup->status != SQLITE_DONE) {
        Error(baton.get());
    }
    else {
        // Fire callbacks.
        Napi::Function cb = baton->callback.Value();
        if (!cb.IsEmpty() && cb.IsFunction()) {
            Napi::Value argv[] = { env.Null(), Napi::Boolean::New(env, backup->status == SQLITE_DONE) };
            TRY_CATCH_CALL(backup->Value(), cb, 2, argv);
        }
    }

    BACKUP_END();
}

Napi::Value Backup::Finish(const Napi::CallbackInfo& info) {
    Backup* backup = this;
    Napi::Env env = backup->Env();

    OPTIONAL_ARGUMENT_FUNCTION(0, callback);

    Baton* baton = new Baton(backup, callback);
    backup->Schedule(Work_BeginFinish, baton);
    return info.This();
}

void Backup::Work_BeginFinish(Baton* baton) {
    BACKUP_BEGIN(Finish);
}

void Backup::Work_Finish(napi_env e, void* data) {
    BACKUP_INIT(Baton);
    backup->FinishSqlite();
}

void Backup::Work_AfterFinish(napi_env e, napi_status status, void* data) {
    std::unique_ptr<Baton> baton(static_cast<Baton*>(data));
    Backup* backup = baton->backup;

    Napi::Env env = backup->Env();
    Napi::HandleScope scope(env);

    backup->FinishAll();

    // Fire callback in case there was one.
    Napi::Function cb = baton->callback.Value();
    if (!cb.IsEmpty() && cb.IsFunction()) {
        TRY_CATCH_CALL(backup->Value(), cb, 0, NULL);
    }

    BACKUP_END();
}

void Backup::FinishAll() {
    assert(!finished);
    if (!completed && !failed) {
        failed = true;
    }
    finished = true;
    CleanQueue();
    FinishSqlite();
    db->Unref();
}

void Backup::FinishSqlite() {
    if (_handle) {
        sqlite3_backup_finish(_handle);
        _handle = NULL;
    }
    if (_otherDb) {
        sqlite3_close(_otherDb);
        _otherDb = NULL;
    }
    _destDb = NULL;
}

Napi::Value Backup::IdleGetter(const Napi::CallbackInfo& info) {
    Backup* backup = this;
    bool idle = backup->inited && !backup->locked && backup->queue.empty();
    return Napi::Boolean::New(this->Env(), idle);
}

Napi::Value Backup::CompletedGetter(const Napi::CallbackInfo& info) {
    Backup* backup = this;
    return Napi::Boolean::New(this->Env(), backup->completed);
}

Napi::Value Backup::FailedGetter(const Napi::CallbackInfo& info) {
    Backup* backup = this;
    return Napi::Boolean::New(this->Env(), backup->failed);
}

Napi::Value Backup::RemainingGetter(const Napi::CallbackInfo& info) {
    Backup* backup = this;
    return Napi::Number::New(this->Env(), backup->remaining);
}

Napi::Value Backup::PageCountGetter(const Napi::CallbackInfo& info) {
    Backup* backup = this;
    return Napi::Number::New(this->Env(), backup->pageCount);
}

Napi::Value Backup::RetryErrorGetter(const Napi::CallbackInfo& info) {
    Backup* backup = this;
    return backup->retryErrors.Value();
}

void Backup::RetryErrorSetter(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Backup* backup = this;
    Napi::Env env = backup->Env();
    if (!value.IsArray()) {
        Napi::Error::New(env, "retryErrors must be an array").ThrowAsJavaScriptException();
        return;
    }
    Napi::Array array = value.As<Napi::Array>();
    backup->retryErrors.Reset(array, 1);
}

void Backup::GetRetryErrors(std::set<int>& retryErrorsSet) {
    retryErrorsSet.clear();
    Napi::Array array = retryErrors.Value();
    int length = array.Length();
    for (int i = 0; i < length; i++) {
        Napi::Value code = (array).Get(i);
        if (code.IsNumber()) {
            retryErrorsSet.insert(code.As<Napi::Number>().Int32Value());
        }
    }
}


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/statement.cc
*/
// #include <string.h>
// #include <napi.h>
// #include <uv.h>

// #include "macros.h"
// #include "database.h"
// #include "statement.h"

using namespace node_sqlite3;

Napi::Object Statement::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function t = DefineClass(env, "Statement", {
      InstanceMethod("bind", &Statement::Bind),
      InstanceMethod("get", &Statement::Get),
      InstanceMethod("run", &Statement::Run),
      InstanceMethod("all", &Statement::All),
      InstanceMethod("each", &Statement::Each),
      InstanceMethod("reset", &Statement::Reset),
      InstanceMethod("finalize", &Statement::Finalize_),
    });

    exports.Set("Statement", t);
    return exports;
}

// A Napi InstanceOf for Javascript Objects "Date" and "RegExp"
bool OtherInstanceOf(Napi::Object source, const char* object_type) {
    if (strncmp(object_type, "Date", 4) == 0) {
        return source.InstanceOf(source.Env().Global().Get("Date").As<Function>());
    } else if (strncmp(object_type, "RegExp", 6) == 0) {
        return source.InstanceOf(source.Env().Global().Get("RegExp").As<Function>());
    }

    return false;
}

void Statement::Process() {
    if (finalized && !queue.empty()) {
        return CleanQueue();
    }

    while (prepared && !locked && !queue.empty()) {
        std::unique_ptr<Call> call(queue.front());
        queue.pop();

        call->callback(call->baton);
    }
}

void Statement::Schedule(Work_Callback callback, Baton* baton) {
    if (finalized) {
        queue.push(new Call(callback, baton));
        CleanQueue();
    }
    else if (!prepared || locked) {
        queue.push(new Call(callback, baton));
    }
    else {
        callback(baton);
    }
}

template <class T> void Statement::Error(T* baton) {
    Statement* stmt = baton->stmt;

    Napi::Env env = stmt->Env();
    Napi::HandleScope scope(env);

    // Fail hard on logic errors.
    assert(stmt->status != 0);
    EXCEPTION(Napi::String::New(env, stmt->message.c_str()), stmt->status, exception);

    Napi::Function cb = baton->callback.Value();

    if (!cb.IsUndefined() && cb.IsFunction()) {
        Napi::Value argv[] = { exception };
        TRY_CATCH_CALL(stmt->Value(), cb, 1, argv);
    }
    else {
        Napi::Value argv[] = { Napi::String::New(env, "error"), exception };
        EMIT_EVENT(stmt->Value(), 2, argv);
    }
}

// { Database db, String sql, Array params, Function callback }
Statement::Statement(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Statement>(info) {
    Napi::Env env = info.Env();
    int length = info.Length();

    if (length <= 0 || !Database::HasInstance(info[0])) {
        Napi::TypeError::New(env, "Database object expected").ThrowAsJavaScriptException();
        return;
    }
    else if (length <= 1 || !info[1].IsString()) {
        Napi::TypeError::New(env, "SQL query expected").ThrowAsJavaScriptException();
        return;
    }
    else if (length > 2 && !info[2].IsUndefined() && !info[2].IsFunction()) {
        Napi::TypeError::New(env, "Callback expected").ThrowAsJavaScriptException();
        return;
    }

    Database* db = Napi::ObjectWrap<Database>::Unwrap(info[0].As<Napi::Object>());
    Napi::String sql = info[1].As<Napi::String>();

    info.This().As<Napi::Object>().DefineProperty(Napi::PropertyDescriptor::Value("sql", sql, napi_default));

    init(db);
    Statement* stmt = this;

    PrepareBaton* baton = new PrepareBaton(db, info[2].As<Napi::Function>(), stmt);
    baton->sql = std::string(sql.As<Napi::String>().Utf8Value().c_str());
    db->Schedule(Work_BeginPrepare, baton);
}

void Statement::Work_BeginPrepare(Database::Baton* baton) {
    assert(baton->db->open);
    baton->db->pending++;
    Napi::Env env = baton->db->Env();
    int status = napi_create_async_work(
        env, NULL, Napi::String::New(env, "sqlite3.Statement.Prepare"),
        Work_Prepare, Work_AfterPrepare, baton, &baton->request
    );
    assert(status == 0);
    napi_queue_async_work(env, baton->request);
}

void Statement::Work_Prepare(napi_env e, void* data) {
    STATEMENT_INIT(PrepareBaton);

    // In case preparing fails, we use a mutex to make sure we get the associated
    // error message.
    STATEMENT_MUTEX(mtx);
    sqlite3_mutex_enter(mtx);

    stmt->status = sqlite3_prepare_v2(
        baton->db->_handle,
        baton->sql.c_str(),
        baton->sql.size(),
        &stmt->_handle,
        NULL
    );

    if (stmt->status != SQLITE_OK) {
        stmt->message = std::string(sqlite3_errmsg(baton->db->_handle));
        stmt->_handle = NULL;
    }

    sqlite3_mutex_leave(mtx);
}

void Statement::Work_AfterPrepare(napi_env e, napi_status status, void* data) {
    std::unique_ptr<PrepareBaton> baton(static_cast<PrepareBaton*>(data));
    Statement* stmt = baton->stmt;

    Napi::Env env = stmt->Env();
    Napi::HandleScope scope(env);

    if (stmt->status != SQLITE_OK) {
        Error(baton.get());
        stmt->Finalize_();
    }
    else {
        stmt->prepared = true;
        if (!baton->callback.IsEmpty() && baton->callback.Value().IsFunction()) {
            Napi::Function cb = baton->callback.Value();
            Napi::Value argv[] = { env.Null() };
            TRY_CATCH_CALL(stmt->Value(), cb, 1, argv);
        }
    }

    STATEMENT_END();
}

template <class T> Values::Field*
                   Statement::BindParameter(const Napi::Value source, T pos) {
    if (source.IsString()) {
        std::string val = source.As<Napi::String>().Utf8Value();
        return new Values::Text(pos, val.length(), val.c_str());
    }
    else if (OtherInstanceOf(source.As<Object>(), "RegExp")) {
        std::string val = source.ToString().Utf8Value();
        return new Values::Text(pos, val.length(), val.c_str());
    }
    else if (source.IsNumber()) {
        if (OtherIsInt(source.As<Napi::Number>())) {
            return new Values::Integer(pos, source.As<Napi::Number>().Int32Value());
        } else {
            return new Values::Float(pos, source.As<Napi::Number>().DoubleValue());
        }
    }
    else if (source.IsBoolean()) {
        return new Values::Integer(pos, source.As<Napi::Boolean>().Value() ? 1 : 0);
    }
    else if (source.IsNull()) {
        return new Values::Null(pos);
    }
    else if (source.IsBuffer()) {
        Napi::Buffer<char> buffer = source.As<Napi::Buffer<char>>();
        return new Values::Blob(pos, buffer.Length(), buffer.Data());
    }
    else if (OtherInstanceOf(source.As<Object>(), "Date")) {
        return new Values::Float(pos, source.ToNumber().DoubleValue());
    }
    else if (source.IsObject()) {
        std::string val = source.ToString().Utf8Value();
        return new Values::Text(pos, val.length(), val.c_str());
    }
    else {
        return NULL;
    }
}

template <class T> T* Statement::Bind(const Napi::CallbackInfo& info, int start, int last) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (last < 0) last = info.Length();
    Napi::Function callback;
    if (last > start && info[last - 1].IsFunction()) {
        callback = info[last - 1].As<Napi::Function>();
        last--;
    }

    T* baton = new T(this, callback);

    if (start < last) {
        if (info[start].IsArray()) {
            Napi::Array array = info[start].As<Napi::Array>();
            int length = array.Length();
            // Note: bind parameters start with 1.
            for (int i = 0, pos = 1; i < length; i++, pos++) {
                baton->parameters.push_back(BindParameter((array).Get(i), pos));
            }
        }
        else if (!info[start].IsObject() || OtherInstanceOf(info[start].As<Object>(), "RegExp") || OtherInstanceOf(info[start].As<Object>(), "Date") || info[start].IsBuffer()) {
            // Parameters directly in array.
            // Note: bind parameters start with 1.
            for (int i = start, pos = 1; i < last; i++, pos++) {
                baton->parameters.push_back(BindParameter(info[i], pos));
            }
        }
        else if (info[start].IsObject()) {
            Napi::Object object = info[start].As<Napi::Object>();
            Napi::Array array = object.GetPropertyNames();
            int length = array.Length();
            for (int i = 0; i < length; i++) {
                Napi::Value name = (array).Get(i);
                Napi::Number num = name.ToNumber();

                if (num.Int32Value() == num.DoubleValue()) {
                    baton->parameters.push_back(
                        BindParameter((object).Get(name), num.Int32Value()));
                }
                else {
                    baton->parameters.push_back(BindParameter((object).Get(name),
                        name.As<Napi::String>().Utf8Value().c_str()));
                }
            }
        }
        else {
            return NULL;
        }
    }

    return baton;
}

bool Statement::Bind(const Parameters & parameters) {
    if (parameters.size() == 0) {
        return true;
    }

    sqlite3_reset(_handle);
    sqlite3_clear_bindings(_handle);

    Parameters::const_iterator it = parameters.begin();
    Parameters::const_iterator end = parameters.end();

    for (; it < end; ++it) {
        Values::Field* field = *it;

        if (field != NULL) {
            unsigned int pos;
            if (field->index > 0) {
                pos = field->index;
            }
            else {
                pos = sqlite3_bind_parameter_index(_handle, field->name.c_str());
            }

            switch (field->type) {
                case SQLITE_INTEGER: {
                    status = sqlite3_bind_int(_handle, pos,
                        ((Values::Integer*)field)->value);
                } break;
                case SQLITE_FLOAT: {
                    status = sqlite3_bind_double(_handle, pos,
                        ((Values::Float*)field)->value);
                } break;
                case SQLITE_TEXT: {
                    status = sqlite3_bind_text(_handle, pos,
                        ((Values::Text*)field)->value.c_str(),
                        ((Values::Text*)field)->value.size(), SQLITE_TRANSIENT);
                } break;
                case SQLITE_BLOB: {
                    status = sqlite3_bind_blob(_handle, pos,
                        ((Values::Blob*)field)->value,
                        ((Values::Blob*)field)->length, SQLITE_TRANSIENT);
                } break;
                case SQLITE_NULL: {
                    status = sqlite3_bind_null(_handle, pos);
                } break;
            }

            if (status != SQLITE_OK) {
                message = std::string(sqlite3_errmsg(db->_handle));
                return false;
            }
        }
    }

    return true;
}

Napi::Value Statement::Bind(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Statement* stmt = this;

    Baton* baton = stmt->Bind<Baton>(info);
    if (baton == NULL) {
        Napi::TypeError::New(env, "Data type is not supported").ThrowAsJavaScriptException();
        return env.Null();
    }
    else {
        stmt->Schedule(Work_BeginBind, baton);
        return info.This();
    }
}

void Statement::Work_BeginBind(Baton* baton) {
    STATEMENT_BEGIN(Bind);
}

void Statement::Work_Bind(napi_env e, void* data) {
    STATEMENT_INIT(Baton);

    STATEMENT_MUTEX(mtx);
    sqlite3_mutex_enter(mtx);
    stmt->Bind(baton->parameters);
    sqlite3_mutex_leave(mtx);
}

void Statement::Work_AfterBind(napi_env e, napi_status status, void* data) {
    std::unique_ptr<Baton> baton(static_cast<Baton*>(data));
    Statement* stmt = baton->stmt;

    Napi::Env env = stmt->Env();
    Napi::HandleScope scope(env);

    if (stmt->status != SQLITE_OK) {
        Error(baton.get());
    }
    else {
        // Fire callbacks.
        Napi::Function cb = baton->callback.Value();
        if (!cb.IsUndefined() && cb.IsFunction()) {
            Napi::Value argv[] = { env.Null() };
            TRY_CATCH_CALL(stmt->Value(), cb, 1, argv);
        }
    }

    STATEMENT_END();
}


Napi::Value Statement::Get(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Statement* stmt = this;

    Baton* baton = stmt->Bind<RowBaton>(info);
    if (baton == NULL) {
        Napi::Error::New(env, "Data type is not supported").ThrowAsJavaScriptException();
        return env.Null();
    }
    else {
        stmt->Schedule(Work_BeginGet, baton);
        return info.This();
    }
}

void Statement::Work_BeginGet(Baton* baton) {
    STATEMENT_BEGIN(Get);
}

void Statement::Work_Get(napi_env e, void* data) {
    STATEMENT_INIT(RowBaton);

    if (stmt->status != SQLITE_DONE || baton->parameters.size()) {
        STATEMENT_MUTEX(mtx);
        sqlite3_mutex_enter(mtx);

        if (stmt->Bind(baton->parameters)) {
            stmt->status = sqlite3_step(stmt->_handle);

            if (!(stmt->status == SQLITE_ROW || stmt->status == SQLITE_DONE)) {
                stmt->message = std::string(sqlite3_errmsg(stmt->db->_handle));
            }
        }

        sqlite3_mutex_leave(mtx);

        if (stmt->status == SQLITE_ROW) {
            // Acquire one result row before returning.
            GetRow(&baton->row, stmt->_handle);
        }
    }
}

void Statement::Work_AfterGet(napi_env e, napi_status status, void* data) {
    std::unique_ptr<RowBaton> baton(static_cast<RowBaton*>(data));
    Statement* stmt = baton->stmt;

    Napi::Env env = stmt->Env();
    Napi::HandleScope scope(env);

    if (stmt->status != SQLITE_ROW && stmt->status != SQLITE_DONE) {
        Error(baton.get());
    }
    else {
        // Fire callbacks.
        Napi::Function cb = baton->callback.Value();
        if (!cb.IsUndefined() && cb.IsFunction()) {
            if (stmt->status == SQLITE_ROW) {
                // Create the result array from the data we acquired.
                Napi::Value argv[] = { env.Null(), RowToJS(env, &baton->row) };
                TRY_CATCH_CALL(stmt->Value(), cb, 2, argv);
            }
            else {
                Napi::Value argv[] = { env.Null() };
                TRY_CATCH_CALL(stmt->Value(), cb, 1, argv);
            }
        }
    }

    STATEMENT_END();
}

Napi::Value Statement::Run(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Statement* stmt = this;

    Baton* baton = stmt->Bind<RunBaton>(info);
    if (baton == NULL) {
        Napi::Error::New(env, "Data type is not supported").ThrowAsJavaScriptException();
        return env.Null();
    }
    else {
        stmt->Schedule(Work_BeginRun, baton);
        return info.This();
    }
}

void Statement::Work_BeginRun(Baton* baton) {
    STATEMENT_BEGIN(Run);
}

void Statement::Work_Run(napi_env e, void* data) {
    STATEMENT_INIT(RunBaton);

    STATEMENT_MUTEX(mtx);
    sqlite3_mutex_enter(mtx);

    // Make sure that we also reset when there are no parameters.
    if (!baton->parameters.size()) {
        sqlite3_reset(stmt->_handle);
    }

    if (stmt->Bind(baton->parameters)) {
        stmt->status = sqlite3_step(stmt->_handle);

        if (!(stmt->status == SQLITE_ROW || stmt->status == SQLITE_DONE)) {
            stmt->message = std::string(sqlite3_errmsg(stmt->db->_handle));
        }
        else {
            baton->inserted_id = sqlite3_last_insert_rowid(stmt->db->_handle);
            baton->changes = sqlite3_changes(stmt->db->_handle);
        }
    }

    sqlite3_mutex_leave(mtx);
}

void Statement::Work_AfterRun(napi_env e, napi_status status, void* data) {
    std::unique_ptr<RunBaton> baton(static_cast<RunBaton*>(data));
    Statement* stmt = baton->stmt;

    Napi::Env env = stmt->Env();
    Napi::HandleScope scope(env);

    if (stmt->status != SQLITE_ROW && stmt->status != SQLITE_DONE) {
        Error(baton.get());
    }
    else {
        // Fire callbacks.
        Napi::Function cb = baton->callback.Value();
        if (!cb.IsUndefined() && cb.IsFunction()) {
            (stmt->Value()).Set(Napi::String::New(env, "lastID"), Napi::Number::New(env, baton->inserted_id));
            (stmt->Value()).Set( Napi::String::New(env, "changes"), Napi::Number::New(env, baton->changes));

            Napi::Value argv[] = { env.Null() };
            TRY_CATCH_CALL(stmt->Value(), cb, 1, argv);
        }
    }

    STATEMENT_END();
}

Napi::Value Statement::All(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Statement* stmt = this;

    Baton* baton = stmt->Bind<RowsBaton>(info);
    if (baton == NULL) {
        Napi::Error::New(env, "Data type is not supported").ThrowAsJavaScriptException();
        return env.Null();
    }
    else {
        stmt->Schedule(Work_BeginAll, baton);
        return info.This();
    }
}

void Statement::Work_BeginAll(Baton* baton) {
    STATEMENT_BEGIN(All);
}

void Statement::Work_All(napi_env e, void* data) {
    STATEMENT_INIT(RowsBaton);

    STATEMENT_MUTEX(mtx);
    sqlite3_mutex_enter(mtx);

    // Make sure that we also reset when there are no parameters.
    if (!baton->parameters.size()) {
        sqlite3_reset(stmt->_handle);
    }

    if (stmt->Bind(baton->parameters)) {
        while ((stmt->status = sqlite3_step(stmt->_handle)) == SQLITE_ROW) {
            Row* row = new Row();
            GetRow(row, stmt->_handle);
            baton->rows.push_back(row);
        }

        if (stmt->status != SQLITE_DONE) {
            stmt->message = std::string(sqlite3_errmsg(stmt->db->_handle));
        }
    }

    sqlite3_mutex_leave(mtx);
}

void Statement::Work_AfterAll(napi_env e, napi_status status, void* data) {
    std::unique_ptr<RowsBaton> baton(static_cast<RowsBaton*>(data));
    Statement* stmt = baton->stmt;

    Napi::Env env = stmt->Env();
    Napi::HandleScope scope(env);

    if (stmt->status != SQLITE_DONE) {
        Error(baton.get());
    }
    else {
        // Fire callbacks.
        Napi::Function cb = baton->callback.Value();
        if (!cb.IsUndefined() && cb.IsFunction()) {
            if (baton->rows.size()) {
                // Create the result array from the data we acquired.
                Napi::Array result(Napi::Array::New(env, baton->rows.size()));
                Rows::const_iterator it = baton->rows.begin();
                Rows::const_iterator end = baton->rows.end();
                for (int i = 0; it < end; ++it, i++) {
                    std::unique_ptr<Row> row(*it);
                    (result).Set(i, RowToJS(env,row.get()));
                }

                Napi::Value argv[] = { env.Null(), result };
                TRY_CATCH_CALL(stmt->Value(), cb, 2, argv);
            }
            else {
                // There were no result rows.
                Napi::Value argv[] = {
                    env.Null(),
                    Napi::Array::New(env, 0)
                };
                TRY_CATCH_CALL(stmt->Value(), cb, 2, argv);
            }
        }
    }

    STATEMENT_END();
}

Napi::Value Statement::Each(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Statement* stmt = this;

    int last = info.Length();

    Napi::Function completed;
    if (last >= 2 && info[last - 1].IsFunction() && info[last - 2].IsFunction()) {
        completed = info[--last].As<Napi::Function>();
    }

    EachBaton* baton = stmt->Bind<EachBaton>(info, 0, last);
    if (baton == NULL) {
        Napi::Error::New(env, "Data type is not supported").ThrowAsJavaScriptException();
        return env.Null();
    }
    else {
        baton->completed.Reset(completed, 1);
        stmt->Schedule(Work_BeginEach, baton);
        return info.This();
    }
}

void Statement::Work_BeginEach(Baton* baton) {
    // Only create the Async object when we're actually going into
    // the event loop. This prevents dangling events.
    EachBaton* each_baton = static_cast<EachBaton*>(baton);
    each_baton->async = new Async(each_baton->stmt, reinterpret_cast<uv_async_cb>(AsyncEach));
    each_baton->async->item_cb.Reset(each_baton->callback.Value(), 1);
    each_baton->async->completed_cb.Reset(each_baton->completed.Value(), 1);

    STATEMENT_BEGIN(Each);
}

void Statement::Work_Each(napi_env e, void* data) {
    STATEMENT_INIT(EachBaton);

    Async* async = baton->async;

    STATEMENT_MUTEX(mtx);

    int retrieved = 0;

    // Make sure that we also reset when there are no parameters.
    if (!baton->parameters.size()) {
        sqlite3_reset(stmt->_handle);
    }

    if (stmt->Bind(baton->parameters)) {
        while (true) {
            sqlite3_mutex_enter(mtx);
            stmt->status = sqlite3_step(stmt->_handle);
            if (stmt->status == SQLITE_ROW) {
                sqlite3_mutex_leave(mtx);
                Row* row = new Row();
                GetRow(row, stmt->_handle);
                NODE_SQLITE3_MUTEX_LOCK(&async->mutex)
                async->data.push_back(row);
                retrieved++;
                NODE_SQLITE3_MUTEX_UNLOCK(&async->mutex)

                uv_async_send(&async->watcher);
            }
            else {
                if (stmt->status != SQLITE_DONE) {
                    stmt->message = std::string(sqlite3_errmsg(stmt->db->_handle));
                }
                sqlite3_mutex_leave(mtx);
                break;
            }
        }
    }

    async->completed = true;
    uv_async_send(&async->watcher);
}

void Statement::CloseCallback(uv_handle_t* handle) {
    assert(handle != NULL);
    assert(handle->data != NULL);
    Async* async = static_cast<Async*>(handle->data);
    delete async;
}

void Statement::AsyncEach(uv_async_t* handle) {
    Async* async = static_cast<Async*>(handle->data);

    Napi::Env env = async->stmt->Env();
    Napi::HandleScope scope(env);

    while (true) {
        // Get the contents out of the data cache for us to process in the JS callback.
        Rows rows;
        NODE_SQLITE3_MUTEX_LOCK(&async->mutex)
        rows.swap(async->data);
        NODE_SQLITE3_MUTEX_UNLOCK(&async->mutex)

        if (rows.empty()) {
            break;
        }

        Napi::Function cb = async->item_cb.Value();
        if (!cb.IsUndefined() && cb.IsFunction()) {
            Napi::Value argv[2];
            argv[0] = env.Null();

            Rows::const_iterator it = rows.begin();
            Rows::const_iterator end = rows.end();
            for (int i = 0; it < end; ++it, i++) {
                std::unique_ptr<Row> row(*it);
                argv[1] = RowToJS(env,row.get());
                async->retrieved++;
                TRY_CATCH_CALL(async->stmt->Value(), cb, 2, argv);
            }
        }
    }

    Napi::Function cb = async->completed_cb.Value();
    if (async->completed) {
        if (!cb.IsEmpty() &&
                cb.IsFunction()) {
            Napi::Value argv[] = {
                env.Null(),
                Napi::Number::New(env, async->retrieved)
            };
            TRY_CATCH_CALL(async->stmt->Value(), cb, 2, argv);
        }
        uv_close(reinterpret_cast<uv_handle_t*>(handle), CloseCallback);
    }
}

void Statement::Work_AfterEach(napi_env e, napi_status status, void* data) {
    std::unique_ptr<EachBaton> baton(static_cast<EachBaton*>(data));
    Statement* stmt = baton->stmt;

    Napi::Env env = stmt->Env();
    Napi::HandleScope scope(env);

    if (stmt->status != SQLITE_DONE) {
        Error(baton.get());
    }

    STATEMENT_END();
}

Napi::Value Statement::Reset(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Statement* stmt = this;

    OPTIONAL_ARGUMENT_FUNCTION(0, callback);

    Baton* baton = new Baton(stmt, callback);
    stmt->Schedule(Work_BeginReset, baton);

    return info.This();
}

void Statement::Work_BeginReset(Baton* baton) {
    STATEMENT_BEGIN(Reset);
}

void Statement::Work_Reset(napi_env e, void* data) {
    STATEMENT_INIT(Baton);

    sqlite3_reset(stmt->_handle);
    stmt->status = SQLITE_OK;
}

void Statement::Work_AfterReset(napi_env e, napi_status status, void* data) {
    std::unique_ptr<Baton> baton(static_cast<Baton*>(data));
    Statement* stmt = baton->stmt;

    Napi::Env env = stmt->Env();
    Napi::HandleScope scope(env);

    // Fire callbacks.
    Napi::Function cb = baton->callback.Value();
    if (!cb.IsUndefined() && cb.IsFunction()) {
        Napi::Value argv[] = { env.Null() };
        TRY_CATCH_CALL(stmt->Value(), cb, 1, argv);
    }

    STATEMENT_END();
}

Napi::Value Statement::RowToJS(Napi::Env env, Row* row) {
    Napi::EscapableHandleScope scope(env);

    Napi::Object result = Napi::Object::New(env);

    Row::const_iterator it = row->begin();
    Row::const_iterator end = row->end();
    for (int i = 0; it < end; ++it, i++) {
        Values::Field* field = *it;

        Napi::Value value;

        switch (field->type) {
            case SQLITE_INTEGER: {
                value = Napi::Number::New(env, ((Values::Integer*)field)->value);
            } break;
            case SQLITE_FLOAT: {
                value = Napi::Number::New(env, ((Values::Float*)field)->value);
            } break;
            case SQLITE_TEXT: {
                value = Napi::String::New(env, ((Values::Text*)field)->value.c_str(), ((Values::Text*)field)->value.size());
            } break;
            case SQLITE_BLOB: {
                value = Napi::Buffer<char>::Copy(env, ((Values::Blob*)field)->value, ((Values::Blob*)field)->length);
            } break;
            case SQLITE_NULL: {
                value = env.Null();
            } break;
        }

        (result).Set(Napi::String::New(env, field->name.c_str()), value);

        DELETE_FIELD(field);
    }

    return scope.Escape(result);
}

void Statement::GetRow(Row* row, sqlite3_stmt* stmt) {
    int rows = sqlite3_column_count(stmt);

    for (int i = 0; i < rows; i++) {
        int type = sqlite3_column_type(stmt, i);
        const char* name = sqlite3_column_name(stmt, i);
        switch (type) {
            case SQLITE_INTEGER: {
                row->push_back(new Values::Integer(name, sqlite3_column_int64(stmt, i)));
            }   break;
            case SQLITE_FLOAT: {
                row->push_back(new Values::Float(name, sqlite3_column_double(stmt, i)));
            }   break;
            case SQLITE_TEXT: {
                const char* text = (const char*)sqlite3_column_text(stmt, i);
                int length = sqlite3_column_bytes(stmt, i);
                row->push_back(new Values::Text(name, length, text));
            } break;
            case SQLITE_BLOB: {
                const void* blob = sqlite3_column_blob(stmt, i);
                int length = sqlite3_column_bytes(stmt, i);
                row->push_back(new Values::Blob(name, length, blob));
            }   break;
            case SQLITE_NULL: {
                row->push_back(new Values::Null(name));
            }   break;
            default:
                assert(false);
        }
    }
}

Napi::Value Statement::Finalize_(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Statement* stmt = this;
    OPTIONAL_ARGUMENT_FUNCTION(0, callback);

    Baton* baton = new Baton(stmt, callback);
    stmt->Schedule(Finalize_, baton);

    return stmt->db->Value();
}

void Statement::Finalize_(Baton* b) {
    std::unique_ptr<Baton> baton(b);
    Napi::Env env = baton->stmt->Env();
    Napi::HandleScope scope(env);

    baton->stmt->Finalize_();

    // Fire callback in case there was one.
    Napi::Function cb = baton->callback.Value();
    if (!cb.IsUndefined() && cb.IsFunction()) {
        TRY_CATCH_CALL(baton->stmt->Value(), cb, 0, NULL);
    }
}

void Statement::Finalize_() {
    assert(!finalized);
    finalized = true;
    CleanQueue();
    // Finalize returns the status code of the last operation. We already fired
    // error events in case those failed.
    sqlite3_finalize(_handle);
    _handle = NULL;
    db->Unref();
}

void Statement::CleanQueue() {
    Napi::Env env = this->Env();
    Napi::HandleScope scope(env);

    if (prepared && !queue.empty()) {
        // This statement has already been prepared and is now finalized.
        // Fire error for all remaining items in the queue.
        EXCEPTION(Napi::String::New(env, "Statement is already finalized"), SQLITE_MISUSE, exception);
        Napi::Value argv[] = { exception };
        bool called = false;

        // Clear out the queue so that this object can get GC'ed.
        while (!queue.empty()) {
            std::unique_ptr<Call> call(queue.front());
            queue.pop();

            std::unique_ptr<Baton> baton(call->baton);
            Napi::Function cb = baton->callback.Value();

            if (prepared && !cb.IsEmpty() &&
                cb.IsFunction()) {
                TRY_CATCH_CALL(Value(), cb, 1, argv);
                called = true;
            }
        }

        // When we couldn't call a callback function, emit an error on the
        // Statement object.
        if (!called) {
            Napi::Value info[] = { Napi::String::New(env, "error"), exception };
            EMIT_EVENT(Value(), 2, info);
        }
    }
    else while (!queue.empty()) {
        // Just delete all items in the queue; we already fired an event when
        // preparing the statement failed.
        std::unique_ptr<Call> call(queue.front());
        queue.pop();
        // We don't call the actual callback, so we have to make sure that
        // the baton gets destroyed.
        delete call->baton;
    }
}


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/src/node_sqlite3.cc
*/
// #include <stdint.h>
// #include <sstream>
// #include <cstring>
// #include <string>
// #include <sqlite3.h>

// #include "macros.h"
// #include "database.h"
// #include "statement.h"
// #include "backup.h"

using namespace node_sqlite3;

namespace {

Napi::Object RegisterModule(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Database::Init(env, exports);
    Statement::Init(env, exports);
    Backup::Init(env, exports);

    exports.DefineProperties({
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_OPEN_READONLY, OPEN_READONLY)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_OPEN_READWRITE, OPEN_READWRITE)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_OPEN_CREATE, OPEN_CREATE)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_OPEN_FULLMUTEX, OPEN_FULLMUTEX)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_OPEN_URI, OPEN_URI)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_OPEN_SHAREDCACHE, OPEN_SHAREDCACHE)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_OPEN_PRIVATECACHE, OPEN_PRIVATECACHE)
        DEFINE_CONSTANT_STRING(exports, SQLITE_VERSION, VERSION)
#ifdef SQLITE_SOURCE_ID
        DEFINE_CONSTANT_STRING(exports, SQLITE_SOURCE_ID, SOURCE_ID)
#endif
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_VERSION_NUMBER, VERSION_NUMBER)

        DEFINE_CONSTANT_INTEGER(exports, SQLITE_OK, OK)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_ERROR, ERROR)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_INTERNAL, INTERNAL)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_PERM, PERM)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_ABORT, ABORT)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_BUSY, BUSY)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_LOCKED, LOCKED)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_NOMEM, NOMEM)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_READONLY, READONLY)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_INTERRUPT, INTERRUPT)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_IOERR, IOERR)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_CORRUPT, CORRUPT)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_NOTFOUND, NOTFOUND)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_FULL, FULL)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_CANTOPEN, CANTOPEN)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_PROTOCOL, PROTOCOL)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_EMPTY, EMPTY)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_SCHEMA, SCHEMA)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_TOOBIG, TOOBIG)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_CONSTRAINT, CONSTRAINT)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_MISMATCH, MISMATCH)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_MISUSE, MISUSE)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_NOLFS, NOLFS)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_AUTH, AUTH)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_FORMAT, FORMAT)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_RANGE, RANGE)
        DEFINE_CONSTANT_INTEGER(exports, SQLITE_NOTADB, NOTADB)
    });

    return exports;
}
}

const char* sqlite_code_string(int code) {
    switch (code) {
        case SQLITE_OK:         return "SQLITE_OK";
        case SQLITE_ERROR:      return "SQLITE_ERROR";
        case SQLITE_INTERNAL:   return "SQLITE_INTERNAL";
        case SQLITE_PERM:       return "SQLITE_PERM";
        case SQLITE_ABORT:      return "SQLITE_ABORT";
        case SQLITE_BUSY:       return "SQLITE_BUSY";
        case SQLITE_LOCKED:     return "SQLITE_LOCKED";
        case SQLITE_NOMEM:      return "SQLITE_NOMEM";
        case SQLITE_READONLY:   return "SQLITE_READONLY";
        case SQLITE_INTERRUPT:  return "SQLITE_INTERRUPT";
        case SQLITE_IOERR:      return "SQLITE_IOERR";
        case SQLITE_CORRUPT:    return "SQLITE_CORRUPT";
        case SQLITE_NOTFOUND:   return "SQLITE_NOTFOUND";
        case SQLITE_FULL:       return "SQLITE_FULL";
        case SQLITE_CANTOPEN:   return "SQLITE_CANTOPEN";
        case SQLITE_PROTOCOL:   return "SQLITE_PROTOCOL";
        case SQLITE_EMPTY:      return "SQLITE_EMPTY";
        case SQLITE_SCHEMA:     return "SQLITE_SCHEMA";
        case SQLITE_TOOBIG:     return "SQLITE_TOOBIG";
        case SQLITE_CONSTRAINT: return "SQLITE_CONSTRAINT";
        case SQLITE_MISMATCH:   return "SQLITE_MISMATCH";
        case SQLITE_MISUSE:     return "SQLITE_MISUSE";
        case SQLITE_NOLFS:      return "SQLITE_NOLFS";
        case SQLITE_AUTH:       return "SQLITE_AUTH";
        case SQLITE_FORMAT:     return "SQLITE_FORMAT";
        case SQLITE_RANGE:      return "SQLITE_RANGE";
        case SQLITE_NOTADB:     return "SQLITE_NOTADB";
        case SQLITE_ROW:        return "SQLITE_ROW";
        case SQLITE_DONE:       return "SQLITE_DONE";
        default:                return "UNKNOWN";
    }
}

const char* sqlite_authorizer_string(int type) {
    switch (type) {
        case SQLITE_INSERT:     return "insert";
        case SQLITE_UPDATE:     return "update";
        case SQLITE_DELETE:     return "delete";
        default:                return "";
    }
}

NODE_API_MODULE(node_sqlite3, RegisterModule)


/*
file none
*/
/*jslint-enable*/
