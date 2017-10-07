#ifndef SERIALIZE_TYPE_TRAITS_H
#define SERIALIZE_TYPE_TRAITS_H

#include <math.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <type_traits>
#include <vector>

// 可以通过聚合类型构建的类型
template <typename T, typename... U>
decltype(void(T{std::declval<U>()...}), std::true_type{})
constructible(int);

// 不可以通过聚合类型构建的类型
template <typename, typename...> std::false_type
constructible(...);

// 是否是容器
template <typename T>
decltype(std::begin(T()), std::true_type{})
containable(int);

template <typename T> std::false_type
containable(...);

// 支持push back的容器
template <typename T>
decltype(T().push_back(*T().begin()), std::true_type{})
pushable(int);

template <typename T> std::false_type
pushable(...);

// 支持insert的容器
template <typename T>
decltype(T().insert(*T().begin()), std::true_type{})
insertable(int);

template <typename T> std::false_type
insertable(...);

// 是否是tuple
template <typename T>
decltype(std::tuple_size<T>::value, std::true_type{})
istuple(int);

template <typename T> std::false_type
istuple(...);


template <typename T, typename... U>
using is_constructible = decltype(constructible<T, U...>(0));

template <typename T>
using is_containable = decltype(containable<T>(0));

template <typename T>
using is_pushable = decltype(pushable<T>(0));

template <typename T>
using is_insertable = decltype(insertable<T>(0));

template <typename T>
using is_tuple = decltype(istuple<T>(0));

// 通配类型，可以转化成任意类型
struct WildcardType {
  template<typename T>
  constexpr operator T() const;
};

// 判断T是否有N个成员变量, 编译期实现, 递归情形
template<typename T, size_t N, typename... U>
struct HasParam {
  constexpr operator bool () {
    // tuple 情形特殊，直接用tuple_size判断
    if constexpr (is_tuple<T>()) {
      return N == std::tuple_size<T>::value;
    } else {
      return HasParam<T, N-1, WildcardType, U...>();
    }
  }
};

// 判断T是否有N个成员变量, 递归终止情形
template<typename T, typename... U>
struct HasParam<T, 0, U...> {
  constexpr operator bool () {
    return is_constructible<T, U...>{};
  }
};

// 对基本类型和容器做支持
template <class T>
constexpr bool is_serialize_type() {
  return !std::is_class<T>() || is_containable<T>();
}

// 是否是容器, 并排除string
template <class T>
constexpr bool is_container_type() {
  return is_containable<T>() && 
         !std::is_same<std::decay_t<T>, std::string>();
}

#endif // SERIALIZE_TYPE_TRAITS_H