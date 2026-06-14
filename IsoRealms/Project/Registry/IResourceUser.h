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

#include <string>

namespace IsoRealms {
  class ProjectFile;
  
  /**
   * Interface to be implemented by components that use resources, to be notified
   * of changes to the resources that they are using.
   */
  template <typename TYPE> class IResourceUser {
    public:
      
    /**
     * To be called when an resource is about to be destructed.
     * 
     * @param resource The resource that is about to be destructed.
     */
    virtual void relinquish(TYPE* resource) = 0;
    virtual bool isReadOnly() const = 0;
    virtual void setOwner(ProjectFile* owner) = 0;
  };
}
