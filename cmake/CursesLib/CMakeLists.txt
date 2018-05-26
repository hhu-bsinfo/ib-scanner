# Copyright (C) 2018 Heinrich-Heine-Universitaet Duesseldorf,
# Institute of Computer Science, Department Operating Systems
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>

project(CursesLib)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${IBPERFMONITOR_SRC_DIR})
 
set(SOURCE_FILES
        ${IBPERFMONITOR_SRC_DIR}/CursesLib/WindowManager.cpp
        ${IBPERFMONITOR_SRC_DIR}/CursesLib/Window.cpp
        ${IBPERFMONITOR_SRC_DIR}/CursesLib/MessageWindow.cpp
        ${IBPERFMONITOR_SRC_DIR}/CursesLib/OkMessageWindow.cpp
        ${IBPERFMONITOR_SRC_DIR}/CursesLib/YesNoMessageWindow.cpp
        ${IBPERFMONITOR_SRC_DIR}/CursesLib/ListWindow.cpp)
 
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

target_link_libraries(${PROJECT_NAME} -lncurses -lpthread)