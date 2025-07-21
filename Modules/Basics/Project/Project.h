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

#include "IsoRealms.h"

#include "ProjectLoader.h"

namespace IsoRealms::Basics {
  class Basics;
  
  /**
   * Resource definition for an iso-realms project, allowing projects to be
   * nested.
   */
  class Project final : public IInputHandler,
                        public IScreen {
    public:
    
    /**********************\
     * Resource Interface *
    \**********************/
    Project(IProject& project, Basics& basics, IResourceData& data);
    Project(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(ResourceAssetRegistry& assets);  
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IPropertyOwner& owner);

    /*********************\
     * Module interfaces *
    \*********************/
    void updateRuntime(unsigned int milliseconds);
    void reset();
    
    /***********************\
     * Scripting Interface *
    \***********************/
    void setRunning(bool running);
    void setEditing(bool editing);
    void prepare(IProjectOptions* options, bool force);
    bool isReady();
    void resetProject();
    IEditable* getDefaultEditor();
    IsoRealms::Project* getProject();
    bool canSave();
    void saveProject();
    void saveProjectAs(const std::string& file);
    bool exists(const std::string& file);
    void requestQuit();
    bool isQuitRequestGranted() const;
    void setProperty(const std::string& id, const std::string& value);
    
    /****************************\
     * Implements IInputHandler *
    \****************************/
    bool input(sf::Event& event) override;
    void resetInput() override;
    
    /*********************\
     * Implements IScreen *
    \*********************/
    void renderScreen(float scale, float aspectRatio) const override;

    /*********************\
     * Implements IAsset *
    \*********************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties(IPropertyOwner& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    static const std::string JSON_EDITING;
    static const std::string JSON_ON_FINISH;
    static const std::string JSON_ON_READY;
    static const std::string JSON_OPTIONS;
    static const std::string JSON_RUNNING;

    // External interfaces.
    IProject& cProject;

    // Definition data
    std::string cDefProjectPath;
    Options cDefProjectOptionsArg;
    Action cDefReadyAction;
    Action cDefEndAction;
    ProjectOptions cDefProjectOptions;
    bool cDefRunning;
    bool cDefEditing;

    // Runtime data
    std::unique_ptr<ProjectLoader> cRuntimeProjectLoader;            /// Contains the project currently being loaded.
    IsoRealms::Project* cRuntimeProject;                             /// The currently loaded project.
    std::vector<std::unique_ptr<ProjectLoader>> cRuntimeOldProjects; /// Projects abandoned before construction completed (can't destroy while construction threads are still inside).
    bool cRuntimeLoading;                                            /// True 
    bool cRuntimeRunning;
    bool cRuntimeEditing;
    bool cRuntimeQuitRequestGranted;
    std::mutex cProjectDestructionMutex;

    // Misc.
    LuaBinding<Project> cLuaBinding;

    // Private functions.
    void prepareInternal(const Options* options, bool force);
  };
}
