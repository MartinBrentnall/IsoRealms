#ifndef RESIZABLE_DIALOG_H
#define RESIZABLE_DIALOG_H

#include <GL/gl.h>
#include <SDL/SDL.h>

#include "Configuration.h"
#include "RectangleComponent.h"
#include "ScreenConfiguration.h"

class ResizableDialog:public RectangleComponent {
  private:
  static const float RESIZE_HANDLE_SIZE;

  bool cResizing;

  bool mouseButtonDown(SDL_Event&);
  bool mouseMotion(SDL_Event&);

  public:
  ResizableDialog(IComponentContainer*, float, float, float, float);

  virtual void renderResizableDialogContent() = 0;
  virtual void updateResizableDialogContent(int) = 0;
  virtual bool inputResizableDialogContent(SDL_Event&) = 0;

  /************************\
   * Implements Rectangle *
  \************************/
  void renderContent();
  void updateContent(int);
  bool inputContent(SDL_Event&);
};

#endif
