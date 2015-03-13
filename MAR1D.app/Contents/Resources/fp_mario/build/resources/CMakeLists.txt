file( GLOB_RECURSE pattern_files RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}/" "*" )
foreach( pattern_file ${pattern_files} )
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${pattern_file}"
        COMMAND cmake -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/${pattern_file}"
        "${CMAKE_CURRENT_BINARY_DIR}/${pattern_file}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${pattern_file}"
        )
    list( APPEND FILES "${pattern_file}" )
endforeach( pattern_file )
add_custom_target(resources ALL DEPENDS ${FILES} )
