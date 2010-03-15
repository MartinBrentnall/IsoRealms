/*
 * Copyright 2009 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "SpindizzyCraftPyramidModelFactory.h"

ISimpleModel* SpindizzyCraftPyramidModelFactory::createModel(Vertex* vertex) {
  return new SpindizzyCraftPyramidModel(vertex);
}

void SpindizzyCraftPyramidModelFactory::destroyModel(ISimpleModel* pyramidModel) {
  delete pyramidModel;
}

std::string SpindizzyCraftPyramidModelFactory::getName() {
  return "Spindizzy Craft Pyramid Model";
}

extern "C" IPlugin* create() {
  return new SpindizzyCraftPyramidModelFactory();
}

extern "C" void destroy(IPlugin* pyramidModel) {
  delete pyramidModel;
}
