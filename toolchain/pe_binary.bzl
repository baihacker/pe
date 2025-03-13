def _impl(ctx):
  # 获取环境变量中的路径
  c_include_paths = []
  library_paths = []

  # 添加用户指定的额外路径
  c_include_paths += ctx.attr.extra_includes
  library_paths += ctx.attr.extra_lib_paths

  # 处理 Windows 风格分号分隔符（兼容性处理）
  c_include_paths += ctx.configuration.default_shell_env.get("C_INCLUDE_PATH", "").split(";")
  library_paths += ctx.configuration.default_shell_env.get("LIBRARY_PATH", "").split(";")

  # 获取所有源文件
  src_files = [f for src in ctx.attr.srcs for f in src.files.to_list()]

  # 输出文件
  output = ctx.actions.declare_file(ctx.attr.name + ".exe")

  # 构建编译链接命令
  args = ctx.actions.args()
  args.add_all(src_files)  # 源文件
  args.add("-o", output)  # 输出文件
  args.add_all(["--std=c++20", "-fno-diagnostics-color", "-O3", "-march=native", "-mtune=native", "-fopenmp", "-lquadmath", "-Wl,--stack,268435456", "-static", "-lbf", "-lgmpxx", "-lflint", "-lmpfr", "-lntl", "-lgmp", "-lprimesieve", "-lprimecount", "-lzmq", "-Wno-delete-incomplete", "-Wno-shift-count-overflow"])
  args.add_all(["-I{}".format(p) for p in c_include_paths])  # 包含路径
  args.add_all(["-L{}".format(p) for p in library_paths])  # 库路径
  args.add_all(["-l{}".format(lib) for lib in ctx.attr.libs])  # 链接库
  args.add_all(["-D{}".format(d) for d in ctx.attr.defines])

  ctx.actions.run(
    inputs = src_files,
    outputs = [output],
    executable = ctx.attr.cc_path,  # 编译器路径
    arguments = [args],
    mnemonic = "CppCompileAndLink",
    use_default_shell_env = True  # 继承当前 shell 环境
  )

  return [DefaultInfo(
        executable = output,
        runfiles = ctx.runfiles(files=[output])
    )]

pe_binary = rule(
  implementation=_impl,
  attrs={
    "srcs": attr.label_list(allow_files = [".cpp", ".c", ".cc"]),
    "defines": attr.string_list(),  # 通过属性定义的宏
    "extra_includes": attr.string_list(),  # 额外的头文件路径
    "extra_lib_paths": attr.string_list(),  # 额外的库文件路径
    "libs": attr.string_list(),  # 要链接的库名称（例如：["m", "ws2_32"]）
    "cc_path": attr.string(default = "g++")  # 允许覆盖编译器路径
  },
  executable = True
)