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
#include "Sequence.h"

#include "Modules/Basics/Basics.h"

namespace IsoRealms::Basics {
  const std::string Sequence::JSON_INSTANCES  = "instances";
  const std::string Sequence::JSON_LOOP       = "loop";
  const std::string Sequence::JSON_NAME       = "name";
  const std::string Sequence::JSON_PLAYING    = "playing";
  const std::string Sequence::JSON_SPEED      = "speed";
  const std::string Sequence::JSON_START_TIME = "startTime";
  const std::string Sequence::JSON_TRACKS     = "tracks";
  const std::string Sequence::JSON_TRACK      = "track";
  const std::string Sequence::JSON_TYPE       = "type";

  Sequence::Sequence(IProject& project, Basics& basics, IResourceData& data) :
            cBasics(basics),
            cDefPlaying(false),
            cDefLoop(false),
            cDefSpeed(project, 1.0f),
            cExposedLength(*this),
            cExposedPosition(*this),
            cLuaBinding(project, this) {
    project.updateRuntime([this](unsigned int milliseconds) {
      float mSpeedMultiplier = cDefSpeed->getValue();
      if (mSpeedMultiplier != 1.0f) {
        float mActualMilliseconds = milliseconds * mSpeedMultiplier;
        milliseconds = std::floor(mActualMilliseconds);
        float mFractional = mActualMilliseconds - milliseconds;
        cRuntimePositionFraction += mFractional;
        if (cRuntimePositionFraction >= 1.0f) {
          milliseconds++;
          cRuntimePositionFraction -= 1.0f;
        }
      }

      if (cRuntimePlaying) {
        for (const std::pair<const std::string, std::unique_ptr<Instance>>& mEntry : cDefInstances) {
          mEntry.second->play(milliseconds);
        }
      }
    });
    
    project.updateEditing([this](unsigned int milliseconds) {
      if (cRuntimePlaying && cDefLoop && cDefPlaying) {
        for (const std::pair<const std::string, std::unique_ptr<Instance>>& mEntry : cDefInstances) {
          mEntry.second->play(milliseconds);
        }
      }
      for (const std::pair<IEditableScreen* const, std::unique_ptr<SequenceEditor>>& mEditor : cEditors) {
        mEditor.second->updateScreen(milliseconds);
      }
    });

    project.reset([this]() {
      reset(); 
    });
  }
  
  Sequence::Sequence(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            Sequence(project, basics, data) {
    cDefPlaying = object.getBoolean(JSON_PLAYING);
    cDefLoop = object.getBoolean(JSON_LOOP);
    cDefSpeed.init(object, JSON_SPEED);
    for (JSONObject mTrackObject : object.getArray(JSON_TRACKS)) {
      cDefTracks.emplace_back(std::make_unique<SequenceTrack>(basics, *this));
      cDefTracks.back()->set(mTrackObject, JSON_TRACK);
    }
    for (JSONObject mInstanceObject : object.getArray(JSON_INSTANCES)) {
      cDefInstances.emplace(mInstanceObject.getString(JSON_NAME), std::make_unique<Instance>(*this, mInstanceObject));
    }
  }

  void Sequence::registerAssets(IAssetRegistry& assets) {
    assets.add(&cLuaBinding, "", "Sequences");
    assets.add(&cExposedLength, "Length", "Sequences");
    assets.add(&cExposedPosition, "Position", "Sequences");
    assets.add(this, "", "Sequences");
    for (const std::pair<const std::string, std::unique_ptr<Instance>>& mEntry : cDefInstances) {
      LocalAssetRegistry mInstanceAssetRegistry(assets, mEntry.first);
      mEntry.second->registerAssets(mInstanceAssetRegistry);
    }
  }
  
  void Sequence::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    assets.remove(&cLuaBinding, relinquish);
    assets.remove(&cExposedLength, relinquish);
    assets.remove(&cExposedPosition, relinquish);
    assets.remove(this, relinquish);
    for (const std::pair<const std::string, std::unique_ptr<Instance>>& mEntry : cDefInstances) {
      mEntry.second->unregisterAssets(assets, relinquish);
    }
  }
  
  void Sequence::save(JSONObject object, IAssetIdentifier& identifier) const {
    object.addBoolean(JSON_PLAYING, cDefPlaying);
    object.addBoolean(JSON_LOOP, cDefLoop);
    cDefSpeed.save(object, JSON_SPEED);
    JSONArray mTrackArray = object.addArray(JSON_TRACKS);
    for (const std::unique_ptr<SequenceTrack>& mTrack : cDefTracks) {
      JSONObject mTrackObject = mTrackArray.addObject();
      mTrack->save(mTrackObject, JSON_TRACK);
    }
    JSONArray mInstanceArray = object.addArray(JSON_INSTANCES);
    for (const std::pair<const std::string, std::unique_ptr<Instance>>& mEntry : cDefInstances) {
      JSONObject mInstanceObject = mInstanceArray.addObject();
      mInstanceObject.addString(JSON_NAME, mEntry.first);
      mEntry.second->save(mInstanceObject);
    }
  }

