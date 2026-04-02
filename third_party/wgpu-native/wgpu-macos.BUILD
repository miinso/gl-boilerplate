##
#  wgpu-native macOS (prebuilt, single-arch)
##
cc_import(
    name = "lib",
    shared_library = "lib/libwgpu_native.dylib",
    static_library = "lib/libwgpu_native.a",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "headers",
    hdrs = glob(["include/**/*.h"]),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)
