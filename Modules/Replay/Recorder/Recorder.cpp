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
#include "Recorder.h"

namespace IsoRealms::Replay {
  const std::string Recorder::JSON_INPUT               = "input";
  const std::string Recorder::JSON_INPUT_CONFIGURATION = "inputConfiguration";
  const std::string Recorder::JSON_INPUTS              = "inputs";
  const std::string Recorder::JSON_KEY                 = "key";
  const std::string Recorder::JSON_ON_FINISH           = "onFinish";
  const std::string Recorder::JSON_TYPE                = "type";

  const std::string Recorder::TYPE_ANALOGUE = "Analogue";
  const std::string Recorder::TYPE_DIGITAL  = "Digital";

  Recorder::Recorder(IProject& project, Replay& replay, IResourceData& data) :
            cParentProject(project),
            cFilenameString(data.getProject(), ""),
            cQuitAction(data.getDummyActionClient()),
            cLuaBinding(project, this) {
    cElapsedTime = 0;
  }

  Recorder::Recorder(IProject& project, Replay& replay, IResourceData& data, JSONObject object, IOptions& options) :
            Recorder(project, replay, data) {
    LocalOptions mLocalOptions("Project", options);
    cProject = std::make_unique<Project>(cParentProject.getApplication(), mLocalOptions, [this](bool forceQuit) {
      cOutput.close();
      cQuitAction.execute();
      if (forceQuit) {
        cParentProject.finish(true);
      }
    }); // TODO: 'user' flag shouldn't always be false

    cInputConfiguration = options.getOption("InputConfiguration");

    // Read the inputs to listen to and add digital listeners to those inputs.
    JSONDocument mInputConfigurationDocument(cInputConfiguration, false);
    JSONObject mInputConfigurationObject = mInputConfigurationDocument.getObject(JSON_INPUT_CONFIGURATION);
    unsigned int mInputID = 0;
    for (JSONObject mInputObject : mInputConfigurationObject.getArray(JSON_INPUTS)) {
      std::string mInputType = mInputObject.getString(JSON_TYPE);
      if (mInputType == TYPE_DIGITAL) {
        cDefDigitalInputs.emplace_back(std::make_unique<Boolean>(*cProject.get(), false, [this, mInputID](bool value) {writeInput(mInputID, value);})).get()->set(mInputObject, JSON_INPUT);
      } else if (mInputType == TYPE_ANALOGUE) {
        cDefAnalogueInputs.emplace_back(std::make_unique<Float>(*cProject.get(), 0.0f, [this, mInputID](float value) {writeInput(mInputID, value);})).get()->set(mInputObject, JSON_INPUT);
      } else {
        // TODO: Throw.
      }
      mInputID++;
    }

    // Do main thread init stuff
    project.mainThreadInit([this]() {
      cProject->initMainThread();
    });
    cQuitAction.init(object, JSON_ON_FINISH);
  }

  bool Recorder::renderIcon() const {
    return false;
  }

  bool Recorder::renderAssetIcon() const {
    return false;
  }

  void Recorder::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void Recorder::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool Recorder::isDefaultConfiguration() const {
    return true;
  }

  void Recorder::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  void Recorder::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IBinding>(&cLuaBinding, "", "Gameplay Recorders");
    assets.add<IScreen>(this, "", "Gameplay Recorders");
    assets.add<IInputHandler>(this, "", "Gameplay Recorders");
    assets.add<IString>(&cFilenameString, "Filename", "Gameplay Recorders");
    assets.add<IAssets>(cProject.get(), "Project", "Gameplay Recorders");
  }
  
  void Recorder::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    // Nothing to do.
  }

  void Recorder::updateRuntime(unsigned int milliseconds) {
    if (cElapsedTime == 0) {

      // Construct date and time string for filename
      time_t mCurrentTime = time(0);
      struct tm* mNow = localtime(&mCurrentTime);
      std::stringstream mDateTimeReader;
      mDateTimeReader << std::setfill('0') << std::setw(2) << (mNow->tm_year - 100)
                                          << std::setw(2) << (mNow->tm_mon + 1)
                                          << std::setw(2) <<  mNow->tm_mday << "-"
                                          << std::setw(2) <<  mNow->tm_hour
                                          << std::setw(2) <<  mNow->tm_min
                                          << std::setw(2) <<  mNow->tm_sec;
      cFilenameString.setValue("Recordings/" + mDateTimeReader.str());
      cOutput = System::openOutputStream(cFilenameString.getValue());

      // Write the Project name first
      std::string mProjectName = cProject->getFilename();
      size_t mLength = mProjectName.length();
      cOutput.write(reinterpret_cast<const char*>(&mLength),         sizeof(mLength));
      cOutput.write(reinterpret_cast<const char*>(&mProjectName[0]), mLength);

      // Write the configuration file used next.
      mLength = cInputConfiguration.length();
      cOutput.write(reinterpret_cast<const char*>(&mLength),                sizeof(mLength));
      cOutput.write(reinterpret_cast<const char*>(&cInputConfiguration[0]), mLength);
    }
    cElapsedTime += milliseconds;
    cProject->updateRuntime(milliseconds);
    cProject->updateRuntimeComplete();
  }

  void Recorder::reset() {
    cProject->reset();
    cElapsedTime = 0;

    if (cOutput.is_open()) {
      cOutput.close();
    }
  }

  void Recorder::writeInput(unsigned int id, bool state) {
    cOutput.write(reinterpret_cast<const char*>(&id),           sizeof(id));
    cOutput.write(reinterpret_cast<const char*>(&state),        sizeof(state));
    cOutput.write(reinterpret_cast<const char*>(&cElapsedTime), sizeof(cElapsedTime));
    cOutput.flush();
  }
  
  void Recorder::writeInput(unsigned int id, float state) {
    cOutput.write(reinterpret_cast<const char*>(&id),           sizeof(id));
    cOutput.write(reinterpret_cast<const char*>(&state),        sizeof(state));
    cOutput.write(reinterpret_cast<const char*>(&cElapsedTime), sizeof(cElapsedTime));
    cOutput.flush();
  }

  void Recorder::renderScreen(float scale, float aspectRatio) const {
    cProject->render(aspectRatio);
  }

  bool Recorder::input(sf::Event& event) {
    return cProject->input(event);
  }

  void Recorder::resetInput() {
    // Nothing to do.
  }

  void Recorder::save(JSONObject object) const {
    cQuitAction.save(object, JSON_ON_FINISH);
  }
}
