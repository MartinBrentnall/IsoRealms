# Copyright 2009,2010 Martin Brentnall
#
# This file is part of Iso-Realms.
#
# Iso-Realms is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Iso-Realms is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
include_directories(${PROJECT_SOURCE_DIR})

find_package(Lua REQUIRED)
if(LUA_FOUND)
  include_directories(${LUA_INCLUDE_DIR})
else(LUA_FOUND)
  message(STATUS "Lua not found.")
endif(LUA_FOUND)

add_library(UI MODULE
  Assets/Client/LayoutLocation.cpp
  Assets/Client/LayoutOffset.cpp
  Assets/Client/MenuItem.cpp
  Assets/Fixed/LayoutOffset/LayoutOffsetAbsolute.cpp
  Assets/Fixed/LayoutOffset/LayoutOffsetLinked.cpp
  Assets/Fixed/LayoutLocation/LayoutLocationAbsolute.cpp
  Assets/Fixed/LayoutLocation/LayoutLocationRelative.cpp
  Assets/Fixed/MenuItem/MenuItemAction.cpp
  Assets/Fixed/MenuItem/MenuItemBoolean.cpp
  Assets/Fixed/MenuItem/MenuItemDigitalInput.cpp
  Assets/Fixed/MenuItem/MenuItemDisplayResolution.cpp
  Assets/Fixed/MenuItem/MenuItemFileList.cpp
  Assets/Fixed/MenuItem/MenuItemSlider.cpp
  Assets/Fixed/Screen/ScreenGradient.cpp
  Assets/Fixed/Screen/ScreenModel.cpp
  Assets/Fixed/Screen/ScreenPanel.cpp
  Assets/Fixed/Screen/ScreenText.cpp
  Assets/Fixed/String/StringTime.cpp
  Layout/Editor/LayoutEditor.cpp
  Layout/Layout.cpp
  Layout/LayoutComponent.cpp
  Layout/LayoutComponentEdge.cpp
  Menu/Menu.cpp
  Prompt/Prompt.cpp
  ScreenFader/ScreenFader.cpp
  Throbber/Throbber.cpp
  VirtualKeyboard/VirtualKeyboard.cpp
  UI.cpp
  UILuaSupport.cpp
)
set_target_properties(UI PROPERTIES OUTPUT_NAME "IsoRealms-UI")
set_target_properties(UI PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../Out")
set_target_properties(UI PROPERTIES PREFIX "")

target_link_libraries(UI IsoRealms ${LUA_LIBRARIES})
