cmake_minimum_required(VERSION 3.27)

project(nocmd)

set(CMAKE_CXX_STANDARD 23)

add_executable(nocmd main.cpp)

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif ()

if (WIN32)
    set(CMAKE_MFC_FLAG 2)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
    add_definitions(-D_AFXDLL)
ENDIF ()
