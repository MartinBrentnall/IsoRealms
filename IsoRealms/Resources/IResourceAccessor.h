#ifndef I_RESOURCE_ACCESSOR_H
#define I_RESOURCE_ACCESSOR_H

#include <string>
#include <vector>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/IScript.h>
#include <IsoRealms/LuaSupport/IArgumentDefinition.h>
#include <IsoRealms/LuaSupport/IArgumentSource.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/ElementType/IElementContainer.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

class IArgumentGenerator;
class I3DModel;
class I3DModelFactory;
class ICamera;
class ICollectables;
class ICollidableSurfaceRegistry;
class IColour;
class IComponentCustomType;
class IFloat;
class IFont;
class IHUDComponentFactory;
class IInteger;
class IProject;
class ISound;
class IString;
class ISurfaceProcessor;
class ITexture;
class IVertex;
class IZoneHandler;

class IResourceAccessor {
  public:
  virtual bool isEditing() = 0;
  
  virtual bool* getDigitalInput(const std::string&) = 0;
  virtual I3DModel* getModel(const std::string&, Vertex*, float = 1.0f) = 0;
  virtual I3DModelFactory* getModelType(const std::string&) = 0;
  virtual ICamera* getCamera(const std::string&) = 0;
  virtual ICollectables* getCollectablesRegistry(const std::string&) = 0;
  virtual ICollidableSurfaceRegistry* getSurfaceRegistry(const std::string&) = 0;
  virtual IColour* getColour(const std::string&) = 0;
  virtual IComponentCustomType* getComponentCustomType(const std::string&) = 0;
  virtual IFloat* getFloat(const std::string&) = 0;
  virtual IFont* getFont(const std::string&) = 0;
  virtual IHUDComponentFactory* getHUDComponentType(const std::string&) = 0;
  virtual IInteger* getInteger(const std::string&) = 0;
  virtual IMap* getMap(const std::string&) = 0;
  virtual ISound* getSound(const std::string&) = 0;
  virtual IString* getString(const std::string&) = 0;
  virtual ISurfaceProcessor* getSurfaceProcessor(const std::string&) = 0;
  virtual ITexture* getTexture(const std::string&) = 0;
  virtual IVertex* getLocation(const std::string&) = 0;
  virtual IZoneHandler* getZoneHandler(const std::string&) = 0;
  
  virtual void loadElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*) = 0;
  virtual void loadDefaultElementGroups(IElementContainer*) = 0;
  virtual IScript* getLuaScript(DOMNodeWrapper*, IArgumentGenerator* = NULL) = 0;
  virtual IArgumentDefinition* getArgumentDefinition(DOMNodeWrapper*) = 0;
  virtual IArgumentDefinition* getRegisteredArgumentDefinition(DOMNodeWrapper*) = 0;
  virtual IArgumentSource* getArgument(DOMNodeWrapper*) = 0;
  virtual IArgumentSource* getRegisteredArgument(DOMNodeWrapper*) = 0;
  
  // TODO: Not sure if this interface is the right place for this
  virtual void notifyChange(IColour*) = 0;
  virtual void notifyChange(ITexture*) = 0;
};

#endif
