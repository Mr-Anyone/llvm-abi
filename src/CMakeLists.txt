set(
    LIB_SOURCE_FILES
    Call.cpp
    Type.cpp
    Function.cpp
)

add_library(
    LLVMAbi
    ${LIB_SOURCE_FILES}
)

if(MSVC)
  target_compile_options(LLVMAbi PRIVATE /W4 /WX)
else()
    target_compile_options(LLVMAbi PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(LLVMAbi LLVMCast)
target_include_directories(LLVMAbi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
