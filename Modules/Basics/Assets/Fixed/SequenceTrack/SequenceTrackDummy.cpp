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
#include "SequenceTrackDummy.h"

namespace IsoRealms::Basics {
  SequenceTrackDummy::SequenceTrackDummy(Sequence& sequence) {
    // Nothing to do.
  }

  void SequenceTrackDummy::registerAssets(IAssetRegistry& assets) {
    // Nothing to do.
  }
  
  void SequenceTrackDummy::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    // Nothing to do.
  }

  bool SequenceTrackDummy::play(unsigned int milliseconds) {
    return false;
  }

  void SequenceTrackDummy::reset() {
    // Nothing to do.
  }

  unsigned int SequenceTrackDummy::getDuration() const {
    return 0;
  }

  void SequenceTrackDummy::setName(const std::string& name) {
    // Track has no name, not supported.
  }

  std::string SequenceTrackDummy::getName() const {
    return "";
  }

  ISequenceTrackEvent* SequenceTrackDummy::createEvent(IProject& project, unsigned int time) {
    return nullptr;
  }

  void SequenceTrackDummy::deleteEvent(ISequenceTrackEvent* event) {
    // Nothing to do.
  }

  std::vector<ISequenceTrackEvent*> SequenceTrackDummy::getEvents() {
    return std::vector<ISequenceTrackEvent*>();
  }

  void SequenceTrackDummy::stopPreview() {
    // Nothing to do.
  }

  void SequenceTrackDummy::setPreviewPosition(long position) {
    // Nothing to do.
  }

  void SequenceTrackDummy::renderIcon() const {
    Utils::renderIconNone();
  }

  void SequenceTrackDummy::render(float left, float bottom, float right, float top, double startTime, double endTime) const {
    // Nothing to do.
  }
  
  bool SequenceTrackDummy::renderAssetIcon() const {
    return false;
  }
  
  void SequenceTrackDummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }
  
  std::vector<std::unique_ptr<IProperty>> SequenceTrackDummy::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackDummy::isDefaultConfiguration() const {
    return true;
  }
}
