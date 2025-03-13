load("//toolchain:pe_toolchain.bzl", "pe_binary")

package(
    default_visibility = [
        "//visibility:public",
    ],
)

[pe_binary(name = x[:-2], srcs = [x]) for x in glob(["*.c"])]
