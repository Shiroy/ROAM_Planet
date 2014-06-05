cmake_minimum_required(VERSION 2.6)

if(UNIX)
    if(EXISTS "/usr/local/lib/OGRE/cmake")
        set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake;${CMAKE_MODULE_PATH}")
    elseif(EXISTS "/usr/lib/OGRE/cmake")
        set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake;${CMAKE_MODULE_PATH}")
    elseif(EXISTS "/usr/share/OGRE/cmake")
        set(CMAKE_MODULE_PATH "/usr/share/OGRE/cmake/modules;${CMAKE_MODULE_PATH}")
    else()
        message(SEND_ERROR "Failed to find module path.")
    endif()
endif()

find_package(OGRE REQUIRED)

find_package(OIS REQUIRED)

if(NOT OIS_FOUND)
    message(SEND_ERROR "OIS is needed")
endif()

file(GLOB_RECURSE src src/*.cpp)
file(GLOB_RECURSE hdr src/*.h)

add_executable(roam ${src} ${hdr})

include_directories(${OIS_INCLUDE_DIRS} ${OGRE_INCLUDE_DIRS})

target_link_libraries(roam ${OGRE_LIBRARIES} ${OIS_LIBRARIES} boost_system noise)
