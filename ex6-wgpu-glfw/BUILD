##
#  wgpu-native + glfw demos
##
cpp_file_names = glob(["*.cpp"])

[
    cc_binary(
        name = cpp_file_name.replace(".cpp", ""),
        srcs = [cpp_file_name],
        linkopts = select({
            "@platforms//os:macos": [],
            "@platforms//os:windows": [],
            "@platforms//os:linux": [
                "-lX11",
            ],
            "//conditions:default": [],
        }),
        deps = [
            "//third_party/glfw3webgpu",
            "//third_party/wgpu-native",
            "@glfw2",
        ],
    )
    for cpp_file_name in cpp_file_names
]
