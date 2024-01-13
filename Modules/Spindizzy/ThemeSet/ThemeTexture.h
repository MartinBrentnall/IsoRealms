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

#include "IsoRealms/IAssetRegistry.h"
#include "IsoRealms/IAssetRemover.h"
#include "IsoRealms/IStateListener.h"
#include "IsoRealms/Types.h"

#include "../IIconAnimator.h"

namespace IsoRealms::Spindizzy {
  class ThemeTexture : public ITexture {
    private:
    ITexture* cTexture;
    IIconAnimator* cAnimator;
    IStateNotifier<ITexture>* cStateNotifier;

    public:
    ThemeTexture(IIconAnimator* animator);
    void registerAssets(IAssetRegistry* assets, const std::string& id);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
      
    void set(ITexture* texture);

    void notifyChange();
    
    /***********************\
     * Implements ITexture *
    \***********************/
    ITexture* getTexture() override;
    void set() const override;
    void hintTextureInUse(bool) override;
    void coord(float x, float y) const override;
  };
}
