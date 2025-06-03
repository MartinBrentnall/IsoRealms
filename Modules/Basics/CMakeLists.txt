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

add_library(Basics MODULE
  AnalogueInput/AnalogueInput.cpp
  AnalogueInput/DigitalToAnalogueMapping.cpp
  Assets/Client/SequenceTrack.cpp
  Assets/Fixed/SequenceTrack/SequenceTrackAction.cpp
  Assets/Fixed/SequenceTrack/SequenceTrackAudio.cpp
  Assets/Fixed/SequenceTrack/SequenceTrackColour.cpp
  Assets/Fixed/SequenceTrack/SequenceTrackFloat.cpp
  BooleanTrigger/BooleanTrigger.cpp
  DigitalInput/DigitalInput.cpp
  FileFont/FileFont.cpp
  FileSound/FileSound.cpp
  FileTexture/FileTexture.cpp
  Function/ArgumentDefinition.cpp
  Function/Binding.cpp
  Function/Function.cpp
  InputGroup/InputGroup.cpp
  InputSwitch/InputSwitch.cpp
  InterruptHandler/InterruptHandler.cpp
  ModelCycler/ModelCycler.cpp
  Project/Project.cpp
  Project/ProjectLoader.cpp
  ProjectConfigurer/ProjectConfigurer.cpp
  ProjectOptions/ProjectOptions.cpp
  Sequence/Sequence.cpp
  Sequence/Editor/SequenceEditor.cpp
  SimpleBoolean/SimpleBoolean.cpp
  SimpleColour/SimpleColour.cpp
  SimpleFloat/SimpleFloat.cpp
  SimpleInteger/SimpleInteger.cpp
  SimpleString/SimpleString.cpp
  SimpleVertex/SimpleVertex.cpp
  Sprite/Sprite.cpp
  Timer/Timer.cpp
  Basics.cpp
  BasicsLuaSupport.cpp
  Script.cpp
)

set_target_properties(Basics PROPERTIES OUTPUT_NAME "IsoRealms-Basics")
set_target_properties(Basics PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../Out")
set_target_properties(Basics PROPERTIES PREFIX "")

target_link_libraries(Basics IsoRealms sfml-window sfml-system sfml-audio sfml-graphics OpenGL::GL ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARIES} ${FREETYPE_LIBRARIES} ${LUA_LIBRARIES})
