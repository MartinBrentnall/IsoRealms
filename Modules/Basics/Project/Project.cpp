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

namespace IsoRealms::Basics {
  const std::string Project::TAG_END     = "End";
  const std::string Project::TAG_OPTIONS = "Options";
  const std::string Project::TAG_READY   = "Ready";

  const std::string Project::ATTRIBUTE_EDITING = "editing";
  const std::string Project::ATTRIBUTE_RUNNING = "running";

  Project::Project(IProject* project, Basics* basics) :
            cProject(project),
            cDefReadyAction(project),
            cDefEndAction(project),
            cDefProjectOptions(project),
            cDefRunning(false),
            cDefEditing(false),
            cRuntimeProject(nullptr),
            cLuaBinding(project, this) {
    project->reset([this]() {

      // Wait to prevent a crash if a project is still under construction..
//       while (cRuntimeLoading) {
//         IsoRealms::Project* mProject = cRuntimeProjectLoader->getLoadedProject();
//         if (mProject != nullptr) {
//           cRuntimeLoading = false;
//         }
//       }
//       cRuntimeOldProjects.clear();
//       cRuntimeLoading            = false;
      cRuntimeRunning            = cDefRunning;
      cRuntimeEditing            = cDefEditing;
      cRuntimeQuitRequestGranted = false;
//       cRuntimeProjectLoader      = nullptr;
//       cRuntimeProject            = nullptr;
    });

    project->updateRuntime([this](unsigned int milliseconds) {
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
            cRuntimeOldProjects.erase(cRuntimeOldProjects.begin() + i);
          }
        }
      }
    });
  }
  
  Project::Project(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            Project(project, basics) {
    cDefRunning = node.getBooleanAttribute(ATTRIBUTE_RUNNING);
    cDefEditing = node.getBooleanAttribute(ATTRIBUTE_EDITING);
    cDefEndAction.init(node, TAG_END);
    cDefReadyAction.init(node, TAG_READY);
    cDefProjectOptions.init(node.getNode(TAG_OPTIONS));

    Options mProjectOptions = options->getFixedOptions();
    project->init([this, &node, mProjectOptions](IAssets* resources) {
      std::string mFile = mProjectOptions.getOption("file");
      if (mFile != "") {
        prepareInternal(&mProjectOptions, false);
      } else {
        prepare(*cDefProjectOptions, false);
      }
    });
  }

  void Project::registerAssets(IAssetRegistry* assets) {
    assets->add(static_cast<IScreen*>(this), "", "Projects");
    assets->add(static_cast<IInputHandler*>(this), "", "Projects");
    assets->add(&cLuaBinding, "", "Projects");
  }
  
  void Project::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(static_cast<IInputHandler*>(this));
    assets->remove(static_cast<IScreen*>(this));
    assets->remove(&cLuaBinding);
  }
  
  void Project::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_RUNNING, cDefRunning);
    node->addAttribute(ATTRIBUTE_EDITING, cDefEditing);
    cDefEndAction.save(node, TAG_END);
    cDefReadyAction.save(node, TAG_READY);
    cDefProjectOptions.save(node, TAG_OPTIONS);
  }

  void Project::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool Project::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> Project::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
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
  
  void Project::reset() {
    cRuntimeProject->reset();
  }
  
  IEditable* Project::getDefaultEditor() const {
    return cRuntimeProject->getDefaultEditable();
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

  void Project::renderScreen(float scale, float aspectRatio) const {
    if (cRuntimeProject != nullptr) {
      cRuntimeProject->preRender();
      if (cRuntimeRunning) {
        glEnable(GL_DEPTH_TEST);
        cRuntimeProject->render(aspectRatio);
      }
    }
  }

  bool Project::renderAssetIcon() const {
    return false;
  }
  
  void Project::prepareInternal(const Options* options, bool force) {
    std::string mFile = options->getOption("file");

    // If it's the same as the current project, nothing to do.
    if (!force && cRuntimeProjectLoader != nullptr && cRuntimeProjectLoader->matches(*options)) {
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
    IApplication* mApplication = cProject->getApplication();
    mApplication->executeAndReturn([this, mApplication]() {
      cRuntimeProjectLoader->loadProject(mApplication);
    });
    cRuntimeLoading = true;
  }
}
