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

#include <iomanip>
#include <vector>

#include "IsoRealms/Editing.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a Timer (Integer) variable in milliseconds.  The
   * Timer counts down in real-time, and upon reaching zero will perform an
   * expiration action and stop (the Timer cannot go below zero).
   *
   * The Scripting interface allows the Timer value to be restarted, set, and
   * allows the Timer to paused/resumed, and the speed of the Timer to be
   * changed (as a multiplier).  Setting the value to zero will trigger the
   * expiration action immediately if the Timer is not already at zero.
   *
   * The Timer is also exposed as a String asset, formatted in a human-friendly
   * "hh:mm:ss" format.
   *
   * The Timer does not support state change notification.
   */
  class Timer final : public IInteger {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    Timer(IProject& project, Basics& basics, IResourceData& data);
    Timer(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IAssetBrowser& browser, IAssetRegistry& assets);

    /***********************\
     * Implements IInteger *
    \***********************/
    int getValue() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    /***********************\
     * Scripting Interface *
    \***********************/
    void start(unsigned int milliseconds);
    void pause();
    void resume();
    void setValue(int value);
    void setSpeed(float speed);
    
    private:

    // JSON members.
    static const std::string JSON_ON_EXPIRATION;
    static const std::string JSON_VALUE;

    // Definition data.
    int cDefMilliseconds;
    Action cDefExpirationAction;

    // Runtime data.
    int cRuntimeMilliseconds;
    bool cRuntimePaused;
    float cRuntimeSpeed;

    // Scripting Interface.
    LuaBinding<Timer> cLuaBinding;

    /**
     * Expose the timer as a human-friendly formatted string (hh:mm:ss).
     */
    class StringTimer : public IString {
      public:
      StringTimer(Timer& parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      Timer& cParent;
    } cStringInterface;
  };
}
