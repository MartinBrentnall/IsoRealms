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

namespace IsoRealms::Basics {
  bool operator==(const Project& a, const Project& b) {
    return &a == &b;
  }

#ifdef __linux__
  extern "C" void initLua(LuaState* luaState) {
#elif _WIN32
  extern "C" void __declspec(dllexport) __stdcall initLua(LuaState* luaState) {
#endif
    sol::state* mLua = luaState->getState();
    mLua->new_usertype<AnalogueInput>("AnalogueInput",           "getInputsString",            &AnalogueInput::getInputsString,
                                                                 "clearCustomInputs",          &AnalogueInput::clearCustomInputs,
                                                                 "addCustomInput",             &AnalogueInput::addCustomInput,
                                                                 "getMappingCount",            &AnalogueInput::getMappingCount,
                                                                 "getMapping",                 &AnalogueInput::getMapping);
    mLua->new_usertype<Basics>("Basics",                         "getSoundVolume",             &Basics::getSoundVolume,
                                                                 "getMusicVolume",             &Basics::getMusicVolume,
                                                                 "setSoundVolume",             &Basics::setSoundVolume,
                                                                 "setMusicVolume",             &Basics::setMusicVolume,
                                                                 "reloadGlobalConfiguration",  &Basics::reloadGlobalConfiguration,
                                                                 "persistGlobalConfiguration", &Basics::persistGlobalConfiguration);
    mLua->new_usertype<DigitalInput>("DigitalInput",             "getInputsString",            &DigitalInput::getInputsString,
                                                                 "clearCustomInputs",          &DigitalInput::clearCustomInputs,
                                                                 "addCustomInput",             &DigitalInput::addCustomInput,
                                                                 "getMappingCount",            &DigitalInput::getMappingCount,
                                                                 "getMapping",                 &DigitalInput::getMapping);
    mLua->new_usertype<InputSwitch>("InputSwitch",               "setInputHandler",            &InputSwitch::setInputHandler);
    mLua->new_usertype<Project>("SubProject",                    "setRunning",                 &Project::setRunning,
                                                                 "setEditing",                 &Project::setEditing,
                                                                 "prepare",                    &Project::prepare,
                                                                 "isReady",                    &Project::isReady,
                                                                 "reset",                      &Project::resetProject,
                                                                 "getDefaultEditor",           &Project::getDefaultEditor,
                                                                 "getProject",                 &Project::getProject,
                                                                 "canSave",                    &Project::canSave,
                                                                 "save",                       &Project::saveProject,
                                                                 "saveAs",                     &Project::saveProjectAs,
                                                                 "exists",                     &Project::exists,
                                                                 "requestQuit",                &Project::requestQuit,
                                                                 "isQuitRequestGranted",       &Project::isQuitRequestGranted);
    mLua->new_usertype<ProjectConfigurer>("ProjectConfigurer",   "setProject",                 &ProjectConfigurer::setProject,
                                                                 "hide",                       &ProjectConfigurer::hide,
                                                                 "show",                       &ProjectConfigurer::show,
                                                                 "isHidden",                   &ProjectConfigurer::isHidden,
                                                                 "getDialogManager",           &ProjectConfigurer::getDialogManager);
    mLua->new_usertype<SequenceInstance>("Sequence",             "play",                       &SequenceInstance::play,
                                                                 "pause",                      &SequenceInstance::pause,
                                                                 "reset",                      &SequenceInstance::reset,
                                                                 "getTime",                    &SequenceInstance::getTime,
                                                                 "setTime",                    &SequenceInstance::setTime);
    mLua->new_usertype<SequenceTrackAudioInstance>("SequenceTrackAudio", "nextTrack",          &SequenceTrackAudioInstance::nextTrack,
                                                                 "previousTrack",              &SequenceTrackAudioInstance::previousTrack);
    mLua->new_usertype<SimpleBoolean>("SimpleBoolean",           "setValue",                   &SimpleBoolean::setValue,
                                                                 "getValue",                   &SimpleBoolean::getValue);
    mLua->new_usertype<SimpleColour>("SimpleColour",             "getRed",                     &SimpleColour::getRed,
                                                                 "getGreen",                   &SimpleColour::getGreen,
                                                                 "getBlue",                    &SimpleColour::getBlue,
                                                                 "getAlpha",                   &SimpleColour::getAlpha,
                                                                 "setRed",                     &SimpleColour::setRed,
                                                                 "setGreen",                   &SimpleColour::setGreen,
                                                                 "setBlue",                    &SimpleColour::setBlue,
                                                                 "setAlpha",                   &SimpleColour::setAlpha);
    mLua->new_usertype<SimpleFloat>("SimpleFloat",               "getValue",                   &SimpleFloat::getValue,
                                                                 "setValue",                   &SimpleFloat::setValue);
    mLua->new_usertype<SimpleInteger>("SimpleInteger",           "getValue",                   &SimpleInteger::getValue,
                                                                 "setValue",                   &SimpleInteger::setValue);
    mLua->new_usertype<SimpleString>("SimpleString",             "getValue",                   &SimpleString::getValue,
                                                                 "setValue",                   &SimpleString::setValue);
    mLua->new_usertype<SimpleVertex>("SimpleVertex",             "getX",                       &SimpleVertex::getX,
                                                                 "getY",                       &SimpleVertex::getY,
                                                                 "getZ",                       &SimpleVertex::getZ,
                                                                 "setX",                       &SimpleVertex::setX,
                                                                 "setY",                       &SimpleVertex::setY,
                                                                 "setZ",                       &SimpleVertex::setZ);
  }
}
