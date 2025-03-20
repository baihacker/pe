load("//toolchain:pe_toolchain.bzl", "pe_library")

package(
    default_visibility = [
        "//visibility:public",
    ],
)

[pe_library(name = x + "_lib", srcs = [x]) for x in glob(["pe_*"])]
