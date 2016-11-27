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

find_package(SDL_image REQUIRED)
if(SDLIMAGE_FOUND)
  include_directories(${SDLIMAGE_INCLUDE_DIR})
else(SDLIMAGE_FOUND)
  message(STATUS "SDL_Image not found.") 
endif(SDLIMAGE_FOUND)

find_package(SDL_mixer REQUIRED)
if(SDLMIXER_FOUND)
  include_directories(${SDLMIXER_INCLUDE_DIR})
else(SDLMIXER_FOUND)
  message(STATUS "SDL_Mixer not found.") 
endif(SDLMIXER_FOUND)

find_package(Freetype REQUIRED)
if(FREETYPE_FOUND)
  include_directories(${FREETYPE_INCLUDE_DIRS})
else(FREETYPE_FOUND)
  message(STATUS "FreeType not found.")
endif(FREETYPE_FOUND)

add_library(IsoRealmsStd MODULE
  DialogBooleanFixed.cpp
  DialogColourFixed.cpp
  DialogElementHUDModel.cpp
  DialogElementHUDRoundedRectangle.cpp
  DialogElementHUDString.cpp
  DialogFontFile.cpp
  DialogIntegerFixed.cpp
  DialogIntegerTimer.cpp
  DialogLayerHUD.cpp
  DialogModelElement.cpp
  DialogModelScriptable.cpp
  DialogModelSetCycleable.cpp
  DialogModelSprite.cpp
  DialogScriptLua.cpp
  DialogSoundFile.cpp
  DialogStringFixed.cpp
  DialogTextureFile.cpp
  DialogVertexFixed.cpp
  ElementHUDModel.cpp
  ElementHUDRoundedRectangle.cpp
  ElementHUDString.cpp
  HUDComponentPosition.cpp
  HUDComponentProxy.cpp
  HUDComponentRelation.cpp
  IsoRealmsModule.cpp
  IsoRealmsScriptSupport.cpp
  LayerHUD.cpp
  ModelCycleable.cpp
  ModelElement.cpp
  ModelScriptable.cpp
  ModelSprite.cpp
  ResourceBooleanFixed.cpp
  ResourceElementHUDAbstract.cpp
  ResourceElementHUDModel.cpp
  ResourceElementHUDRoundedRectangle.cpp
  ResourceElementHUDString.cpp
  ResourceFontFile.cpp
  ResourceIntegerFixed.cpp
  ResourceIntegerTimer.cpp
  ResourceLayerHUD.cpp
  ResourceModelElement.cpp
  ResourceModelScriptable.cpp
  ResourceModelSetCycleable.cpp
  ResourceModelSprite.cpp
  ResourceSoundFile.cpp
  ResourceStringFixed.cpp
  ResourceTextureFile.cpp
  ResourceVertexFixed.cpp
  ScreenRelation.cpp
  ScreenRelative.cpp
)

set_target_properties(IsoRealmsStd PROPERTIES OUTPUT_NAME "Module")
set_target_properties(IsoRealmsStd PROPERTIES PREFIX "lib")

target_link_libraries(IsoRealmsStd ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLMIXER_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${FREETYPE_LIBRARIES})
