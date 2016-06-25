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
ADD_EXECUTABLE(configure-chucho
               boolean_editable_item.cpp
               boolean_editable_item.hpp
               brain.cpp
               brain.hpp
               cerr_writer_item.hpp
               cout_writer_item.hpp
               creator_item.cpp
               creator_item.hpp
               creator_from_list_item.cpp
               creator_from_list_item.hpp
               editable_item.cpp
               editable_item.hpp
               emittable.cpp
               emittable.hpp
               file_writer_item.cpp
               file_writer_item.hpp
               filter_creator_item.cpp
               filter_creator_item.hpp
               filter_item.hpp
               from_list_editable_item.cpp
               from_list_editable_item.hpp
               level_editable_item.cpp
               level_editable_item.hpp
               level_threshold_filter_item.cpp
               level_threshold_filter_item.hpp
               logger_creator_item.cpp
               logger_creator_item.hpp
               logger_editor.cpp
               logger_editor.hpp
               logger_editable_item.cpp
               logger_editable_item.hpp
               logger_win_controller.cpp
               logger_win_controller.hpp
               main.cpp
               pattern_formatter_writer_item.cpp
               pattern_formatter_writer_item.hpp
               text_editable_item.cpp
               text_editable_item.hpp
               writer_item.cpp
               writer_item.hpp
               writer_creator_item.cpp
               writer_creator_item.hpp
               ${CHUCHO_UI_GEN}
               ${CHUCHO_RES_GEN})
TARGET_LINK_LIBRARIES(configure-chucho chucho Qt4::QtGui)