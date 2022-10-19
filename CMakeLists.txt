cmake_minimum_required(VERSION 3.5)

option(USE_LIBPNG "Visualize using libPNG" False)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Ant)

if(USE_LIBPNG)
    if(CMAKE_SYSTEM_NAME MATCHES Linux)
        # Судя по всему, в FindZLIB.cmake присутствует баг
        set(ZLIB_ROOT /usr/lib/x86_64-linux-gnu)
        cmake_policy(SET CMP0074 NEW)
    endif()
    find_package(PNG REQUIRED)
endif()


set(SOURCES
    main.cpp
    imaging.h
    finally.h
    tracker.h
    tracker.cpp
    traversal_stack.h
    traversal_queue.h
    point.h
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

if(TARGET PNG::PNG)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_LIBPNG) # прочие дефайны должны добавиться автоматически при линковке PNG::PNG
    target_link_libraries(${PROJECT_NAME} PRIVATE PNG::PNG)
    target_sources(${PROJECT_NAME} PRIVATE
        gio.h
        gio.cpp
    )
endif()