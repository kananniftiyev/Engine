set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)


add_executable(app WIN32 ${SRC_FILES} main.cpp)

find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)


target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(app d3d11 d3dcompiler engine dxgi imgui::imgui editor implot::implot)

