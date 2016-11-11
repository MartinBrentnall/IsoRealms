/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef RESOURCE_ELEMENT_HUD_STRING_H
#define RESOURCE_ELEMENT_HUD_STRING_H

#include <string>

#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/ElementType/Element.h>
#include <IsoRealms/Resources/Font/IFont.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceRegistry.h>
#include <IsoRealms/Resources/String/IString.h>

class ResourceElementHUDString:public IElementType,
                               public IElementBounds,
                               public Element {
  private:
  IString* cText;
  IFont* cFont;

  public:
  ResourceElementHUDString(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, IResourceLocator*);

  /***************************\
   * Implements IElementType *
  \***************************/
  void loadElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*, IResourceAccessor*, bool, bool);
  void configureElement();
  void renderEditingPreview(Vertex&);
  void renderIcon();
  void updateIcon(unsigned int);
  void destroy(IElement*);
  void removeElement(IElement*);
  Vertex* editorCursorStopped(Vertex*);
  bool inputEdit(SDL_Event&, ILayerEditingContext*);

  /***********************\
   * Implements IElement *
  \***********************/
  IElementType* getElementType();
  void renderStatic();
  void setDirty();
  IElementBounds* getBounds();
  void renderRuntime();
  void updateRuntime(unsigned int);
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);

  /*****************************\
   * Implements IElementBounds *
  \*****************************/
  float getWest();
  float getEast();
  float getSouth();
  float getNorth();
  float getTop();
  float getBottom();
  
  virtual ~ResourceElementHUDString() {}
};

#endif
