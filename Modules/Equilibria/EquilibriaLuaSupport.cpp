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
#include "EquilibriaLuaSupport.h"

#include <sol.hpp>

#include "Assets/Fixed/ZoneViewType/ZoneViewOverview.h"
#include "AlienType/AlienType.h"
#include "PlayerType/PlayerType.h"
#include "Equilibria.h"
#include "TerrainState/TerrainState.h"
#include "ThemeSet/ThemeSet.h"
#include "World/DebrisGenerator/DebrisGenerator.h"
#include "World/Object/Zone/Zone.h"
#include "World/Object/ZoneObject/Traits/Boundary/Boundary.h"
#include "World/Object/ZoneObject/Traits/Model/Model.h"
#include "World/Object/ZoneObject/Traits/Spinner/Spinner.h"
#include "World/World.h"
#include "WorldView/WorldView.h"

namespace IsoRealms {
  template <typename TYPE> void LuaBinding<TYPE>::bind(const std::string& bindFunction) const {
    cDefLuaState[bindFunction](cDefValue);
  }
  
  template class LuaBinding<Equilibria::Alien>;
  template class LuaBinding<Equilibria::AlienType>;
  template class LuaBinding<Equilibria::Boundary>;
  template class LuaBinding<Equilibria::CameraGameplay>;
  template class LuaBinding<Equilibria::CameraTransitional>;
  template class LuaBinding<Equilibria::DebrisGenerator>;
  template class LuaBinding<Equilibria::Model>;
  template class LuaBinding<Equilibria::PickUp>;
  template class LuaBinding<Equilibria::Player>;
  template class LuaBinding<Equilibria::PlayerType>;
  template class LuaBinding<Equilibria::Equilibria>;
  template class LuaBinding<Equilibria::Spinner>;
  template class LuaBinding<Equilibria::TerrainState>;
  template class LuaBinding<Equilibria::ThemeColour>;
  template class LuaBinding<Equilibria::ThemeSet>;
  template class LuaBinding<Equilibria::Wall>;
  template class LuaBinding<Equilibria::World>;
  template class LuaBinding<Equilibria::WorldView>;
  template class LuaBinding<Equilibria::Zone>;
  template class LuaBinding<Equilibria::ZoneViewOverview>;

  namespace Equilibria {

#ifdef __linux__
    extern "C" void initLua(LuaState* luaState) {
#elif _WIN32
    extern "C" void __declspec(dllexport) __stdcall initLua(LuaState* luaState) {
#endif
      sol::state& mLua = luaState->getState();
      mLua.new_usertype<Alien>("Alien",                           "getZone",                  &Alien::getZone);
      mLua.new_usertype<AlienType>("AlienType",                   "setSpinSpeed",             &AlienType::setSpinSpeed);
      mLua.new_usertype<Boundary>("Boundary",                     "isEnabled",                &Boundary::isEnabled,
                                                                  "setEnabled",               &Boundary::setEnabled);
      mLua.new_usertype<CameraGameplay>("CameraGameplay",         "faceNorthWest",            &CameraGameplay::faceNorthWest,
                                                                  "faceNorthEast",            &CameraGameplay::faceNorthEast,
                                                                  "faceSouthWest",            &CameraGameplay::faceSouthWest,
                                                                  "faceSouthEast",            &CameraGameplay::faceSouthEast,
                                                                  "rotateLeft",               &CameraGameplay::rotateLeft,
                                                                  "rotateRight",              &CameraGameplay::rotateRight,
                                                                  "flip",                     &CameraGameplay::flip);
      mLua.new_usertype<CameraTransitional>("CameraTransitional", "transitionToStart",        &CameraTransitional::transitionToStart,
                                                                  "transitionToEnd",          &CameraTransitional::transitionToEnd);
      mLua.new_usertype<DebrisGenerator>("DebrisGenerator",       "generateDebris",           &DebrisGenerator::generateDebris,
                                                                  "clear",                    &DebrisGenerator::clear);
      mLua.new_usertype<Model>("Model",                           "isEnabled",                &Model::isEnabled,
                                                                  "setEnabled",               &Model::setEnabled);
      mLua.new_usertype<Player>("Player",                         "destroy",                  &Player::destroy,
                                                                  "stop",                     &Player::stop,
                                                                  "setStepReach",             &Player::setStepReach,
                                                                  "getLocation",              &Player::getLocation,
                                                                  "getMomentum",              &Player::getMomentum,
                                                                  "setBounceFactor",          &Player::setBounceFactor,
                                                                  "isAlive",                  &Player::isAlive,
                                                                  "getZone",                  &Player::getZone);
      mLua.new_usertype<PlayerType>("PlayerType",                 "setSpinSpeed",             &PlayerType::setSpinSpeed);
      mLua.new_usertype<Equilibria>("Equilibria",                   "setPause",                 &Equilibria::setPause);
      mLua.new_usertype<Spinner>("Spinner",                       "isEnabled",                &Spinner::isEnabled,
                                                                  "setEnabled",               &Spinner::setEnabled);
      mLua.new_usertype<TerrainState>("TerrainState",             "setValue",                 &TerrainState::setValue);
      mLua.new_usertype<ThemeSet>("ThemeSet",                     "setDefaultTheme",          &ThemeSet::setDefaultTheme);
      mLua.new_usertype<Wall>("Wall",                             "isAtZoneEdge",             &Wall::isAtZoneEdge);
      mLua.new_usertype<World>("World",                           "getZoneCount",             &World::getZoneCount,
                                                                  "getPickUpCount",           &World::getPickUpCount);
      mLua.new_usertype<WorldView>("WorldView",                   "setZone",                  &WorldView::setZone);
      mLua.new_usertype<Zone>("Zone",                             "isVisited",                &Zone::isVisited,
                                                                  "getRemainingPickUpCount",  &Zone::getRemainingPickUpCount,
                                                                  "getTheme",                 &Zone::getTheme,
                                                                  "reset",                    &Zone::reset,
                                                                  "resetDynamics",            &Zone::resetDynamics,
                                                                  "resetLifts",               &Zone::resetLifts,
                                                                  "setActive",                &Zone::setActive,
                                                                  "setVisited",               &Zone::setVisited,
                                                                  "getHintCount",             &Zone::getHintCount,
                                                                  "giveHint",                 &Zone::giveHint);
      mLua.new_usertype<ZoneViewOverview>("ZoneViewOverview",     "setColour",                &ZoneViewOverview::setColour,
                                                                  "getColour",                &ZoneViewOverview::getColour);
    }
  }
}
