#! python3
# -*- coding: UTF-8 -*-
import os
import sys
import shutil
import subprocess

CURRENT_DIRECTORY = os.getcwd()
PARENT_DIRECTORY = os.pardir


def main():
  ret = 0
  for file in os.listdir(PARENT_DIRECTORY):
    if not file.startswith('pe'):
      continue
    content = '#include <%s>\n int main(){}' % file
    with open('main.cc', 'wb') as tempf:
      tempf.write(content.encode('utf8'))
    print('Compile %s' % file)
    ret = os.system('pe++.py main.cc')
    os.remove('main.cc')
    print()
    if ret != 0:
      break
  os.system('del a.exe')
  return ret


if __name__ == "__main__":
  sys.exit(main())