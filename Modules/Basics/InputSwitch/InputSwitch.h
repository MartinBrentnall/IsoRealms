/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <vector>

#include "IsoRealms.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Component definition for a switchable input handler.
   */
  class InputSwitch final : public IInputHandler {
    public:

    /***********************\
     * Component Interface *
    \***********************/
    InputSwitch(Basics& basics, IComponentData& data);
    void define(IComponentDefiner& definer);
    void publish(ResourcePublisher& publisher);

    /*********************\
     * Module interfaces *
    \*********************/
    void reset();

    /****************************\
     * Implements IInputHandler *
    \****************************/
    bool input(sf::Event& event) override;
    void resetInput() override;
    
    /***********************\
     * Scripting Interface *
    \***********************/
    void setInputHandler(IInputHandler* inputHandler);
    
    private:
    
    // Definition data.
    InputHandler cDefInputHandler;       /// Initial value.

    // Runtime data.
    IInputHandler* cRuntimeInputHandler; /// Current value.

    // Scripting Interface.
    LuaBinding<InputSwitch> cLuaBinding;
  };
}
