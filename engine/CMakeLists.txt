file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)


add_library (engine STATIC ${SRC_FILES} ${HEADER_FILES})

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET engine PROPERTY CXX_STANDARD 20)
endif()

include(FetchContent)

FetchContent_Declare(
  mINI
  GIT_REPOSITORY https://github.com/metayeti/mINI.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(mINI)

find_package(imgui CONFIG REQUIRED)
 target_link_libraries(engine PRIVATE imgui::imgui editor)

target_include_directories(engine PUBLIC ${mINI_SOURCE_DIR}/src)
message(STATUS ${mINI_SOURCE_DIR})