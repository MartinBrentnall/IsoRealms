/*
 * Copyright 2023 Martin Brentnall
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
#include "LiteralModel.h"

namespace IsoRealms {
  LiteralModel::ModelInstance LiteralModel::DUMMY;
  
  I3DModel* LiteralModel::createModel() {
    return &DUMMY;
  }
  
  bool LiteralModel::renderPreview() const {
    return false;
  }

  bool LiteralModel::renderAssetIcon() const {
    return false;
  }
  
  void LiteralModel::ModelInstance::update(unsigned int milliseconds) {
    // Nothing to do
  }
  
  void LiteralModel::ModelInstance::render() const {
    // TODO: Dummy implementation
  }
}
