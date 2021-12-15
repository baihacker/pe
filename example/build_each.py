#! python3
# -*- coding: UTF-8 -*-
import os
import sys
import shutil
import subprocess
import time

CURRENT_DIRECTORY = os.getcwd()


def TimePartsFromNs(duration):
  min_part = duration // 1000000000 // 60
  sec_part = duration // 1000000000 % 60
  millisec_part = duration // 1000000 % 1000
  return (min_part, sec_part, millisec_part)


def FormatTimeNs(duration):
  return '%d:%02d.%03d' % TimePartsFromNs(duration)


def main():
  ret = 0
  for file in os.listdir(CURRENT_DIRECTORY):
    filename, file_ext_name = os.path.splitext(file)
    if file_ext_name != '.c':
      continue
    print('Compile %s' % file)
    start_time = time.perf_counter_ns()
    ret = os.system('pe++.py %s' % file)
    time_usage = FormatTimeNs(time.perf_counter_ns() - start_time)
    print('Done, return code = %d, time usage = %s' % (ret, time_usage))
    print()
    if ret != 0:
      print('Failed to compile %s' % file)
      break
  if os.path.exists('a.exe'):
    os.remove('a.exe')
  return ret


if __name__ == "__main__":
  sys.exit(main())
