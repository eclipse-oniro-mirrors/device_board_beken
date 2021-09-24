#-*- encoding: utf-8 -*-
 
import json
import sys
import binascii
from io import BytesIO
import struct
import time
import string
import datetime


def replaceit(instr):
    str_list = list(instr)
    cnt = 0
    for i in instr:
        cnt = cnt + 1
        if cnt == 10:
            str_list.insert(cnt," ")
        outStr = "".join(str_list)
    return outStr

def get_date_data_plus():
    time_data = {"YEAR":10, "MONTH":10, "DAY":10,"TIME":10}
    data = (datetime.datetime.now()-datetime.timedelta(minutes=3)).strftime("%Y-%m-%d-%H:%M:%S")
    data_str = data.split("-")
    time_data["YEAR"] = int( data_str[0] )
    time_data["MONTH"] = int( data_str[1] )
    time_data["DAY"] = int( data_str[2] )
    time_data["TIME"] = str( data_str[3] )
    return time_data

def get_date_data():
    time_data = {"YEAR":10, "MONTH":10, "DAY":10}
    data = time.strftime('%Y-%m-%d-%H-%M-%S',time.localtime(time.time()))
    data_str = data.split("-")
    time_data["YEAR"] = int( data_str[0] )
    time_data["MONTH"] = int( data_str[1] )
    time_data["DAY"] = int( data_str[2] )
    return time_data

def get_date_from_str(data):
    tm = {"YEAR":1234, "MONTH":12, "DAY":15}
    res = data.split("-")
    tm["YEAR"] = int(res[0])
    tm["MONTH"] = int(res[1])
    da = res[2]
    res = da.split("T")
    tm["DAY"] = int(res[0])
    return tm

def unix_time(dt):
    # 转换成时间数组
    timeArray = time.strptime(dt, "%Y-%m-%d %H:%M:%S")
    # 转换成时间戳
    timestamp = int(time.mktime(timeArray))
    return timestamp

def get_timestamp_commit(data):
    res = data.split("T")
    time_str = res[0] + " 00:00:00"
    return unix_time(time_str)

def get_timestamp_commit_update(data):
    res = data.split("T")
    time_str = res[0] + res[1][0:8]
    time_str = replaceit(time_str)
    return unix_time(time_str)

def get_timestamp_now():
    tm = get_date_data()
    time_str = ("%d-%d-%d 00:00:00" % (tm["YEAR"], tm["MONTH"], tm["DAY"]) )
    return unix_time(time_str)

def get_timestamp_now_plus():
    tm = get_date_data_plus()
    time_str = ("%d-%d-%d %s" % (tm["YEAR"], tm["MONTH"], tm["DAY"],tm["TIME"]) )
    return unix_time(time_str)
