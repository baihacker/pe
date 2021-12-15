#! python3
# -*- coding: UTF-8 -*-
import os
import sys
import shutil
import subprocess
import time

CURRENT_DIRECTORY = os.getcwd()
PARENT_DIRECTORY = os.pardir


def DurationPartsFromNs(duration):
  min_part = duration // 1000000000 // 60
  sec_part = duration // 1000000000 % 60
  millisec_part = duration // 1000000 % 1000
  return (min_part, sec_part, millisec_part)


def FormatNs(duration):
  return '%d:%02d.%03d' % DurationPartsFromNs(duration)


def main():
  ret = 0
  for file in os.listdir(PARENT_DIRECTORY):
    if not file.startswith('pe'):
      continue
    filename, file_ext_name = os.path.splitext(file)
    if file_ext_name != '':
      continue
    content = '#include <%s>\n int main(){}' % file
    with open('main.cc', 'wb') as tempf:
      tempf.write(content.encode('utf8'))
    print('Compile %s' % file)
    start_time = time.perf_counter_ns()
    ret = os.system('pe++.py main.cc -hc')
    time_usage = FormatNs(time.perf_counter_ns() - start_time)
    if os.path.exists('main.cc'):
      os.remove('main.cc')
    print('Done, return code = %d, time usage = %s' % (ret, time_usage))
    print()
    if ret != 0:
      break
  if os.path.exists('a.exe'):
    os.remove('a.exe')
  return ret


if __name__ == "__main__":
  sys.exit(main())