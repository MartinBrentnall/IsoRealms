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

#include <filesystem>
#include <functional>
#include <string>

#include "IAssets.h"

namespace IsoRealms {
  template<class TYPE> class IStateListener;

  class Application;
  class File;
  class IApplication;
  class IAssets;
  class IEditable;
  class IFloat;
  class IScreen;
  class IScreenListener;
  class LuaState;
  
  /**
   * Project interface made available to modules and their resources.
   */
  class IProject : public IAssets {
    public:
    virtual bool isLoading() const = 0;
      
    /**
     * Perform the specified allocation task function on the main thread.
     * Allocation tasks will be performed before initialisation tasks.
     * This function is useful when OpenGL textures must be allocated that
     * may be needed for initialisation later.
     * 
     * @param task The allocation task to perform.
     */
    virtual void mainThreadAlloc(std::function<void()> task) = 0;
    
    /**
     * Perform the specified initialisation function on the main thread.
     * initialisation tasks will be performed after allocation tasks.
     * This function is useful for performing OpenGL initialisation,
     * since it is not allowed to call OpenGL functions outside the main
     * thread.
     * 
     * @param task The initialisation task to perform.
     */
    virtual void mainThreadInit(std::function<void()> task) = 0;
    virtual void mainThreadCleanUp(std::function<void()> task) = 0;

    /**
     * Create a callback for initialising asset references.  The callback will
     * be called by the project after all resources and assets have been
     * constructed.
     * 
     * @param initialiser The asset initialisation function.
     */
    virtual void init(std::function<void(IAssets&)> initialiser) = 0;
    
    /**
     * Defer the specified function to be performed following completion of the
     * current update cycle.  This may be useful in situations where a task
     * is to be performed in response to an event, and it is not known if more
     * events may trigger the same task, causing it to be performed multiple
     * times redundantly.
     * 
     * @param task The task to perform at the end of the current update cycle.
     */
    virtual void updateLater(std::function<void()> task) = 0;    

    /**
     * Retrieve the engine Lua state object.
     * 
     * @return The Lua state object.
     */
    virtual LuaState* const getLuaState() = 0;
    virtual void finish(bool forceQuit) = 0;
    
    // TODO: Shouldn't be exposed to modules
    virtual std::string getDataPath(bool user) = 0;

    virtual bool isUserProject() = 0;
    virtual void addScreenListener(IScreenListener* listener) = 0;
    virtual void removeScreenListener(IScreenListener* listener) = 0;
    virtual void screenPreRender(IScreen* screen) = 0;
    virtual void screenPostRender(IScreen* screen) = 0;
    virtual bool isProcessingInput() = 0;
    virtual void postponeAction(IAction* action) = 0;

    // TODO: Probably shouldn't be here?
    virtual void addStateChangeListener(const IFloat* asset, IStateListener<IFloat*>* listener) = 0;
    virtual IEditable* getDefaultEditable() = 0;

    virtual std::filesystem::file_time_type getLastWriteTime() = 0;

    virtual void initMainThread() = 0;

    virtual IProject& getAssetManager() = 0; // TODO: Remove this (used for prototyping)
    virtual bool isReadOnly() const = 0;     // TODO: Remove this (used for prototyping)
    virtual File* getFile() = 0;
    virtual IApplication& getApplication() = 0;
  };
}
