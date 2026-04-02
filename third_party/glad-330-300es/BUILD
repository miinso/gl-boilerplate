load("@rules_cc//cc:cc_library.bzl", "cc_library")

# APIs: gl=3.3, gles2=3.0
# Profile: core
cc_library(
    name = "glad-330-300es",
    srcs = ["src/glad.c"],
    hdrs = [
        "include/GLES2/gl2ext.h",
        "include/GLES3/gl3.h",
        "include/KHR/khrplatform.h",
        "include/glad/glad.h",
    ],
    includes = ["include"],
    visibility = ["//visibility:public"],
)
