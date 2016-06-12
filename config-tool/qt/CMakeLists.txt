#
# Copyright 2013-2016 Will Mason
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

SET(CMAKE_AUTOMOC TRUE)
QT4_WRAP_UI(CHUCHO_UI_GEN chucho.ui)
QT4_ADD_RESOURCES(CHUCHO_RES_GEN chucho.qrc)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")
ADD_EXECUTABLE(config-chucho
               brain.cpp
               brain.hpp
               logger_emittable.cpp
               logger_emittable.hpp
               logger_win_controller.cpp
               logger_win_controller.hpp
               main.cpp
               ${CHUCHO_UI_GEN}
               ${CHUCHO_RES_GEN})
TARGET_LINK_LIBRARIES(config-chucho chucho Qt4::QtGui)