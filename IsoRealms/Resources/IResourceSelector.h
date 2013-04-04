#ifndef I_RESOURCE_SELECTOR_H
#define I_RESOURCE_SELECTOR_H

#include <IsoRealms/Resources/Colour/IColour.h>
#include <IsoRealms/Resources/Texture/ITexture.h>

#include "IResourceSelectionListener.h"

class IResourceSelector {
  public:
  virtual void addResourceSelectionListener(IResourceSelectionListener<IColour>*) = 0;
  virtual void addResourceSelectionListener(IResourceSelectionListener<ITexture>*) = 0;
  
  virtual void removeResourceSelectionListener(IResourceSelectionListener<IColour>*) = 0;
  virtual void removeResourceSelectionListener(IResourceSelectionListener<ITexture>*) = 0;
  
  virtual void notifyResourceReleased(IColour*) = 0;
  
  virtual void notifyResourceOwned(IColour*) = 0;
};

#endif
