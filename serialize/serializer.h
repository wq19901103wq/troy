#ifndef SERIALIZE_SERIALIZER_H
#define SERIALIZE_SERIALIZER_H

#include <math.h>
#include <any>
#include <vector>
#include <type_traits>
#include <functional>
#include <boost/preprocessor.hpp>
#include "serialize/type_traits.h"

class Serializer {
 public:
  Serializer() = delete;
  Serializer(std::ostream& os): os_(os) {}
  
  // virtual 不支持模板, 需要全部列出
  virtual void Serialize(char s) = 0;
  virtual void Serialize(int16_t s) = 0;
  virtual void Serialize(uint16_t s) = 0;
  virtual void Serialize(int32_t s) = 0;
  virtual void Serialize(uint32_t s) = 0;
  virtual void Serialize(int64_t s) = 0;
  virtual void Serialize(uint64_t s) = 0;
  virtual void Serialize(double s) = 0;
  virtual void Serialize(float s) = 0;
  virtual void Serialize(const std::string& s) = 0;

  // 处理容器
  template <class T>
  typename std::enable_if<is_container_type<T>(), void>::type
      Serialize(const T& t) {
    size_t size = t.size();
    Serialize(size);
    // 依次序列化容器内部对象
    for (const auto& inner : t) {Serialize(inner);}
  }

  // 变长调用, 递归情形
  template<typename T, typename ...U>
  inline void Serialize(const T& t, const U&... u) {
    Serialize(t);
    Serialize(u...);
  }

  // trivial standard-layout 但是内部可以有容器, string, 指针等
  template <class T>
  typename std::enable_if<!is_serialize_type<T>(), void>::type
      Serialize(const T& t) {
// 类成员个数限制, 过大会影响编译速度
#define PARAM_LIMIT 64

/*
  类成员变量为N个的情形
  聚合类型判断不能过滤小于参数个数的情况，故而从大到小遍历
  structured binding 不支持变长，用preprocessor展开来做
*/
#define IF_STRUCT_PARAMS_EQUAL_SERIALIZE(n)                                \
    if constexpr (HasParam<std::decay_t<T>,                                \
        BOOST_PP_SUB(PARAM_LIMIT, n)>()) {                                 \
      auto&& [BOOST_PP_ENUM_PARAMS(BOOST_PP_SUB(PARAM_LIMIT, n), p)] = t;  \
      Serialize(BOOST_PP_ENUM_PARAMS(BOOST_PP_SUB(PARAM_LIMIT, n), p));    \
    } else                                                                 \


// 利用boost preprocessor纵向展开代码, 遍历参数个数情形
#define BOOST_PP_LOCAL_MACRO(n) IF_STRUCT_PARAMS_EQUAL_SERIALIZE(n)
#define BOOST_PP_LOCAL_LIMITS   (0, PARAM_LIMIT-1)
#include BOOST_PP_LOCAL_ITERATE()

    // 最后1个else, 空struct
    {}
  }

 protected:
  // 输出结果, 默认标准输出方便调试
  std::ostream& os_ = std::cout;
};


#endif // SERIALIZE_SERIALIZER_H