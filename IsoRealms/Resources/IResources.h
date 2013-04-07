#ifndef I_RESOURCES_H
#define I_RESOURCES_H

#include <IsoRealms/IProject.h>
#include <IsoRealms/IScript.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/3DModel/I3DModelFactory.h>
#include <IsoRealms/Resources/Camera/ICamera.h>
#include <IsoRealms/Resources/Colour/IColour.h>
#include <IsoRealms/Resources/Float/IFloat.h>
#include <IsoRealms/Resources/Font/IFont.h>
#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/Resources/Sound/ISound.h>
#include <IsoRealms/Resources/String/IString.h>

#include "IRuntimeContext.h"

class IResources:public IResourceAccessor {
  public:
  virtual void add(I3DModel*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(I3DModelFactory*,            std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(IArgumentDefinitionType*,    std::vector<std::string>, const std::string&) = 0;
  virtual void add(IBoolean*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(ICamera*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(ICollectables*,              std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(ICollidableSurfaceRegistry*, std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(IColour*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(IComponentCustomType*,       std::vector<std::string>, const std::string&) = 0;
  virtual void add(IElementGroupType*,          std::vector<std::string>, const std::string&) = 0;
  virtual void add(IElementType*,               std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(IFont*,                      std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(IHUDComponentFactory*,       std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(IInteger*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(IMap*,                       std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(IResource*,                                                                DOMNodeWrapper*) = 0;
  virtual void add(ISound*,                     std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(IString*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(ISurfaceProcessor*,          std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(ITexture*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  virtual void add(IVertex*,                    std::vector<std::string>, const std::string&) = 0;
  virtual void add(IZoneHandler*,               std::vector<std::string>, const std::string&, DOMNodeWrapper*) = 0;
  
  virtual void registerArgument(const std::string&, IArgumentSource* source, std::vector<std::string>, const std::string&) = 0;
  virtual void addDynamicElement(IDynamicElement*) = 0;
  
  virtual void addListener(IResourceUseListener<I3DModelFactory>*) = 0;
  virtual void addListener(IResourceUseListener<IBoolean>*) = 0;       
  virtual void addListener(IResourceUseListener<ICamera>*) = 0;        
  virtual void addListener(IResourceUseListener<ICollectables>*) = 0;  
  virtual void addListener(IResourceUseListener<ICollidableSurfaceRegistry>*) = 0;
  virtual void addListener(IResourceUseListener<IColour>*) = 0;                   
  virtual void addListener(IResourceUseListener<IComponentCustomType>*) = 0;      
  virtual void addListener(IResourceUseListener<IElementType>*) = 0;              
  virtual void addListener(IResourceUseListener<IFloat>*) = 0;                    
  virtual void addListener(IResourceUseListener<IFont>*) = 0;                     
  virtual void addListener(IResourceUseListener<IHUDComponentFactory>*) = 0;      
  virtual void addListener(IResourceUseListener<IInteger>*) = 0;                  
  virtual void addListener(IResourceUseListener<ISound>*) = 0;                    
  virtual void addListener(IResourceUseListener<IString>*) = 0;                   
  virtual void addListener(IResourceUseListener<ISurfaceProcessor>*) = 0;         
  virtual void addListener(IResourceUseListener<ITexture>*) = 0;                  
  virtual void addListener(IResourceUseListener<IVertex>*) = 0;                   
  virtual void addListener(IResourceUseListener<IZoneHandler>*) = 0;              
};

#endif
