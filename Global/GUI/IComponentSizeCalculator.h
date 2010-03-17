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
#ifndef I_COMPONENT_SIZE_CALCULATOR_H
#define I_COMPONENT_SIZE_CALCULATOR_H

/**
 * This interface is to be implemented by classes that are capable of
 * determining the size of a component required to display it in full.  In most
 * common cases, this interface will be implemented by a component itself,
 * where the component is able to determine its own size via its content.
 */
class IComponentSizeCalculator {
  public:

  /**
   * Get the width of the component.
   */
  virtual float getWidth() = 0;
  
  /**
   * Get the height of the component.
   */
  virtual float getHeight() = 0;

  virtual ~IComponentSizeCalculator() {}
};

#endif
