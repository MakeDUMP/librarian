# --[ Project ] ---------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)
project(librarian LANGUAGES CXX)
# set(CMAKE_VERBOSE_MAKEFILE ON)

# --[ Sources ] ---------------------------------------------------------------
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB_RECURSE SOURCES_TO_REMOVE ${CMAKE_CURRENT_LIST_DIR}/src/_*.cpp)
if(NOT ${SOURCES_TO_REMOVE} STREQUAL "")
    list(REMOVE_ITEM SOURCES ${SOURCES_TO_REMOVE})
endif()

# --[ Execute ] ---------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
# set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON) # -fPIC
set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_RPATH ".")

# --[ Flags, Options, Defines ] -----------------------------------------------
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
# target_compile_definitions(${PROJECT_NAME} PRIVATE MY_DEFINE)
# target_include_directories(${PROJECT_NAME} PRIVATE src)
if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -Wl,-static,-lpthread)
endif()

# --[ Libraries ] -------------------------------------------------------------
add_subdirectory(logger)
target_link_libraries(${PROJECT_NAME} logger)
