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
#ifndef SPINDIZZY_TEXTURE_SET_DUMMY_H
#define SPINDIZZY_TEXTURE_SET_DUMMY_H

#include <GL/gl.h>
#include <vector>

#include <IsoRealms/DummyPlugin.h>
#include <IsoRealms/ICommandInfo.h>

#include "ISpindizzyTexture.h"
#include "ISpindizzyTextureSet.h"

class SpindizzyTextureSetDummy:public ISpindizzyTextureSet,
                               public DummyPlugin {
  private:
  ISpindizzyTexture* cDummyTexture;

  class SpindizzyTextureDummy:public ISpindizzyTexture {

    public:
    SpindizzyTextureDummy();

    /********************************\
     * Implements ISpindizzyTexture *
    \********************************/
    void set();
    void texCoord2f(float, float);
    Colour* getColour(float, float);
    Mapping getMapping();
  };

  public:

  /***********************************\
   * Implements ISpindizzyTextureSet *
  \***********************************/
  SpindizzyTextureSetDummy();
  ISpindizzyTexture* getTexture(const std::string&);

  ~SpindizzyTextureSetDummy();
};

#endif
