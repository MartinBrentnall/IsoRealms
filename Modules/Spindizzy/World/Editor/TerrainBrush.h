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
#include <GL/glew.h>

#include "IsoRealms/AnimatedFloat.h"
#include "IsoRealms/Condition/Condition.h"
#include "IsoRealms/Condition/ConditionElement.h"
#include "IsoRealms/Literals.h"

#include "Modules/Spindizzy/World/Editor/SignalInputID.h"

namespace IsoRealms::Spindizzy {
  class TerrainType;

  class TerrainBrush {
    public:
    TerrainBrush();

    // Configuration functions
    void toggleEditing();
    bool isEditing() const;
    void raiseCorner(unsigned int x, unsigned int y);
    void lowerCorner(unsigned int x, unsigned int y);
    void toggleSplit();
    void toggleSteppedBottom();

    // Automatic configuration functions
    void reset();

    // Block preparation functions
    int getNorthWestHeight() const;
    int getNorthEastHeight() const;
    int getSouthWestHeight() const;
    int getSouthEastHeight() const;
    bool isSplitNorthWestSouthEast() const;
    bool isAlternativeSplit() const;

    void renderPreview(const TerrainType* type, float pinnedX, float pinnedY, float pinnedZ, float cursorX, float cursorY, float cursorZ, bool steppedBottom) const;
    void renderEditing(const TerrainType* type, float pinnedX, float pinnedY, float pinnedZ, float cursorX, float cursorY, float cursorZ, bool steppedBottom) const;

    void updateEditing(unsigned int milliseconds);

    float getHeight(float x, float y) const;

    bool input(SignalInputID id, double yaw);
    bool isCursorLocked() const;
    void update(unsigned int milliseconds, double yaw, double xSpeed, double ySpeed);
    bool isSplit() const;

    void stepWest();
    void stepEast();
    void stepSouth();
    void stepNorth();
    void stepLeft(double yaw);
    void stepRight(double yaw);
    void stepDown(double yaw);
    void stepUp(double yaw);

    private:
    AnimatedFloat cCornerHeight[2][2];
    bool cAlternativeSplit;

    // Editing
    // TODO: Maybe we can make some kind of generic cursor class out of the following fields
    bool cRuntimeEditing;
    double cRuntimeCursorX;
    double cRuntimeCursorY;
    int cDefAnalogueSensitivity; // TODO: This should be global?

    bool lowerSelected();
    bool raiseSelected();

    int getXSlope() const;
    int getYSlope() const;
    float getXSlopeAnimation() const;
    float getYSlopeAnimation() const;
    bool isSplitAnimation() const;
    double getCursorXSpeed() const;
    double getCursorYSpeed() const;
  };
}
