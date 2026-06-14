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
#include "Modules/UI/UI.h"

#include "MenuItemRegistry.h"

namespace IsoRealms::UI {
  MenuItemRegistry::MenuItemRegistry(IComponentTypeRegistry& registry) :
            cAction(           registry.getResourceMetadata("MenuItemAction")),
            cBoolean(          registry.getResourceMetadata("MenuItemBoolean")),
            cDigitalInput(     registry.getResourceMetadata("MenuItemDigitalInput")),
            cDisplayResolution(registry.getResourceMetadata("MenuItemDisplayResolution")),
            cFileList(         registry.getResourceMetadata("MenuItemFileList")),
            cLauncherList(     registry.getResourceMetadata("MenuItemLauncherList")),
            cSlider(           registry.getResourceMetadata("MenuItemSlider")) {
    add(&cAction,            "Action",            "Action");
    add(&cBoolean,           "Boolean",           "Boolean");
    add(&cDigitalInput,      "DigitalInput",      "Digital Input");
    add(&cDisplayResolution, "DisplayResolution", "Display Resolution");
    add(&cFileList,          "FileList",          "File List");
    add(&cLauncherList,      "LauncherList",      "Launcher List");
    add(&cSlider,            "Slider",            "Slider");
  }
}
