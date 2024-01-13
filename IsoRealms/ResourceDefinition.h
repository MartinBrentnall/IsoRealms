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

#include <set>
#include <string>

#include "Assets/Registry/IAssetUser.h"
#include "Assets/Type/IScreenListener.h"
#include "Exception/ResourceInitException.h"
#include "IAssetIdentifier.h"
#include "IAssetLiterals.h"
#include "IAssetRegistry.h"
#include "IAssetRemover.h"
#include "IAssets.h"
#include "IModuleHandle.h"
#include "IProject.h"
#include "IResourceData.h"
#include "Options/IOptions.h"
#include "Persistence/DOMNode.h"
#include "Persistence/DOMNodeWriter.h"
#include "Property/IProperty.h"
#include "Property/IPropertyAppearance.h"
#include "Property/IPropertyListener.h"
