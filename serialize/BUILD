load("//:troy.bzl", "cc_gtest")
load("//:troy.bzl", "cpp_binary")

cc_gtest(
    name = "serializer_test",
    srcs = ["serializer_test.cpp", "baseline_serializer.h", "serializer.h", "type_traits.h"],
    deps = ["@gmock_archive//:gtest"],
)

cc_gtest(
    name = "deserializer_test",
    srcs = ["deserializer_test.cpp", "baseline_deserializer.h", "baseline_serializer.h", "serializer.h", "deserializer.h", "type_traits.h"],
    deps = ["@gmock_archive//:gtest"],
)

cc_gtest(
    name = "type_traits_test",
    srcs = ["type_traits_test.cpp", "type_traits.h"],
    deps = ["@gmock_archive//:gtest"],
)

cpp_binary(
    name = "example",
    srcs = ["example.cpp", "baseline_deserializer.h", "baseline_serializer.h", "serializer.h", "deserializer.h", "type_traits.h"],
    deps = ["@gmock_archive//:gtest"],
)