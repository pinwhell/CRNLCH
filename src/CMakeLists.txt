add_library(CRNLCH CRNLCH.cpp Utility.cpp)
target_include_directories(CRNLCH PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_subdirectory(CLI)