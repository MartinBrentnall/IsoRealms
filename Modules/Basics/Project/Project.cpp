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
#include "Project.h"

namespace IsoRealms::Basics {
  Project::Project(Basics& basics, IResourceData& data) :
            cProject(data.getProject()),
            cDefReadyAction(data.getDummyActionContext()),
            cDefEndAction(data.getDummyActionContext()),
            cDefErrorAction(data.getDummyActionContext()),
            cDefRunning(false),
            cDefEditing(false),
            cRuntimeProject(nullptr),
            cRuntimeResetPostponed(false),
            cLuaBinding(data.getProject().getLuaState(), this) {
    cRuntimeProjectLoader = std::make_unique<ProjectLoader>([this](bool quitRequestGranted) {
      cRuntimeRunning = false;
      cRuntimeEditing = true; // TODO: Why do we assume a switch to editing mode?
      cRuntimeQuitRequestGranted = quitRequestGranted;
      cRuntimeProject->reset(); // TODO: Why do we reset here?
      cDefEndAction.execute();
    });
    Application& mApplication = cProject.getApplication();
    cRuntimeProjectLoader->newProject(mApplication);
    cRuntimeProject = cRuntimeProjectLoader->getLoadedProject();
  }
  
  void Project::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IScreen>(this, "", "Projects");
    assets.add<IInputHandler>(this, "", "Projects");
    assets.add<IBinding>(&cLuaBinding, "", "Projects");
  }
  
  void Project::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool Project::renderIcon() const {
    return false;
  }

  void Project::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeString( "file",     [this]() {return cDefProjectPath;}, [this](const std::string& value) {cDefProjectPath = value;});
    owner.createPropertyNativeBoolean("user",     [this]() {return cDefProjectUser;}, [this](bool value)               {cDefProjectUser = value;});
    owner.createPropertyNativeBoolean("running",  [this]() {return cDefRunning;},     [this](bool value)               {cDefRunning     = value;});
    owner.createPropertyNativeBoolean("editing",  [this]() {return cDefEditing;},     [this](bool value)               {cDefEditing     = value;});
    owner.createPropertyTreeSelector( "onFinish", cDefEndAction);
    owner.createPropertyTreeSelector( "onError",  cDefErrorAction);
    owner.createPropertyTreeSelector( "onReady",  cDefReadyAction);
  }

  void Project::removed() {
    // Nothing to do.
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
    if (cDefProjectPath != "") {
      prepare(cDefProjectPath, cDefProjectUser, false);
    }
  }

  void Project::updateRuntime(unsigned int milliseconds) {
    if (cRuntimeLoading) {
      try {
        IsoRealms::Project* mProject = cRuntimeProjectLoader->getLoadedProject();
        if (mProject != nullptr) {
          cRuntimeProject = mProject;
          cRuntimeProject->reset();
          cDefReadyAction.execute();
          cRuntimeLoading = false;
        }
      } catch (ParseException& parseException) {
        cDefErrorAction.execute();
        cRuntimeLoading = false;
        return;
      }

      if (cRuntimeResetPostponed) {
        cRuntimeResetPostponed = false;
        cRuntimeProject->reset(cRuntimeOptions);
      }
    }

    if (cRuntimeRunning && cRuntimeProject != nullptr) {
      cRuntimeProject->updateRuntime(milliseconds);
      cRuntimeProject->updateRuntimeComplete();
    }
    if (cRuntimeEditing && cRuntimeProject != nullptr) {
      /*
       * This call is needed when a new editor view is added causing
       * more view dependent Textures to be created in the C64 Terrain
       * Texture Set.  TODO: I think there is a more elegant way of
       * handling this situation.
       */
      cProject.getApplication().initMainThread();
      cRuntimeProject->updateEditing(milliseconds);
    }

    // Clean up any old projects that could have still been being constructed.
    if (!cRuntimeLoading) {
      for (int i = static_cast<int>(cRuntimeOldProjects.size()) - 1; i >= 0; i--) {
        if (cRuntimeOldProjects[i]->isDestructReady()) {
          cRuntimeOldProjects[i]->setDestructing();
          Application& mApplication = cProject.getApplication();
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

  void Project::prepareNewProject() {
    cRuntimeProjectLoader = std::make_unique<ProjectLoader>([this](bool quitRequestGranted) {
      cRuntimeRunning = false;
      cRuntimeEditing = true; // TODO: Why do we assume a switch to editing mode?
      cRuntimeQuitRequestGranted = quitRequestGranted;
      cRuntimeProject->reset(); // TODO: Why do we reset here?
      cDefEndAction.execute();
    });

    cRuntimeLoading = true;
    cRuntimeProject = nullptr;
    Application& mApplication = cProject.getApplication();
    mApplication.executeAndReturn([this, &mApplication]() {
      cRuntimeProjectLoader->newProject(mApplication);
    });
  }

  void Project::prepare(const std::string& file, bool user, bool force) {
    
    // If it's the same as the current project, nothing to do.
    if (!force && cRuntimeProjectLoader != nullptr && cRuntimeProjectLoader->matches(file, user)) {
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
        if (cRuntimeOldProjects[i]->matches(file, user)) {
          cRuntimeProjectLoader = std::move(cRuntimeOldProjects[i]);
          cRuntimeOldProjects.erase(cRuntimeOldProjects.begin() + i);
          cRuntimeLoading = true;
          cRuntimeProject = nullptr;
          return;
        }
      }
    }

    // No existing project loader match; create a new one.
    cRuntimeProjectLoader = std::make_unique<ProjectLoader>(file, user, [this](bool quitRequestGranted) {
      cRuntimeRunning = false;
      cRuntimeEditing = true; // TODO: Why do we assume a switch to editing mode?
      cRuntimeQuitRequestGranted = quitRequestGranted;
      cRuntimeProject->reset(); // TODO: Why do we reset here?
      cDefEndAction.execute();
    });
    Application& mApplication = cProject.getApplication();
    mApplication.executeAndReturn([this, &mApplication]() {
      cRuntimeProjectLoader->loadProject(mApplication);
    });
    cRuntimeLoading = true;
    cRuntimeProject = nullptr;
  }

  bool Project::isReady() {
    return cRuntimeProject != nullptr;
  }
  
  void Project::resetProject() {
    Options mNoOptions;
    cRuntimeProject->reset(mNoOptions);
  }

  void Project::resetLauncher(const ProjectLaunchConfiguration* launcher) {
    cRuntimeProject->reset(launcher);
  }
  
  void Project::resetOptions(Options& options) {
    if (cRuntimeLoading) {
      cRuntimeOptions.assign(options);
      cRuntimeResetPostponed = true;
    } else {
      cRuntimeProject->reset(options);
    }
  }

  IEditable* Project::getDefaultEditor() {
    return cRuntimeProject->getDefaultEditable();
  }

  IsoRealms::Project* Project::getProject() {
    return cRuntimeProject;
  }

  bool Project::isUser() {
    return cRuntimeProject != nullptr && cRuntimeProject->isUser();
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

  void Project::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool Project::isDefaultConfiguration() const {
    return true;
  }
}
