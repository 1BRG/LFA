cmake_minimum_required(VERSION 3.30)
project(lfa)

set(CMAKE_CXX_STANDARD 14)

add_executable(lfa
        src/lfa1.cpp
        src/Input.cpp
        include/Input.h
        include/States.h
        src/States.cpp
        include/Sigma.h
        src/Sigma.cpp
        include/Transitions.h
        src/Transitions.cpp
        include/Automat.h
        src/Automat.cpp
)
