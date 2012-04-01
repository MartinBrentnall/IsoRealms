#ifndef I_RESOURCES_H
#define I_RESOURCES_H

#include "I3DModel.h"
#include "I3DModelFactory.h"
#include "IColour.h"
#include "IProject.h"
#include "IRuntimeContext.h"

class IResources {
  public:
  virtual bool isEditing() = 0;
  virtual IProject* getProject() = 0;
  virtual Script* getScript(DOMNodeWrapper*) = 0;
  virtual IColour* getColour(DOMNodeWrapper*) = 0;
  virtual ITexture* getTexture(DOMNodeWrapper*) = 0;
  virtual I3DModel* getModel(DOMNodeWrapper*, Vertex*) = 0;
  virtual I3DModel* getModel(const std::string&, Vertex*) = 0;
  virtual void add(ICommand*, std::vector<std::string>, std::string) = 0;
  virtual void add(IColour*, std::vector<std::string>, std::string) = 0;
  virtual void add(ITexture*, std::vector<std::string>, std::string) = 0;
  virtual void add(I3DModelFactory*, std::vector<std::string>, std::string) = 0;
};

#endif
