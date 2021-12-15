#! python3
# -*- coding: UTF-8 -*-
import os
import sys
import shutil
import subprocess

CURRENT_DIRECTORY = os.getcwd()


def main():
  ret = 0
  for file in os.listdir(CURRENT_DIRECTORY):
    filename, fileExtension = os.path.splitext(file)
    if fileExtension != '.c':
      continue
    print('Compile %s' % file)
    ret = os.system('pe++.py %s' % file)
    print()
    if ret != 0:
      print('Failed to compile %s' % file)
      break
  os.system('del a.exe')
  return ret


if __name__ == "__main__":
  sys.exit(main())
