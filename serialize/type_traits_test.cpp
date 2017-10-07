
#include <gtest/gtest.h>
#include "serialize/type_traits.h"

struct A {
  int a = 0;
  double b = 0.2f;
  std::string c = "dsdas";
};

using B = std::tuple<int, double, std::string, A>;

TEST(Serializer, SerializerTest) {
  EXPECT_TRUE(is_serialize_type<std::string>());
  EXPECT_FALSE(is_serialize_type<A>());
  EXPECT_FALSE(is_serialize_type<A>());
  bool g = HasParam<A, 4>();
  EXPECT_FALSE(g);
  bool k = HasParam<A, 3>();
  EXPECT_TRUE(k);
  bool b = HasParam<A, 2>();
  EXPECT_TRUE(b);
  bool q = HasParam<B, 6>();
  EXPECT_FALSE(q);
  bool o = HasParam<B, 4>();
  EXPECT_TRUE(o);
  bool e = is_containable<std::vector<std::string>>();
  EXPECT_TRUE(e);
  bool z = is_containable<std::map<uint32_t, double>>();
  EXPECT_TRUE(z);
  bool v = is_containable<A>();
  EXPECT_FALSE(v);
  bool l = is_containable<double>();
  EXPECT_FALSE(l);
  bool w = is_pushable<std::vector<std::string>>();
  EXPECT_TRUE(w);
  bool n = is_pushable<std::map<uint32_t, std::string>>();
  EXPECT_FALSE(n);
  bool a = is_insertable<std::vector<std::string>>();
  EXPECT_FALSE(a);
  bool i = is_insertable<std::map<uint32_t, std::string>>();
  EXPECT_TRUE(i);
  bool j = is_container_type<std::string>();
  EXPECT_FALSE(j);
  bool s = is_container_type<const std::string>();
  EXPECT_FALSE(s);
  bool f = is_tuple<double>();
  EXPECT_FALSE(f);
  bool d = is_tuple<std::tuple<int, int, std::string>>();
  EXPECT_TRUE(d);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}