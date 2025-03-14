### 公共工具函数 ###
def _process_paths(ctx, extra_includes, extra_lib_paths):
    """处理包含路径和库路径的公共函数"""
    c_include_paths = list(depset(
        extra_includes +
        ctx.configuration.default_shell_env.get("C_INCLUDE_PATH", "").split(";")
    ).to_list())
    library_paths = list(depset(
        extra_lib_paths +
        ctx.configuration.default_shell_env.get("LIBRARY_PATH", "").split(";")
    ).to_list())

    # 过滤空值并展开变量
    return (
        [ctx.expand_make_variables("", p, {}) for p in c_include_paths if p],
        [ctx.expand_make_variables("", p, {}) for p in library_paths if p]
    )

def _get_common_compile_flags(ctx):
    """获取公共编译参数"""
    return [
        "--std=c++20",
        "-fno-diagnostics-color",
        "-O3",
        "-march=native",
        "-mtune=native",
        "-fopenmp",
        "-Wno-delete-incomplete",
        "-Wno-shift-count-overflow"
    ] + ctx.attr.extra_cc_flags

CompilationContext = provider(
    fields = [
        "c_include_paths",
        "library_paths",
        "compile_flags",
        "defines",
        "cc_path",
        "ar_path",
    ]
)

LinkContext = provider(
    fields = [
        "link_flags",
        "libs",
    ]
)

### 公共构建函数 ###
def _create_compilation_context(ctx, extra_includes, extra_lib_paths):
    """创建编译上下文"""
    # 处理路径
    c_include_paths, library_paths = _process_paths(ctx, extra_includes, extra_lib_paths)

    # 获取工具链信息
    return CompilationContext(
        c_include_paths = c_include_paths,
        library_paths = library_paths,
        compile_flags = _get_common_compile_flags(ctx) + ctx.attr.extra_cc_flags,
        defines = ctx.attr.defines,
        cc_path = ctx.attr.cc_path,
        ar_path = ctx.attr.ar_path,
    )

def _compile_sources(ctx, compilation_ctx, srcs):
    """通用编译函数：生成对象文件"""
    obj_files = []
    for src in srcs:
        obj_file = ctx.actions.declare_file(
            src.basename + ".o",
            sibling = src
        )
        obj_files.append(obj_file)

        # 构建编译命令
        args = ctx.actions.args()
        args.add("-c")
        args.add("-x", "c++")
        args.add("-o", obj_file)
        args.add(src.path)
        args.add_all(compilation_ctx.compile_flags)
        args.add_all(["-I{}".format(p) for p in compilation_ctx.c_include_paths])
        args.add_all(["-D{}".format(d) for d in compilation_ctx.defines])

        ctx.actions.run(
            inputs = [src],
            outputs = [obj_file],
            executable = compilation_ctx.cc_path,
            arguments = [args],
            mnemonic = "CppCompile",
            progress_message = "Compiling {}".format(src.basename),
            use_default_shell_env = True
        )
    return obj_files

def _create_archive(ctx, compilation_ctx, obj_files, output_name):
    """创建静态库"""
    output_lib = ctx.actions.declare_file(output_name)
    ar_args = ctx.actions.args()
    ar_args.add("rcs", output_lib)
    ar_args.add_all([obj.path for obj in obj_files])

    ctx.actions.run(
        inputs = obj_files,
        outputs = [output_lib],
        executable = compilation_ctx.ar_path,
        arguments = [ar_args],
        mnemonic = "CppArchive",
        progress_message = "Archiving {}".format(output_lib.basename)
    )
    return output_lib

### 规则实现更新 ###
def _pe_library_impl(ctx):
    # 创建编译上下文
    compilation_ctx = _create_compilation_context(
        ctx,
        ctx.attr.extra_includes,
        ctx.attr.extra_lib_paths
    )

    # 编译源文件
    obj_files = _compile_sources(ctx, compilation_ctx, ctx.files.srcs)

    output_lib = _create_archive(ctx, compilation_ctx, obj_files, "lib{}.a".format(ctx.attr.name))

    return [DefaultInfo(
        files = depset([output_lib]),
        providers = [
            OutputGroupInfo(
                objects = depset(obj_files),
                archive = depset([output_lib])
            )
        ]
    )]

