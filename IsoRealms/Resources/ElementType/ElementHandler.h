/*
 * Copyright 2015 Martin Brentnall
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
#ifndef ELEMENT_HANDLER_H
#define ELEMENT_HANDLER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <limits>
#include <vector>

#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/System.h>

#include "ElementBounds.h"
#include "IElement.h"

class ElementHandler {
  private:
  bool cSpawnThreads;
  SDL_mutex* cElementQueueMutex;
  SDL_cond* cThreadInitComplete;
  unsigned int cLiveThreads;
  bool cEditing;
  IElementBounds* cStaticBounds;

  bool cUpdateStatic;
  GLuint cDisplayList;
  GLuint cEditingDisplayList;
  bool cDirty;
  std::vector<IElement*> cElements;
  std::vector<IElement*> cDynamicElementsRuntime;
  std::vector<IElement*> cDynamicElementsEditing;
  std::vector<IElement*> cVisualElementsRuntime;
  std::vector<IElement*> cVisualElementsEditing;
  std::vector<IElement*> cDirtyElements;
  std::vector<IElement*> cDirtyElementsRemaining;
  std::vector<IElement*> cCleanElements;
  
  int getIndex(IElement*);
  
  void updateStatic();

  bool initMultiThreaded();
  bool initSingleThreaded();
  
  IElement* getElementToInit();
  void initElementHandler(unsigned int, bool);
  int getElementIndex(IElement*);
  void updateStaticBounds();
  
  public:
  ElementHandler();
    
  void setMultiThreaded(bool);
  void initThread();

  void addElement(IElement*);
  void removeElement(IElement*);
  void setAllDirty();
  bool contains(IElement*);
  void setDirty(IElement*);
  bool isDirty();
  void updateEditing(unsigned int);
  void updateRuntime(unsigned int);
  bool init(unsigned int, bool);
  void renderStatic();
  void renderRuntime();
  void renderEditing();
  void staticChanged();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  
  IElementBounds* getStaticBounds();
};

#endif
