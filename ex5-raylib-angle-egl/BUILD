##
#  raylib with ANGLE examples
#  Run with: bazel run --config=angle //ex5-raylib-angle-egl:<target>
##

load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name = "raylib_angle_basic",
    srcs = ["raylib_angle_basic.c"],
    copts = select({
        "@platforms//os:windows": ["/std:c11"],
        "//conditions:default": [
            "-std=gnu99",
            "-fno-strict-aliasing",
            "-Wno-missing-braces",
        ],
    }),
    deps = [
        "@raylib-latest//:raylib-angle",
    ],
)

cc_binary(
    name = "raylib_angle_shader_300es",
    srcs = ["raylib_angle_shader_300es.c"],
    copts = select({
        "@platforms//os:windows": ["/std:c11"],
        "//conditions:default": [
            "-std=gnu99",
            "-fno-strict-aliasing",
            "-Wno-missing-braces",
        ],
    }),
    deps = [
        "@raylib-latest//:raylib-angle",
    ],
)
