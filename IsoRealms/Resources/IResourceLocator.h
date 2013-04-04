#ifndef I_RESOURCE_LOCATOR_H
#define I_RESOURCE_LOCATOR_H

#include <string>

class IHUDComponentFactory;
class IColour;
class IFont;
class ICamera;
class ICollectables;
class IInteger;
class ISound;
class I3DModel;
class I3DModelFactory;
class IZoneHandler;
class IZone;
class IFloat;
class IBoolean;
class IProject;
class IVertex;
class ITexture;
class IString;
class IElementType;
class IElementGroupType;
class ISurfaceProcessor;
class ICollidableSurfaceRegistry;
class IArgumentSource;
class IScript;
class DOMNodeWriter;
class IElement;

class IResourceLocator {
  public:
  virtual std::string getPath(I3DModel*) = 0;
  virtual std::string getPath(I3DModelFactory*) = 0;
  virtual std::string getPath(IArgumentSource*) = 0;
  virtual std::string getPath(IBoolean*) = 0;
  virtual std::string getPath(ICamera*) = 0;
  virtual std::string getPath(ICollectables*) = 0;
  virtual std::string getPath(ICollidableSurfaceRegistry*) = 0;
  virtual std::string getPath(IColour*) = 0;
  virtual std::string getPath(IElementGroupType*) = 0;
  virtual std::string getPath(IElementType*) = 0;
  virtual std::string getPath(IFloat*) = 0;
  virtual std::string getPath(IFont*) = 0;
  virtual std::string getPath(IHUDComponentFactory*) = 0;
  virtual std::string getPath(IInteger*) = 0;
  virtual std::string getPath(IProject*) = 0;
  virtual std::string getPath(ISound*) = 0;
  virtual std::string getPath(IString*) = 0;
  virtual std::string getPath(ISurfaceProcessor*) = 0;
  virtual std::string getPath(ITexture*) = 0;
  virtual std::string getPath(IVertex*) = 0;
//  virtual std::string getPath(IZone*) = 0;
  virtual std::string getPath(IZoneHandler*) = 0;
  
  virtual void saveScript(DOMNodeWriter*, const std::string&, IScript*) = 0;
  
  virtual bool isImplicit(IElement*) = 0;
};

#endif
