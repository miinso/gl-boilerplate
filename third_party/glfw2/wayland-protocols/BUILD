##
#  pre-generated wayland protocol headers for GLFW (no wayland-scanner needed)
#  regenerate: for p in @glfw2//deps/wayland/*.xml; do n=$(basename $p .xml);
#    wayland-scanner client-header $p $n-client-protocol.h;
#    wayland-scanner private-code  $p $n-client-protocol-code.h; done
##
cc_library(
    name = "wayland-protocols",
    hdrs = glob(["*.h"]),
    includes = ["."],
    visibility = ["//visibility:public"],
)
