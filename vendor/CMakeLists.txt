
# A jank fix for casting 
add_library(
    LLVMCast
    INTERFACE
)

target_include_directories(LLVMCast INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
