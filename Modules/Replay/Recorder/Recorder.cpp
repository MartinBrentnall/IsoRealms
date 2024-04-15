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
#include "Recorder.h"

namespace IsoRealms::Replay {
  const std::string Recorder::TAG_INPUT       = "Input";
  const std::string Recorder::TAG_QUIT_ACTION = "QuitAction";

  Recorder::Recorder(IProject* project, Replay* replay) :
            cParentProject(project),
            cFilenameString(""),
            cQuitAction(project),
            cLuaBinding(project, this) {
    cElapsedTime = 0;
  }

  Recorder::Recorder(IProject* project, Replay* replay, DOMNode& node, IOptions* options, IResourceData* data) :
            Recorder(project, replay) {
    if (options == nullptr) {
      throw ArgumentException("ERROR: Recorder::Recorder: Options are required for this resource.");
    }
    
    LocalOptions mLocalOptions("Project", options);
    cProject = std::make_unique<Project>(cParentProject->getApplication(), &mLocalOptions, [this](bool forceQuit) {
      cOutput.close();
      cQuitAction.execute();
      if (forceQuit) {
        cParentProject->finish(true);
      }
    }); // TODO: 'user' flag shouldn't always be false
    
    cInputConfiguration = options->getOption("InputConfiguration");

    // Read the inputs to listen to and add digital listeners to those inputs.
    DOMNode mInputConfigurationNode(cInputConfiguration, DOMNode::Type::PROGRAM);
    DOMNode mRecorderConfigurationNode = mInputConfigurationNode.getNode("RecorderConfiguration");
    unsigned int mInputID = 0;
    for (DOMNode& mNode : mRecorderConfigurationNode) {
      std::string mInputType = mNode.getName();
      if (mInputType == "Digital") {
        cDefDigitalInputs.emplace_back(std::make_unique<Boolean>(cProject.get(), false, [this, mInputID](bool value) {writeInput(mInputID, value);})).get()->init(mNode, TAG_INPUT);
      } else if (mInputType == "Analogue") {
        cDefAnalogueInputs.emplace_back(std::make_unique<Float>(cProject.get(), 0.0f, [this, mInputID](float value) {writeInput(mInputID, value);})).get()->init(mNode, TAG_INPUT);
      } else {
        // TODO: Throw.
      }
      mInputID++;
    }

    project->updateRuntime([this](unsigned int milliseconds) {
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
    });
    
    project->reset([this]() {
      cProject->reset();
      cElapsedTime = 0;

      if (cOutput.is_open()) {
        cOutput.close();
      }
    });
    
    // Do main thread init stuff
    project->mainThreadInit([this]() {
      cProject->initMainThread();
    });
    cQuitAction.init(node, TAG_QUIT_ACTION);
  }

  bool Recorder::renderIcon() const {
    return false;
  }

  bool Recorder::renderAssetIcon() const {
    return false;
  }

  void Recorder::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  void Recorder::registerAssets(IAssetRegistry* assets) {
    assets->add(&cLuaBinding, "", "Gameplay Recorders");
    assets->add(static_cast<IScreen*>(this), "", "Gameplay Recorders");
    assets->add(static_cast<IInputHandler*>(this), "", "Gameplay Recorders");
    assets->add(&cFilenameString, "Filename", "Gameplay Recorders");
    assets->add(cProject.get(), "Project", "Gameplay Recorders");
  }
  
  void Recorder::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(&cLuaBinding);
    assets->remove(static_cast<IScreen*>(this));
    assets->remove(static_cast<IInputHandler*>(this));
    assets->remove(&cFilenameString);
    assets->remove(cProject.get());
  }
  
  std::vector<IProperty*> Recorder::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
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

  void Recorder::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    // Not supported
  }
}
