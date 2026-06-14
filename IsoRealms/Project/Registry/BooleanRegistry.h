 /*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <functional>

#include "IsoRealms/Resources/Providers/ResourceLiteralDummy.h"
#include "IsoRealms/Resources/Type/IBoolean.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Metadata.h"
#include "IsoRealms/Utils.h"

#include "ResourceClientManager.h"
#include "IResourceUser.h"

namespace IsoRealms {
  class Project;

  class BooleanRegistry : public ResourceClientManager<BooleanRegistry, IComponentData, IBoolean> {
    public:
    BooleanRegistry();

    IBoolean* literal(IResourceUser<IBoolean>* client, IComponentData& owner, bool value) {
      IResourceProvider<IComponentData, IBoolean>* mProvider = cRegistry.getProvider(value ? "True" : "False", true);
      IBoolean* mBoolean = mProvider->getResource(owner);
      registerClient(client, mProvider, mBoolean);
      return mBoolean;
    }

    private:
    class LiteralBoolean : public IBoolean {
      public:
      LiteralBoolean(bool value) :
                cValue(value) {
      }

      /***********************\
       * Implements IBoolean *
      \***********************/
      bool getValue() const override {
        return cValue;
      }

      /**************************************\
       * Implements IResource from IBoolean *
      \**************************************/
      bool renderResourceIcon() const override {
        if (cValue) {
          Utils::renderIconTick();
        } else {
          Utils::renderIconNone();
        }
        return true;
      }

      private:
      bool cValue; /// The value of this Boolean.
    };

    LiteralBoolean cLiteralFalse = false;
    LiteralBoolean cLiteralTrue  = true;
  };
}

