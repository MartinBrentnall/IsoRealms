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
#include "DummyPluginRegistry.h"

std::map<std::string, IPlugin*> DummyPluginRegistry::cDummyPlugins;  

bool DummyPluginRegistry::isDummyPlugin(IPlugin* instance) {
  for (std::map<std::string, IPlugin*>::iterator i = cDummyPlugins.begin(); i != cDummyPlugins.end(); i++) {
    if (instance == i->second) {
      return true;
    }
  }
  return false;
}

IPlugin* DummyPluginRegistry::getDummyPlugin(const std::string& type) {
  IPlugin* mDummyPlugin = cDummyPlugins[type];
  if (mDummyPlugin == NULL) {
    std::string mPluginLocation = System::getConfigurationResource("Plugins/" + type + "/libDummy");
    void* mPluginSO = dlopen(mPluginLocation.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!mPluginSO) {
      // TODO: This causes segmentation fault
      throw InitException("Cannot load library: " + std::string(dlerror()));
    }
    createPlugin* createPluginFunction = cast_voidptr_to_funcptr<createPlugin*>(dlsym(mPluginSO, "create"));
    const char* mDlsymError = dlerror();
    if (mDlsymError) {
      throw InitException("Cannot load symbol: " + std::string(mDlsymError));
    }
    mDummyPlugin = createPluginFunction(NULL);
    cDummyPlugins[type] = mDummyPlugin;
  }
  return mDummyPlugin;
}

