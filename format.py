import os
import sys
import shutil
import subprocess

CURRENT_DIRECTORY = os.getcwd()
# BINARY_DIRECTORY = os.path.dirname(os.path.realpath(__file__))

compile_commands_template = '[{ "directory": "$(CURRENT_DIRECTORY)", "file": "pe", "arguments": ["clang++.exe", "-xc++", "pe", "--driver-mode=g++", "-c", "--std=c++17", "-O3", "-march=native", "-mtune=native", "--target=x86_64-w64-windows-gnu", "-fopenmp"]}]'


def tidy():
  with open('compile_commands.json', 'wb') as tempf:
    tempf.write(
        compile_commands_template.replace(
            '$(CURRENT_DIRECTORY)', CURRENT_DIRECTORY.replace('\\', '\\\\')))
  os.system(
      'run-clang-tidy.py -checks=-*,google-readability-casting,google-readability-braces-around-statements,google-readability-namespace-comments,performance-*,modernize-use-*,-modernize-use-trailing-return-type,-modernize-use-nodiscard,misc-unused-parameters -header-filter=pe.* -export-fixes=format-fixes.yaml -fix pe'
  )
  os.remove('compile_commands.json')
  os.remove('format-fixes.yaml')


def format():
  for rt, dirs, files in os.walk(CURRENT_DIRECTORY):
    if rt.find(".git") != -1:
      continue
    for f in files:
      fpath = os.path.join(rt, f)
      filename, fileExtension = os.path.splitext(fpath)
      if f in ['parallel_cal_prime_pi.c']:
        continue
      if len(fileExtension) == 0 or fileExtension in [
          '.h', '.hpp', '.c', '.cxx', '.cpp'
      ]:
        print fpath
        ret = subprocess.call(
            'clang-format -style=Google -sort-includes=0 -i %s' % fpath)


if __name__ == "__main__":
  tidy()
  #format()