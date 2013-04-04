#ifndef I_RESOURCE_MANAGER_H
#define I_RESOURCE_MANAGER_H

#include "IResourceListener.h"
#include "IResourceType.h"

class IResourceManager:public IResourceLocator {
  public:
  virtual std::vector<I3DModelFactory*> getAllModelTypes() = 0;
  virtual std::vector<IBoolean*> getAllBooleans() = 0;
  virtual std::vector<ICamera*> getAllCameras() = 0;
  virtual std::vector<ICollidableSurfaceRegistry*> getAllSurfaceRegistries() = 0;
  virtual std::vector<ICollectables*> getAllCollectableRegistries() = 0;
  virtual std::vector<IColour*> getAllColours() = 0;
  virtual std::vector<IComponentCustomType*> getAllComponentCustomTypes() = 0;
  virtual std::vector<IElementType*> getAllElementTypes() = 0;
  virtual std::vector<IFloat*> getAllFloats() = 0;
  virtual std::vector<IFont*> getAllFonts() = 0;
  virtual std::vector<IHUDComponentFactory*> getAllHUDComponentTypes() = 0;
  virtual std::vector<IInteger*> getAllIntegers() = 0;
  virtual std::vector<ISound*> getAllSounds() = 0;
  virtual std::vector<IString*> getAllStrings() = 0;
  virtual std::vector<ISurfaceProcessor*> getAllSurfaceProcessors() = 0;
  virtual std::vector<ITexture*> getAllTextures() = 0;
  virtual std::vector<IVertex*> getAllLocations() = 0;
  virtual std::vector<IZoneHandler*> getAllZoneHandlers() = 0;
  
  virtual void addResourceListener(IResourceListener<I3DModelFactory>*) = 0;          
  virtual void addResourceListener(IResourceListener<IBoolean>*) = 0;                 
  virtual void addResourceListener(IResourceListener<ICamera>*) = 0;                  
  virtual void addResourceListener(IResourceListener<ICollidableSurfaceRegistry>*) = 0;
  virtual void addResourceListener(IResourceListener<ICollectables>*) = 0;        
  virtual void addResourceListener(IResourceListener<IColour>*) = 0;              
  virtual void addResourceListener(IResourceListener<IComponentCustomType>*) = 0; 
  virtual void addResourceListener(IResourceListener<IElementType>*) = 0;         
  virtual void addResourceListener(IResourceListener<IFloat>*) = 0;               
  virtual void addResourceListener(IResourceListener<IFont>*) = 0;                
  virtual void addResourceListener(IResourceListener<IHUDComponentFactory>*) = 0; 
  virtual void addResourceListener(IResourceListener<IInteger>*) = 0;         
  virtual void addResourceListener(IResourceListener<ISound>*) = 0;           
  virtual void addResourceListener(IResourceListener<IString>*) = 0;          
  virtual void addResourceListener(IResourceListener<ISurfaceProcessor>*) = 0;
  virtual void addResourceListener(IResourceListener<ITexture>*) = 0;         
  virtual void addResourceListener(IResourceListener<IVertex>*) = 0;          
  virtual void addResourceListener(IResourceListener<IZoneHandler>*) = 0;     
  
