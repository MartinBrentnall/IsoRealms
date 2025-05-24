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

#include <string>
#include <vector>

namespace IsoRealms::Spindizzy {
  class IBoundaryType;
  class ICamera;
  class IPhysicalObjectType;
  class ISurfacePattern;
  class IWallPattern;
  class IWorldEditorTool;
  class IZoneObjectTypeTrait;
  class IZoneViewType;

  class ISpindizzyAssetBrowser {
    public:
    virtual std::vector<std::string> getAllBoundaryTypes()        = 0;
    virtual std::vector<std::string> getAllCameras()              = 0;
    virtual std::vector<std::string> getAllPhysicalObjectTypes()  = 0;
    virtual std::vector<std::string> getAllSurfacePatterns()      = 0;
    virtual std::vector<std::string> getAllWallPatterns()         = 0;
    virtual std::vector<std::string> getAllWorldEditorTools()     = 0;
    virtual std::vector<std::string> getAllZoneObjectTypeTraits() = 0;
    virtual std::vector<std::string> getAllZoneViewTypes()        = 0;

    virtual std::string getID(const IBoundaryType*        asset) const = 0;
    virtual std::string getID(const ICamera*              asset) const = 0;
    virtual std::string getID(const IPhysicalObjectType*  asset) const = 0;
    virtual std::string getID(const ISurfacePattern*      asset) const = 0;
    virtual std::string getID(const IWallPattern*         asset) const = 0;
    virtual std::string getID(const IWorldEditorTool*     asset) const = 0;
    virtual std::string getID(const IZoneObjectTypeTrait* asset) const = 0;
    virtual std::string getID(const IZoneViewType*        asset) const = 0;

    virtual bool renderBoundaryTypeIcon(       const std::string& id) const = 0;
    virtual bool renderCameraIcon(             const std::string& id) const = 0;
    virtual bool renderPhysicalObjectTypeIcon( const std::string& id) const = 0;
    virtual bool renderSurfacePatternIcon(     const std::string& id) const = 0;
    virtual bool renderWallPatternIcon(        const std::string& id) const = 0;
    virtual bool renderWorldEditorToolIcon(    const std::string& id) const = 0;
    virtual bool renderZoneObjectTypeTraitIcon(const std::string& id) const = 0;
    virtual bool renderZoneViewTypeIcon(       const std::string& id) const = 0;

    virtual bool isBoundaryTypeConfigurable(       const std::string& id) const = 0;
    virtual bool isCameraConfigurable(             const std::string& id) const = 0;
    virtual bool isPhysicalObjectTypeConfigurable( const std::string& id) const = 0;
    virtual bool isSurfacePatternConfigurable(     const std::string& id) const = 0;
    virtual bool isWallPatternConfigurable(        const std::string& id) const = 0;
    virtual bool isWorldEditorToolConfigurable(    const std::string& id) const = 0;
    virtual bool isZoneObjectTypeTraitConfigurable(const std::string& id) const = 0;
    virtual bool isZoneViewTypeConfigurable(       const std::string& id) const = 0;
  };
}

