@echo off
@setlocal enableextensions
@cd /d "%~dp0"

rem Get the current directory path
for %%A in ("%~dp0.") do (
    set "currentDir=%%~fA"
)

echo Delete and Create the "release" folder and its contents
rd /S /Q "release_win"
rd /S /Q "publish_win"
rd /S /Q "build_win"
mkdir publish_win
mkdir release_win
mkdir build_win

echo Folders are Recreated successfully.

xcopy /E /I /Y "%currentDir%\original_test_files" "%currentDir%"

echo Testing Application with Coverage
echo Configure CMAKE
call cmake -B build_win -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX:PATH=publish_win
echo Build CMAKE Debug/Release
call cmake --build build_win --config Debug -j4
call cmake --build build_win --config Release -j4
call cmake --install build_win --strip

xcopy /E /I /Y "%currentDir%\original_test_files" "%currentDir%\build_win\src\tests\fitness_management_test"

echo Test CMAKE
cd build_win
call ctest -C Debug --output-on-failure
cd ..

echo Running Test Executable

call .\publish_win\bin\fitness_management_tests.exe
call .\publish_win\bin\fitness_management_app.exe

echo Files and folders copied successfully.

echo Package Publish Windows Binaries
xcopy /E /I /Y "%currentDir%\original_test_files" "%currentDir%\publish_win\bin"
tar -czvf release\windows-publish-binaries.tar.gz -C publish .

echo Package Publish Windows Binaries
call robocopy src\fitness_management_lib\header "build_win\build\Release" /E
call robocopy src\fitness_management_app\header "build_win\build\Release" /E
xcopy /E /I /Y "%currentDir%\original_test_files" "%currentDir%\publish_win\bin"
tar -czvf release_win\windows-release-binaries.tar.gz -C build_win\build\Release .

echo Package Publish Debug Windows Binaries
call robocopy src\fitness_management_lib\header "build_win\build\Debug" /E
call robocopy src\fitness_management_app\header "build_win\build\Debug" /E
tar -czvf release_win\windows-debug-binaries.tar.gz -C build_win\build\Debug .

echo ....................
echo Operation Completed!
pause