cmake_minimum_required(VERSION 3.12)

# set the project name
project(ctest C)

# specify the C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)


set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)

add_executable(main src/main.c src/utils.c src/pipeline.c src/particlefuncs.c src/bitfunctions.c src/particlefacts.c)
if(MSVC)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /MT")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
endif()


target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/include)


find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)
find_package(cglm CONFIG REQUIRED)
target_link_libraries(main PRIVATE cglm::cglm)
find_package(GLEW REQUIRED)
target_link_libraries(main PRIVATE GLEW::GLEW)

