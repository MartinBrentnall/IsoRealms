/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "UILuaSupport.h"

#include <sol.hpp>

#include "Assets/Fixed/MenuItem/MenuItemAction.h"
#include "Assets/Fixed/MenuItem/MenuItemBoolean.h"
#include "Assets/Fixed/MenuItem/MenuItemDigitalInput.h"
#include "Assets/Fixed/MenuItem/MenuItemDisplayResolution.h"
#include "Assets/Fixed/MenuItem/MenuItemFileList.h"
#include "Assets/Fixed/MenuItem/MenuItemLauncherList.h"
#include "Assets/Fixed/MenuItem/MenuItemSlider.h"
#include "Layout/LayoutComponent.h"
#include "Menu/Menu.h"
#include "Prompt/Prompt.h"
#include "UI.h"
#include "VirtualKeyboard/VirtualKeyboard.h"

namespace IsoRealms {
  template <typename TYPE> void LuaBinding<TYPE>::bind(const std::string& bindFunction) const {
    cDefLuaState[bindFunction](cDefValue);
  }
  
  template class LuaBinding<UI::LayoutComponent>;
  template class LuaBinding<UI::Menu>;
  template class LuaBinding<UI::MenuItemAction>;
  template class LuaBinding<UI::MenuItemBoolean>;
  template class LuaBinding<UI::MenuItemDigitalInput>;
  template class LuaBinding<UI::MenuItemDisplayResolution>;
  template class LuaBinding<UI::MenuItemFileList>;
  template class LuaBinding<UI::MenuItemLauncherList>;
  template class LuaBinding<UI::MenuItemSlider>;
  template class LuaBinding<UI::Prompt>;
  template class LuaBinding<UI::VirtualKeyboard>;

  namespace UI {
    bool operator==(const Menu& a, const Menu& b) {
      return &a == &b;
    }
    
    bool operator==(const MenuItemAction& a, const MenuItemAction& b) {
      return &a == &b;
    }
    
    bool operator==(const MenuItemBoolean& a, const MenuItemBoolean& b) {
      return &a == &b;
    }
    
    bool operator==(const MenuItemDigitalInput& a, const MenuItemDigitalInput& b) {
      return &a == &b;
    }

    bool operator==(const MenuItemDisplayResolution& a, const MenuItemDisplayResolution& b) {
      return &a == &b;
    }

    bool operator==(const MenuItemFileList& a, const MenuItemFileList& b) {
      return &a == &b;
    }

    bool operator==(const MenuItemSlider& a, const MenuItemSlider& b) {
      return &a == &b;
    }
    
    bool operator==(const Prompt& a, const Prompt& b) {
      return &a == &b;
    }

    bool operator==(const VirtualKeyboard& a, const VirtualKeyboard& b) {
      return &a == &b;
    }

#ifdef __linux__
    extern "C" void initLua(LuaState* luaState) {
#elif _WIN32
    extern "C" void __declspec(dllexport) __stdcall initLua(LuaState* luaState) {
#endif
      sol::state& mLua = luaState->getState();
      mLua.new_usertype<LayoutComponent>("LayoutComponent",                     "setScreen",       &LayoutComponent::setScreen);
      mLua.new_usertype<Menu>("Menu",                                           "reset",           &Menu::reset);
      mLua.new_usertype<MenuItemAction>("MenuItemAction",                       "setValue",        &MenuItemAction::setValue);
      mLua.new_usertype<MenuItemBoolean>("MenuItemBoolean",                     "setValue",        &MenuItemBoolean::setValue,
                                                                                "getValue",        &MenuItemBoolean::getValue);
      mLua.new_usertype<MenuItemDigitalInput>("MenuItemDigitalInput",           "addMapping",      &MenuItemDigitalInput::addMapping,
                                                                                "clear",           &MenuItemDigitalInput::clear,
                                                                                "getMappingCount", &MenuItemDigitalInput::getMappingCount,
                                                                                "getMapping",      &MenuItemDigitalInput::getMapping);
      mLua.new_usertype<MenuItemDisplayResolution>("MenuItemDisplayResolution", "setValue",        &MenuItemDisplayResolution::setValue,
                                                                                "getValue",        &MenuItemDisplayResolution::getValue);
      mLua.new_usertype<MenuItemLauncherList>("MenuItemLauncherList",           "setProject",      &MenuItemLauncherList::setProject);
      mLua.new_usertype<MenuItemSlider>("MenuItemSlider",                       "setValue",        &MenuItemSlider::setValue,
                                                                                "getValue",        &MenuItemSlider::getValue);
      mLua.new_usertype<Prompt>("Prompt",                                       "reset",           &Prompt::reset);
      mLua.new_usertype<VirtualKeyboard>("VirtualKeyboard",                     "reset",           &VirtualKeyboard::reset);
    }
  }
}
