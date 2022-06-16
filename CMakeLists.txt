cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME Portal)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

#set (SOURCE_DIR ${PROJECT_SOURCE_DIR}/Source source)
#set (VENDOR_DIR ${PROJECT_SOURCE_DIR}/Dependencies vendor)
#set (GLAD_DIR ${PROJECT_SOURCE_DIR}/Dependencies/GLAD)

file(GLOB_RECURSE SOURCE_FILEGAME "${PROJECT_SOURCE_DIR}/game/src/*.cpp")
file(GLOB_RECURSE INCLUDE_FILEGAME "${PROJECT_SOURCE_DIR}/game/include/*.h")
file(GLOB_RECURSE SOURCE_FILEGL "${PROJECT_SOURCE_DIR}/libGL/src/*.cpp")
file(GLOB_RECURSE INCLUDE_FILEGL "${PROJECT_SOURCE_DIR}/libGL/include/*.h")

add_executable(${PROJECT_NAME}
			${SOURCE_FILEGAME}
			${INCLUDE_FILEGAME}
			${SOURCE_FILEGL}
			${INCLUDE_FILEGL})

source_group("Source Files\\OpenGL" FILES ${SOURCE_FILEGL})
source_group("Header Files\\OpenGL" FILES ${INCLUDE_FILEGL})
source_group("Source Files\\Portal" FILES ${SOURCE_FILEGAME})
source_group("Header Files\\Portal" FILES ${INCLUDE_FILEGAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY

VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

#add GLAD
target_include_directories(${PROJECT_NAME} PUBLIC
	"external/GLAD/include")
target_sources(${PROJECT_NAME} PUBLIC "external/GLAD/src/glad.cpp")

#add GLFW
set(GLFW3_ROOT "${PROJECT_SOURCE_DIR}/external/GLFW")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake")
find_package(GLFW3 REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW3_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${GLFW3_LIBRARY})

#add Bullet
set(BULLET_ROOT "external/BULLET")
find_package(Bullet REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${BULLET_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC
	debug ${BULLET_COLLISION_LIBRARY_DEBUG}
	debug ${BULLET_DYNAMICS_LIBRARY_DEBUG}
	debug ${BULLET_LINEAR_MATH_DEBUG}
	optimized ${BULLET_COLLISION_LIBRARY}
	optimized ${BULLET_DYNAMICS_LIBRARY}
	optimized ${BULLET_LINEAR_MATH}
)

#add ASSIMP
set(ASSIMP_ROOT "external/ASSIMP")
find_package(Assimp REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC
	debug ${ASSIMP_LIBRARY_DEBUG}
	optimized ${ASSIMP_LIBRARY}
)

#add RapidJASON
target_include_directories(${PROJECT_NAME} PUBLIC "external/RapidJASON")

#add STB
target_include_directories(${PROJECT_NAME} PUBLIC "external/STB")

#add IrrKlang
target_include_directories(${PROJECT_NAME} PUBLIC "external/IrrKlang/include")
target_link_libraries(${PROJECT_NAME} PUBLIC "../external/IrrKlang/lib/irrKlang")

#add Maths Library
set(MATHS_DIR "libmaths/LibMaths")
file(GLOB_RECURSE MATH_SOURCES "${MATHS_DIR}/*.cpp" "${MATHS_DIR}/*.h")
add_library(libMaths STATIC EXCLUDE_FROM_ALL "${MATH_SOURCES}")
target_include_directories(libMaths PRIVATE ${MATHS_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${MATHS_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${MATHS_LIB})

#add ImGUI
file(GLOB_RECURSE IMGUI_SOURCES "external/ImGUI/*.cpp" "external/ImGUI/*.h")
add_library(ImGui STATIC EXCLUDE_FROM_ALL "${IMGUI_SOURCES}")
target_include_directories(ImGui PUBLIC "external/ImGUI")
target_include_directories(ImGui PUBLIC ${GLFW3_INCLUDE_DIR})
target_include_directories(ImGui PUBLIC "external/GLAD/include")
target_include_directories(ImGui PUBLIC ${MATHS_DIR})
target_include_directories(ImGui PUBLIC ${ASSIMP_INCLUDE_DIR})
target_include_directories(ImGui PUBLIC ${BULLET_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ImGui)