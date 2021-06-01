find_package(Catch2 REQUIRED)

add_executable(fheap_test
  fheap_test.cpp
  )

add_executable(fheap_test_complex
  fheap_test.cpp
  )

target_include_directories(fheap_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(fheap_test PUBLIC fheap Catch2::Catch2)
target_compile_features(fheap_test PUBLIC cxx_std_17)
target_compile_definitions(fheap_test PRIVATE NUM_TEST_REPEAT=1)

target_include_directories(fheap_test_complex PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(fheap_test_complex PUBLIC fheap Catch2::Catch2)
target_compile_features(fheap_test_complex PUBLIC cxx_std_17)
target_compile_definitions(fheap_test_complex PRIVATE NUM_TEST_REPEAT=20)




