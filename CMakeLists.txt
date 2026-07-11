cmake_minimum_required(VERSION 3.16)
project(BookFlow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

set(BOOKFLOW_SOURCES
    src/Book.cpp
    src/Reader.cpp
    src/Loan.cpp
    src/Reservation.cpp
    src/Category.cpp
    src/Settings.cpp
    src/Utils.cpp
    src/Logger.cpp
    src/FileManager.cpp
    src/SearchEngine.cpp
    src/ReportGenerator.cpp
    src/Library.cpp
    src/Application.cpp
)

add_library(bookflow_core ${BOOKFLOW_SOURCES})

add_executable(BookFlow src/main.cpp)
target_link_libraries(BookFlow PRIVATE bookflow_core)

enable_testing()
add_executable(BookFlowTests tests/test_library.cpp)
target_link_libraries(BookFlowTests PRIVATE bookflow_core)
add_test(NAME BookFlowTests COMMAND BookFlowTests)
