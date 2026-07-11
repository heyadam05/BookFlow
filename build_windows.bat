@echo off
setlocal

if not exist build mkdir build

g++ -std=c++1z -Iinclude ^
 src/Book.cpp src/Reader.cpp src/Loan.cpp src/Reservation.cpp src/Category.cpp src/Settings.cpp ^
 src/Utils.cpp src/Logger.cpp src/FileManager.cpp src/SearchEngine.cpp src/ReportGenerator.cpp ^
 src/Library.cpp src/Application.cpp src/main.cpp ^
 -o build/BookFlow.exe

g++ -std=c++1z -Iinclude ^
 src/Book.cpp src/Reader.cpp src/Loan.cpp src/Reservation.cpp src/Category.cpp src/Settings.cpp ^
 src/Utils.cpp src/Logger.cpp src/FileManager.cpp src/SearchEngine.cpp src/ReportGenerator.cpp ^
 src/Library.cpp tests/test_library.cpp ^
 -o build/BookFlowTests.exe

echo Build finished.
