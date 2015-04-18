/*
 * Copyright 2015 Martin Brentnall
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
#ifndef I_RESOURCE_BROWSER_H
#define I_RESOURCE_BROWSER_H

template <class T> class Icon;

template <class T> class IResourceBrowser {
  public:
  virtual void editResource(T*) = 0;
  virtual void removeResource(T*) = 0;
  virtual void setSelected(Icon<T>*) = 0;
  virtual std::string getResourceLocation(T*) = 0;
};

#endif
