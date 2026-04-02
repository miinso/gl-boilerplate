load("@rules_cc//cc:cc_library.bzl", "cc_library")

##
#  glad2 (egl + gl + gles) with OES | EXT | ARB | KHR | ANGLE
##
cc_library(
    name = "glad2",
    srcs = [
        "src/egl.c",
        "src/gl.c",
        "src/gles2.c",
    ],
    hdrs = [
        "include/EGL/egl.h",  # TODO: this should be provided by ANGLE, not glad2
        "include/EGL/eglplatform.h",
        "include/KHR/khrplatform.h",
        "include/glad/egl.h",
        "include/glad/gl.h",
        "include/glad/gles2.h",
    ],
    includes = ["include"],
    visibility = ["//visibility:public"],
)
