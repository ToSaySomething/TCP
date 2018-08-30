#! /usr/bin/env python
#coding = utf-8
#pyexcel_xls 以 OrderedDict 结构处理数据

from pyexcel_xls import OrderedDict
from pyexcel_xls import get_data
from pyexcel_xls import save_data

def read_xls_file():
	xls_data = get_data(r"D:\read_test.xlsx")
	print "Get data type:", type(xls_data)
	for sheet_n in xls_data.keys():
		print sheet_n, ":", xls_data[sheet_n]

if __name__ == '__main__':
	read_xls_file()
