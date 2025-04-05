add_executable(
    test_abi
    test.cpp
)

add_executable(
    test_type
    type.cpp
)

target_link_libraries(test_abi  LLVMAbi)
target_link_libraries(test_type  LLVMAbi)
