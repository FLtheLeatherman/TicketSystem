cmake_minimum_required(VERSION 3.22)

project(TicketSystem)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(code
        src/utility.cpp
        src/user.cpp
        src/train.cpp
        src/ticket.cpp
        src/main.cpp
)