#
# Copyright 2013-2014 Will Mason
# 
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

IF(CURSES_HAVE_CURSES_H)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_CURSES_H)
ELSEIF(CURSES_HAVE_NCURSES_CURSES_H)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_NCURSES_CURSES_H)
ELSEIF(CURSES_HAVE_NCURSES_H)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_NCURSES_H)
ELSEIF(CURSES_HAVE_NCURSES_NCURSES_H)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_NCURSES_NCURSES_H)
ENDIF()

INCLUDE_DIRECTORIES(../common)

ADD_EXECUTABLE(chucho-config
               cancellable.cpp
               cancellable.hpp
               chucho_curses.h
               chucho_keys.hpp
               controller.cpp
               controller.hpp
               logger_win.cpp
               logger_win.hpp
               loggers_win.cpp
               loggers_win.hpp
               main.cpp
               scrollable.cpp
               scrollable.hpp
               util.cpp
               util.hpp
               win_size.hpp)
TARGET_LINK_LIBRARIES(chucho-config common-config ${CURSES_LIBRARIES})
