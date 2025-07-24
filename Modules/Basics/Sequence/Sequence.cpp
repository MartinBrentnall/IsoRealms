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
#include "Sequence.h"

#include "Modules/Basics/Basics.h"

#include "Editor/SequenceEditor.h"
#include "SequenceInstance.h"

namespace IsoRealms::Basics {
  const std::string Sequence::JSON_INSTANCES = "instances";
  const std::string Sequence::JSON_LOOP      = "loop";
  const std::string Sequence::JSON_NAME      = "name";
  const std::string Sequence::JSON_PLAYING   = "playing";
  const std::string Sequence::JSON_SPEED     = "speed";
  const std::string Sequence::JSON_TRACKS    = "tracks";
  const std::string Sequence::JSON_TRACK     = "track";
  const std::string Sequence::JSON_TYPE      = "type";

  Sequence::Sequence(IProject& project, Basics& basics, IResourceData& data) :
            cBasics(basics),
            cResourceData(data),
            cDefPlaying(false),
            cDefLoop(false),
            cDefSpeed(data, 1.0f),
            cExposedLength(*this),
            cLuaBinding(project, this) {
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
      cDefInstances.emplace(mInstanceObject.getString(JSON_NAME), std::make_unique<SequenceInstance>(*this, mInstanceObject));
    }
  }

  void Sequence::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IBinding>(&cLuaBinding, "", "Sequences");
    assets.add<IInteger>(&cExposedLength, "Length", "Sequences");
    assets.add<IEditable>(this, "", "Sequences");
    for (const std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      mEntry.second->registerAssets(assets, mEntry.first);
    }
  }
  
  void Sequence::save(JSONObject object) const {
    object.addBoolean(JSON_PLAYING, cDefPlaying);
    object.addBoolean(JSON_LOOP, cDefLoop);
    cDefSpeed.save(object, JSON_SPEED);
    JSONArray mTrackArray = object.addArray(JSON_TRACKS);
    for (const std::unique_ptr<SequenceTrack>& mTrack : cDefTracks) {
      JSONObject mTrackObject = mTrackArray.addObject();
      mTrack->save(mTrackObject, JSON_TRACK);
    }
    JSONArray mInstanceArray = object.addArray(JSON_INSTANCES);
    for (const std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
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

  void Sequence::getProperties(PropertyMaker& owner) {
    owner.createPropertyEditor("Content", this);

    owner.createPropertyNativeBoolean("Playing", [this]() {return cDefPlaying;}, [this](bool value) {cDefPlaying = value;});
    owner.createPropertyNativeBoolean("Loop",    [this]() {return cDefLoop;},    [this](bool value) {cDefLoop    = value;});
    owner.createPropertyAsset<Float>( "Speed",   cDefSpeed);
    for (std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      owner.createPropertyStruct("Instance", mEntry.first, [this, &owner, &mEntry]() {
        return mEntry.second->getProperties(owner);
      }, [this, &mEntry]() {
        cDefInstances.erase(mEntry.first);
      });
    }
    owner.createPropertyAdd("Instance", "Add...", [this, &owner]() {
      std::string mKey = Utils::getAvailableKey(cDefInstances, "Instance");
      std::unique_ptr<SequenceInstance>& mInstance = cDefInstances.emplace(mKey, std::make_unique<SequenceInstance>(*this)).first->second;
      // mInstance->registerAssets(assets, mKey);
      return owner.createPropertyStruct("Instance", "Edit...", [this, &owner, &mInstance]() {
        return mInstance->getProperties(owner);
      }, [this, mKey]() {
        cDefInstances.erase(mKey);
      });
    });
  }

  void Sequence::updateRuntime(unsigned int milliseconds) {
    process(milliseconds);
  }
  
  void Sequence::updateEditing(unsigned int milliseconds) {
    if (cDefLoop && cDefPlaying) {
      process(milliseconds);
    }
    for (const std::pair<IEditableScreen* const, std::unique_ptr<SequenceEditor>>& mEditor : cEditors) {
      mEditor.second->updateScreen(milliseconds);
    }
  }

  void Sequence::reset() {
    resetSequence();
  }
  
  bool Sequence::isPlaying() const {
    return cDefPlaying;
  }

  bool Sequence::isLooped() const {
    return cDefLoop;
  }

  Basics& Sequence::getBasics() const {
    return cBasics;
  }

  IResourceData& Sequence::getResourceData() {
    return cResourceData;
  }

  std::string Sequence::getInstanceName(SequenceInstance& instance) const {
    return Utils::reverseLookupUnique(cDefInstances, instance);
  }

  bool Sequence::setInstanceName(SequenceInstance& instance, const std::string& name) {
    std::string mOldName = getInstanceName(instance);
    if (mOldName == name) {
      return true;
    }
    if (cDefInstances.find(name) != cDefInstances.end()) {
      return false;
    }
    cDefInstances.emplace(name, std::move(cDefInstances[mOldName]));
    cDefInstances.erase(mOldName);
    refreshAssetRegistration();
    return true;
  }

  IEditableScreen* Sequence::createEditableScreen(IsoRealms::Project* project, IDialogManager& dialogManager) {
    std::unique_ptr<SequenceEditor> mScreen = std::make_unique<SequenceEditor>(*this, dialogManager);
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

  void Sequence::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool Sequence::isDefaultConfiguration() const {
    return true;
  }

  void Sequence::resetSequence() {
    cRuntimePositionFraction = 0.0f;
    for (std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      mEntry.second->reset();
    }
  }

  void Sequence::stopPreview() {
    for (std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      mEntry.second->stopPreview();
    }
  }

  void Sequence::setPreviewPosition(long position) {
    for (std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      mEntry.second->setPreviewPosition(position);
    }
  }
  
  void Sequence::preview(unsigned int milliseconds) {
    for (std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      mEntry.second->updatePreview(milliseconds);
    }
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
    SequenceTrack* mTrack = cDefTracks.emplace_back(std::make_unique<SequenceTrack>(cBasics, *this)).get();
    for (std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      ISequenceTrackInstance* mTrackInstance = (*mTrack)->createTrackInstance(*mEntry.second.get());
      mEntry.second->addTrackInstance(mTrackInstance);
    }
    refreshAssetRegistration();
  }

  void Sequence::refreshAssetRegistration() {
    cBasics.refreshAssetRegistration(*this);
  }

  Sequence::Length::Length(Sequence& parent) :
            cParent(parent) {
  }

  int Sequence::Length::getValue() const {
    return cParent.getDuration();
  }

  bool Sequence::Length::renderAssetIcon() const {
    return false;
  }

  void Sequence::Length::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void Sequence::Length::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool Sequence::Length::isDefaultConfiguration() const {
    return true;
  }

  void Sequence::process(unsigned int milliseconds) {
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

    for (const std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      mEntry.second->update(milliseconds);
    }
  }
}
