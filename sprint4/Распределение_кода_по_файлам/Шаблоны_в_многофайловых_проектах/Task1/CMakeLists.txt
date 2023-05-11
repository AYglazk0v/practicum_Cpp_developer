cmake_minimum_required(VERSION 3.12)
project(SearchServer)

set(CMAKE_CXX_STANDARD 20)

# Добавьте пути к исходным файлам вашего проекта
set(SOURCES
        document.cpp
        document.h
        main.cpp
        paginator.h
        read_input_functions.cpp
        read_input_functions.h
        request_queue.cpp
        request_queue.h
        search_server.cpp
        search_server.h
        string_processing.cpp
        string_processing.h
)

# Создайте исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

