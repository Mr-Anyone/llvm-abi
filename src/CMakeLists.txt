llvm_map_components_to_libnames(llvm_libs support)

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
  target_compile_options(LLVMAbi PRIVATE /W4)
else()
    target_compile_options(LLVMAbi PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(LLVMAbi ${llvm_libs})
target_include_directories(LLVMAbi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
