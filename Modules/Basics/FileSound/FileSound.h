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
#pragma once

#include <deque>
#include <mutex>

#include <SFML/Audio.hpp>

#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a sound loaded from a file that can be played as
   * an action.  File type support is provided by SFML.
   */
  class FileSound final : public IActionType,
                          public IAction {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    FileSound(IProject* project, Basics* basics);
    FileSound(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    // Interface called by module when adjusting global sound volume.
    void setVolume(float volume);

    /**************************\
     * Implements IActionType *
    \**************************/
    IAction* createAction(DOMNode& node, IProject* project, IBindingRegistry* localObjects) override;
    IAction* createAction(IProject* project, IBindingRegistry* localArgs) override;
    void destroyAction(IAction* action, IAssets* assets) override;
    bool renderAssetIcon() const override;

    /**********************\
      * Implements IAction *
    \**********************/
    void execute() override;
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier, const std::string& tag) const override;
    const IActionType* getActionType() const override;
    bool hasConfiguration() const override;

    private:

    // DOM strings.
    static const std::string ATTRIBUTE_FILE;

    // Definition data.
    Basics* cDefBasics;   /// Module holding the global sound volume.
    std::string cDefFile; /// Filename containing the sound to play.

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
