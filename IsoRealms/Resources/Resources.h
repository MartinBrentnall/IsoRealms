#ifndef RESOURCES_H
#define RESOURCES_H

#include <IsoRealms/IZoneContextListener.h>
#include <IsoRealms/LuaSupport/IArgumentGenerator.h>
#include <IsoRealms/LuaSupport/IArgumentDefinitionType.h>
#include <IsoRealms/LuaSupport/LuaGlobalVariable.h>
#include <IsoRealms/LuaSupport/ILuaScript.h>
#include <IsoRealms/LuaSupport/LuaScript.h>
#include <IsoRealms/LuaSupport/LuaScriptWithArgs.h>
#include <IsoRealms/Resources/Camera/DummyCamera.h>
#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/Colour/ColourRegistry.h>
#include <IsoRealms/Resources/Font/DummyFont.h>
#include <IsoRealms/Resources/HUDComponents/IHUDComponentFactory.h>
#include <IsoRealms/Resources/Integer/IntegerRegistry.h>
#include <IsoRealms/Resources/Sound/DummySound.h>
#include <IsoRealms/Resources/String/StringRegistry.h>
#include <IsoRealms/Resources/SurfaceProcessor/ISurfaceProcessor.h>
#include <IsoRealms/Resources/Texture/Texture.h>
#include <IsoRealms/Resources/ZoneHandler/IZoneHandler.h>
#include <IsoRealms/Resources/ZoneHandler/ZoneHandlerRegistry.h>

#include "IResourceInstanceListener.h"
#include "IResourceListener.h"
#include "IResourceManager.h"
#include "IResources.h"
#include "IResourceUseListener.h"
#include "ResourceInitialiser.h"
#include "ResourceRegistry.h"

