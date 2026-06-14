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
#include "IsoRealms/ComponentLoader.h"
#include "SequenceInstance.h"

namespace IsoRealms::Basics {
  Sequence::Sequence(Basics& basics, IComponentData& data) :
            cBasics(basics),
            cComponentData(data),
            cDefSpeed(data, 1.0f),
            cExposedLength(*this),
            cLuaBinding(data.getProject().getLuaState(), this) {
  }

  void Sequence::define(IComponentDefiner& definer) {
    definer.propertyEditor(  "Content", this);
    definer.propertyBoolean( "playing", [this]() {return cDefPlaying;}, [this](bool value) {cDefPlaying = value;});
    definer.propertyBoolean( "loop",    [this]() {return cDefLoop;},    [this](bool value) {cDefLoop    = value;});
    definer.propertyResource("speed",   cDefSpeed);
    definer.array("instances", cDefInstances, [](const std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry) -> SequenceInstance& {return *mEntry.second;}, [this, &definer](SequenceInstance& instance) {
      definer.scope("Instance", getInstanceName(instance), [this, &instance](IComponentDefiner& definer) {
        instance.define(definer);
      }, [this, &instance]() {
        cDefInstances.erase(getInstanceName(instance));
      });
    }, [this]() -> SequenceInstance& {
      std::string mKey = Utils::getAvailableKey(cDefInstances, "Instance");
      return *cDefInstances.emplace(mKey, std::make_unique<SequenceInstance>(*this)).first->second;
    });

    // Tracks.
    definer.array("tracks", cDefTracks, [](const std::unique_ptr<SequenceTrack>& mTrack) -> SequenceTrack& {return *mTrack;}, [this, &definer](SequenceTrack& track) {
      Options mHint;
      mHint.addOption(Options::PROPERTY_IMMEDIATE, "true");
      definer.propertyResource("track", track, mHint);
      track.stateChanged();
    }, [this]() -> SequenceTrack& {
      SequenceTrack* mTrack = cDefTracks.emplace_back(std::make_unique<SequenceTrack>(*this)).get();
      for (std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
        ISequenceTrackInstance* mTrackInstance = (*mTrack)->createTrackInstance(*mEntry.second.get());
        mEntry.second->addTrackInstance(mTrackInstance);
      }
      return *mTrack;
    });

    if (definer.loadsPersistedValues()) {
      cComponentData.republish();
    }
  }

  void Sequence::publish(ResourcePublisher& publisher) {
    publisher.publish<IBinding>(&cLuaBinding, "", "Sequences");
    publisher.publish<IInteger>(&cExposedLength, "Length", "Sequences");
    publisher.publish<IEditable>(this, "", "Sequences");
    for (const std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      mEntry.second->publish(publisher, mEntry.first);
    }
  }
  
  void Sequence::load(IComponentData& resourceData, JSONObject object) {
    // Nothing to do.
  }

  void Sequence::save(IComponentData& resourceData, JSONObject object) const {
    // Nothing to do.
  }
  
  Basics& Sequence::getResourceManager() {
    return cBasics;
  }

  IsoRealms::Project& Sequence::getProject() const {
    return cBasics.getProject();
  }

  bool Sequence::isReadOnly() const {
    return cComponentData.isReadOnly();
  }

  void Sequence::setOwner(ProjectFile* owner) {
    cComponentData.setOwner(owner);
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

  IComponentData& Sequence::getComponentData() {
    return cComponentData;
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
    refreshResourceRegistration();
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
    refreshResourceRegistration();
  }

  void Sequence::refreshResourceRegistration() {
    cBasics.refreshResourceRegistration(*this);
  }

  void Sequence::deleteTrack(unsigned int track) {
    cDefTracks.erase(cDefTracks.begin() + track);
    for (std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      mEntry.second->deleteTrackInstance(track);
    }
    refreshResourceRegistration();
  }

  void Sequence::trackStateChanged(SequenceTrack& track) {
    unsigned int mTrackIndex = getTrackIndex(track);
    for (std::pair<const std::string, std::unique_ptr<SequenceInstance>>& mEntry : cDefInstances) {
      ISequenceTrackInstance* mTrackInstance = track->createTrackInstance(*mEntry.second.get());
      mEntry.second->refreshTrackInstance(mTrackInstance, mTrackIndex);
    }
    refreshResourceRegistration();
  }

  Sequence::Length::Length(Sequence& parent) :
            cParent(parent) {
  }

  int Sequence::Length::getValue() const {
    return cParent.getDuration();
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
