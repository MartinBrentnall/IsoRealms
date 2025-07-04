/*
 * Copyright 2023 Martin Brentnall
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
#include "Project.h"

#include "IsoRealms/IApplication.h"
#include "IsoRealms/System.h"

namespace IsoRealms::Basics {
  const std::string Project::JSON_EDITING   = "editing";
  const std::string Project::JSON_ON_FINISH = "onFinish";
  const std::string Project::JSON_ON_READY  = "onReady";
  const std::string Project::JSON_OPTIONS   = "options";
  const std::string Project::JSON_RUNNING   = "running";

  Project::Project(IProject& project, Basics& basics, IResourceData& data) :
            cProject(project),
            cDefReadyAction(data.getDummyActionClient()),
            cDefEndAction(data.getDummyActionClient()),
            cDefProjectOptions(data),
            cDefRunning(false),
            cDefEditing(false),
            cRuntimeProject(nullptr),
            cLuaBinding(project, this) {
  }
  
  Project::Project(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            Project(project, basics, data) {
    cDefRunning = object.getBoolean(JSON_RUNNING);
    cDefEditing = object.getBoolean(JSON_EDITING);
    cDefEndAction.init(object, JSON_ON_FINISH);
    cDefReadyAction.init(object, JSON_ON_READY);
    cDefProjectOptions.init(object, JSON_OPTIONS);
    cDefProjectOptionsArg = options.getFixedOptions();
  }

  void Project::registerAssets(IAssetRegistry& assets) {
    assets.add(static_cast<IScreen*>(this), "", "Projects");
    assets.add(static_cast<IInputHandler*>(this), "", "Projects");
    assets.add(&cLuaBinding, "", "Projects");
  }
  
  void Project::save(JSONObject object) const {
    object.addBoolean(JSON_RUNNING, cDefRunning);
    object.addBoolean(JSON_EDITING, cDefEditing);
    cDefEndAction.save(object, JSON_ON_FINISH);
    cDefReadyAction.save(object, JSON_ON_READY);
    cDefProjectOptions.save(object, JSON_OPTIONS);

  }

  void Project::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool Project::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> Project::getProperties(IResourceData& owner) {
    return std::vector<std::unique_ptr<IProperty>>();
  }
  
  void Project::reset() {

    // Wait to prevent a crash if a project is still under construction..
    if (cRuntimeProjectLoader != nullptr) {
      while (cRuntimeLoading) {
        IsoRealms::Project* mProject = cRuntimeProjectLoader->getLoadedProject();
        if (mProject != nullptr) {
          cRuntimeLoading = false;
        }
      }
    }
    cRuntimeOldProjects.clear();
    cRuntimeLoading            = false;
    cRuntimeRunning            = cDefRunning;
    cRuntimeEditing            = cDefEditing;
    cRuntimeQuitRequestGranted = false;
    
    // Start loading the project, ready action will be executed when it's done.
    std::string mFile = cDefProjectOptionsArg.getOption("file");
    if (mFile != "") {
      prepareInternal(&cDefProjectOptionsArg, false);
    } else {
      prepare(*cDefProjectOptions, false);
    }
  }

  void Project::updateRuntime(unsigned int milliseconds) {
    if (cRuntimeLoading) {
      IsoRealms::Project* mProject = cRuntimeProjectLoader->getLoadedProject();
      if (mProject != nullptr) {
        cRuntimeProject = mProject;
        cRuntimeProject->reset();
        cDefReadyAction.execute();
        cRuntimeLoading = false;
      }
    }

    if (cRuntimeRunning && cRuntimeProject != nullptr) {
      cRuntimeProject->updateRuntime(milliseconds);
      cRuntimeProject->updateRuntimeComplete();
    }
    if (cRuntimeEditing && cRuntimeProject != nullptr) {
      cRuntimeProject->updateEditing(milliseconds);
    }

    // Clean up any old projects that could have still been being constructed.
    if (!cRuntimeLoading) {
      for (int i = static_cast<int>(cRuntimeOldProjects.size()) - 1; i >= 0; i--) {
        if (cRuntimeOldProjects[i]->isDestructReady()) {
          cRuntimeOldProjects[i]->setDestructing();
          IApplication& mApplication = cProject.getApplication();
          mApplication.executeAndReturn([this, i, &mApplication]() {
            cRuntimeOldProjects[i]->destruct();
          });
        }

        if (cRuntimeOldProjects[i]->isDestructed()) {
          cRuntimeOldProjects.erase(cRuntimeOldProjects.begin() + i);
        }
      }
    }
  }

  void Project::setRunning(bool running) {
    cRuntimeRunning = running;
  }

  void Project::setEditing(bool editing) {
    cRuntimeEditing = editing;
  }

  void Project::prepare(IProjectOptions* options, bool force) {
    Options mProjectOptions = options->getFixedOptions();
    prepareInternal(&mProjectOptions, force);
  }

  bool Project::isReady() {
    return cRuntimeProject != nullptr;
  }
  
  void Project::resetProject() {
    cRuntimeProject->reset();
  }
  
  IEditable* Project::getDefaultEditor() {
    return cRuntimeProject->getDefaultEditable();
  }

  IsoRealms::Project* Project::getProject() {
    return cRuntimeProject;
  }

  bool Project::canSave() {
    return cRuntimeProject != nullptr && cRuntimeProject->canSave();
  }

  void Project::saveProject() {
    if (cRuntimeProject != nullptr) {
      cRuntimeProject->save();
    }
  }

  void Project::saveProjectAs(const std::string& name) {
    if (cRuntimeProject != nullptr) {
      cRuntimeProject->save(name + ".isorealms");
    }
  }

  bool Project::exists(const std::string& file) {
    return System::fileExists(file + ".isorealms", true);
  }

  void Project::requestQuit() {
    cRuntimeProject->requestQuit();
  }

  bool Project::isQuitRequestGranted() const {
    return cRuntimeQuitRequestGranted;
  }

  void Project::setProperty(const std::string& id, const std::string& value) {
    cRuntimeProject->setProperty(id, value);
  }

  bool Project::input(sf::Event& event) {
    if (cRuntimeProject != nullptr) {
      return cRuntimeProject->input(event);
    }
    return false;
  }

  void Project::resetInput() {
    // Nothing to do.
  }

  void Project::renderScreen(float scale, float aspectRatio) const {
    if (cRuntimeProject != nullptr) {
      if (cRuntimeRunning) {
        glEnable(GL_DEPTH_TEST);
        cRuntimeProject->render(aspectRatio);
      }
    }
  }

  bool Project::renderAssetIcon() const {
    return false;
  }

  void Project::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Project::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Project::isDefaultConfiguration() const {
    return true;
  }

  void Project::prepareInternal(const Options* options, bool force) {

    // If it's the same as the current project, nothing to do.
    if (!force && cRuntimeProjectLoader != nullptr && cRuntimeProjectLoader->matches(*options)) {
      cRuntimeLoading = true;
      cRuntimeProject = nullptr;
      return;
    }
    
    // Set any existing project to be destructed.
    if (cRuntimeProjectLoader != nullptr) {
      cRuntimeOldProjects.emplace_back(std::move(cRuntimeProjectLoader));
      cRuntimeProjectLoader = nullptr;
    }
    
    // If the requested project matches any awaiting destruction, cancel the destruction and use it.
    if (!force) {
      for (unsigned int i = 0; i < cRuntimeOldProjects.size(); i++) {
        if (cRuntimeOldProjects[i]->matches(*options)) {
          cRuntimeProjectLoader = std::move(cRuntimeOldProjects[i]);
          cRuntimeOldProjects.erase(cRuntimeOldProjects.begin() + i);
          cRuntimeLoading = true;
          cRuntimeProject = nullptr;
          return;
        }
      }
    }
    
    // No existing project loader match; create a new one.
    cRuntimeProjectLoader = std::make_unique<ProjectLoader>(*options, [this](bool quitRequestGranted) {
      cRuntimeRunning = false;
      cRuntimeEditing = true; // TODO: Why do we assume a switch to editing mode?
      cRuntimeQuitRequestGranted = quitRequestGranted;
      cRuntimeProject->reset(); // TODO: Why do we reset here?
      cDefEndAction.execute();
    });
    IApplication& mApplication = cProject.getApplication();
    mApplication.executeAndReturn([this, &mApplication]() {
      cRuntimeProjectLoader->loadProject(mApplication);
    });
    cRuntimeLoading = true;
    cRuntimeProject = nullptr;
  }
}
