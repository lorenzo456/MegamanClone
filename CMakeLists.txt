cmake_minimum_required(VERSION 3.16)
project(MyGame)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add sources
# set(SOURCES
#     main.cpp
#     player.cpp
#     obstacle.cpp
#     bullet.cpp
#     enemy.cpp
#     boss.cpp
# )

# Add all .cpp files from src/ automatically
file(GLOB SOURCES "src/*.cpp")

include_directories(include)


# Find Raylib
if(WIN32)
    # Windows: specify include and lib paths
    set(RAYLIB_INCLUDE_DIR "C:/raylib/include")
    set(RAYLIB_LIBRARY "C:/raylib/lib/libraylib.a") # or .lib if dynamic
else()
    # Linux: use pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(RAYLIB REQUIRED raylib)
    include_directories(${RAYLIB_INCLUDE_DIRS})
    link_directories(${RAYLIB_LIBRARY_DIRS})
endif()

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link Raylib and system libraries
if(WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARY} opengl32 gdi32 winmm shlwapi)
else()
    target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARIES})
endif()
