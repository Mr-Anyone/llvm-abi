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

target_include_directories(LLVMAbi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
