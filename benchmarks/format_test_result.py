#! python
#-*- coding: utf8 -*-
import os
import pprint


def load_db_data(file):
  file = file.decode(encoding='utf8', errors='ignore').encode(encoding='gbk',
                                                              errors='ignore')
  if os.path.exists(file):
    with open(file, 'rb') as tempf:
      result = tempf.read()
      return True, result
  return False, ''


def load_db():
  data = load_db_data('perf_test_result.txt')[1]
  return eval(data)


class MyPrettyPrinter(pprint.PrettyPrinter):

  def format(self, object, context, maxlevels, level):
    return pprint.PrettyPrinter.format(self, object, context, maxlevels, level)


def export(db, file):
  with open(file, 'w') as tempf:
    pp = MyPrettyPrinter(stream=tempf)
    pp.pprint(db)
    #tempf.write(str(db))


if __name__ == '__main__':
  db = load_db()
  #export(db, "formated.txt")