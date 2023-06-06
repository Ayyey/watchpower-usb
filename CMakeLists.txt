cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

SET (CMAKE_C_COMPILER_WORKS 1)
SET (CMAKE_CXX_COMPILER_WORKS 1)
SET (CMAKE_CXX_STANDARD 17)

SET(OPENSSL_USE_STATIC_LIBS TRUE)

project(mch_USB_WATCHPOWER VERSION 1.0)

add_subdirectory(./external/libusb-cmake-main/)
add_subdirectory(./external/cpp-httplib-master/)
add_subdirectory(./external/json-develop/)
find_package(OpenSSL REQUIRED)

file(GLOB sources 
    ./*.cpp 
    ./*.hpp
    ./src/*.cpp
    ./src/*.hpp
)

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} usb-1.0)
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)
target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)

target_link_libraries(${PROJECT_NAME} httplib)

target_link_libraries(${PROJECT_NAME} nlohmann_json)

