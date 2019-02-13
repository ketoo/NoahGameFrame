@echo off 
echo cleaning......

echo  *.aps *.idb *.ncp *.obj *.pch *.tmp *.sbr  
echo  *.tmp *.pdb *.bsc *.ilk *.res *.ncb *.opt   
echo  *.suo *.manifest  *.dep  

del /f /q /s *.aps
del /f /q /s *.idb
del /f /q /s *.ncp
del /f /q /s *.obj
del /f /q /s *.pch
del /f /q /s *.tmp
del /f /q /s *.sbr
del /f /q /s *.pdb
del /f /q /s *.bsc
del /f /q /s *.ilk
del /f /q /s *.res
del /f /q /s *.ncb
del /f /q /s *.opt
del /f /q /s *.suo
del /f /q /s *.dep
del /f /q /s *.manifest
echo done! 
echo. & pause 