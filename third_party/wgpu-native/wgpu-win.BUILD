##
#  wgpu-native Windows x86_64 (prebuilt)
##
cc_import(
    name = "lib",
    interface_library = "lib/wgpu_native.dll.lib",
    shared_library = "lib/wgpu_native.dll",
    static_library = "lib/wgpu_native.lib",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "headers",
    hdrs = glob(["include/**/*.h"]),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)
