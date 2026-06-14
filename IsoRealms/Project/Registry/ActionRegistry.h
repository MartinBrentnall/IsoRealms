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
#include "IsoRealms/Resources/Type/IAction.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Utils.h"

#include "ResourceClientManager.h"
#include "IResourceUser.h"

namespace IsoRealms {
  class ActionRegistry : public ResourceClientManager<ActionRegistry, IActionContext, IAction> {
    public:
    ActionRegistry();

    private:
    class Dummy : public IAction {
      public:

      /**********************\
       * Implements IAction *
      \**********************/
      void execute() override;
      bool renderResourceIcon() const override;
    };

    ResourceLiteralDummy<IActionContext, IAction, Dummy> cLiteral;
  };
}
