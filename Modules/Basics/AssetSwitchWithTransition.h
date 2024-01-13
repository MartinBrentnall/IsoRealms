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

#include <algorithm>
#include <functional>
#include <vector>

namespace IsoRealms::Basics {

  /**
   * Resource definition for a switchable Asset with a transition period.  The
   * period of the transition from the current value to a new value can be
   * specified when the new value is set.
   */
  template <class TYPE> class AssetSwitchWithTransition : public TYPE {
    protected:

    // Runtime data.
    class Value {
      private:
      TYPE* cRuntimeValue;             /// A current value.
      int cRuntimeTransitionDuration;  /// Duration until fully reaching this value.
      int cRuntimeTransitionRemaining; /// Remaining time until fully reaching this value.

      public:
      Value(TYPE* value, int transitionDuration) :
                cRuntimeValue(value),
                cRuntimeTransitionDuration(std::max(transitionDuration, 1)),
                cRuntimeTransitionRemaining(transitionDuration) {
      }

      TYPE* getValue() const {
        return cRuntimeValue;
      }

      float getWeight() const {
        return -(cRuntimeTransitionRemaining / (float) cRuntimeTransitionDuration) + 1.0f;
      }

      bool update(unsigned int milliseconds) {
        cRuntimeTransitionRemaining = std::max(0, cRuntimeTransitionRemaining - static_cast<int>(milliseconds));
        return cRuntimeTransitionRemaining == 0;
      }
    };
    std::vector<Value> cRuntimeValues; /// Mixed runtime values.

    /***************\
     * Constructor *
    \***************/
    AssetSwitchWithTransition(IProject* project, std::function<TYPE*()> getValue) {
      project->updateRuntime([this](unsigned int milliseconds) {
        unsigned int mBaseIndex = 0;
        for (unsigned int i = 1; i < cRuntimeValues.size(); i++) {
          if (cRuntimeValues[i].update(milliseconds)) {
            mBaseIndex = i;
          }
        }

        for (unsigned int i = 0; i < mBaseIndex; i++) {
          cRuntimeValues.erase(cRuntimeValues.begin());
        }
      });

      project->reset([this, getValue]() {
        cRuntimeValues.clear();
        cRuntimeValues.emplace_back(getValue(), 0);
      });
    }

    public:

    /**********************\
     * Resource Interface *
    \**********************/
    void hintInUse(bool inUse) {
      // Nothing to do.
    }

    bool renderIcon() const {
      return false;
    }

    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
      return std::vector<IProperty*>({
      });
    }

    /***********************\
     * Scripting Interface *
    \***********************/
    void setValue(TYPE* value, int transitionDuration) {
      if (transitionDuration <= 0) {
        cRuntimeValues.clear();
      }
      cRuntimeValues.emplace_back(Value(value, transitionDuration));
    }
  };
}
