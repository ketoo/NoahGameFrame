# encoding : utf-8
import os
import sys
import xlrd
from xml.dom import minidom
from ctypes import *

#print颜色
FOREGROUND_WHITE = 0x0007
FOREGROUND_BLUE = 0x01 # text color contains blue.
FOREGROUND_GREEN = 0x02 # text color contains green.
FOREGROUND_LIGHT_GREEN = 0xa # text color contains green.
FOREGROUND_RED = 0x04 # text color contains red.
FOREGROUND_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN

COLOR_BLACK = 0			# 黑色
COLOR_DARK_BLUE = 1		# 深蓝色
COLOR_DARK_GREEN = 2	# 深绿色
COLOR_DARK_CYAN = 3		# 深青色
COLOR_DARK_RED = 4		# 深红色
COLOR_DARK_MAGENTA = 5	# 深紫红色
COLOR_BROWN = 6			# 深黄色
COLOR_DARK_WHITE = 7	# 深白色
COLOR_BLUE = 9			# 蓝色
COLOR_GREEN = 10		# 绿色
COLOR_CYAN = 11			# 青色
COLOR_RED = 12			# 红色
COLOR_MAGENTA = 13		# 紫红色
COLOR_YELLOW = 14		# 黄色
COLOR_WHITE = 15		# 白色

STD_OUTPUT_HANDLE= -11
std_out_handle = windll.kernel32.GetStdHandle(STD_OUTPUT_HANDLE)
def set_color(color, handle=std_out_handle):
    result = windll.kernel32.SetConsoleTextAttribute(handle, color)
    return result

def PrintRed(s):
    set_color(COLOR_RED)
    print(s)
    set_color(COLOR_WHITE)

def PrintGreen(s):
    set_color(COLOR_GREEN)
    print(s)
    set_color(COLOR_WHITE)

def PrintYellow(s):
    set_color(COLOR_YELLOW)
    print(s)
    set_color(COLOR_WHITE)

def PrintBlue(s):
    set_color(COLOR_BLUE)
    print(s)
    set_color(COLOR_WHITE)

def PrintWhite(s):
    set_color(COLOR_WHITE)
    print(s)
    set_color(COLOR_WHITE)

#目录设定
excel_ini_dir = "/Excel_Ini/"
excel_struct_dir = "/Excel_Struct/"

xml_ini_dir = "/Ini/NPC/"
xml_struct_dir = "/Struct/Class/"
logic_class_file = "Struct/LogicClass.xml"

def excel_to_xml(path, file):
    print("Logic config file translate to xml : " + path + file)
    xls_file = xlrd.open_workbook(path + file)

    #新建一个xml文件
    xml_doc = minidom.Document()
    xml_doc.appendChild(xml_doc.createComment("this is a class xml"))

    # 写入XML root标签
    root_node = xml_doc.createElement("XML")
    xml_doc.appendChild(root_node)

    for sheet_index in range(0, xls_file.nsheets):
        sheet_name = xls_file.sheet_names()[sheet_index]
        sheet_data = xls_file.sheets()[sheet_index]
        sheet_row_count = sheet_data.nrows
        sheet_col_count = sheet_data.ncols
        if sheet_name.lower() == "property":
            #写入Propertys标签
            property_nodes = xml_doc.createElement("Propertys")
            root_node.appendChild(property_nodes)
            #开始写Property数据
            for row_num in range(1, sheet_row_count):
                property_node = xml_doc.createElement("Property")
                property_nodes.appendChild(property_node)

                for col_num in range(0, sheet_col_count):
                    attr_name = sheet_data.row_values(0)[col_num]
                    attr_value = sheet_data.row_values(row_num)[col_num]
                    if type(attr_value) == float:
                        attr_value = int(attr_value)
                    property_node.setAttribute(str(attr_name), str(attr_value))
        elif sheet_name.lower() == "component":
            #TODO component的处理
            print("next step is to process component")
        else:
            #写入Records标签
            record_nodes = xml_doc.createElement("Records")
            root_node.appendChild(record_nodes)
            #0行是标签名 0行0列-0行9列是Record信息值 0行10列-0行n列是record中的列名
            #1行是标签的值
            #2行10列-2行n列是record列名的值
            #for record_row_num in range(1, record_row_count):
            record_node = xml_doc.createElement("Record")
            record_nodes.appendChild(record_node)

            set_col_num = 0
            for record_col_num in range(0, 10):
                record_info_name = sheet_data.row_values(0)[record_col_num]
                record_info_value = sheet_data.row_values(1)[record_col_num]
                #因为xlrd中将int默认读取为float
                if type(record_info_value) == float:
                    record_info_value = int(record_info_value)

                record_node.setAttribute(str(record_info_name), str(record_info_value))
                if record_info_name == "Col":
                    set_col_num = record_info_value

            for record_col_num in range(10, 10 + int(set_col_num)):
                col_name = sheet_data.row_values(0)[record_col_num]
                col_value = sheet_data.row_values(1)[record_col_num]
                #col_default = record_table.row_values(2)[record_col_num]
                #写入Col标签
                col_node = xml_doc.createElement("Col")
                record_node.appendChild(col_node)

                col_node.setAttribute(str("Type"), str(col_value))
                col_node.setAttribute(str("Tag"), str(col_name))
    #####################################################################################
    include_nodes = xml_doc.createElement("Includes")
    root_node.appendChild(include_nodes)

    component_nodes = xml_doc.createElement("Components")
    root_node.appendChild(component_nodes)
    #####################################################################################
    xml_name = file.rsplit(".")[0]
    xml_file = open(path + "../" + xml_struct_dir + xml_name + str(".xml"), "w")
    xml_doc.writexml(xml_file, indent="\n", addindent="    ", newl='', encoding="utf-8")
    xml_file.close()

