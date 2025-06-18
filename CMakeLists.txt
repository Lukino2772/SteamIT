cmake_minimum_required(VERSION 3.30)
project(library)

set(CMAKE_CXX_STANDARD 14)

include_directories("../../OneDrive/Desktop/Desktop files/library")

add_executable(library
        SteamIT_MPU.cpp
        SteamIT_MPU.h
        KUBit.ino)
