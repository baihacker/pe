#- * - coding : utf8 - * -
import os
import sys
import subprocess

CURRENT_DIRECTORY = os.path.dirname(os.path.realpath(__file__))

def main(argv):
  for rt, dirs, files in os.walk(CURRENT_DIRECTORY):
    for f in files:
      fpath = os.path.join(rt, f)
      filename, fileExtension = os.path.splitext(fpath)
      if len(fileExtension) == 0 or fileExtension in ['.h', '.hpp', '.c', '.cxx', '.cpp']:
        print fpath
        ret = subprocess.call('clang-format -style=Google -sort-includes=0 -i %s' % fpath)

if __name__ == '__main__':
  main(sys.argv)