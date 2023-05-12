cmake_minimum_required(VERSION 3.20)
project(SearchServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        document.cpp
        document.h
        main.cpp
        paginator.h
        profiling.cpp
        profiling.h
	log_duration.h
        read_input_functions.cpp
        read_input_functions.h
        request_queue.cpp
        request_queue.h
        search_server.cpp
        search_server.h
        string_processing.cpp
        string_processing.h
)

add_executable(${PROJECT_NAME} ${SOURCES})