  void Sequence::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool Sequence::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> Sequence::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyEditor>("Content", "TODO", this));

    for (std::pair<const std::string, std::unique_ptr<Instance>>& mEntry : cDefInstances) {
      mProperties.emplace_back(std::make_unique<PropertyStruct>(mEntry.first, "TODO", "Edit...", [this, &browser, &assets, &mEntry]() {
        return mEntry.second->getProperties(browser, assets);
      }));
    }

    mProperties.emplace_back(std::make_unique<PropertyNativeBoolean>("Playing", "Specifies the initial state of this sequence", [this]() {return cDefPlaying;}, [this](bool value) {cDefPlaying = value;}, cBasics.getProject()));
    mProperties.emplace_back(std::make_unique<PropertyNativeBoolean>("Loop", "Specifies whether this Sequence repeats from the beginning after reaching the end", [this]() {return cDefLoop;}, [this](bool value) {cDefLoop = value;}, cBasics.getProject()));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Float>>("Speed", "The sequence plays at multiple speed of the specified value", cDefSpeed));
    mProperties.emplace_back(std::make_unique<PropertyAdd>(   "Instance", "TODO", "Add...", [this, &browser, &assets]() {
      std::string mKey = Utils::getAvailableKey(cDefInstances, "Instance");
      std::unique_ptr<Instance>& mInstance = cDefInstances.emplace(mKey, std::make_unique<Instance>(*this)).first->second;
      return std::make_unique<PropertyStruct>("Instance", "TODO", "Edit...", [this, &browser, &assets, &mInstance]() {
        return mInstance->getProperties(browser, assets);
      });
    }));
    return mProperties;
  }

  std::string Sequence::getInstanceName(Instance& instance) const {
    return Utils::reverseLookupUnique(cDefInstances, instance);
  }

  bool Sequence::setInstanceName(Instance& instance, const std::string& name) {
    std::string mOldName = getInstanceName(instance);
    if (mOldName == name) {
      return true;
    }
    if (cDefInstances.find(name) != cDefInstances.end()) {
      return false;
    }
    cDefInstances.emplace(name, std::move(cDefInstances[mOldName]));
    cDefInstances.erase(mOldName);
    return true;
  }

  IEditableScreen* Sequence::createEditableScreen(IsoRealms::Project* project) {
    std::unique_ptr<SequenceEditor> mScreen = std::make_unique<SequenceEditor>(*this);
    IEditableScreen* mReturnValue = mScreen.get();
    cEditors[mReturnValue] = std::move(mScreen);
    return mReturnValue;
  }

  bool Sequence::renderAssetIcon() const {
    return renderIcon();
  }

  void Sequence::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Sequence::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Sequence::isDefaultConfiguration() const {
    return true;
  }

  void Sequence::play() {
    cRuntimePlaying = true;
  }

  void Sequence::pause() {
    cRuntimePlaying = false;
  }

  void Sequence::reset() {
    cRuntimePlaying = cDefPlaying;
    cRuntimePosition = 0;
    cRuntimePositionFraction = 0.0f;
    for (std::pair<const std::string, std::unique_ptr<Instance>>& mEntry : cDefInstances) {
      mEntry.second->reset();
    }
  }

  void Sequence::stopPreview() {
    for (std::pair<const std::string, std::unique_ptr<Instance>>& mEntry : cDefInstances) {
      mEntry.second->stopPreview();
    }
  }

  void Sequence::setPreviewPosition(long position) {
    cRuntimePosition = position;
    for (std::pair<const std::string, std::unique_ptr<Instance>>& mEntry : cDefInstances) {
      mEntry.second->setPreviewPosition(position);
    }
  }
  
  void Sequence::preview(unsigned int milliseconds) {
    for (std::pair<const std::string, std::unique_ptr<Instance>>& mEntry : cDefInstances) {
      mEntry.second->play(milliseconds);
    }
  }

  int Sequence::getTime() const {
    return cRuntimePosition;
  }

  void Sequence::setTime(int time) {
    setPreviewPosition(std::max(0, std::min(static_cast<int>(getDuration()), time)));
  }

  IProject& Sequence::getProject() const {
    return cBasics.getProject();
  }

  unsigned int Sequence::getTrackCount() const {
    return static_cast<unsigned int>(cDefTracks.size());
  }

  SequenceTrack& Sequence::getTrack(unsigned int track) const {
    return *cDefTracks[track].get();
  }

  unsigned int Sequence::getDuration() const {
    unsigned int mDuration = 0;
    for (const std::unique_ptr<SequenceTrack>& mTrack : cDefTracks) {
      mDuration = std::max(mDuration, (*mTrack)->getDuration());
    }
    return mDuration;
  }

  void Sequence::addTrack() {
    cDefTracks.emplace_back(std::make_unique<SequenceTrack>(cBasics, *this));
  }

  Sequence::Instance::Instance(Sequence& parent) :
            cParent(parent) {
  }

  Sequence::Instance::Instance(Sequence& parent, JSONObject object) :
            cParent(parent),
            cDefStartTime(object.getInteger(JSON_START_TIME)),
            cDefSpeed(object.getFloat(JSON_SPEED, 1.0f)) {
    for (std::unique_ptr<SequenceTrack>& mTrack : cParent.cDefTracks) {
      ISequenceTrackInstance* mTrackInstance = (*mTrack)->createTrackInstance();
      if (mTrackInstance != nullptr) {
        cTrackInstances.emplace_back(mTrackInstance);
      }
    }
  }

  void Sequence::Instance::registerAssets(IAssetRegistry& assets) {
    for (ISequenceTrackInstance* mTrack : cTrackInstances) {
      mTrack->registerAssets(assets);
    }
  }

  void Sequence::Instance::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    for (ISequenceTrackInstance* mTrack : cTrackInstances) {
      mTrack->unregisterAssets(assets, relinquish);
    }
  }

  void Sequence::Instance::reset() {
    for (ISequenceTrackInstance* mTrack : cTrackInstances) {
      mTrack->reset();
    }
    cRuntimePosition = 0;
    cRuntimePositionFraction = 0.0f;
    play(cDefStartTime);
  }

  void Sequence::Instance::stopPreview() {
    for (ISequenceTrackInstance* mTrack : cTrackInstances) {
      mTrack->stopPreview();
    }
  }

  void Sequence::Instance::setPreviewPosition(long position) {
    for (ISequenceTrackInstance* mTrack : cTrackInstances) {
      mTrack->setPreviewPosition(position);
    }
  }

  void Sequence::Instance::play(unsigned int milliseconds) {
    if (cDefSpeed != 1.0f) {
      float mActualMilliseconds = milliseconds * cDefSpeed;
      milliseconds = std::floor(mActualMilliseconds);
      float mFractional = mActualMilliseconds - milliseconds;
      cRuntimePositionFraction += mFractional;
      if (cRuntimePositionFraction >= 1.0f) {
        milliseconds++;
        cRuntimePositionFraction -= 1.0f;
      }
    }

    cRuntimePosition += milliseconds;

    bool mSequenceFinished = true;
    for (ISequenceTrackInstance* mTrack : cTrackInstances) {
      if (mTrack->play(milliseconds)) {
        mSequenceFinished = false;
      }
    }

    if (mSequenceFinished && cParent.cDefLoop) {
      for (ISequenceTrackInstance* mTrack : cTrackInstances) {
        cRuntimePosition = 0;
        mTrack->reset();
      }
    }
  }

  void Sequence::Instance::save(JSONObject object) const {
    object.addInteger(JSON_START_TIME, cDefStartTime);
    object.addFloat(JSON_SPEED, cDefSpeed, 1.0f);
  }

  std::vector<std::unique_ptr<IProperty>> Sequence::Instance::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeString>( "Instance Name", "TODO", [this]() {return cParent.getInstanceName(*this);}, [this](const std::string& value) {return cParent.setInstanceName(*this, value);}));
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>("Start Time",    "TODO", [this]() {return cDefStartTime;}, [this](int value) {cDefStartTime = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>(  "Speed",         "TODO", [this]() {return cDefSpeed;},     [this](int value) {cDefSpeed     = value; return true;}));
    return mProperties;
  }

  Sequence::Length::Length(Sequence& parent) :
            cParent(parent) {
  }

  std::string Sequence::Length::getValue() const {
    int mLength = cParent.getDuration();
    int mMilliseconds = mLength % 1000;
    int mSeconds = mLength / 1000;
    int mMinutes = mSeconds / 60;
    int mHours   = mMinutes / 60;
    mMinutes = mMinutes % 60;
    mSeconds = mSeconds % 60;
    std::stringstream mStringStream;
    mStringStream << mHours << ":" << std::setfill('0') << std::setw(2) << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setfill('0') << std::setw(2) << (mMilliseconds / 10);
    return mStringStream.str();
  }

  bool Sequence::Length::renderAssetIcon() const {
    return false;
  }

  void Sequence::Length::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Sequence::Length::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Sequence::Length::isDefaultConfiguration() const {
    return true;
  }

  Sequence::Position::Position(Sequence& parent) :
            cParent(parent) {
  }

  std::string Sequence::Position::getValue() const {
    int mMilliseconds = cParent.cRuntimePosition % 1000;
    int mSeconds = cParent.cRuntimePosition / 1000;
    int mMinutes = mSeconds / 60;
    int mHours   = mMinutes / 60;
    mMinutes = mMinutes % 60;
    mSeconds = mSeconds % 60;
    std::stringstream mStringStream;
    mStringStream << mHours << ":" << std::setfill('0') << std::setw(2) << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setfill('0') << std::setw(2) << (mMilliseconds / 10);
    return mStringStream.str();
  }

  bool Sequence::Position::renderAssetIcon() const {
    return false;
  }

  void Sequence::Position::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Sequence::Position::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Sequence::Position::isDefaultConfiguration() const {
    return true;
  }
}