# 遍历某个文件夹下的文件，生成xml
def traverse_struct_xls(path):
    print("Logic config file path is : " + path)
    xml_doc = minidom.Document()
    xml_doc.appendChild(xml_doc.createComment("this is logic class xml"))
    root_node = xml_doc.createElement("XML")
    xml_doc.appendChild(root_node)

    for root, dirs, files in os.walk(path):
        for file in files:
            if os.path.isfile(root + file):
                prename = str(file).rsplit(".")[0]
                ext = os.path.splitext(file)[1]  #此处ext还带着点
                #print(ext)
                if ext == ".xls" or ext == ".xlsx":
                    #print("Filename:" + file)
                    class_node = xml_doc.createElement("Class")
                    root_node.appendChild(class_node)
                    class_node.setAttribute(str("Id"), str(prename))
                    class_node.setAttribute(str("Type"), str("TYPE_") + str(prename).upper())
                    class_node.setAttribute(str("Path"),
                                            str("../../NFDataCfg") + xml_struct_dir + str(prename) + str(".xml"))
                    class_node.setAttribute(str("InstancePath"),
                                            str("../../NFDataCfg") + xml_ini_dir + str(prename) + str(".xml"))
                    class_node.setAttribute(str("Public"), str("0"))
                    class_node.setAttribute(str("Desc"), str(prename))
                    #print(file + ", function:traverse_struct_xls")
                    excel_to_xml(root, file)

    xml_file = open(path + "../" + str(logic_class_file), "w")
    xml_doc.writexml(xml_file, indent="\n", addindent="    ", newl='', encoding="utf-8")
    xml_file.close()

#遍历Excel_Ini中的文件 数据配置
def traverse_ini_xls(path):
    print("Data config file path is : " + path)
    for root, dirs, files in os.walk(path):
        for file in files:
            #print(root)
            #print(file)
            if os.path.isfile(root + file):
                prename = str(file).rsplit(".")[0]
                ext = os.path.splitext(file)[1]  #此处ext还带着点
                if ext == ".xls" or ext == ".xlsx":
                    print("Data config file translate to xml : " + root + file)
                    #打开Excel文件
                    xls_file = xlrd.open_workbook(root + file)
                    sheet_data = xls_file.sheets()[0]

                    #新建xml配置文件
                    xml_doc = minidom.Document()
                    xml_doc.appendChild(xml_doc.createComment("this is config xml"))
                    root_node = xml_doc.createElement("XML")
                    xml_doc.appendChild(root_node)

                    for ini_row_num in range(1, sheet_data.nrows):
                        object_node = xml_doc.createElement("Object")
                        root_node.appendChild(object_node)
                        for ini_col_num in range(0, sheet_data.ncols):
                            cell_name = sheet_data.row_values(0)[ini_col_num]
                            cell_value = sheet_data.row_values(ini_row_num)[ini_col_num]
                            if type(cell_value) == float:
                                cell_value = int(cell_value)

                            #写入具体数据
                            object_node.setAttribute(str(cell_name), str(cell_value))
                    #####################################################################################
                    xml_file = open(path + "../" + xml_ini_dir + prename + str(".xml"), "w")
                    xml_doc.writexml(xml_file, indent="\n", addindent="    ", newl='', encoding="utf-8")
                    xml_file.close()

#main
if __name__ == "__main__":
    current_dir = os.getcwd()
    print("Current execution path is : " + current_dir)

    #遍历Excel_Struct中的文件
    traverse_struct_xls(current_dir + "/../" + excel_struct_dir)

    #遍历Excel_Ini中的文件
    traverse_ini_xls(current_dir + "/../" + excel_ini_dir)

    #excel_to_xml("Player.xlsx")
    PrintGreen("Congratulations，all excel files translate to xml successed!")
    os.system("pause")