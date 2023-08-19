project(Fwogisms)

add_custom_target(copy_data ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)   

set(sourceFiles
    Main.cpp
    Application.cpp
    FwogismsApplication.cpp
)

add_executable(Fwogisms ${sourceFiles})
add_dependencies(Fwogisms copy_data)

target_include_directories(Fwogisms PRIVATE Include)
target_link_libraries(Fwogisms PRIVATE fwog glad glfw imgui glm stb_image spdlog TracyClient debugbreak)