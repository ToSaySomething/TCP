#! /usr/bin/python  
# -*- coding:utf-8 -*-

import os
import sys
import cv2
import numpy
from pyExcelerator import * 
import xlwt
import xlrd
import time
from xlrd import open_workbook 
from xlwt import easyxf 


def CheckIdenticalImages(img1, img2):
    image1=cv2.imread(img1)
    image2=cv2.imread(img2)

    if os.path.splitext(img1)[1]!=os.path.splitext(img2)[1]:
        return 0

    elif image1.shape!=image2.shape:
        return 0

    else:
        check=cv2.subtract(image1, image2)
        out=numpy.any(check)

        if out==False:
            return 1
        else:
            return 0

def to_unicode_or_bust(obj, encoding='utf-8'):

	if isinstance(obj, basestring):   #check whether a str
		if not isinstance(obj, unicode):   #check encode is unicode 
			obj = unicode(obj, encoding,"ignore")   #with UTF-8，change to unicode
	return obj

# 正例测试人员图片，全部放在一个文件夹下，命名规则：人员姓名-人员代号-组织单位
def getVipRecogMemberName(folder_path):
    cur_encoding = sys.getfilesystemencoding()
    image_files = [f for f in os.listdir(folder_path) if f.endswith('.jpg')] 
    image_names = []
    for image_file in image_files:
        imag_name = image_file.split('-')[0].decode(cur_encoding)
        image_names.append(imag_name)
    
    """ # save to txt file，used for test whether to get the correct chinese word
    # print (image_files[0].decode(cur_encoding))
    fo = open("test.txt", "w")
    for imgf in image_files:
        fo.writelines( imgf )
        fo.writelines( '\n' )
    fo.close() """
    
    return image_names,image_files

# 当前时间属于时间段内
def is_time_correct(time_current,time_begin,time_end):
    s_time = time.mktime(time.strptime(time_begin,'%Y-%m-%d %H:%M:%S')) # get the seconds for specify date
    e_time = time.mktime(time.strptime(time_end,'%Y-%m-%d %H:%M:%S'))
    log_time = time.mktime(time.strptime(time_current,'%Y-%m-%d %H:%M:%S')) 
    if (float(log_time) >= float(s_time)) and (float(log_time) <= float(e_time)):
        return True
    else:
        return False

# 当前姓名是否在人员测试集       
def is_name_correct(name_current,image_file_names):
    if name_current in image_file_names:
        return True
    else:
        return False
# 基于VIP姓名拿到本地对应的图片名字
def get_correspond_img_name (vip_name,image_file_names):
    for img_name in image_file_names:
        if vip_name in img_name:
            return img_name
        else:
            return None

# mehod 1: 基于姓名统计判断识别人数
# return: 总共识别人数，误识成其它人的人数
def get_VIPrcg_number_from_excel_name(image_file_names, excel_path, recg_time_start, recg_time_end):
    total_recog_num = 0
    wrong_recog_num = 0
    wrong_names = []
    recg_workbook=xlrd.open_workbook(excel_path)
    sheet_names = recg_workbook._sheet_names
    for sheet_name in sheet_names:
        cur_sheet = recg_workbook.sheet_by_name(sheet_name)
        col_names = cur_sheet.col_values(0)
        record_times = cur_sheet.col_values(5)
        for i in range(2,len(record_times)):
            if is_time_correct(record_times[i],recg_time_start,recg_time_end):
                total_recog_num += 1
                cur_name=col_names[i]
                if not is_name_correct(cur_name,image_file_names):
                    wrong_recog_num += 1
                    wrong_names.append(cur_name)
    return total_recog_num, wrong_recog_num, wrong_names

# mehod 3: 基于姓名统计判断识别人数.识别分数统计
# return: 总共识别人数，误识成其它人的人数
def get_VIPrcg_number_from_excel_recgScore(image_file_names, excel_path, recg_time_start, recg_time_end, recog_score):
    total_recog_num = 0
    wrong_recog_num = 0
    leakage_recog_num = 0
    corr_recog_num = 0
    wrong_names = []
    recg_workbook=xlrd.open_workbook(excel_path)
    sheet_names = recg_workbook._sheet_names
    for sheet_name in sheet_names:
        cur_sheet = recg_workbook.sheet_by_name(sheet_name)
        col_names = cur_sheet.col_values(0)
        record_times = cur_sheet.col_values(5)
        record_scores = cur_sheet.col_values(4)
        for i in range(2,len(record_times)):
            if is_time_correct(str(record_times[i]),recg_time_start,recg_time_end):
                total_recog_num += 1
                cur_name=col_names[i]
                if float(record_scores[i]) >= recog_score:                    
                    if is_name_correct(cur_name,image_file_names):
                        corr_recog_num += 1   #正确识别率个数
                    else:
                        wrong_recog_num += 1  #误识率个数
                        wrong_names.append(cur_name)
                else:
                    if is_name_correct(cur_name,image_file_names):
                        leakage_recog_num += 1   #漏识率个数
                    else:
                        wrong_recog_num += 1  #误识率个数
                        wrong_names.append(cur_name)  
    return total_recog_num,corr_recog_num, wrong_recog_num,leakage_recog_num, wrong_names

