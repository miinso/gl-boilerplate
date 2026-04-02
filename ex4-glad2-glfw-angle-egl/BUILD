load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

##
#  explicitly uses egl but with angle
##
cpp_file_names = glob(["*.cpp"])

[
    cc_binary(
        name = cpp_file_name.replace(".cpp", ""),
        srcs = [cpp_file_name],
        copts = [],
        includes = [],
        linkopts = select({
            "@platforms//os:linux": [
                "-lGL",
                "-lGLU",
                "-lX11",
            ],
            "@platforms//os:macos": [
                "-framework",
                "OpenGL",
            ],
            "@platforms//os:windows": [
                "-DEFAULTLIB:opengl32.lib",
            ],
            "//conditions:default": [],
        }),
        deps = [
            "//third_party/angle",  # this line is the only change
            "//third_party/glad2",
            "@glfw2",
            "@glm",
        ],
    )
    for cpp_file_name in cpp_file_names
]