def _link_executable(ctx, compilation_ctx, link_ctx, obj_files):
    """链接可执行文件"""
    output = ctx.actions.declare_file(ctx.attr.name + ".exe")
    args = ctx.actions.args()
    args.add_all(obj_files)
    args.add("-o", output)
    args.add_all(link_ctx.link_flags)
    args.add_all(["-L{}".format(p) for p in compilation_ctx.library_paths])
    args.add_all(["-l{}".format(lib) for lib in link_ctx.libs])

    ctx.actions.run(
        inputs = obj_files,
        outputs = [output],
        executable = compilation_ctx.cc_path,
        arguments = [args],
        mnemonic = "CppLink",
        progress_message = "Linking {}".format(output.basename),
        use_default_shell_env = True
    )
    return output

def _build_combined(ctx, compilation_ctx, link_ctx):
    """单命令构建模式"""
    output = ctx.actions.declare_file(ctx.attr.name + ".exe")
    args = ctx.actions.args()
    args.add_all(["-x", "c++"])
    args.add_all(ctx.files.srcs)
    args.add("-o", output)
    args.add_all(compilation_ctx.compile_flags)
    args.add_all(link_ctx.link_flags)
    args.add_all(["-I{}".format(p) for p in compilation_ctx.c_include_paths])
    args.add_all(["-D{}".format(d) for d in compilation_ctx.defines])
    args.add_all(["-L{}".format(p) for p in compilation_ctx.library_paths])
    args.add_all(["-l{}".format(lib) for lib in link_ctx.libs])

    ctx.actions.run(
        inputs = ctx.files.srcs,
        outputs = [output],
        executable = compilation_ctx.cc_path,
        arguments = [args],
        mnemonic = "CppCombine",
        progress_message = "Building {}".format(output.basename),
        use_default_shell_env = True
    )
    return output

def _pe_binary_impl(ctx):
    # 创建编译上下文
    compilation_ctx = _create_compilation_context(
        ctx,
        ctx.attr.extra_includes,
        ctx.attr.extra_lib_paths
    )

    # 创建链接上下文
    link_ctx = LinkContext(
        link_flags = [
            "-static",
            "-Wl,--stack,268435456",
            "-lquadmath",
            "-lbf",
            "-lgmpxx",
            "-lflint",
            "-lmpfr",
            "-lntl",
            "-lgmp",
            "-lprimesieve",
            "-lprimecount",
            "-lzmq"
        ],
        libs = ctx.attr.libs
    )

    if ctx.attr.split_compile:
        # 分步编译模式
        obj_files = _compile_sources(ctx, compilation_ctx, ctx.files.srcs)
        output = _link_executable(ctx, compilation_ctx, link_ctx, obj_files)
    else:
        # 单命令模式
        output = _build_combined(ctx, compilation_ctx, link_ctx)

    return [DefaultInfo(
        executable = output,
        runfiles = ctx.runfiles(files=[output])
    )]

pe_library = rule(
    implementation = _pe_library_impl,
    attrs = {
        "srcs": attr.label_list(allow_files = [".cpp", ".c", ".cc"]),
        "defines": attr.string_list(),
        "extra_includes": attr.string_list(),
        "extra_lib_paths": attr.string_list(),
        "extra_cc_flags": attr.string_list(),
        "cc_path": attr.string(default = "g++"),
        "ar_path": attr.string(default = "ar"),
    }
)


pe_binary = rule(
    implementation = _pe_binary_impl,
    attrs = {
        "srcs": attr.label_list(allow_files = [".cpp", ".c", ".cc"]),
        "defines": attr.string_list(),
        "extra_includes": attr.string_list(),
        "extra_lib_paths": attr.string_list(),
        "libs": attr.string_list(),
        "extra_cc_flags": attr.string_list(),
        "cc_path": attr.string(default = "g++"),
        "ar_path": attr.string(default = "ar"),
        "split_compile": attr.bool(default = False)
    },
    executable = True
)