cmake_minimum_required(VERSION 3.16)
project(ObjectOrientedLibraryManager LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIBRARY_SOURCES
    src/Library.cpp
    src/Storage.cpp
    src/Book.cpp
    src/Member.cpp
    src/Loan.cpp
)

add_executable(library_manager src/main.cpp ${LIBRARY_SOURCES})
target_include_directories(library_manager PRIVATE include)

add_executable(library_tests tests/test_library.cpp ${LIBRARY_SOURCES})
target_include_directories(library_tests PRIVATE include)

enable_testing()
add_test(NAME LibraryManagerTests COMMAND library_tests)

if(MSVC)
    target_compile_options(library_manager PRIVATE /W4)
    target_compile_options(library_tests PRIVATE /W4)
else()
    target_compile_options(library_manager PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(library_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()
