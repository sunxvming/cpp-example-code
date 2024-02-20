@echo off

if not exist ".\out" md out

echo start... 
for %%i in (.\proto\*.proto) do (     
    echo %%i ok
    .\bin\protoc.exe -I=.\proto --cpp_out=.\out %%i    
)  
echo end...  
pause 