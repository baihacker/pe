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

PeLibraryInfo = provider(
    fields = {
        "archive": "生成的静态库文件 (.a)",
    }
)

### 公共构建函数 ###
def _create_compilation_context(ctx, extra_includes, extra_lib_paths):
    path_separator = ctx.configuration.host_path_separator

    """创建编译上下文"""
    c_include_paths = list(depset(
        extra_includes +
        ctx.configuration.default_shell_env.get("C_INCLUDE_PATH", "").split(path_separator)
    ).to_list())

    library_paths = list(depset(
        extra_lib_paths +
        ctx.configuration.default_shell_env.get("LIBRARY_PATH", "").split(path_separator)
    ).to_list())

    c_include_paths = [ctx.expand_make_variables("", p, {}) for p in c_include_paths if p]
    library_paths = [ctx.expand_make_variables("", p, {}) for p in library_paths if p]

    # 获取工具链信息
    return CompilationContext(
        c_include_paths = c_include_paths,
        library_paths = library_paths,
        compile_flags = [
            "--std=c++20",
            "-Wno-delete-incomplete",
            "-Wno-shift-count-overflow",
            "-O3",
            "-march=native",
            "-mtune=native",
            "-fopenmp",
            "-fno-diagnostics-color",
        ] + ctx.attr.copts,
        defines = ctx.attr.defines,
        cc_path = ctx.attr.cc_path,
        ar_path = ctx.attr.ar_path,
    )

def _create_link_context(ctx):
    """创建编译上下文"""
    return LinkContext(
        link_flags = [
            "-static",
            "-Wl,--stack,268435456",
            "-fopenmp",
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
        ] + ctx.attr.linkopts,
        libs = ctx.attr.libs
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
        args.add(src.path)
        args.add("-o", obj_file)

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
            ),
            OutputGroupInfo(
                objects = depset(obj_files),
                archive = depset([output_lib])
            ),
            PeLibraryInfo(
                archive = output_lib,           # 静态库文件
            )
          ]

def _link_executable(ctx, compilation_ctx, link_ctx, obj_files, dep_archives):
    """链接可执行文件"""
    output = ctx.actions.declare_file(ctx.attr.name + ".exe")
    args = ctx.actions.args()
    args.add_all(obj_files)
    args.add_all(dep_archives)
    args.add("-o", output)

    args.add_all(link_ctx.link_flags)
    args.add_all(["-L{}".format(p) for p in compilation_ctx.library_paths])
    args.add_all(["-l{}".format(lib) for lib in link_ctx.libs])

    ctx.actions.run(
        inputs = obj_files + dep_archives,
        outputs = [output],
        executable = compilation_ctx.cc_path,
        arguments = [args],
        mnemonic = "CppLink",
        progress_message = "Linking {}".format(output.basename),
        use_default_shell_env = True
    )
    return output

def _build_combined(ctx, compilation_ctx, link_ctx, dep_archives):
    """单命令构建模式"""
    output = ctx.actions.declare_file(ctx.attr.name + ".exe")
    args = ctx.actions.args()
    args.add_all(["-x", "c++"])
    args.add_all(ctx.files.srcs)
    args.add_all(["-x", "none"])
    args.add_all(dep_archives)
    args.add("-o", output)

    args.add_all(compilation_ctx.compile_flags)
    args.add_all(["-I{}".format(p) for p in compilation_ctx.c_include_paths])
    args.add_all(["-D{}".format(d) for d in compilation_ctx.defines])

    args.add_all(link_ctx.link_flags)
    args.add_all(["-L{}".format(p) for p in compilation_ctx.library_paths])
    args.add_all(["-l{}".format(lib) for lib in link_ctx.libs])

    ctx.actions.run(
        inputs = ctx.files.srcs + dep_archives,
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

    # 收集依赖项的静态库
    dep_archives = []  # 依赖项的静态库

    for dep in ctx.attr.deps:
        if PeLibraryInfo in dep:
            dep_archives.append(dep[PeLibraryInfo].archive)
        else:
            print("Warning: Unsupported dependency type:", dep)

    # 创建链接上下文
    link_ctx = _create_link_context(ctx)

    if ctx.attr.split_compile:
        # 分步编译模式
        obj_files = _compile_sources(ctx, compilation_ctx, ctx.files.srcs)
        output = _link_executable(ctx, compilation_ctx, link_ctx, obj_files, dep_archives)
    else:
        # 单命令模式
        output = _build_combined(ctx, compilation_ctx, link_ctx, dep_archives)

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
        "copts": attr.string_list(),
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
        "copts": attr.string_list(),
        "linkopts": attr.string_list(),
        "deps": attr.label_list(),
        "cc_path": attr.string(default = "g++"),
        "ar_path": attr.string(default = "ar"),
        "split_compile": attr.bool(default = False)
    },
    executable = True
)