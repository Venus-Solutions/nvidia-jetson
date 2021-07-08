cmake_minimum_required (VERSION 2.8)

project(face_detect)

set(CURL_LIBRARY "-lcurl")

find_package(OpenCV REQUIRED)
find_package(CUDA)
find_package(CURL REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CURL_INCLUDE_DIR})

add_executable(cv_face_detect face_detect.cpp)

target_link_libraries(cv_face_detect ${OpenCV_LIBS} ${CURL_LIBRARIES})