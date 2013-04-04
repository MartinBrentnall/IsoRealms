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

find_package(SDL_image REQUIRED)
if(SDLIMAGE_FOUND)
  include_directories(${SDLIMAGE_INCLUDE_DIR})
else(SDLIMAGE_FOUND)
  message(STATUS "SDL_Image not found.") 
endif(SDLIMAGE_FOUND)

add_library(TextureSet_ImageTextureSet MODULE 
  DialogImageTextureSelector.cpp
  ImageTexture.cpp
  ImageTextureSet.cpp
)
set_target_properties(TextureSet_ImageTextureSet PROPERTIES OUTPUT_NAME "Module")
set_target_properties(TextureSet_ImageTextureSet PROPERTIES PREFIX "lib")

target_link_libraries(TextureSet_ImageTextureSet ${SDLIMAGE_LIBRARY})
