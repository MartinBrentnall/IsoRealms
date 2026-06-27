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
#include "ProjectLaunchConfiguration.h"

#include "IsoRealms/Utils.h"
#include "Project.h"

namespace IsoRealms {
  ProjectLaunchConfiguration::ProjectLaunchConfiguration(Project& parent, ProjectFile& owner, const std::string& name) :
            cDefName(name.empty() ? parent.makeLaunchConfigurationName() : name),
            cDefOwner(parent, &owner) {
  }

  std::string ProjectLaunchConfiguration::getName() const {
    return cDefName;
  }

  ProjectFile* ProjectLaunchConfiguration::getOwnerProjectFile() const {
    return cDefOwner.getProjectFile();
  }

  bool ProjectLaunchConfiguration::needsSaving(const ProjectFile* savingProject) const {
    return savingProject != nullptr && savingProject == cDefOwner.getProjectFile();
  }

  void ProjectLaunchConfiguration::define(IComponentDefiner& definer, Project& project) {
    definer.propertyString("launchConfigurationName", [this]() {return cDefName;}, [this](const std::string& value) {cDefName = value;}, cDefName, [this, &project](const std::string& value) {return !project.isLaunchConfigurationNameUsed(value, this);});
    definer.keyedArray("options", "launchConfigurationOptionAdd", cDefOptions, [](const std::unique_ptr<Option>& option) -> Option& {return *option;}, [this, &definer](Option& option) {
      definer.scope("launchConfigurationOption", option.getName(), [&option, this, &definer]() {
        option.getProperties(definer, *this, IComponentDefiner::HINT_INLINE);
      }, [this, &option]() {
        Utils::removeElementUnique(cDefOptions, &option);
      });
    }, [this, &project](const std::string& key) -> Option& {
      return *cDefOptions.emplace_back(std::make_unique<Option>(project, *this, key));
    });
  }

  bool ProjectLaunchConfiguration::isOptionNameUsed(const std::string& name, Option* option) const {
    for (const std::unique_ptr<Option>& mOption : cDefOptions) {
      if (option != mOption.get() && mOption->getName() == name) {
        return true;
      }
    }
    return false;
  }

  std::string ProjectLaunchConfiguration::makeOptionName() const {
    std::string mDesiredName = "Option";
    std::string mProposedName = mDesiredName;
    int mCount = 1;
    while (isOptionNameUsed(mProposedName, nullptr)) {
      mProposedName = mDesiredName + " " + Utils::toString(mCount++);
    }
    return mProposedName;
  }

  bool ProjectLaunchConfiguration::isOwnedBy(const ProjectFile& project) const {
    return needsSaving(&project);
  }

  void ProjectLaunchConfiguration::getOptions(Options& options) const {
    options.clear();
    for (const std::unique_ptr<Option>& mOption : cDefOptions) {
      options.addOption(mOption->getName(), mOption->getValue());
    }
  }

  ProjectLaunchConfiguration::Option::Option(Project& parent, ProjectLaunchConfiguration& launch, const std::string& name) :
            cDefName(name.empty() ? launch.makeOptionName() : name),
            cDefValue(parent) {
  }

  std::string ProjectLaunchConfiguration::Option::getName() const {
    return cDefName;
  }

  std::string ProjectLaunchConfiguration::Option::getValue() const {
    return cDefValue->getValue();
  }

  void ProjectLaunchConfiguration::Option::getProperties(IComponentDefiner& definer, ProjectLaunchConfiguration& launch, const Options& hint) {
    definer.propertyString("launchConfigurationOptionName", [this]() {return cDefName;}, [this](const std::string& value) {cDefName = value;}, cDefName, [this, &launch](const std::string& value) {return !launch.isOptionNameUsed(value, this);});
    definer.propertyResource("launchConfigurationOptionValue", cDefValue, hint);
  }
}