# method 2： 基于注册图片统计判断识别人数
# return: 总共识别人数，误识成其它人的人数，保存误识人的图片到本地路径
def get_VIPrcg_number_from_excel_img(vip_image_names, img_path, excel_path, recg_time_start, recg_time_end):
    total_recog_num = 0
    wrong_recog_num = 0
    wrong_names = []
    recg_workbook=xlrd.open_workbook(excel_path)
    sheet_names = recg_workbook._sheet_names
    for sheet_name in sheet_names:
        cur_sheet = recg_workbook.sheet_by_name(sheet_name)
        col_names = cur_sheet.col_values(0)
        register_imgs = cur_sheet.col_values(2)
        record_times = cur_sheet.col_values(5)
        for i in range(2,len(record_times)):
            if is_time_correct(record_times[i],recg_time_start,recg_time_end):
                total_recog_num += 1
                cur_name=col_names[i]                
                local_register_image = os.path.join(img_path,get_correspond_img_name (cur_name,vip_image_names))
                export_reg_image = register_imgs[i]                
                if not CheckIdenticalImages(local_register_image, export_reg_image):
                    wrong_recog_num += 1
                    wrong_names.append(cur_name)
    return total_recog_num, wrong_recog_num, wrong_names


register_image = 'D:/caroline/python_scripts/registerImage.jpg'
export_image = 'D:/caroline/python_scripts/exportImage.jpg'
capture_image = 'D:/caroline/python_scripts/capture.jpg'

excel_path_0523 = 'D:/caroline/python_scripts/VIP20180726102500-20180727235959_0523.xls'
folder_path = 'D:/caroline/python_scripts/test_images'
excel_path_0126 = 'D:/caroline/python_scripts/VIP20180726102500-20180727235959_0126.xls'
recg_time_start_menkou = "2018-07-26 10:20:00"
recg_time_end_menkou= "2018-07-27 18:06:00"

VIPRecog_member_names,VIP_image_names = getVipRecogMemberName(folder_path)
scores=[0.67,0.70,0.72,0.75]

#40-fea
""" recg_time_start = ["2018-07-30 13:42:00","2018-07-30 13:49:00","2018-07-30 13:56:00"]
recg_time_end = ["2018-07-30 13:45:30","2018-07-30 13:52:55","2018-07-30 13:59:15"]
excel_path_0330_97 = 'D:/caroline/python_scripts/40fea-20180730134000-20180730235959.xls'
out_put_txt =  'D:/caroline/python_scripts/output_40fea.txt' """
#97-fea
""" recg_time_start = ["2018-07-30 11:23:00","2018-07-30 11:30:00","2018-07-30 11:40:00"]
recg_time_end = ["2018-07-30 11:26:55","2018-07-30 11:32:55","2018-07-30 11:44:38"]
excel_path_0330_97 = 'D:/caroline/python_scripts/97fea-20180730112300-20180730235959.xls'
out_put_txt =  'D:/caroline/python_scripts/output_97fea.txt' """

#97-stan
""" recg_time_start = ["2018-07-30 16:48:06","2018-07-30 15:04:49","2018-07-30 16:20:20"]
recg_time_end = ["2018-07-30 16:50:30","2018-07-30 15:07:13","2018-07-30 16:22:44"]
excel_path_0330_97 = 'D:/caroline/python_scripts/97stan-20180730114500-20180730235959.xls'
out_put_txt =  'D:/caroline/python_scripts/output_97stan.txt' """

#40-stan
recg_time_start = ["2018-07-30 16:53:41","2018-07-30 15:08:40","2018-07-30 16:20:01"]
recg_time_end = ["2018-07-30 16:55:16","2018-07-30 15:10:17","2018-07-30 16:21:37"]
excel_path_0330_97 = 'D:/caroline/python_scripts/40stan-20180730115200-20180730235959.xls'
out_put_txt =  'D:/caroline/python_scripts/output_40stan.txt'

testing_case = ['1wan','5wan','10wan']
file_object = open(out_put_txt, 'w')

for i in range(0,len(recg_time_start)):
    file_object.write('tesing case: ' + testing_case[i] + '\n') 
    for score in scores:
        file_object.write('recognize score: ' + str(score) + '\n') 
        total_recog_num,corr_recog_num, wrong_recog_num,leakage_recog_num, wrong_names = \
                        get_VIPrcg_number_from_excel_recgScore(VIPRecog_member_names,excel_path_0330_97,recg_time_start[i],recg_time_end[i],score)
        if not total_recog_num == 0:
            corr_recog_num_rate = float(corr_recog_num*100)/float(total_recog_num)
            wrong_recog_num_rate = float(wrong_recog_num*100)/float(total_recog_num )  
            leakage_recog_num_rate = float(leakage_recog_num*100)/float(total_recog_num)
            file_object.write('total_recog_num: ' + str(total_recog_num) + ', ' + 'corr_recog_num: ' + str(corr_recog_num) + ', '  \
            'wrong_recog_num: ' + str(wrong_recog_num) + ', ' + 'leakage_recog_num: ' + str(leakage_recog_num) + ', ' '\n')
            file_object.write('corr_recog_num_rate: ' + str(corr_recog_num_rate) + '%' + '\n')          #识别率
            file_object.write('wrong_recog_num_rate: ' + str(wrong_recog_num_rate) + '%' + '\n')        #误识率
            file_object.write('leakage_recog_num_rate: ' + str(leakage_recog_num_rate) + '%' + '\n')    #漏识率
            file_object.write('\n')
file_object.close( )

total_recog_num_name_0523, wrong_recog_num_name_0523, wrong_names_name_0523 = \
                get_VIPrcg_number_from_excel_name(VIPRecog_member_names,excel_path_0523,recg_time_start,recg_time_end)
total_recog_num_name_0126, wrong_recog_num_name_0126, wrong_names_name_0126 = \
                 get_VIPrcg_number_from_excel_name(VIPRecog_member_names,excel_path_0126,recg_time_start,recg_time_end)

# total_recog_num_img, wrong_recog_num_img, wrong_names_img = get_VIPrcg_number_from_excel_img(VIP_image_names, folder_path, excel_path, recg_time_start, recg_time_end)
# CheckIdenticalImages(register_image, capture_image)