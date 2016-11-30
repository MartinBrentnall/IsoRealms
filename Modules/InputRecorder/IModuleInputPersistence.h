/*
 * Copyright 2016 Martin Brentnall
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
#ifndef I_MODULE_INPUT_PERSISTENCE_H
#define I_MODULE_INPUT_PERSISTENCE_H

#include <fstream>
#include <iostream>

#include <IsoRealms/Project.h>

class IModuleInputPersistence {
  public:
  virtual Project* getProject() = 0;
  virtual std::ifstream* getRecording() = 0;
  virtual void setFilename(const std::string&) = 0;
  virtual void quit() = 0;
};

#endif

