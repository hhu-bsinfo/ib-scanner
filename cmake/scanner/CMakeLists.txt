project(scanner)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${IBSCANNER_SRC_DIR})

set(SOURCE_FILES
        ${IBSCANNER_SRC_DIR}/scanner/BuildConfig.cpp
        ${IBSCANNER_SRC_DIR}/scanner/MonitorWindow.cpp
        ${IBSCANNER_SRC_DIR}/scanner/Scanner.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -I/usr/include/infiniband/")

target_link_libraries(${PROJECT_NAME} detector curses -libverbs -libmad -libnetdisc)
