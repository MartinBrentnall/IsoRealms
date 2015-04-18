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
#ifndef RESOURCES_H
#define RESOURCES_H

#include <IsoRealms/Configuration.h>
#include <IsoRealms/Input/InputCommands.h>

#include "Boolean/BooleanRegistry.h"
#include "Camera/CameraRegistry.h"
#include "Camera/DummyCamera.h"
#include "Colour/Colour.h"
#include "Colour/ColourRegistry.h"
#include "Font/DummyFont.h"
#include "GeometryProcessor/IGeometryProcessor.h"
#include "HUDComponents/IHUDComponentType.h"
#include "Integer/IntegerRegistry.h"
#include "IResourceInstanceListener.h"
#include "IResourceListener.h"
#include "IResourceManager.h"
#include "IResources.h"
#include "IResourceTypeRegistry.h"
#include "IResourceUseListener.h"
#include "Layer/ILayer.h"
#include "ResourceInitialiser.h"
#include "ResourceRegistry.h"
#include "Script/IArgumentValueRegistry.h"
#include "Script/IScript.h"
#include "Sound/DummySound.h"
#include "String/StringRegistry.h"
#include "Texture/Texture.h"

class Resources:public IResources,
                public IResourceManager,
                public IResourceTypeRegistry,
                public IArgumentValueRegistry {
  private:
  bool cEditing;

  std::vector<ResourceInitialiser*> cResourcesToInitialise;
  
  std::map<std::string, std::vector<I3DModel*> > cInstantiatedModels;
  
  InputCommands cInputCommands;
  ResourceRegistry<I3DModel> c3DModels;
  ResourceRegistry<I3DModelType> c3DModelTypes;
//  ResourceRegistry<IArgumentDefinitionType> cArgumentDefinitionTypes;
  ResourceRegistry<IArgumentValue> cArgumentValues;
  BooleanRegistry cBooleans;
  ResourceRegistry<IBoundaries> cBoundaries;
  CameraRegistry cCameras;
  ColourRegistry cColours;
  ResourceRegistry<IComponentCustomType> cComponentCustomTypes;
  ResourceRegistry<IElementGroupType> cElementGroupTypes;
  ResourceRegistry<IElementType> cElementTypes;
  ResourceRegistry<IFloat> cFloats;
  ResourceRegistry<IFont> cFonts;
  ResourceRegistry<IGlobalVariable> cGlobalVariables;
  ResourceRegistry<IHUDComponentType> cHUDComponentTypes;
  IntegerRegistry cIntegers;
  ResourceRegistry<ILayerType> cLayerTypes;
  ResourceRegistry<IScript> cScripts;
  ResourceRegistry<ISound> cSounds;
  StringRegistry cStrings;
  ResourceRegistry<ISurfaceRegistry> cSurfaceRegistries;
  ResourceRegistry<IGeometryProcessor> cGeometryProcessors;
  ResourceRegistry<ITexture> cTextures;
  ResourceRegistry<IVertex> cVertices;
  
  std::vector<IDynamicElement*> cDynamicElements;
  
  std::map<IElementType*, IElementGroupType*> cDefaultElementGroups;
  
  std::vector<IElement*> cImplicitElements;
  
  template <class T> T* getDirectory(T*, std::vector<std::string>);
  
  void addResourceToInitialise(IResource*, DOMNodeWrapper*);
  
  public:
  Resources();
  
  void loadInputConfiguration(DOMNodeWrapper*, std::vector<std::string>);
  void loadInstance(DOMNodeWrapper*);
  void input(SDL_Event&);
  void saveInputConfiguration(DOMNodeWriter*);
    
  void setEditing(bool, IProject*);
    
  void saveDefaultElementGroups(DOMNodeWriter*);
  void registerDefaultElementGroup(DOMNodeWrapper*);
//   void loadInstances(DOMNodeWrapper*);

  void initialise();
  std::vector<IDynamicElement*> getDynamicElements();
  
  /*************************\
   * Implements IResources *
  \*************************/
  void add(IResource*, DOMNodeWrapper*);
  
  void add(I3DModel*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(I3DModelType*,               std::vector<std::string>, const std::string&, DOMNodeWrapper*);
//  void add(IArgumentDefinitionType*,    std::vector<std::string>, const std::string&);
  void add(IArgumentValue*,             std::vector<std::string>, const std::string&, const std::string&);  
  void add(IBoolean*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IBoundaries*,                std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(ICamera*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IColour*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IComponentCustomType*,       std::vector<std::string>, const std::string&);
  void add(IElementGroupType*,          std::vector<std::string>, const std::string&);
  void add(IElementType*,               std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IFont*,                      std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IGeometryProcessor*,         std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IHUDComponentType*,          std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IGlobalVariable*,            std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IInteger*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(ILayerType*,                 std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IScript*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(ISound*,                     std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IString*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(ISurfaceRegistry*,           std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(ITexture*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IVertex*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*);

  void addDynamicElement(IDynamicElement*);
  
  void loadDefaultElementGroups(IElementContainer*);
  void loadElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);

  void addListener(IResourceUseListener<I3DModelType>*);
  void addListener(IResourceUseListener<IBoolean>*);       
  void addListener(IResourceUseListener<IBoundaries>*);  
  void addListener(IResourceUseListener<ICamera>*);        
  void addListener(IResourceUseListener<IColour>*);                   
  void addListener(IResourceUseListener<IComponentCustomType>*);      
  void addListener(IResourceUseListener<IElementType>*);              
  void addListener(IResourceUseListener<IFloat>*);                    
  void addListener(IResourceUseListener<IFont>*);                     
  void addListener(IResourceUseListener<IGeometryProcessor>*);         
  void addListener(IResourceUseListener<IHUDComponentType>*);      
  void addListener(IResourceUseListener<IInteger>*);                  
  void addListener(IResourceUseListener<ILayerType>*);
  void addListener(IResourceUseListener<IScript>*);
  void addListener(IResourceUseListener<ISound>*);                    
  void addListener(IResourceUseListener<IString>*);                   
  void addListener(IResourceUseListener<ISurfaceRegistry>*);
  void addListener(IResourceUseListener<ITexture>*);                  
  void addListener(IResourceUseListener<IVertex>*);                   
  
  /********************************\
   * Implements IResourceAccessor *
  \********************************/
  bool isEditing();
  I3DModel* getModelInstance(const std::string&);
  
  bool*                 getDigitalInput(const std::string&);
  I3DModel*             getModel(const std::string&, Vertex*, float = 1.0f);
  I3DModelType*         getModelType(const std::string&);
  IBoolean*             getBoolean(const std::string&);
  IBoundaries*          getBoundaries(const std::string&);
  ICamera*              getCamera(const std::string&);
  IColour*              getColour(const std::string&);
  IComponentCustomType* getComponentCustomType(const std::string&);
  IFloat*               getFloat(const std::string&);
  IFont*                getFont(const std::string&);
  IGeometryProcessor*   getGeometryProcessor(const std::string&);
  IHUDComponentType*    getHUDComponentType(const std::string&);
  IInteger*             getInteger(const std::string&);
  ILayerType*           getLayerType(const std::string&);
  IScript*              getScript(const std::string&);
  ISound*               getSound(const std::string&);
  IString*              getString(const std::string&);
  ISurfaceRegistry*     getSurfaceRegistry(const std::string&);
  ITexture*             getTexture(const std::string&);
  IVertex*              getVertex(const std::string&);
    
  IScriptCall* getScriptCall(DOMNodeWrapper*, IArgumentValueRegistry*);
  IArgumentValue* getArgumentValue(DOMNodeWrapper*);
  IArgumentValue* getArgumentValueCustom(DOMNodeWrapper*); 

  /*******************************\
   * Implements IResourceLocator *
  \*******************************/
  std::string getPath(I3DModel*);
  std::string getPath(IArgumentValue*);
  std::string getPath(I3DModelType*);
  std::string getPath(IBoolean*);
  std::string getPath(IBoundaries*);
  std::string getPath(ICamera*);
  std::string getPath(IColour*);
  std::string getPath(IComponentCustomType*);
  std::string getPath(IElementGroupType*);
  std::string getPath(IElementType*);
  std::string getPath(IFloat*);
  std::string getPath(IFont*);
  std::string getPath(IGeometryProcessor*);
  std::string getPath(IGlobalVariable*);
  std::string getPath(IHUDComponentType*);
  std::string getPath(IInteger*);
  std::string getPath(ILayerType*);
  std::string getPath(IScript*);
  std::string getPath(ISound*);
  std::string getPath(IString*);
  std::string getPath(ISurfaceRegistry*);
  std::string getPath(ITexture*);
  std::string getPath(IVertex*);

  void saveScript(DOMNodeWriter*, const std::string&, IScriptCall*);
  bool isImplicit(IElement*);  
  
  /******************************\
   * Implements ResourceManager *
  \******************************/
  std::vector<I3DModelType*>         getAllModelTypes();
  std::vector<IBoolean*>             getAllBooleans();
  std::vector<IBoundaries*>          getAllBoundaries();
  std::vector<ICamera*>              getAllCameras();
  std::vector<IColour*>              getAllColours();
  std::vector<IComponentCustomType*> getAllComponentCustomTypes();
  std::vector<IElementType*>         getAllElementTypes();
  std::vector<IFloat*>               getAllFloats();
  std::vector<IFont*>                getAllFonts();
  std::vector<IGeometryProcessor*>   getAllGeometryProcessors();
  std::vector<IHUDComponentType*>    getAllHUDComponentTypes();
  std::vector<IInteger*>             getAllIntegers();
  std::vector<ILayerType*>           getAllLayerTypes();
  std::vector<IScript*>              getAllScripts();
  std::vector<ISound*>               getAllSounds();
  std::vector<IString*>              getAllStrings();
  std::vector<ISurfaceRegistry*>     getAllSurfaceRegistries();
  std::vector<ITexture*>             getAllTextures();
  std::vector<IVertex*>              getAllVertices();
  
  void addResourceListener(IResourceListener<I3DModelType>*);          
  void addResourceListener(IResourceListener<IBoolean>*);                 
  void addResourceListener(IResourceListener<IBoundaries>*);        
  void addResourceListener(IResourceListener<ICamera>*);                  
  void addResourceListener(IResourceListener<IColour>*);              
  void addResourceListener(IResourceListener<IComponentCustomType>*); 
  void addResourceListener(IResourceListener<IElementType>*);         
  void addResourceListener(IResourceListener<IFloat>*);               
  void addResourceListener(IResourceListener<IFont>*);                
  void addResourceListener(IResourceListener<IGeometryProcessor>*);
  void addResourceListener(IResourceListener<IHUDComponentType>*); 
  void addResourceListener(IResourceListener<IInteger>*);         
  void addResourceListener(IResourceListener<ILayerType>*);
  void addResourceListener(IResourceListener<IScript>*);
  void addResourceListener(IResourceListener<ISound>*);           
  void addResourceListener(IResourceListener<IString>*);          
  void addResourceListener(IResourceListener<ISurfaceRegistry>*);
  void addResourceListener(IResourceListener<ITexture>*);         
  void addResourceListener(IResourceListener<IVertex>*);          
  
  void addResourceType(IResourceType<I3DModelType>*,         const std::string&);
  void addResourceType(IResourceType<IBoolean>*,             const std::string&);
  void addResourceType(IResourceType<IBoundaries>*,          const std::string&);
  void addResourceType(IResourceType<ICamera>*,              const std::string&);
  void addResourceType(IResourceType<IColour>*,              const std::string&);
  void addResourceType(IResourceType<IComponentCustomType>*, const std::string&);
  void addResourceType(IResourceType<IElementType>*,         const std::string&);
  void addResourceType(IResourceType<IFloat>*,               const std::string&);
  void addResourceType(IResourceType<IFont>*,                const std::string&);
  void addResourceType(IResourceType<IGeometryProcessor>*,   const std::string&);
  void addResourceType(IResourceType<IHUDComponentType>*,    const std::string&);
  void addResourceType(IResourceType<IInteger>*,             const std::string&);
  void addResourceType(IResourceType<ILayerType>*,           const std::string&);
  void addResourceType(IResourceType<IScript>*,              const std::string&);
  void addResourceType(IResourceType<ISound>*,               const std::string&);
  void addResourceType(IResourceType<IString>*,              const std::string&);
  void addResourceType(IResourceType<ISurfaceRegistry>*,     const std::string&);
  void addResourceType(IResourceType<ITexture>*,             const std::string&);
  void addResourceType(IResourceType<IVertex>*,              const std::string&);

  void editResource(I3DModelType*,         IResourceAccessor*, IEditingContext*);
  void editResource(IBoolean*,             IResourceAccessor*, IEditingContext*);
  void editResource(IBoundaries*,          IResourceAccessor*, IEditingContext*);
  void editResource(ICamera*,              IResourceAccessor*, IEditingContext*);
  void editResource(IColour*,              IResourceAccessor*, IEditingContext*);
  void editResource(IComponentCustomType*, IResourceAccessor*, IEditingContext*);
  void editResource(IElementType*,         IResourceAccessor*, IEditingContext*);
  void editResource(IFloat*,               IResourceAccessor*, IEditingContext*);
  void editResource(IFont*,                IResourceAccessor*, IEditingContext*);
  void editResource(IGeometryProcessor*,   IResourceAccessor*, IEditingContext*);
  void editResource(IHUDComponentType*,    IResourceAccessor*, IEditingContext*);
  void editResource(IInteger*,             IResourceAccessor*, IEditingContext*);
  void editResource(ILayerType*,           IResourceAccessor*, IEditingContext*);
  void editResource(IScript*,              IResourceAccessor*, IEditingContext*);
  void editResource(ISound*,               IResourceAccessor*, IEditingContext*);
  void editResource(IString*,              IResourceAccessor*, IEditingContext*);
  void editResource(ISurfaceRegistry*,     IResourceAccessor*, IEditingContext*);
  void editResource(ITexture*,             IResourceAccessor*, IEditingContext*);
  void editResource(IVertex*,              IResourceAccessor*, IEditingContext*);

  void removeResource(I3DModelType*,         IResourceAccessor*);
  void removeResource(IBoolean*,             IResourceAccessor*);
  void removeResource(IBoundaries*,          IResourceAccessor*);
  void removeResource(ICamera*,              IResourceAccessor*);
  void removeResource(IColour*,              IResourceAccessor*);
  void removeResource(IComponentCustomType*, IResourceAccessor*);
  void removeResource(IElementType*,         IResourceAccessor*);
  void removeResource(IFloat*,               IResourceAccessor*);
  void removeResource(IFont*,                IResourceAccessor*);
  void removeResource(IGeometryProcessor*,   IResourceAccessor*);
  void removeResource(IHUDComponentType*,    IResourceAccessor*);
  void removeResource(IInteger*,             IResourceAccessor*);
  void removeResource(ILayerType*,           IResourceAccessor*);
  void removeResource(IScript*,              IResourceAccessor*);
  void removeResource(ISound*,               IResourceAccessor*);
  void removeResource(IString*,              IResourceAccessor*);
  void removeResource(ISurfaceRegistry*,     IResourceAccessor*);
  void removeResource(ITexture*,             IResourceAccessor*);
  void removeResource(IVertex*,              IResourceAccessor*);
 
  /****************\
   * Implements ? *
  \****************/
  void notifyChange(IColour*);
  void notifyChange(ITexture*);
};
#endif
