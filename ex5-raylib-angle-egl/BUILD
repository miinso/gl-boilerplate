##
#  raylib with ANGLE examples
#  Run with: bazel run --config=angle //ex5-raylib-angle-egl:<target>
##

cc_binary(
    name = "raylib_angle_basic",
    srcs = ["raylib_angle_basic.c"],
    copts = [
        "-std=gnu99",
        "-fno-strict-aliasing",
        "-Wno-missing-braces",
    ],
    deps = [
        "@raylib-latest//:raylib-angle",
    ],
)

cc_binary(
    name = "raylib_angle_shader_300es",
    srcs = ["raylib_angle_shader_300es.c"],
    copts = [
        "-std=gnu99",
        "-fno-strict-aliasing",
        "-Wno-missing-braces",
    ],
    deps = [
        "@raylib-latest//:raylib-angle",
    ],
)
