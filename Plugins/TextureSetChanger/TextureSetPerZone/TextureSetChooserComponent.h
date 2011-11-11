/*
 * Copyright 2009 Martin Brentnall
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
#ifndef TEXTURE_SET_CHOOSER_COMPONENT_H
#define TEXTURE_SET_CHOOSER_COMPONENT_H

#include <SDL/SDL.h>
#include <vector>

#include <IsoRealms/GUI/ComponentEdgeLayout.h>
#include <IsoRealms/GUI/EdgeRelation.h>
#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/GUI/WrappingGridComponent.h>
#include <IsoRealms/IsoRealmsConstants.h>

class TextureSetChooserComponent:public ResizableDialog {
  private:

  class TextureIcon:public ISizedComponent {
    private:
    TextureSetChooserComponent* cParent;

    bool mouseButtonDown(SDL_Event&);

    public:
    TextureIcon(TextureSetChooserComponent*);

    /******************************\
     * Implements ISizedComponent *
    \******************************/
    float getWidth();
    float getHeight();
    void render();
    void update(int);
    bool input(SDL_Event&);
  };

  public:
  TextureSetChooserComponent(IComponentContainer*);

  /******************************\
   * Implements ResizableDialog *
  \******************************/
  void renderResizableDialogContent();
  void updateResizableDialogContent(int);
  bool inputResizableDialogContent(SDL_Event&);
};

#endif
