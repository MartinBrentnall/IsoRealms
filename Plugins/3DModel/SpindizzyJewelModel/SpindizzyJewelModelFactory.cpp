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
#include "SpindizzyJewelModelFactory.h"

SpindizzyJewelModelFactory::SpindizzyJewelModelFactory(IRuntimeContext* runtimeContext) {
  runtimeContext->add(this, "Jewel");
}

I3DModel* SpindizzyJewelModelFactory::createModel(Vertex* vertex, float scale) {
  return new SpindizzyJewelModel(vertex, scale);
}

void SpindizzyJewelModelFactory::destroyModel(I3DModel* jewelModel) {
  delete jewelModel;
}

extern "C" IPlugin* create(IRuntimeContext* runtimeContext) {
  return new SpindizzyJewelModelFactory(runtimeContext);
}

extern "C" void destroy(IPlugin* jewelModelFactory) {
  delete jewelModelFactory;
}
