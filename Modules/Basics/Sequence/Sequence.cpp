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

#include <algorithm>

#include "Modules/Basics/Basics.h"

#include "Editor/SequenceEditor.h"
#include "IsoRealms/PropertyLoader.h"
#include "SequenceInstance.h"

namespace IsoRealms::Basics {
  Sequence::Sequence(Basics& basics, IResourceData& data) :
            cBasics(basics),
            cResourceData(data),
            cDefPlaying(false),
            cDefLoop(false),
            cDefSpeed(data, 1.0f),
            cExposedLength(*this),
            cLuaBinding(data.getProject().getLuaState(), this) {
  }
  
  void Sequence::load(IResourceData& resourceData, JSONObject object) {
    // Nothing to do.
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

  void Sequence::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyEditor(       "Content",    this);
    owner.createPropertyNativeBoolean(JSON_PLAYING, [this]() {return cDefPlaying;}, [this](bool value) {cDefPlaying = value;});
    owner.createPropertyNativeBoolean(JSON_LOOP,    [this]() {return cDefLoop;},    [this](bool value) {cDefLoop    = value;});
    owner.createPropertyTreeSelector(JSON_SPEED,    cDefSpeed);
    owner.createPropertyArray(JSON_INSTANCES, cDefInstances, [](const std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry) -> SequenceInstance& {return *mEntry.second;}, [this, &owner, &metadata](SequenceInstance& instance) {
      owner.createPropertyStruct("Instance", getInstanceName(instance), [this, &instance, &metadata](IPropertyMaker& owner) {
        instance.getProperties(owner, metadata);
      }, [this, &instance]() {
        cDefInstances.erase(getInstanceName(instance));
      });
    }, [this]() -> SequenceInstance& {
      std::string mKey = Utils::getAvailableKey(cDefInstances, "Instance");
      return *cDefInstances.emplace(mKey, std::make_unique<SequenceInstance>(*this)).first->second;
    });

    // Tracks.
    owner.createPropertyArray(JSON_TRACKS, cDefTracks, [](const std::unique_ptr<SequenceTrack>& mTrack) -> SequenceTrack& {return *mTrack;}, [this, &owner, &metadata](SequenceTrack& track) {
      Options mHint;
      mHint.addOption(Options::PROPERTY_IMMEDIATE, "true");
      owner.createPropertyTreeSelector(JSON_TRACK, track, mHint);
      track.stateChanged();
    }, [this]() -> SequenceTrack& {
      SequenceTrack* mTrack = cDefTracks.emplace_back(std::make_unique<SequenceTrack>(*this)).get();
      for (std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
        ISequenceTrackInstance* mTrackInstance = (*mTrack)->createTrackInstance(*mEntry.second.get());
        mEntry.second->addTrackInstance(mTrackInstance);
      }
      return *mTrack;
    });

    if (owner.loadsPersistedValues()) {
      cResourceData.reregisterAssets();
    }
  }

  void Sequence::removed() {
    // Nothing to do.
  }

  Basics& Sequence::getAssetManager() {
    return cBasics;
  }

  IsoRealms::Project& Sequence::getProject() const {
    return cBasics.getProject();
  }

  bool Sequence::isReadOnly() const {
    return cResourceData.isReadOnly();
  }

  void Sequence::setOwner(ProjectFile* owner) {
    cResourceData.setOwner(owner);
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

  void Sequence::setInstanceName(SequenceInstance& instance, const std::string& name) {
    std::string mOldName = getInstanceName(instance);
    if (mOldName == name) {
      return;
    }
    cDefInstances.emplace(name, std::move(cDefInstances[mOldName]));
    cDefInstances.erase(mOldName);
    refreshAssetRegistration();
  }

  bool Sequence::isInstanceNameAllowed(SequenceInstance& instance, const std::string& name) {
    std::string mOldName = getInstanceName(instance);
    if (mOldName == name) {
      return true;
    }
    if (cDefInstances.find(name) != cDefInstances.end()) {
      return false;
    }
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

  void Sequence::getAssetProperties(IPropertyMaker& owner) {
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

  unsigned int Sequence::getTrackCount() const {
    return static_cast<unsigned int>(cDefTracks.size());
  }

  unsigned int Sequence::getTrackIndex(SequenceTrack& track) const {
    std::vector<std::unique_ptr<SequenceTrack>>::const_iterator it = std::find_if(cDefTracks.begin(), cDefTracks.end(), [&track](const std::unique_ptr<SequenceTrack>& ptr) {
      return ptr.get() == &track;
    });
    return static_cast<unsigned int>(it - cDefTracks.begin());
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
    SequenceTrack* mTrack = cDefTracks.emplace_back(std::make_unique<SequenceTrack>(*this)).get();
    for (std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      ISequenceTrackInstance* mTrackInstance = (*mTrack)->createTrackInstance(*mEntry.second.get());
      mEntry.second->addTrackInstance(mTrackInstance);
    }
    refreshAssetRegistration();
  }

  void Sequence::refreshAssetRegistration() {
    cBasics.refreshAssetRegistration(*this);
  }

  void Sequence::deleteTrack(unsigned int track) {
    cDefTracks.erase(cDefTracks.begin() + track);
    for (std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      mEntry.second->deleteTrackInstance(track);
    }
    refreshAssetRegistration();
  }

  void Sequence::trackStateChanged(SequenceTrack& track) {
    unsigned int mTrackIndex = getTrackIndex(track);
    for (std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      ISequenceTrackInstance* mTrackInstance = track->createTrackInstance(*mEntry.second.get());
      mEntry.second->refreshTrackInstance(mTrackInstance, mTrackIndex);
    }
    refreshAssetRegistration();
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

  void Sequence::Length::getAssetProperties(IPropertyMaker& owner) {
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
