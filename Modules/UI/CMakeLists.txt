# Copyright 2009,2010 Martin Brentnall
#
# This file is part of IsoRealms.
#
# IsoRealms is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# IsoRealms is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
include_directories(${PROJECT_SOURCE_DIR})

find_package(Lua REQUIRED)
if(LUA_FOUND)
  include_directories(${LUA_INCLUDE_DIR})
else(LUA_FOUND)
  message(STATUS "Lua not found.")
endif(LUA_FOUND)

add_library(UI MODULE
  Layout/Editor/LayoutEditor.cpp
  Layout/Layout.cpp
  Layout/LayoutComponent.cpp
  Layout/LayoutComponentEdge.cpp
  Menu/Menu.cpp
  Prompt/Prompt.cpp
  Resources/Client/LayoutLocation.cpp
  Resources/Client/LayoutOffset.cpp
  Resources/Client/MenuItem.cpp
  Resources/Registry/LayoutLocationRegistry.cpp
  Resources/Registry/LayoutOffsetRegistry.cpp
  Resources/Registry/MenuItemRegistry.cpp
  Resources/Fixed/LayoutOffset/LayoutOffsetAbsolute.cpp
  Resources/Fixed/LayoutOffset/LayoutOffsetLinked.cpp
  Resources/Fixed/LayoutLocation/LayoutLocationAbsolute.cpp
  Resources/Fixed/LayoutLocation/LayoutLocationRelative.cpp
  Resources/Fixed/MenuItem/MenuItemAction.cpp
  Resources/Fixed/MenuItem/MenuItemBoolean.cpp
  Resources/Fixed/MenuItem/MenuItemDigitalInput.cpp
  Resources/Fixed/MenuItem/MenuItemDisplayResolution.cpp
  Resources/Fixed/MenuItem/MenuItemFileList.cpp
  Resources/Fixed/MenuItem/MenuItemLauncherList.cpp
  Resources/Fixed/MenuItem/MenuItemSlider.cpp
  Resources/Fixed/Screen/ScreenGradient.cpp
  Resources/Fixed/Screen/ScreenModel.cpp
  Resources/Fixed/Screen/ScreenPanel.cpp
  Resources/Fixed/Screen/ScreenText.cpp
  Resources/Fixed/String/StringTime.cpp
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
