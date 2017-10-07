#include <gtest/gtest.h>
#include "serialize/baseline_serializer.h"
#include "serialize/baseline_deserializer.h"

struct B {
  int a = 1;
  double b = 0.3f;
};

struct A {
  int a = 0;
  double b = 0.2f;
  std::string c;
  B d;
  std::vector<int> e;
  std::map<std::string, B> f;
};

TEST(Desrializer, DesrializerTest) {
  std::stringstream os;
  std::shared_ptr<Serializer> serializer(new BaselineSerializer(os));
  A a;
  a.c = "base";
  a.d.b = 0.7f;
  a.e.push_back(2);
  a.e.push_back(3);
  a.e.push_back(5);
  a.f["wangqian"].a = 2;
  a.f["joying"].b = 0.5f;
  serializer->Serialize(a);
  std::shared_ptr<Deserializer> deserializer(new BaselineDeserializer(os));
  A b;
  EXPECT_TRUE(deserializer->Deserialize(b));
  EXPECT_FLOAT_EQ(b.b, 0.2f);
  EXPECT_EQ(b.c, "base");
  EXPECT_FLOAT_EQ(b.d.b, 0.7);
  EXPECT_EQ(b.e.size(), 3);
  EXPECT_EQ(b.e[0], 2);
  EXPECT_EQ(b.e[1], 3);
  EXPECT_EQ(b.e[2], 5);
  EXPECT_EQ(b.f.size(), 2);
  EXPECT_EQ(b.f["wangqian"].a, 2);
  EXPECT_EQ(b.f["joying"].b, 0.5);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}