load("@rules_cc//cc:cc_library.bzl", "cc_library")

# APIs: gl=4.6, gles2=3.2
# Profile: compatibility
cc_library(
    name = "glad-460-320es",
    srcs = ["src/glad.c"],
    hdrs = [
        "include/KHR/khrplatform.h",
        "include/glad/glad.h",
    ],
    includes = ["include"],
    visibility = ["//visibility:public"],
)
