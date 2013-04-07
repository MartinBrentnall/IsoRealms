#ifndef BACKGROUND_CHANGER_H
#define BACKGROUND_CHANGER_H

#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/ElementType/IElement.h>

class BackgroundChanger:public IElement {
  private:
  float cChangeSpeed;
  IElementType* cElementType;
  IColour* cPreviousBackgroundColour;
  IColour* cTargetBackgroundColour;
  float cProgressBackgroundColour;
  
  public:
  BackgroundChanger(IElementType*);
  
  void setColour(IColour*);
  void setChangeSpeed(float);

  /***********************\
   * Implements IElement *
  \***********************/
  IPlugin* getElementSet();
  IElementType* getElementType();
  bool initElement(unsigned int);
  void renderStatic();
  void renderRuntime();
  void renderEditing();
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  void input(SDL_Event&);
  bool isVisualRuntime();
  bool isVisualEditing();
  bool isDynamicRuntime();
  bool isDynamicEditing();
  bool isInteractive();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setDirty();
};

#endif
