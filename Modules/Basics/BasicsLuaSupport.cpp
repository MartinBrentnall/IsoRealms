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
#include "BasicsLuaSupport.h"

#include <sol.hpp>

#include "AnalogueControl/AnalogueControl.h"
#include "Basics.h"
#include "InputSwitch/InputSwitch.h"
#include "Project/Project.h"
#include "ProjectConfigurer/ProjectConfigurer.h"
#include "Sequence/SequenceInstance.h"
#include "SimpleBoolean/SimpleBoolean.h"
#include "SimpleColour/SimpleColour.h"
#include "SimpleFloat/SimpleFloat.h"
#include "SimpleInteger/SimpleInteger.h"
#include "SimpleString/SimpleString.h"
#include "SimpleVertex/SimpleVertex.h"

namespace IsoRealms {
  template <typename TYPE> void LuaBinding<TYPE>::bind(const std::string& bindFunction) const {
    cLuaState.getState()[bindFunction](cDefValue);
  }
  
  template class LuaBinding<Basics::AnalogueControl>;
  template class LuaBinding<Basics::Basics>;
  template class LuaBinding<Basics::InputSwitch>;
  template class LuaBinding<Basics::Project>;
  template class LuaBinding<Basics::ProjectConfigurer>;
  template class LuaBinding<Basics::Sequence>;
  template class LuaBinding<Basics::SequenceInstance>;
  template class LuaBinding<Basics::SequenceTrackAudioInstance>;
  template class LuaBinding<Basics::SimpleBoolean>;
  template class LuaBinding<Basics::SimpleColour>;
  template class LuaBinding<Basics::SimpleFloat>;
  template class LuaBinding<Basics::SimpleInteger>;
  template class LuaBinding<Basics::SimpleString>;
  template class LuaBinding<Basics::SimpleVertex>;

  namespace Basics {
    bool operator==(const Project& a, const Project& b) {
      return &a == &b;
    }

#ifdef __linux__
    extern "C" void initLua(LuaState* luaState) {
#elif _WIN32
    extern "C" void __declspec(dllexport) __stdcall initLua(LuaState* luaState) {
#endif
      sol::state& mLua = luaState->getState();
      mLua.new_usertype<AnalogueControl>("AnalogueControl",               "getInputsString",            &AnalogueControl::getInputsString,
                                                                          "clearCustomInputs",          &AnalogueControl::clearCustomInputs,
                                                                          "addCustomInput",             &AnalogueControl::addCustomInput,
                                                                          "getMappingCount",            &AnalogueControl::getMappingCount,
                                                                          "getMapping",                 &AnalogueControl::getMapping);
      mLua.new_usertype<Basics>("Basics",                                 "getSoundVolume",             &Basics::getSoundVolume,
                                                                          "getMusicVolume",             &Basics::getMusicVolume,
                                                                          "setSoundVolume",             &Basics::setSoundVolume,
                                                                          "setMusicVolume",             &Basics::setMusicVolume,
                                                                          "reloadGlobalConfiguration",  &Basics::reloadGlobalConfiguration,
                                                                          "persistGlobalConfiguration", &Basics::persistGlobalConfiguration);
      mLua.new_usertype<DigitalControl>("DigitalControl",                 "getInputsString",            &DigitalControl::getInputsString,
                                                                          "clearCustomInputs",          &DigitalControl::clearCustomInputs,
                                                                          "addCustomInput",             &DigitalControl::addCustomInput,
                                                                          "getMappingCount",            &DigitalControl::getMappingCount,
                                                                          "getMapping",                 &DigitalControl::getMapping);
      mLua.new_usertype<InputSwitch>("InputSwitch",                       "setInputHandler",            &InputSwitch::setInputHandler);
      mLua.new_usertype<Project>("SubProject",                            "setRunning",                 &Project::setRunning,
                                                                          "setEditing",                 &Project::setEditing,
                                                                          "prepare",                    &Project::prepare,
                                                                          "prepareNew",                 &Project::prepareNewProject,
                                                                          "isReady",                    &Project::isReady,
                                                                          "reset",                      &Project::resetProject,
                                                                          "resetLauncher",              &Project::resetLauncher,
                                                                          "resetOptions",               &Project::resetOptions,
                                                                          "getDefaultEditor",           &Project::getDefaultEditor,
                                                                          "getProject",                 &Project::getProject,
                                                                          "isUser",                     &Project::isUser,
                                                                          "save",                       &Project::saveProject,
                                                                          "saveAs",                     &Project::saveProjectAs,
                                                                          "exists",                     &Project::exists,
                                                                          "requestQuit",                &Project::requestQuit,
                                                                          "isQuitRequestGranted",       &Project::isQuitRequestGranted);
      mLua.new_usertype<ProjectConfigurer>("ProjectConfigurer",           "setProject",                 &ProjectConfigurer::setProject,
                                                                          "hide",                       &ProjectConfigurer::hide,
                                                                          "show",                       &ProjectConfigurer::show,
                                                                          "isHidden",                   &ProjectConfigurer::isHidden,
                                                                          "getDialogManager",           &ProjectConfigurer::getDialogManager);
      mLua.new_usertype<SequenceInstance>("Sequence",                     "play",                       &SequenceInstance::play,
                                                                          "pause",                      &SequenceInstance::pause,
                                                                          "reset",                      &SequenceInstance::reset,
                                                                          "getTime",                    &SequenceInstance::getTime,
                                                                          "setTime",                    &SequenceInstance::setTime);
      mLua.new_usertype<SequenceTrackAudioInstance>("SequenceTrackAudio", "nextTrack",                  &SequenceTrackAudioInstance::nextTrack,
                                                                          "previousTrack",              &SequenceTrackAudioInstance::previousTrack);
      mLua.new_usertype<SimpleBoolean>("SimpleBoolean",                   "setValue",                   &SimpleBoolean::setValue,
                                                                          "getValue",                   &SimpleBoolean::getValue);
      mLua.new_usertype<SimpleColour>("SimpleColour",                     "getRed",                     &SimpleColour::getRed,
                                                                          "getGreen",                   &SimpleColour::getGreen,
                                                                          "getBlue",                    &SimpleColour::getBlue,
                                                                          "getAlpha",                   &SimpleColour::getAlpha,
                                                                          "setRed",                     &SimpleColour::setRed,
                                                                          "setGreen",                   &SimpleColour::setGreen,
                                                                          "setBlue",                    &SimpleColour::setBlue,
                                                                          "setAlpha",                   &SimpleColour::setAlpha);
      mLua.new_usertype<SimpleFloat>("SimpleFloat",                       "getValue",                   &SimpleFloat::getValue,
                                                                          "setValue",                   &SimpleFloat::setValue);
      mLua.new_usertype<SimpleInteger>("SimpleInteger",                   "getValue",                   &SimpleInteger::getValue,
                                                                          "setValue",                   &SimpleInteger::setValue);
      mLua.new_usertype<SimpleString>("SimpleString",                     "getValue",                   &SimpleString::getValue,
                                                                          "setValue",                   &SimpleString::setValue);
      mLua.new_usertype<SimpleVertex>("SimpleVertex",                     "getX",                       &SimpleVertex::getX,
                                                                          "getY",                       &SimpleVertex::getY,
                                                                          "getZ",                       &SimpleVertex::getZ,
                                                                          "setX",                       &SimpleVertex::setX,
                                                                          "setY",                       &SimpleVertex::setY,
                                                                          "setZ",                       &SimpleVertex::setZ);
    }
  }
}
