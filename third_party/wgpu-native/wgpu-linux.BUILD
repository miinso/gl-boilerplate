##
#  wgpu-native Linux x86_64 (prebuilt)
##
cc_import(
    name = "lib",
    shared_library = "lib/libwgpu_native.so",
    static_library = "lib/libwgpu_native.a",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "headers",
    hdrs = glob(["include/**/*.h"]),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)
