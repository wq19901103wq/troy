#include <list>
#include <gtest/gtest.h>
#include "serialize/baseline_serializer.h"
#include "serialize/baseline_deserializer.h"

struct C {
  int a;
  std::string b;
  double c;
};

struct B {
  int a = 1;
  double b = 0.3f;
  std::list<C> c;
};

struct A {
  int a = 0;
  double b = 0.2f;
  std::string c;
  B d;
  std::vector<int> e;
  std::map<std::string, B> f;
};

template <typename T>
std::string SerializeToString(const T& t) {
  std::stringstream os;
  std::shared_ptr<Serializer> serializer(new BaselineSerializer(os));
  serializer->Serialize(t);
  return os.str();
}

template <typename T>
std::optional<T> ParseFromString(const std::string& str) {
  std::stringstream os;
  os << str;
  std::shared_ptr<Deserializer> deserializer(new BaselineDeserializer(os));
  T ret;
  if (deserializer->Deserialize(ret)) {
    return ret;
  } else {
    return {};
  }
}

int main(int argc, char **argv) {
  A a;
  a.c = "base";
  a.d.b = 0.7f;
  a.e.push_back(2);
  a.e.push_back(3);
  a.e.push_back(5);
  a.f["wangqian"].a = 2;
  a.f["joying"].b = 0.5f;
  std::string str = SerializeToString(a);
  if (auto opb =  ParseFromString<A>(str)) {
    auto& b = *opb;
    std::cout << b.b << std::endl;                // should be 0.2f;
    std::cout << b.c << std::endl;                // should be "base";
    std::cout << b.d.b << std::endl;              // should be 0.7;
    std::cout << b.e.size() << std::endl;         // should be 3;
    std::cout << b.e[0] << std::endl;             // should be 2;
    std::cout << b.e[1] << std::endl;             // should be 3;
    std::cout << b.e[2] << std::endl;             // should be 2;
    std::cout << b.f.size() << std::endl;         // should be 2;
    std::cout << b.f["wangqian"].a << std::endl;  // should be 2;
    std::cout << b.f["joying"].b << std::endl;    // should be 0.5;
  } else {
    std::cout << "Not Parsed Correctly." << std::endl;
  }

  C tmp = {1, "das", 0.2};
  // 和C同构的tuple
  using IsomorphismC = std::tuple<int, std::string, double>;
  str = SerializeToString(tmp);
  if (auto opc =  ParseFromString<IsomorphismC>(str)) {
    std::cout << std::get<0>(*opc) << std::endl;  // should be 1;
    std::cout << std::get<1>(*opc) << std::endl;  // should be "das";
    std::cout << std::get<2>(*opc) << std::endl;  // should be 0.2f;
  } else {
    std::cout << "Not Parsed Correctly." << std::endl;
  }
}