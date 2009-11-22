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
#ifndef I_SIMPLE_MODEL_FACTORY_H
#define I_SIMPLE_MODEL_FACTORY_H

#include "ISimpleModel.h"

class ISimpleModelFactory:public virtual IPlugin {
  public:
  virtual ~ISimpleModelFactory() {}

  /**
   * Returns an model instance.  Clients should not assume that the instance
   * will be unique; in cases where a model instance is sufficiently simple to
   * be reused, the same instance may be used across all usages.
   */
  virtual ISimpleModel* createModel() = 0;
};

#endif
