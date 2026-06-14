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
#pragma once

#include <deque>
#include <mutex>

#include <SFML/Audio.hpp>

#include "IsoRealms.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Component definition for a sound loaded from a file that can be played as
   * an action.  File type support is provided by SFML.
   */
  class FileSound final : public IAction {
    public:

    /***********************\
     * Component Interface *
    \***********************/
    FileSound(Basics& basics, IComponentData& data);
    void define(IComponentDefiner& definer);
    void publish(ResourcePublisher& publisher);
    bool renderIcon() const;
    
    // Interface called by module when adjusting global sound volume.
    void setVolume(float volume);

    /**********************\
     * Implements IAction *
    \**********************/
    void execute() override;
    bool renderResourceIcon() const override;

    private:
    
    // External interfaces.
    Basics& cDefBasics; /// Module holding the global sound volume.
    
    // Definition data.
    File cDefFile;      /// Filename containing the sound to play.

    // Runtime data.
    static std::mutex cRuntimeLoadMutex;  /// Make sure only one sound is loaded at a time.
    sf::SoundBuffer cRuntimeSoundData;    /// Actual sound data loaded from the file.
    std::deque<sf::Sound> cRuntimeSounds;

    /**********************\
     * Internal Functions *
    \**********************/
    void reloadData();
  };
}
