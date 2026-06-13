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
   * Component definition for an iso-realms project, allowing projects to be
   * nested.
   */
  class Project final : public IInputHandler,
                        public IScreen {
    public:
    
    /***********************\
     * Component Interface *
    \***********************/
    Project(Basics& basics, IComponentData& data);
    void registerAssets(ComponentAssetRegistry& assets);  
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);

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
    void prepareNewProject();
    void prepare(const std::string& file, bool user, bool force);
    bool isReady();
    void resetProject();
    void resetLauncher(const ProjectLaunchConfiguration* launcher);
    void resetOptions(Options& options);
    IEditable* getDefaultEditor();
    IsoRealms::Project* getProject();
    bool isUser();
    void saveProject();
    void saveProjectAs(const std::string& file);
    bool exists(const std::string& file);
    void requestQuit();
    bool isQuitRequestGranted() const;
    
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
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    
    // External interfaces.
    IsoRealms::Project& cProject;

    // Definition data
    std::string cDefProjectPath;
    bool cDefProjectUser;
    Action cDefReadyAction;
    Action cDefEndAction;
    Action cDefErrorAction;
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
    bool cRuntimeIgnoreTasks;
    Options cRuntimeOptions;
    bool cRuntimeResetPostponed;

    // Misc.
    LuaBinding<Project> cLuaBinding;
  };
}
