#ifndef GUI_ICON_H
#define GUI_ICON_H

#include <IsoRealms/Configuration.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>

#include "AbstractRectangularComponent.h"

class GUIIcon:public AbstractRectangularComponent {
  private:
  I3DModel* cIconModel;
   
  public:
  GUIIcon(I3DModel*);

  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
};

#endif