  virtual void addResourceType(IResourceType<I3DModelFactory>*,            const std::string&) = 0;
  virtual void addResourceType(IResourceType<IBoolean>*,                   const std::string&) = 0;
  virtual void addResourceType(IResourceType<ICamera>*,                    const std::string&) = 0;
  virtual void addResourceType(IResourceType<ICollidableSurfaceRegistry>*, const std::string&) = 0;
  virtual void addResourceType(IResourceType<ICollectables>*,              const std::string&) = 0;
  virtual void addResourceType(IResourceType<IColour>*,                    const std::string&) = 0;
  virtual void addResourceType(IResourceType<IComponentCustomType>*,       const std::string&) = 0;
  virtual void addResourceType(IResourceType<IElementType>*,               const std::string&) = 0;
  virtual void addResourceType(IResourceType<IFloat>*,                     const std::string&) = 0;
  virtual void addResourceType(IResourceType<IFont>*,                      const std::string&) = 0;
  virtual void addResourceType(IResourceType<IHUDComponentFactory>*,       const std::string&) = 0;
  virtual void addResourceType(IResourceType<IInteger>*,                   const std::string&) = 0;
  virtual void addResourceType(IResourceType<ISound>*,                     const std::string&) = 0;
  virtual void addResourceType(IResourceType<IString>*,                    const std::string&) = 0;
  virtual void addResourceType(IResourceType<ISurfaceProcessor>*,          const std::string&) = 0;
  virtual void addResourceType(IResourceType<ITexture>*,                   const std::string&) = 0;
  virtual void addResourceType(IResourceType<IVertex>*,                    const std::string&) = 0;
  virtual void addResourceType(IResourceType<IZoneHandler>*,               const std::string&) = 0;

  virtual void editResource(I3DModelFactory*,            IResourceAccessor*) = 0;
  virtual void editResource(IBoolean*,                   IResourceAccessor*) = 0;
  virtual void editResource(ICamera*,                    IResourceAccessor*) = 0;
  virtual void editResource(ICollidableSurfaceRegistry*, IResourceAccessor*) = 0;
  virtual void editResource(ICollectables*,              IResourceAccessor*) = 0;
  virtual void editResource(IColour*,                    IResourceAccessor*) = 0;
  virtual void editResource(IComponentCustomType*,       IResourceAccessor*) = 0;
  virtual void editResource(IElementType*,               IResourceAccessor*) = 0;
  virtual void editResource(IFloat*,                     IResourceAccessor*) = 0;
  virtual void editResource(IFont*,                      IResourceAccessor*) = 0;
  virtual void editResource(IHUDComponentFactory*,       IResourceAccessor*) = 0;
  virtual void editResource(IInteger*,                   IResourceAccessor*) = 0;
  virtual void editResource(ISound*,                     IResourceAccessor*) = 0;
  virtual void editResource(IString*,                    IResourceAccessor*) = 0;
  virtual void editResource(ISurfaceProcessor*,          IResourceAccessor*) = 0;
  virtual void editResource(ITexture*,                   IResourceAccessor*) = 0;
  virtual void editResource(IVertex*,                    IResourceAccessor*) = 0;
  virtual void editResource(IZoneHandler*,               IResourceAccessor*) = 0;

  virtual void removeResource(I3DModelFactory*,            IResourceAccessor*) = 0;
  virtual void removeResource(IBoolean*,                   IResourceAccessor*) = 0;
  virtual void removeResource(ICamera*,                    IResourceAccessor*) = 0;
  virtual void removeResource(ICollidableSurfaceRegistry*, IResourceAccessor*) = 0;
  virtual void removeResource(ICollectables*,              IResourceAccessor*) = 0;
  virtual void removeResource(IColour*,                    IResourceAccessor*) = 0;
  virtual void removeResource(IComponentCustomType*,       IResourceAccessor*) = 0;
  virtual void removeResource(IElementType*,               IResourceAccessor*) = 0;
  virtual void removeResource(IFloat*,                     IResourceAccessor*) = 0;
  virtual void removeResource(IFont*,                      IResourceAccessor*) = 0;
  virtual void removeResource(IHUDComponentFactory*,       IResourceAccessor*) = 0;
  virtual void removeResource(IInteger*,                   IResourceAccessor*) = 0;
  virtual void removeResource(ISound*,                     IResourceAccessor*) = 0;
  virtual void removeResource(IString*,                    IResourceAccessor*) = 0;
  virtual void removeResource(ISurfaceProcessor*,          IResourceAccessor*) = 0;
  virtual void removeResource(ITexture*,                   IResourceAccessor*) = 0;
  virtual void removeResource(IVertex*,                    IResourceAccessor*) = 0;
  virtual void removeResource(IZoneHandler*,               IResourceAccessor*) = 0;
};

#endif
