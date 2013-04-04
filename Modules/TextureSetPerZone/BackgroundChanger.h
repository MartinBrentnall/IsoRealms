#ifndef BACKGROUND_CHANGER_H
#define BACKGROUND_CHANGER_H

#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/ElementType/IElement.h>

class BackgroundChanger:public IElement,
                        public IDynamicElement {
  private:
  Colour* cPreviousBackgroundColour;
  Colour* cTargetBackgroundColour;
  float cProgressBackgroundColour;
  
  public:
  BackgroundChanger();

  /******************************\
   * Implements IDynamicElement *
  \******************************/
  void update(int);  
};

#endif
