cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project (BigBoxEngine)

find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# we do this to avoid having to manually add new source files to the CMakeLists.txt
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.cpp")

add_executable (${PROJECT_NAME} BigBoxEngine.cpp ${ENGINE_SOURCES})

# allows us to include headers from the Source folder without having to specify the full path
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Source)

# Disable RTTI and exceptions, use utf-8 encoding for MSVC
if(MSVC)
    string(REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    target_compile_options(${PROJECT_NAME} PRIVATE /GR- /EHs-c- /utf-8)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _HAS_EXCEPTIONS=0)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -fno-rtti -fno-exceptions)
endif()

# nlohmann/json
target_compile_definitions(${PROJECT_NAME} PRIVATE JSON_NOEXCEPTION)

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE imguizmo::imguizmo)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)



file(COPY ${CMAKE_SOURCE_DIR}/Data DESTINATION ${CMAKE_BINARY_DIR})

configure_file(
    "${CMAKE_SOURCE_DIR}/launch.vs.json"
    "${CMAKE_SOURCE_DIR}/.vs/launch.vs.json"
    COPYONLY
)