class Resources:public IResources,
                public IResourceManager,
                public IArgumentGenerator {
  private:
  bool cEditing;
  IProject* cProject;

  std::vector<ResourceInitialiser*> cResourcesToInitialise;
  std::vector<IZoneContextListener*> cZoneContextListeners;
  
  std::vector<LuaGlobalVariable*> cGlobalVariables;
  
  std::map<std::string, std::vector<I3DModel*> > cInstantiatedModels;
  
  std::map<std::string, ILuaScript*> cScriptRegistry;
  ResourceRegistry<I3DModel> c3DModelRegistry;
  ResourceRegistry<I3DModelFactory> c3DModelFactoryRegistry;
  ResourceRegistry<IArgumentDefinitionType> cArgumentDefinitionTypes;
  ResourceRegistry<IArgumentSource> cArgumentValues;
  ResourceRegistry<IBoolean> cBooleanRegistry;
  ResourceRegistry<ICamera> cCameras;
  ResourceRegistry<ICollidableSurfaceRegistry> cSurfaceRegistries;
  ResourceRegistry<ICollectables> cCollectablesRegistry;
  ColourRegistry cColourRegistry;
  ResourceRegistry<IComponentCustomType> cComponentCustomTypes;
  ResourceRegistry<IElementGroupType> cElementGroupTypeRegistry;
  ResourceRegistry<IElementType> cElementTypeRegistry;
  ResourceRegistry<IFloat> cFloatRegistry;
  ResourceRegistry<IFont> cFontRegistry;
  ResourceRegistry<IHUDComponentFactory> cHUDComponentRegistry;
  IntegerRegistry cIntegerRegistry;
  ResourceRegistry<ISound> cSoundRegistry;
  StringRegistry cStrings;
  ResourceRegistry<ISurfaceProcessor> cSurfaceProcessors;
  ResourceRegistry<ITexture> cTextureRegistry;
  ResourceRegistry<IVertex> cLocations;
  ZoneHandlerRegistry cZoneHandlers;
  
  std::vector<IDynamicElement*> cDynamicElements;
  
  std::map<IElementType*, IElementGroupType*> cDefaultElementGroups;
  
  std::vector<IElement*> cImplicitElements;
  
  template <class T> T* getDirectory(T*, std::vector<std::string>);
  
  void addResourceToInitialise(IResource*, DOMNodeWrapper*);
  
  public:
  Resources();
  
  void setEditing(bool);
  void setProject(IProject*);
    
  void loadScript(DOMNodeWrapper*);
  void saveScripts(DOMNodeWriter*);
  void saveDefaultElementGroups(DOMNodeWriter*);
  void saveGlobalVariables(DOMNodeWriter*);
  void registerGlobalVariable(DOMNodeWrapper*);
  void registerDefaultElementGroup(DOMNodeWrapper*);

  void initialise();
  std::vector<IDynamicElement*> getDynamicElements();
  
  void zoneContextChanged(IMap*, IZone*);
  void initResources(IZone*, unsigned int);
  
  /*************************\
   * Implements IResources *
  \*************************/
  void add(IResource*, DOMNodeWrapper*);
  void add(IColour*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(ITexture*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(I3DModel*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(I3DModelFactory*,            std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(ISound*,                     std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IInteger*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IBoolean*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IElementType*,               std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IFont*,                      std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IHUDComponentFactory*,       std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(ISurfaceProcessor*,          std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(ICollidableSurfaceRegistry*, std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(ICollectables*,              std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IVertex*,                    std::vector<std::string>, const std::string&);
  void add(ICamera*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IString*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IZoneHandler*,               std::vector<std::string>, const std::string&, DOMNodeWrapper*);
  void add(IElementGroupType*,          std::vector<std::string>, const std::string&);
  void add(IArgumentDefinitionType*,    std::vector<std::string>, const std::string&);
  void add(IComponentCustomType*,       std::vector<std::string>, const std::string&);
  void registerArgument(const std::string&, IArgumentSource* source, std::vector<std::string>, const std::string&);  
  void addDynamicElement(IDynamicElement*);
  
  void loadDefaultElementGroups(IElementContainer*);
  void loadElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);

  void addListener(IResourceUseListener<I3DModelFactory>*);
  void addListener(IResourceUseListener<IBoolean>*);       
  void addListener(IResourceUseListener<ICamera>*);        
  void addListener(IResourceUseListener<ICollectables>*);  
  void addListener(IResourceUseListener<ICollidableSurfaceRegistry>*);
  void addListener(IResourceUseListener<IColour>*);                   
  void addListener(IResourceUseListener<IComponentCustomType>*);      
  void addListener(IResourceUseListener<IElementType>*);              
  void addListener(IResourceUseListener<IFloat>*);                    
  void addListener(IResourceUseListener<IFont>*);                     
  void addListener(IResourceUseListener<IHUDComponentFactory>*);      
  void addListener(IResourceUseListener<IInteger>*);                  
  void addListener(IResourceUseListener<ISound>*);                    
  void addListener(IResourceUseListener<IString>*);                   
  void addListener(IResourceUseListener<ISurfaceProcessor>*);         
  void addListener(IResourceUseListener<ITexture>*);                  
  void addListener(IResourceUseListener<IVertex>*);                   
  void addListener(IResourceUseListener<IZoneHandler>*);              
  
  /********************************\
   * Implements IResourceAccessor *
  \********************************/
  IProject* getProject();
  bool isEditing();
  I3DModel* getModelInstance(const std::string&);
  
  I3DModel* getModel(const std::string&, Vertex*, float = 1.0f);
  I3DModelFactory* getModelType(const std::string&);
  ICamera* getCamera(const std::string&);
  ICollectables* getCollectablesRegistry(const std::string&);
  ICollidableSurfaceRegistry* getSurfaceRegistry(const std::string&);
  IColour* getColour(const std::string&);
  IComponentCustomType* getComponentCustomType(const std::string&);
  IFloat* getFloat(const std::string&);
  IFont* getFont(const std::string&);
  IHUDComponentFactory* getHUDComponentType(const std::string&);
  IInteger* getInteger(const std::string&);
  ISound* getSound(const std::string&);
  IString* getString(const std::string&);
  ISurfaceProcessor* getSurfaceProcessor(const std::string&);
  ITexture* getTexture(const std::string&);
  IVertex* getLocation(const std::string&);
  IZoneHandler* getZoneHandler(const std::string&);
    
  IScript* getLuaScript(DOMNodeWrapper*, IArgumentGenerator*);
  IArgumentDefinition* getArgumentDefinition(DOMNodeWrapper*);
  IArgumentDefinition* getRegisteredArgumentDefinition(DOMNodeWrapper*);
  IArgumentSource* getArgument(DOMNodeWrapper*);
  IArgumentSource* getRegisteredArgument(DOMNodeWrapper*);

  /*******************************\
   * Implements IResourceLocator *
  \*******************************/
  std::string getPath(IHUDComponentFactory*);
  std::string getPath(IColour*);
  std::string getPath(IFont*);
  std::string getPath(ICamera*);
  std::string getPath(IInteger*);
  std::string getPath(ISound*);
  std::string getPath(I3DModel*);
  std::string getPath(I3DModelFactory*);
  std::string getPath(IZoneHandler*);
  std::string getPath(IBoolean*);
  std::string getPath(IFloat*);
  std::string getPath(IProject*);
  std::string getPath(IVertex*);
  std::string getPath(ITexture*);
  std::string getPath(IString*);
  std::string getPath(IElementType*);
  std::string getPath(IElementGroupType*);
  std::string getPath(ISurfaceProcessor*);
  std::string getPath(ICollidableSurfaceRegistry*);
  std::string getPath(ICollectables*);
  std::string getPath(IArgumentSource*);
  void saveScript(DOMNodeWriter*, const std::string&, IScript*);
  bool isImplicit(IElement*);
  
  /******************************\
   * Implements ResourceManager *
  \******************************/
  std::vector<I3DModelFactory*> getAllModelTypes();
  std::vector<IBoolean*> getAllBooleans();
  std::vector<ICamera*> getAllCameras();
  std::vector<ICollidableSurfaceRegistry*> getAllSurfaceRegistries();
  std::vector<ICollectables*> getAllCollectableRegistries();
  std::vector<IColour*> getAllColours();
  std::vector<IComponentCustomType*> getAllComponentCustomTypes();
  std::vector<IElementType*> getAllElementTypes();
  std::vector<IFloat*> getAllFloats();
  std::vector<IFont*> getAllFonts();
  std::vector<IHUDComponentFactory*> getAllHUDComponentTypes();
  std::vector<IInteger*> getAllIntegers();
  std::vector<ISound*> getAllSounds();
  std::vector<IString*> getAllStrings();
  std::vector<ISurfaceProcessor*> getAllSurfaceProcessors();
  std::vector<ITexture*> getAllTextures();
  std::vector<IVertex*> getAllLocations();
  std::vector<IZoneHandler*> getAllZoneHandlers();
  
  void addResourceListener(IResourceListener<I3DModelFactory>*);          
  void addResourceListener(IResourceListener<IBoolean>*);                 
  void addResourceListener(IResourceListener<ICamera>*);                  
  void addResourceListener(IResourceListener<ICollidableSurfaceRegistry>*);
  void addResourceListener(IResourceListener<ICollectables>*);        
  void addResourceListener(IResourceListener<IColour>*);              
  void addResourceListener(IResourceListener<IComponentCustomType>*); 
  void addResourceListener(IResourceListener<IElementType>*);         
  void addResourceListener(IResourceListener<IFloat>*);               
  void addResourceListener(IResourceListener<IFont>*);                
  void addResourceListener(IResourceListener<IHUDComponentFactory>*); 
  void addResourceListener(IResourceListener<IInteger>*);         
  void addResourceListener(IResourceListener<ISound>*);           
  void addResourceListener(IResourceListener<IString>*);          
  void addResourceListener(IResourceListener<ISurfaceProcessor>*);
  void addResourceListener(IResourceListener<ITexture>*);         
  void addResourceListener(IResourceListener<IVertex>*);          
  void addResourceListener(IResourceListener<IZoneHandler>*);     
  
  void addResourceType(IResourceType<I3DModelFactory>*,            const std::string&);
  void addResourceType(IResourceType<IBoolean>*,                   const std::string&);
  void addResourceType(IResourceType<ICamera>*,                    const std::string&);
  void addResourceType(IResourceType<ICollidableSurfaceRegistry>*, const std::string&);
  void addResourceType(IResourceType<ICollectables>*,              const std::string&);
  void addResourceType(IResourceType<IColour>*,                    const std::string&);
  void addResourceType(IResourceType<IComponentCustomType>*,       const std::string&);
  void addResourceType(IResourceType<IElementType>*,               const std::string&);
  void addResourceType(IResourceType<IFloat>*,                     const std::string&);
  void addResourceType(IResourceType<IFont>*,                      const std::string&);
  void addResourceType(IResourceType<IHUDComponentFactory>*,       const std::string&);
  void addResourceType(IResourceType<IInteger>*,                   const std::string&);
  void addResourceType(IResourceType<ISound>*,                     const std::string&);
  void addResourceType(IResourceType<IString>*,                    const std::string&);
  void addResourceType(IResourceType<ISurfaceProcessor>*,          const std::string&);
  void addResourceType(IResourceType<ITexture>*,                   const std::string&);
  void addResourceType(IResourceType<IVertex>*,                    const std::string&);
  void addResourceType(IResourceType<IZoneHandler>*,               const std::string&);

  void editResource(I3DModelFactory*,            IResourceAccessor*);
  void editResource(IBoolean*,                   IResourceAccessor*);
  void editResource(ICamera*,                    IResourceAccessor*);
  void editResource(ICollidableSurfaceRegistry*, IResourceAccessor*);
  void editResource(ICollectables*,              IResourceAccessor*);
  void editResource(IColour*,                    IResourceAccessor*);
  void editResource(IComponentCustomType*,       IResourceAccessor*);
  void editResource(IElementType*,               IResourceAccessor*);
  void editResource(IFloat*,                     IResourceAccessor*);
  void editResource(IFont*,                      IResourceAccessor*);
  void editResource(IHUDComponentFactory*,       IResourceAccessor*);
  void editResource(IInteger*,                   IResourceAccessor*);
  void editResource(ISound*,                     IResourceAccessor*);
  void editResource(IString*,                    IResourceAccessor*);
  void editResource(ISurfaceProcessor*,          IResourceAccessor*);
  void editResource(ITexture*,                   IResourceAccessor*);
  void editResource(IVertex*,                    IResourceAccessor*);
  void editResource(IZoneHandler*,               IResourceAccessor*);

  void removeResource(I3DModelFactory*,            IResourceAccessor*);
  void removeResource(IBoolean*,                   IResourceAccessor*);
  void removeResource(ICamera*,                    IResourceAccessor*);
  void removeResource(ICollidableSurfaceRegistry*, IResourceAccessor*);
  void removeResource(ICollectables*,              IResourceAccessor*);
  void removeResource(IColour*,                    IResourceAccessor*);
  void removeResource(IComponentCustomType*,       IResourceAccessor*);
  void removeResource(IElementType*,               IResourceAccessor*);
  void removeResource(IFloat*,                     IResourceAccessor*);
  void removeResource(IFont*,                      IResourceAccessor*);
  void removeResource(IHUDComponentFactory*,       IResourceAccessor*);
  void removeResource(IInteger*,                   IResourceAccessor*);
  void removeResource(ISound*,                     IResourceAccessor*);
  void removeResource(IString*,                    IResourceAccessor*);
  void removeResource(ISurfaceProcessor*,          IResourceAccessor*);
  void removeResource(ITexture*,                   IResourceAccessor*);
  void removeResource(IVertex*,                    IResourceAccessor*);
  void removeResource(IZoneHandler*,               IResourceAccessor*);
 
  /****************\
   * Implements ? *
  \****************/
  void notifyChange(IColour*);
  void notifyChange(ITexture*);
};
#endif
