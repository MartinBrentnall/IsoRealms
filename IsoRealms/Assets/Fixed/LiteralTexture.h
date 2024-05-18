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
#pragma once

#include <GL/glew.h>
#include <iostream>
#include <vector>

#include "IsoRealms/Assets/Type/ITexture.h"
#include "IsoRealms/Assets/Type/ITextureUseListener.h"
#include "IsoRealms/IProject.h"

namespace IsoRealms {
  class LiteralTexture : public ITexture {
    private:
    GLuint cTexture;
    GLuint cFrameBuffer;
    std::vector<ITextureUseListener*> cUseListeners;

    public:
    LiteralTexture();
    LiteralTexture(IProject* project, bool clampX = false, bool clampY = false);
    
    void setRenderTarget();
    void addUseListener(ITextureUseListener*);

    /***********************\
    * Implements ITexture *
    \***********************/
    ITexture* getTexture() override;
    void set() const override;
    void hintTextureInUse(bool) override;
    void coord(float x, float y) const override;
    void saveAsset(JSONObject object) const override;

    ~LiteralTexture();
  };
}
