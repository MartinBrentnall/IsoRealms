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

namespace IsoRealms {
  
  // Forward declaration of asset type classes.
  class DOMNodeWriter;
  class I3DModel;
  class I3DModelType;
  class IAction;
  class IActionType;
  class IAssets;
  class IBinding;
  class IBoolean;
  class IColour;
  class IEditable;
  class IFloat;
  class IFont;
  class IInputHandler;
  class IInteger;
  class IScreen;
  class IProjectOptions;
  class IString;
  class ITexture;
  class IVertex;

  /**
   * Interface via which ID's of assets can be queried.
   */
  class IAssetIdentifier {
    public:
      
    /**
     * Retrieve the ID of the specified model type.
     * 
     * @param asset Model type to retrieve ID of.
     * @return ID of the model type.
     */
    virtual std::string getID(const I3DModelType* asset) const = 0;
      
    /**
     * Retrieve the ID of the specified action type.
     * 
     * @param asset Action type to retrieve ID of.
     * @return ID of the action type.
     */
    virtual std::string getID(const IActionType* asset) const = 0;

    /**
     * Retrieve the ID of the specified assets.
     *
     * @param asset Assets to retrieve ID of.
     * @return ID of the binding.
     */
    virtual std::string getID(const IAssets* asset) const = 0;

    /**
     * Retrieve the ID of the specified binding.
     *
     * @param asset Binding to retrieve ID of.
     * @return ID of the binding.
     */
    virtual std::string getID(const IBinding* asset) const = 0;

    /**
     * Retrieve the ID of the specified boolean.
     * 
     * @param asset Boolean to retrieve ID of.
     * @return ID of the boolean.
     */
    virtual std::string getID(const IBoolean* asset) const = 0;
      
    /**
     * Retrieve the ID of the specified colour.
     * 
     * @param asset Colour to retrieve ID of.
     * @return ID of the colour.
     */
    virtual std::string getID(const IColour* asset) const = 0;
      
    /**
     * Retrieve the ID of the specified editor context.
     * 
     * @param asset Editor context to retrieve ID of.
     * @return ID of the editor context.
     */
    virtual std::string getID(const IEditable* asset) const = 0;
      
    /**
     * Retrieve the ID of the specified float.
     * 
     * @param asset Float to retrieve ID of.
     * @return ID of the float.
     */
    virtual std::string getID(const IFloat* asset) const = 0;
      
    /**
     * Retrieve the ID of the specified font.
     * 
     * @param asset Font to retrieve ID of.
     * @return ID of the font.
     */
    virtual std::string getID(const IFont* asset) const = 0;
      
    /**
     * Retrieve the ID of the specified input handler.
     * 
     * @param asset Input handler to retrieve ID of.
     * @return ID of the input handler.
     */
    virtual std::string getID(const IInputHandler* asset) const = 0;
      
    /**
     * Retrieve the ID of the specified integer.
     * 
     * @param asset Integer to retrieve ID of.
     * @return ID of the integer.
     */
    virtual std::string getID(const IInteger* asset) const = 0;

    /**
     * Retrieve the ID of the specified visual.
     * 
     * @param asset Visual to retrieve ID of.
     * @return ID of the visual.
     */
    virtual std::string getID(const IScreen* asset) const = 0;
      
    /**
     * Retrieve the ID of the specified project options.
     * 
     * @param asset Project options to retrieve ID of.
     * @return ID of the project options.
     */
    virtual std::string getID(const IProjectOptions* asset) const = 0;
      
    /**
     * Retrieve the ID of the specified string.
     * 
     * @param asset String to retrieve ID of.
     * @return ID of the string.
     */
    virtual std::string getID(const IString* asset) const = 0;
      
    /**
     * Retrieve the ID of the specified texture.
     * 
     * @param asset Texture to retrieve ID of.
     * @return ID of the texture.
     */
    virtual std::string getID(const ITexture* asset) const = 0;
      
    /**
     * Retrieve the ID of the specified vertex.
     * 
     * @param asset Vertex to retrieve ID of.
     * @return ID of the vertex.
     */
    virtual std::string getID(const IVertex* asset) const = 0;

    virtual void save(DOMNodeWriter* node, IActionType*     asset) const = 0;
    virtual void save(DOMNodeWriter* node, IAssets*         asset) const = 0;
    virtual void save(DOMNodeWriter* node, I3DModelType*    asset) const = 0;
    virtual void save(DOMNodeWriter* node, IBinding*        asset) const = 0;
    virtual void save(DOMNodeWriter* node, IBoolean*        asset) const = 0;
    virtual void save(DOMNodeWriter* node, IColour*         asset) const = 0;
    virtual void save(DOMNodeWriter* node, IEditable*       asset) const = 0;
    virtual void save(DOMNodeWriter* node, IFloat*          asset) const = 0;
    virtual void save(DOMNodeWriter* node, IFont*           asset) const = 0;
    virtual void save(DOMNodeWriter* node, IInputHandler*   asset) const = 0;
    virtual void save(DOMNodeWriter* node, IInteger*        asset) const = 0;
    virtual void save(DOMNodeWriter* node, IScreen*         asset) const = 0;
    virtual void save(DOMNodeWriter* node, IProjectOptions* asset) const = 0;
    virtual void save(DOMNodeWriter* node, IString*         asset) const = 0;
    virtual void save(DOMNodeWriter* node, ITexture*        asset) const = 0;
    virtual void save(DOMNodeWriter* node, IVertex*         asset) const = 0;
  };
}
