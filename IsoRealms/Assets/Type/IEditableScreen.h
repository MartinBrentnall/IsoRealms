/*
 * Copyright 2011 Martin Brentnall
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
#pragma once

#include "IBoolean.h"
#include "IFont.h"
#include "IInputHandler.h"
#include "IScreen.h"

namespace IsoRealms {
  class Action;

  class IEditableScreen : public IScreen,
                          public IInputHandler {
    public:
    virtual void notifyVisible() = 0;
    virtual void notifyHidden() = 0;
    virtual void notifyLostFocus() = 0;
    virtual void notifyGainedFocus() = 0;

    virtual std::vector<std::string> getDigitalInputs() const = 0;
    virtual std::vector<std::string> getAnalogueInputs() const = 0;
    virtual void setDigitalInput(const std::string& name, IBoolean* input) = 0;
    virtual void setAnalogueInput(const std::string& name, IFloat* input) = 0;
    virtual void setExitAction(Action* action) = 0;

    virtual void setAppearance(IFont* font, float scale) = 0;
    virtual IScreen* screen() = 0;

    IInputHandler* inputHandler() {return this;}

    virtual ~IEditableScreen() {}
  };
}
