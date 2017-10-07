#ifndef SERIALIZE_DESERIALIZER_H
#define SERIALIZE_DESERIALIZER_H

#include <math.h>
#include <any>
#include <vector>
#include <type_traits>
#include <functional>
#include <boost/preprocessor.hpp>
#include "serialize/type_traits.h"

class Deserializer {
 public:
  Deserializer() = delete;
  Deserializer(std::istream& is): is_(is) {}

  // virtual 不支持模板, 需要全部列出
  virtual bool Deserialize(char& s) = 0;
  virtual bool Deserialize(int16_t& s) = 0;
  virtual bool Deserialize(uint16_t& s) = 0;
  virtual bool Deserialize(int32_t& s) = 0;
  virtual bool Deserialize(uint32_t& s) = 0;
  virtual bool Deserialize(int64_t& s) = 0;
  virtual bool Deserialize(uint64_t& s) = 0;
  virtual bool Deserialize(double& s) = 0;
  virtual bool Deserialize(float& s) = 0;
  virtual bool Deserialize(std::string& s) = 0;

  // 处理容器
  template <class T>
  typename std::enable_if<is_container_type<T>(), bool>::type
      Deserialize(T& t) {
    size_t size;
    if (!Deserialize(size)) {return false;}
    // 容器内类型推导, 移除const 和引用
    using InnerType = std::decay_t<decltype(*t.begin())>;
    for (size_t i = 0; i < size; i++) {
      InnerType inner;
      if (!Deserialize(inner)) {return false;}
      if constexpr (is_pushable<T>()) {
        *std::back_inserter(t) = inner;
      } else if constexpr (is_insertable<T>()) {
        t.insert(inner);
      } else {
        static_assert(is_insertable<T>() || is_pushable<T>(),
                      "Container must be pushable or insertable");
      }
    }
    return true;
  }

  template<typename T>
  inline bool Deserialize(const T& t) {
    return Deserialize(const_cast<T&>(t));
  }

  template<typename T, typename ...U>
  inline bool Deserialize(T& t, U&... u) {
    return Deserialize(t) && Deserialize(u...);
  }

  template <class T>
  typename std::enable_if<!is_serialize_type<T>(), bool>::type
      Deserialize(T& t) {
// 类成员个数限制, 过大会影响编译速度
#define PARAM_LIMIT 64

// 类成员变量为N个的情形
#define IF_STRUCT_PARAMS_EQUAL_DESERIALIZE(n)                              \
    /* 聚合类型判断不能过滤小于参数个数的情况，故而从大到小遍历 */         \
    if constexpr (HasParam<std::decay_t<T>,                                           \
        BOOST_PP_SUB(PARAM_LIMIT, n)>()) {                                 \
      /* structured binding 不支持变长，用preprocessor展开来做*/           \
      auto&& [BOOST_PP_ENUM_PARAMS(BOOST_PP_SUB(PARAM_LIMIT, n), p)] = t;  \
      return Deserialize(                                                  \
          BOOST_PP_ENUM_PARAMS(BOOST_PP_SUB(PARAM_LIMIT, n), p));          \
    } else                                                                 \

// 利用preprocessor纵向展开代码, 遍历参数个数情形
#define BOOST_PP_LOCAL_MACRO(n) IF_STRUCT_PARAMS_EQUAL_DESERIALIZE(n)
#define BOOST_PP_LOCAL_LIMITS   (0, PARAM_LIMIT-1)
#include BOOST_PP_LOCAL_ITERATE()

    // 最后1个else, 空struct
    {
      return true;
    }
  }

 protected:
  // 默认标准输入方便调试
  std::istream& is_ = std::cin;
};


#endif // SERIALIZE_DESERIALIZER_H