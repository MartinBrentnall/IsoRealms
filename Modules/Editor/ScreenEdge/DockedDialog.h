#ifndef DOCKED_DIALOG_H
#define DOCKED_DIALOG_H

#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/IHUDComponent.h>

class DockedDialog:public IHUDComponent {
  private:
  Dialog* cDialog;
  
  public:
  DockedDialog(Dialog*);
    
  bool input(SDL_Event&);
  void update(unsigned int);
  void render();
  bool contains(float, float);
  float getHeight();   
  float getWidth();
  void setSize(float, float, float, float);
};

#endif
