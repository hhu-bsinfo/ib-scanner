project(IbPerfMon)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${IBPERFMONITOR_SRC_DIR})

set(SOURCE_FILES
        ${IBPERFMONITOR_SRC_DIR}/IbPerfMon/MonitorWindow.cpp
        ${IBPERFMONITOR_SRC_DIR}/IbPerfMon/IbPerfMon.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -I/usr/include/infiniband/")

target_link_libraries(${PROJECT_NAME} IbPerfLib CursesLib -libverbs -libmad -libnetdisc)
