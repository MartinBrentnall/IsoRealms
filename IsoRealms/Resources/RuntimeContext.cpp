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
#include "RuntimeContext.h"

RuntimeContext::RuntimeContext(IResources* resources, std::vector<std::string> entityPath) {
  cResources = resources;
  cEntityPath = entityPath;
}

std::string RuntimeContext::getModule() {
  std::string mModule;
  for (unsigned int i = 0; i < cEntityPath.size(); i++) {
    mModule += cEntityPath[i] + "/";
  }
  return mModule;
}

bool RuntimeContext::isEditing() {
  return cResources->isEditing();
}

void RuntimeContext::add(I3DModel* model,                             const std::string& name, DOMNodeWrapper* node) {cResources->add(model,               cEntityPath, name, node);}
void RuntimeContext::add(I3DModelType* modelType,                     const std::string& name, DOMNodeWrapper* node) {cResources->add(modelType,           cEntityPath, name, node);}
void RuntimeContext::add(IArgumentValue* value,                       const std::string& type, const std::string& name) {cResources->add(value,            cEntityPath, type, name);}
void RuntimeContext::add(IBoolean* value,                             const std::string& name, DOMNodeWrapper* node) {cResources->add(value,               cEntityPath, name, node);}
void RuntimeContext::add(ICamera* camera,                             const std::string& name, DOMNodeWrapper* node) {cResources->add(camera,              cEntityPath, name, node);}
void RuntimeContext::add(IBoundaries* boundaries,                     const std::string& name, DOMNodeWrapper* node) {cResources->add(boundaries,          cEntityPath, name, node);}
void RuntimeContext::add(IColour* colour,                             const std::string& name, DOMNodeWrapper* node) {cResources->add(colour,              cEntityPath, name, node);}
void RuntimeContext::add(IComponentCustomType* componentCustomType,   const std::string& name                      ) {cResources->add(componentCustomType, cEntityPath, name);}
void RuntimeContext::add(IElementType* type,                          const std::string& name, DOMNodeWrapper* node) {cResources->add(type,                cEntityPath, name, node);}
void RuntimeContext::add(IFont* font,                                 const std::string& name, DOMNodeWrapper* node) {cResources->add(font,                cEntityPath, name, node);}
void RuntimeContext::add(IHUDComponentType* type,                     const std::string& name, DOMNodeWrapper* node) {cResources->add(type,                cEntityPath, name, node);}
void RuntimeContext::add(IGlobalVariable* variable,                   const std::string& name, DOMNodeWrapper* node) {cResources->add(variable,            cEntityPath, name, node);}
void RuntimeContext::add(IInteger* integer,                           const std::string& name, DOMNodeWrapper* node) {cResources->add(integer,             cEntityPath, name, node);}
void RuntimeContext::add(ILayerType* layerType,                       const std::string& name, DOMNodeWrapper* node) {cResources->add(layerType,           cEntityPath, name, node);}
void RuntimeContext::add(IScript* script,                             const std::string& name, DOMNodeWrapper* node) {cResources->add(script,              cEntityPath, name, node);}
void RuntimeContext::add(IResource* resource,                                                  DOMNodeWrapper* node) {cResources->add(resource,                               node);}
void RuntimeContext::add(ISound* sound,                               const std::string& name, DOMNodeWrapper* node) {cResources->add(sound,               cEntityPath, name, node);}
void RuntimeContext::add(IString* string,                             const std::string& name, DOMNodeWrapper* node) {cResources->add(string,              cEntityPath, name, node);}
void RuntimeContext::add(ITexture* texture,                           const std::string& name, DOMNodeWrapper* node) {cResources->add(texture,             cEntityPath, name, node);}
void RuntimeContext::add(IVertex* vertex,                             const std::string& name, DOMNodeWrapper* node) {cResources->add(vertex,              cEntityPath, name, node);}

// void RuntimeContext::add(IArgumentDefinitionType* argumentDefinitionType, const std::string& name) {
//   argumentDefinitionType->setType(getModule() + name);
//   cResources->add(argumentDefinitionType, cEntityPath, name);
// }

void RuntimeContext::addDynamicElement(IDynamicElement* element) {
  cResources->addDynamicElement(element);
}

void RuntimeContext::addListener(IResourceUseListener<IColour>* listener) {
  cResources->addListener(listener);
}

void RuntimeContext::addListener(IResourceUseListener<ITexture>* listener) {
  cResources->addListener(listener);
}

