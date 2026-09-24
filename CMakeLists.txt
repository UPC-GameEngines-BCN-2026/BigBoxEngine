# CMakeList.txt : CMake project for BigBoxEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)          # request C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) # require it
set(CMAKE_CXX_EXTENSIONS OFF)       # optional: disable compiler-specific extensions

project (BigBoxEngine)

find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)


# Disable RTTI for this project
if(MSVC)
    string(REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    add_compile_options(/GR- /EHs-c-)
    add_compile_definitions(_HAS_EXCEPTIONS=0)
else()
    add_compile_options(-fno-rtti -fno-exceptions)
endif()

# Add source to this project's executable.
add_executable (${PROJECT_NAME} BigBoxEngine.cpp)




target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE imguizmo::imguizmo)

# For generating the release, we need to copy assets folder to build folder
file(COPY ${CMAKE_SOURCE_DIR}/Data DESTINATION ${CMAKE_BINARY_DIR})

# Setting custom working directory within root's 'launch.vs.json and copying within .vs folder
configure_file(
    "${CMAKE_SOURCE_DIR}/launch.vs.json"
    "${CMAKE_SOURCE_DIR}/.vs/launch.vs.json"
    COPYONLY
)