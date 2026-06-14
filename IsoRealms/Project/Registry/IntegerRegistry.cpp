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
#include "IntegerRegistry.h"

#include "IsoRealms/Application.h"
#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  IntegerRegistry::IntegerRegistry(Application& application) :
            ResourceClientManager(&cLiteral, "Literal", "Literal"),
            cLiteral(application.getMetadata("LiteralInteger")) {
  }

  IntegerRegistry::Literal::Instance::Instance(Project& project, const int value):
          cMetadata(project.getApplication().getMetadata("LiteralInteger")),
          cValue(value) {
    // Nothing to do.
  }

  int IntegerRegistry::Literal::Instance::getValue() const {
    return cValue;
  }

  void IntegerRegistry::Literal::Instance::saveResource(JSONObject object) const {
    object.addInteger(JSON_VALUE, cValue);
  }

  void IntegerRegistry::Literal::Instance::getResourceProperties(IComponentDefiner& definer) {
    definer.propertyInteger(JSON_VALUE, [this]() {return cValue;}, [this](int value) {cValue = value;});
  }

  bool IntegerRegistry::Literal::Instance::isDefaultConfiguration() const {
    return cValue == 0;
  }
}
