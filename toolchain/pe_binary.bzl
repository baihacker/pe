def _impl(ctx):
  return ctx.actions.run(
      use_default_shell_env = True,
      inputs = ctx.files.srcs,
      executable = 'g++',
      outputs = [ctx.outputs.target],
      arguments = [x.path for x in ctx.files.srcs] + ["-o", ctx.outputs.target.path, "--std=c++20", "-O3", "-march=native", "-mtune=native", "-fopenmp", "-lquadmath", "-Wl,--stack,268435456", "-static", "-lbf", "-lgmpxx", "-lflint", "-lmpfr", "-lntl", "-lgmp", "-lprimesieve", "-lprimecount", "-Wno-delete-incomplete", "-Wno-shift-count-overflow"] + ["-I" + path for path in ctx.configuration.default_shell_env.get("C_INCLUDE_PATH", "").split(";")] + ["-L" + path for path in ctx.configuration.default_shell_env.get("LIBRARY_PATH", "").split(";")],
      progress_message = "Building " + ctx.outputs.target.path
  )

pe_binary = rule(
  implementation=_impl,
  attrs={
    "srcs": attr.label_list(allow_files=True),
    "compiler": attr.string(default="gcc"),
    "options": attr.string_list(),
  },
  outputs={"target": "%{name}.exe"},
)