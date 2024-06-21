cmake_minimum_required(VERSION 3.27.0)
project(example VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()


function(make_includable input_file output_file)
    file(READ ${input_file} content)
    set(delim "for_c++_include")
    set(content "R\"${delim}(\n${content})${delim}\"")
    file(WRITE ${output_file} "${content}")
endfunction(make_includable)



set(FOLDER_PATH "shaders")

file(GLOB FILES_IN_FOLDER "${FOLDER_PATH}/*")

message("Files in folder:")
foreach(FILE ${FILES_IN_FOLDER})
    get_filename_component(FILE_NAME ${FILE} NAME)
    make_includable(${FILE} generated/${FILE_NAME})
endforeach()

set(RESOURCES_PATH "resources")

file(GLOB RESOURCES "${RESOURCES_PATH}/*")


foreach(TEXTURE ${RESOURCES})
    get_filename_component(filename ${TEXTURE} NAME)
    get_filename_component(parent ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY)
    set(TEX ${parent}/resources/${filename})
    add_custom_command(
        OUTPUT ${TEX}
        COMMAND ${CMAKE_COMMAND} -E copy ${TEXTURE}  ${TEX}
        DEPENDS ${TEXTURE}
    )
    list(APPEND RESOURCE_FILES ${TEX})
endforeach()

add_custom_target(CopyResources ALL DEPENDS ${RESOURCE_FILES})

add_executable(${PROJECT_NAME} triangle.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_link_libraries(${PROJECT_NAME} PUBLIC framework)

add_dependencies(${PROJECT_NAME} CopyResources)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)