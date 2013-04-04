/*
 * Copyright 2009,2010 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef RESIZABLE_DIALOG_H
#define RESIZABLE_DIALOG_H

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <string>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/Resources/Font/IFont.h>
#include <IsoRealms/ScreenConfiguration.h>

#include "Dialog.h"

class ResizableDialog:public Dialog {
  private:
  static const float RESIZE_HANDLE_SIZE;

  bool cResizing;

  bool mouseButtonDown(SDL_Event&);
  bool mouseMotion(SDL_Event&);

  public:
  ResizableDialog(IComponentContainer*, const std::string&, IResourceAccessor*);
  ResizableDialog(IComponentContainer*, const std::string&, float, float, float, float);

  virtual void renderResizableDialogContent() {}
  virtual void updateResizableDialogContent(int) {}
  virtual bool inputResizableDialogContent(SDL_Event&) {return false;}

  /************************\
   * Implements Rectangle *
  \************************/
  void renderDialogContent();
  void updateDialogContent(int);
  bool inputDialogContent(SDL_Event&);
};

#endif
