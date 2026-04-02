load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

##
#  glad2 + glfw + angle (static) demos
##
cpp_file_names = glob(["*.cpp"])

[
    cc_binary(
        name = cpp_file_name.replace(".cpp", ""),
        srcs = [cpp_file_name],
        linkopts = select({
            "@platforms//os:linux": [
                "-lX11",
            ],
            "@platforms//os:macos": [],
            "@platforms//os:windows": [],
            "//conditions:default": [],
        }),
        deps = [
            "//third_party/angle-static",
            "//third_party/glad2",
            "@glfw2",
            "@glm",
        ],
    )
    for cpp_file_name in cpp_file_names
]
