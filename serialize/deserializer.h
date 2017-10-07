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

  // virtual 不支持模板, 需要全部列出, 继承类需要实现以下方法
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
    // parse container size
    if (!Deserialize(size)) {return false;}
    // type deduction, remove const and reference
    using InnerType = std::decay_t<decltype(*t.begin())>;
    for (size_t i = 0; i < size; i++) {
      InnerType inner;
      if (!Deserialize(inner)) {return false;}
      if constexpr (is_pushable<T>()) {
        // 支持push back的容器, 直接用back_inserter
        *std::back_inserter(t) = inner;
      } else if constexpr (is_insertable<T>()) {
        // 支持insert的容器, 直接insert
        t.insert(inner);
      } else {
        // TODO(qianwang) 是否还有其他容器
        static_assert(is_insertable<T>() || is_pushable<T>(),
                      "Container must be pushable or insertable");
      }
    }
    return true;
  }

  // 部分容器内部类型含有const, 如std::map, 移除const
  template<typename T>
  inline bool Deserialize(const T& t) {
    return Deserialize(const_cast<T&>(t));
  }

  // 变长调用, 递归情形
  template<typename T, typename ...U>
  inline bool Deserialize(T& t, U&... u) {
    return Deserialize(t) && Deserialize(u...);
  }

  // trivial standard-layout 但是内部可以有容器, string, 指针等
  template <class T>
  typename std::enable_if<!is_serialize_type<T>(), bool>::type
      Deserialize(T& t) {
// 类成员个数限制, 过大会影响编译速度
#define PARAM_LIMIT 64

/*
  类成员变量为N个的情形
  聚合类型判断不能过滤小于参数个数的情况，故而从大到小遍历
  structured binding 不支持变长，用preprocessor展开来做
*/
#define IF_STRUCT_PARAMS_EQUAL_DESERIALIZE(n)                              \
    if constexpr (HasParam<std::decay_t<T>,                                \
        BOOST_PP_SUB(PARAM_LIMIT, n)>()) {                                 \
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