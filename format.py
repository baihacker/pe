import os
import sys
import shutil
import subprocess

CURRENT_DIRECTORY = os.path.dirname(os.path.realpath(__file__))

compile_commands_template='[{ "directory": "$(CURRENT_DIRECTORY)", "file": "pe", "output": "X:\\\\TEMP\\\\pe.o", "arguments": ["clang++.exe", "-xc++", "pe", "--driver-mode=g++", "-c", "--std=c++11", "-O3", "-march=native", "-mtune=native", "--target=x86_64-w64-windows-gnu"]}]'

def tidy():
  with open('compile_commands.json', 'wb') as tempf:
    tempf.write(compile_commands_template.replace('$(CURRENT_DIRECTORY)', CURRENT_DIRECTORY.replace('\\','\\\\')))
  os.system('run-clang-tidy.py -checks=-*,google-readability-casting,google-readability-namespace-comments,modernize-use-emplace,hicpp-use-auto,performance-*,misc-unused-parameters -header-filter=pe.* -export-fixes=format-fixes.yaml -fix pe')

def format():
  for rt, dirs, files in os.walk(CURRENT_DIRECTORY):
    for f in files:
      fpath = os.path.join(rt, f)
      filename, fileExtension = os.path.splitext(fpath)
      if f == 'parallel_cal_pi_1e8.c':
        continue
      if len(fileExtension) == 0 or fileExtension in ['.h', '.hpp', '.c', '.cxx', '.cpp']:
        print fpath
        ret = subprocess.call('clang-format -style=Google -sort-includes=0 -i %s' % fpath)

if __name__ == "__main__":
  tidy()
  format()