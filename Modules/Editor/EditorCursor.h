/*
 * Copyright 2009 Martin Brentnall
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
#ifndef EDITOR_CURSOR_H
#define EDITOR_CURSOR_H

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <iostream>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>
#include <IsoRealms/IsoRealmsConstants.h>

/**
 * TODO: Class description
 */
class EditorCursor:public BlockLocation,
                   public IsoRealmsConstants {
  private:
  IElementType* cElementBrush;

  bool keyDown(SDLKey&);

  public:
  EditorCursor();

  /**
   * Render the cursor at its current location
   */
  void render();                                        

  void setElementType(IElementType*);

  /**
   * Forward input events directed to the cursor.
   */
  bool input(SDL_Event&);

  BlockLocation getLocation();                                  // Get the location of the cursor
  void moveToCamera();                                  // Position the camera at the cursor
  void restrainTo(BlockLocation, BlockLocation);  // Bound the cursor to within the specified cuboid

//  IElementContainer* pushElement(IElement*);
//  IElementContainer* pushMapElement(IElement*);
  IElement* popElement();
  void setDirty(IElement*);

//   Map* getMap();

  ~EditorCursor();
};

#endif
