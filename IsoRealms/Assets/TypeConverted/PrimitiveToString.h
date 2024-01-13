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

#include <iostream>
#include <string>

#include "IsoRealms/Assets/Registry/IAssetUser.h"
#include "IsoRealms/IAssetLiterals.h"
#include "IsoRealms/IAssetRemover.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Types.h"
#include "IsoRealms/Utils.h"

#include "ITypeConvertedAsset.h"

namespace IsoRealms {
  
  /**
   * String asset interface wrapper around an primitive asset. // TODO: Explain better
   */
  template <class TYPE> class PrimitiveToString : public IString,
                                                  public IAssetUser<TYPE>, // TODO: Should be managed via Client asset classes
                                                  public ITypeConvertedAsset {
    private:
    IAssetRemover* cAssets; /// To manage removal of ourself in case the wrapped primitive is removed.
    TYPE* cValue;           /// Wrapped primitive.

    public:
      
    /**
     * Construct a String asset wrapper around the specified primitive.
     * 
     * @param asset The primitive asset to wrap.
     */
    PrimitiveToString(IAssetRemover* assets, std::function<TYPE*(IAssetUser<TYPE>*)> assetAccessorFunction) :
              cAssets(assets),
              cValue(assetAccessorFunction(this)) {
    }
    
    /**********************\
     * Implements IString *
    \**********************/
    std::string getValue() const override {
      return Utils::toString(cValue->getValue());
    }
    
    bool renderAssetIcon() const override {
      return false;
    }

    /*******************************\
     * Implements IAssetUser<TYPE> *
    \*******************************/
    void relinquish(TYPE* asset) override {
      cAssets->remove(this);
    }

    /**********************************\
     * Implements ITypeConvertedAsset *
    \**********************************/
    void release(IAssets* releaser) override {
      releaser->release(this, cValue);
    }
  };
}
