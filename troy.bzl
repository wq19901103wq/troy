# Apply a patch_file to the repository root directory
# Runs 'patch -p1'

def troy_workspace(path_prefix="", tf_repo_name=""):
  native.new_http_archive(
      name = "gmock_archive",
      urls = [
          "http://mirror.bazel.build/github.com/google/googletest/archive/release-1.8.0.zip",
          "https://github.com/google/googletest/archive/release-1.8.0.zip",
      ],
      sha256 = "f3ed3b58511efd272eb074a3a6d6fb79d7c2e6a0e374323d1e6bcbcc1ef141bf",
      strip_prefix = "googletest-release-1.8.0",
      build_file = str(Label("//third_party:gmock.BUILD")),
  )

  native.bind(
      name = "gtest",
      actual = "@gmock_archive//:gtest",
  )

  native.bind(
      name = "gtest_main",
      actual = "@gmock_archive//:gtest_main",
  )

def cc_gtest(name,
             srcs,
             deps=[],
             linkstatic=0,
             tags=[],
             data=[],
             size="medium",
             suffix="",
             args=None,
             linkopts=[],
             copts=[]):
  native.cc_test(
      name=name,
      srcs=srcs,
      size=size,
      args=args,
      copts=["-std=gnu++17"] + copts,
      data=data,
      deps=deps,
      linkopts= linkopts,
      linkstatic=linkstatic,
      tags=tags)


def cpp_binary(name,
               srcs,
               deps=[],
               linkstatic=0,
               tags=[],
               data=[],
               suffix="",
               args=None,
               linkopts=[],
               copts=[]):
  native.cc_binary(
      name=name,
      srcs=srcs,
      args=args,
      copts=["-std=gnu++17"] + copts,
      data=data,
      deps=deps,
      linkopts= linkopts,
      linkstatic=linkstatic,
      tags=tags)
