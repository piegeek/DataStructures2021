find_package(Catch2 REQUIRED)

add_executable(AVLTree_test
  AVLTree_test.cpp
  )

target_include_directories(AVLTree_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(AVLTree_test PUBLIC AVLTree Catch2::Catch2)

target_compile_features(AVLTree_test PUBLIC cxx_std_17)
