#ifndef RESIZABLE_DIALOG_H
#define RESIZABLE_DIALOG_H

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <string>

#include "../Configuration.h"
#include "../IFont.h"
#include "../ScreenConfiguration.h"

#include "Dialog.h"

class ResizableDialog:public Dialog {
  private:
  static const float RESIZE_HANDLE_SIZE;

  bool cResizing;
  std::vector<IHUDComponent*> cChildren;
  IHUDComponent* cFocusedComponent;

  void testFocusChange(SDL_Event& event);
  bool mouseButtonDown(SDL_Event&);
  bool mouseMotion(SDL_Event&);

  public:
  ResizableDialog(IComponentContainer*, const std::string&, float, float, float, float);
  void addComponent(IHUDComponent*);
  void setFocusedComponent(IHUDComponent*);

  virtual void renderResizableDialogContent() {}
  virtual void updateResizableDialogContent(int) {}
  virtual bool inputResizableDialogContent(SDL_Event&) {return false;}

  /************************\
   * Implements Rectangle *
  \************************/
  void renderContent();
  void updateContent(int);
  bool inputContent(SDL_Event&);

  virtual ~ResizableDialog();
};

#endif
