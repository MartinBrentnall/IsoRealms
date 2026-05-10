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
#include "SpindizzyLuaSupport.h"

#include <sol.hpp>

#include "Assets/Fixed/ZoneViewType/ZoneViewOverview.h"
#include "AlienType/AlienType.h"
#include "ModelCycler/ModelCycler.h"
#include "PlayerType/PlayerType.h"
#include "Spindizzy.h"
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
  
  template <typename TYPE> void LocalLuaBinding<TYPE>::bind(const std::string& bindFunction) const {
    cDefLuaState[bindFunction](cDefValue);
  }

  template class LuaBinding<Spindizzy::Alien>;
  template class LuaBinding<Spindizzy::AlienType>;
  template class LuaBinding<Spindizzy::Boundary>;
  template class LuaBinding<Spindizzy::CameraGameplay>;
  template class LuaBinding<Spindizzy::CameraTransitional>;
  template class LuaBinding<Spindizzy::DebrisGenerator>;
  template class LuaBinding<Spindizzy::ModelCycler>;
  template class LuaBinding<Spindizzy::Model>;
  template class LuaBinding<Spindizzy::PickUp>;
  template class LuaBinding<Spindizzy::Player>;
  template class LuaBinding<Spindizzy::PlayerType>;
  template class LuaBinding<Spindizzy::Spindizzy>;
  template class LuaBinding<Spindizzy::Spinner>;
  template class LuaBinding<Spindizzy::TerrainState>;
  template class LuaBinding<Spindizzy::ThemeColour>;
  template class LuaBinding<Spindizzy::ThemeSet>;
  template class LuaBinding<Spindizzy::Wall>;
  template class LuaBinding<Spindizzy::World>;
  template class LuaBinding<Spindizzy::WorldView>;
  template class LuaBinding<Spindizzy::Zone>;

  template class LocalLuaBinding<Spindizzy::Alien>;
  template class LocalLuaBinding<IFloat>;
  template class LocalLuaBinding<IVertex>;
  template class LocalLuaBinding<Spindizzy::Player>;
  template class LocalLuaBinding<Spindizzy::Wall>;
  template class LocalLuaBinding<Spindizzy::Zone>;
  template class LocalLuaBinding<Spindizzy::ZoneViewOverview>;

  namespace Spindizzy {

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
      mLua.new_usertype<ModelCycler>("ModelCycler",               "next",                     &ModelCycler::next,
                                                                  "previous",                 &ModelCycler::previous);
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
      mLua.new_usertype<Spindizzy>("Spindizzy",                   "setPause",                 &Spindizzy::setPause);
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
