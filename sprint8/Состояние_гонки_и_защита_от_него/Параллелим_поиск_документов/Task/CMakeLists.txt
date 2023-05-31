cmake_minimum_required(VERSION 3.20)
project(search_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER g++)

add_executable(search_server 
    document.cpp 
    document.h 
    log_duration.h 
    main.cpp 
    paginator.h 
    process_queries.cpp 
    process_queries.h 
    profiling.cpp 
    profiling.h 
    read_input_functions.cpp 
    read_input_functions.h 
    remove_duplicates.cpp 
    remove_duplicates.h 
    request_queue.cpp 
    request_queue.h 
    search_server.cpp 
    search_server.h 
    string_processing.cpp 
    string_processing.h
    concurrent_map.h
)
target_compile_options(search_server PRIVATE -O2)
target_compile_options(search_server PRIVATE -DPARALLEL)
target_link_libraries(search_server PRIVATE -lstdc++ -lpthread -pthread -std=c++20)
