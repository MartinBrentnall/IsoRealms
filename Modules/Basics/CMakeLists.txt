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

add_library(Basics MODULE
  AnalogueControl/AnalogueControl.cpp
  BooleanTrigger/BooleanTrigger.cpp
  DigitalControl/DigitalControl.cpp
  FileFont/FileFont.cpp
  FileSound/FileSound.cpp
  FileTexture/FileTexture.cpp
  Function/ArgumentDefinition.cpp
  Function/Binding.cpp
  Function/Function.cpp
  InputGroup/InputGroup.cpp
  InputSwitch/InputSwitch.cpp
  InterruptHandler/InterruptHandler.cpp
  Project/Project.cpp
  Project/ProjectLoader.cpp
  ProjectConfigurer/ProjectConfigurer.cpp
  Resources/Client/SequenceTrack.cpp
  Resources/Fixed/AnalogueInput/DigitalToAnalogueMapping.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackAction.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackActionEvent.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackActionInstance.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackAudio.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackAudioEvent.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackAudioInstance.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackColour.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackColourEvent.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackColourInstance.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackFloat.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackFloatEvent.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackFloatInstance.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackScreen.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackScreenEvent.cpp
  Resources/Fixed/SequenceTrack/SequenceTrackScreenInstance.cpp
  Resources/Registry/SequenceTrackRegistry.cpp
  Sequence/Editor/SequenceEditor.cpp
  Sequence/Sequence.cpp
  Sequence/SequenceInstance.cpp
  SimpleBoolean/SimpleBoolean.cpp
  SimpleColour/SimpleColour.cpp
  SimpleFloat/SimpleFloat.cpp
  SimpleInteger/SimpleInteger.cpp
  SimpleString/SimpleString.cpp
  SimpleVertex/SimpleVertex.cpp
  Sprite/Sprite.cpp
  Basics.cpp
  BasicsLuaSupport.cpp
  Script.cpp
)

set_target_properties(Basics PROPERTIES OUTPUT_NAME "IsoRealms-Basics")
set_target_properties(Basics PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../Out")
set_target_properties(Basics PROPERTIES PREFIX "")

target_link_libraries(Basics IsoRealms sfml-window sfml-system sfml-audio sfml-graphics OpenGL::GL ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARIES} ${FREETYPE_LIBRARIES} ${LUA_LIBRARIES})
