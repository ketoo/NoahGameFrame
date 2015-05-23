# encoding:utf-8
from cx_Freeze import setup, Executable

setup(
name = "xls2xml",
version = "0.2",
description = "COPY (C) NFrame Stuido www.yowoyo.com",
executables = [Executable("xls2xml.py", icon = "setting.ico")])