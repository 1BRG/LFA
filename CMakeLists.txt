cmake_minimum_required(VERSION 3.30)
project(lfa)

set(CMAKE_CXX_STANDARD 14)

add_executable(lfa
        src/main.cpp
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
        include/json.hpp
        src/LFA-Assignment2_Regex_DFA.json
        src/CFG.cpp
        include/CFG.h
)
