set(CONFER_EXAMPLES
    "example"
)

function(add_example source executable)
    add_executable(${executable} ${source})
    target_link_libraries(${executable} confer)
endfunction()

foreach(example ${CONFER_EXAMPLES})
    set(example_source "${CMAKE_CURRENT_SOURCE_DIR}/${example}.cpp")
    string(REPLACE "/" "_" example_executable confer_${example})
    add_example(${example_source} ${example_executable})
endforeach()
