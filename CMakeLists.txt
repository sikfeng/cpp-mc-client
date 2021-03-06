cmake_minimum_required (VERSION 3.5)
project (main)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SOURCES "./src/*.cpp")
file(GLOB_RECURSE HEADERS "./include/*.hpp")
include_directories(include)
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

#Configure easlylogging++
set(LOGGER_DEFINITIONS "-DELPP_THREAD_SAFE -DELPP_FEATURE_PERFORMANCE_TRACKING")
if (LINUX)
  set(LOGGER_DENITIONS "${LOGGER_DEFINITIPNS} -DELPP_FEATURE_CRASH_LOG -DELPP_STL_LOGGING")
endif ()
add_definitions(${LOGGER_DEFINITIONS})

#Set compiler's flags and setup platfrom-dependent libraries
if (CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wno-unused-parameter")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -p")
    set(PLATFORM_LIBRARIES "stdc++fs")
endif ()

if(MSVC AND CMAKE_BUILD_TYPE MATCHES Release)
	target_compile_definitions(${PROJECT_NAME} PRIVATE USE_SDL_MAIN)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
endif(MSVC AND CMAKE_BUILD_TYPE MATCHES Release)

target_link_libraries(${PROJECT_NAME} ${PLATFORM_LIBRARIES})

#Setup dependencies (GLEW, glm, nlohmann/json, easylogging++)
add_subdirectory(external)
target_include_directories(${PROJECT_NAME} PUBLIC external/include)
target_link_libraries(${PROJECT_NAME} deps)

#Setup SDL2
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIR})

#Setup SDL2_net
find_package(SDL2_net REQUIRED)
target_link_libraries(${PROJECT_NAME} ${SDL2_NET_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_NET_INCLUDE_DIRS})

#Setup Zlib
find_package(ZLIB REQUIRED)
target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${ZLIB_INCLUDE_DIRS})

