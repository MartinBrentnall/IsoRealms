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

#include <exception>
#include <iostream>
#include <string>

namespace IsoRealms {
  
  /**
   * Exception to be thrown when attempting to assign an ID that cannot be
   * used to an asset.
   */
  class ComponentInitException : public std::exception {
    private:
    const std::string cWhat;
    
    public:
    ComponentInitException(const std::string& what);
    
    /*****************************\
     * Implements std::exception *
    \*****************************/
    const char* what() const noexcept override;
  };
}
