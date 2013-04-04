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
#ifndef I_ENTITY_CLASS_H
#define I_ENTITY_CLASS_H

#include <vector>
#include <string>

/**
 * Implement this interface if you wish to use the instantiation dialog to
 * instantiate something.
 */
class IEntityClass {
  public:  

  /**
   * Return the human readable name of this entity class.
   * 
   * @returns  The human readable name of this entity class.
   */
  virtual std::string getEntityClassName() = 0;

  /**
   * Create a new instance.
   * 
   * @param std::string&  The type to instantiate.
   * @param std::string&  The name of the new instance.
   */
  virtual void instantiate(std::string&) = 0;

  /**
   * Remove an existing instance.
   * 
   * @param std::string&  The name of the instance to remove.
   */
  virtual void remove(std::string&) = 0;

  /**
   * Invoke a command that allows the specified instance to be configured.
   * 
   * @param std::string  The name of the instance to configure.
   */
  virtual void configure(std::string&) = 0;

  /**
   * Get a list of existing instances.
   * 
   * @returns  A vector containing names of existing instances.
   */
  virtual std::vector<std::string*> getInstances() = 0;

  /**
   * Get a list of implementations from which instances can be created.
   * 
   * @return  A vector containing names of available implementations.
   */
  virtual std::vector<std::string*> getImplementations() = 0;
};

#endif
