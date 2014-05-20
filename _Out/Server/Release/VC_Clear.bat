@echo off 
echo 正在清除编译产生的垃圾文件，请稍等...... 

del /f /q /s *.obj
del /f /q /s *.pdb
del /f /q /s *.idb
del /f /q /s *.pch
del /f /q /s *.ilk
echo 清除垃圾文件完成！ 
echo. & pause 