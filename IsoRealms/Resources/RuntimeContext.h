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
#ifndef RUNTIME_CONTEXT_H
#define RUNTIME_CONTEXT_H

#include "IResources.h"
#include "IRuntimeContext.h"

class RuntimeContext:public IRuntimeContext {
  private:
  IResources* cResources;
  std::vector<std::string> cEntityPath;
  
  public:
  RuntimeContext(IResources*, std::vector<std::string>);

  /******************************\
   * Implements IRuntimeContext *
  \******************************/
  std::string getModule();
  bool isEditing();
  void add(I3DModel*,                   const std::string&, DOMNodeWrapper*);
  void add(I3DModelFactory*,            const std::string&, DOMNodeWrapper*);
  void add(IArgumentDefinitionType*,    const std::string&);
  void add(IBoolean*,                   const std::string&, DOMNodeWrapper*);
  void add(ICamera*,                    const std::string&, DOMNodeWrapper*);
  void add(ICollectables*,              const std::string&, DOMNodeWrapper*);
  void add(ICollidableSurfaceRegistry*, const std::string&, DOMNodeWrapper*);
  void add(IColour*,                    const std::string&, DOMNodeWrapper*);
  void add(IComponentCustomType*,       const std::string&);
  void add(IElementGroupType*,          const std::string&);
  void add(IElementType*,               const std::string&, DOMNodeWrapper*);
  void add(IFont*,                      const std::string&, DOMNodeWrapper*);
  void add(IHUDComponentFactory*,       const std::string&, DOMNodeWrapper*);
  void add(IInteger*,                   const std::string&, DOMNodeWrapper*);
  void add(IMap*,                       const std::string&, DOMNodeWrapper*);
  void add(IResource*,                                      DOMNodeWrapper*);
  void add(ISound*,                     const std::string&, DOMNodeWrapper*);
  void add(IString*,                    const std::string&, DOMNodeWrapper*);
  void add(ISurfaceProcessor*,          const std::string&, DOMNodeWrapper*);
  void add(ITexture*,                   const std::string&, DOMNodeWrapper*);
  void add(IVertex*,                    const std::string&);
  void add(IZoneHandler*,               const std::string&, DOMNodeWrapper*);
  
  void registerArgument(const std::string&, const std::string&, IArgumentSource*);
  void addDynamicElement(IDynamicElement*);
  
  void addListener(IResourceUseListener<IColour>*);
  void addListener(IResourceUseListener<ITexture>*);
};

#endif
