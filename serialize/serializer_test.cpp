#include <gtest/gtest.h>
#include "serialize/baseline_serializer.h"

struct C {
  std::string a = "dsa";
  double b = 0.7f;
};

struct B {
  int a = 1;
  double b = 0.3f;
};

struct A {
  int a = 0;
  double b = 0.2f;
  B c;
  std::string d = "dsdas";
};

TEST(Serializer, SerializerTest) {
  std::ostringstream os;
  std::shared_ptr<Serializer> serializer(new BaselineSerializer(os));
  A a;
  serializer->Serialize(a);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}