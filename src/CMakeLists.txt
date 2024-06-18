add_library(CRNLCH CRNLCH.cpp Utility.cpp)
target_include_directories(CRNLCH PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(CRNLCH dll-injector)

add_subdirectory(CLI)