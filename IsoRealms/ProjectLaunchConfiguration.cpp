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

#include "Project.h"

namespace IsoRealms {
  ProjectLaunchConfiguration::ProjectLaunchConfiguration(Project& parent, ProjectFile& owner) :
            cDefName(parent.makeLaunchConfigurationName()),
            cDefOwner(parent, &owner),
            cDefOptionPreparationAction(parent) {
  }

  ProjectLaunchConfiguration::ProjectLaunchConfiguration(Project& parent, ProjectFile& owner, JSONThing thing) :
            cDefName(thing.getName()),
            cDefOwner(parent, &owner),
            cDefOptionPreparationAction(parent) {
    JSONObject mLaunchConfigurationObject = thing.getValue();
    cDefOptionPreparationAction.init(mLaunchConfigurationObject, JSON_PREPARATION_ACTION);
    for (JSONThing mOptionThing : mLaunchConfigurationObject.getObject(JSON_OPTIONS)) {
      cDefOptions.emplace_back(std::make_unique<Option>(parent, mOptionThing));
    }
  }

  std::string ProjectLaunchConfiguration::getName() const {
    return cDefName;
  }

  void ProjectLaunchConfiguration::getProperties(PropertyMaker& owner, const Metadata& metadata, Project& project) {
    owner.createPropertyNativeString(metadata.getPropertyData("LaunchConfigurationName"), [this]() {return cDefName;}, [this](const std::string& value) {cDefName = value;}, [this, &project](const std::string& value) {return !project.isLaunchConfigurationNameUsed(value, this);});
    owner.createPropertyAsset(metadata.getPropertyData("LaunchConfigurationOwner"), cDefOwner);
    owner.createPropertyAsset(metadata.getPropertyData("LaunchConfigurationPreparationAction"), cDefOptionPreparationAction);
    owner.createPropertyArray(metadata.getPropertyData("LaunchConfigurationOptionAdd"), cDefOptions, [](const std::unique_ptr<Option>& i)->Option& {return *i;}, [this, &project, &owner, &metadata](Option& option) {
      owner.createPropertyStruct(metadata.getPropertyData("LaunchConfigurationOption"), option.getName(), [this, &metadata, &option](PropertyMaker& owner) {
        option.getProperties(owner, metadata, *this);
      }, [this, &option]() {
        Utils::removeElementUnique(cDefOptions, &option);
      });
    }, [this, &project]()->Option& {
      return *cDefOptions.emplace_back(std::make_unique<Option>(project, *this));
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

  void ProjectLaunchConfiguration::save(JSONObject object, ProjectFile& savingProject) const {
    if (&savingProject == cDefOwner.getProjectFile()) {
      JSONObject mLaunchConfigurationObject = object.addObject(cDefName);
      cDefOptionPreparationAction.save(mLaunchConfigurationObject, JSON_PREPARATION_ACTION);
      JSONObject mOptionsObject = mLaunchConfigurationObject.addObject(JSON_OPTIONS);
      for (const std::unique_ptr<Option>& mOption : cDefOptions) {
        mOption->save(mOptionsObject);
      }
    }
  }

  bool ProjectLaunchConfiguration::isOwnedBy(ProjectFile& project) {
    return &project == cDefOwner.getProjectFile();
  }

  ProjectLaunchConfiguration::Option::Option(Project& parent, ProjectLaunchConfiguration& launch) :
            cDefName(launch.makeOptionName()),
            cDefValue(parent) {
  }

  ProjectLaunchConfiguration::Option::Option(Project& parent, JSONThing thing) :
            cDefName(thing.getName()),
            cDefValue(parent) {
    cDefValue.init(thing);
  }

  std::string ProjectLaunchConfiguration::Option::getName() const {
    return cDefName;
  }

  void ProjectLaunchConfiguration::Option::getProperties(PropertyMaker& owner, const Metadata& metadata, ProjectLaunchConfiguration& launch) {
    owner.createPropertyNativeString(metadata.getPropertyData("LaunchConfigurationOptionName"), [this]() {return cDefName;}, [this](const std::string& value) {cDefName = value;}, [this, &launch](const std::string& value) {return !launch.isOptionNameUsed(value, this);});
    owner.createPropertyAsset(metadata.getPropertyData("LaunchConfigurationOptionValue"), cDefValue);
  }

  void ProjectLaunchConfiguration::Option::save(JSONObject object) const {
    cDefValue.save(object, cDefName);
  }

  const std::string ProjectLaunchConfiguration::JSON_PREPARATION_ACTION = "preparationAction";
  const std::string ProjectLaunchConfiguration::JSON_OPTIONS            = "options";
}
