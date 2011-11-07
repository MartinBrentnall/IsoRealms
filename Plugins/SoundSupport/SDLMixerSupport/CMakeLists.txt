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

find_package(SDL_mixer REQUIRED)
if(SDLMIXER_FOUND)
  include_directories(${SDLMIXER_INCLUDE_DIR})
else(SDLMIXER_FOUND)
  message(STATUS "SDL not found.") 
endif(SDLMIXER_FOUND)

add_library(SoundSupport_SDLMixerSupport MODULE 
  SDLMixerSound.cpp
  SDLMixerSupport.cpp
  SoundConfigurationDialog.cpp
)
set_target_properties(SoundSupport_SDLMixerSupport PROPERTIES OUTPUT_NAME "Module")
set_target_properties(SoundSupport_SDLMixerSupport PROPERTIES PREFIX "lib")

target_link_libraries(SoundSupport_SDLMixerSupport ${SDLMIXER_LIBRARY})
