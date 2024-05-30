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
 * MERCHANTABILITY or FITNESS FOR A PARTIC  ULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "IsoRealms/Literals.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Spindizzy {
  class IAppearance;
  class IWorldEditorTool;
  class World;
  class WorldEditor;
  
  /**
   * Interface to be implemented by tools for world editing.
   */
  class IWorldEditorToolInstance {
    public:
    
    /**
     * Determines whether this instance was derived from the specified tool.
     *
     * @param tool The tool to test.
     * @return true If this instance was derived from the specified tool.
     */
    virtual bool isTool(IWorldEditorTool* tool) const = 0;
    
    /**
     * Render an icon to represent this tool.
     *
     * @param yaw The yaw angle of the icon (corresponds with the view in the
     *         world being edited).
     */
    virtual bool renderIcon(float yaw) const = 0;
    
    /**
     * Render a preview of this tool within the specified world, at the
     * specified location.  The preview typically gives some visual indication
     * of how use of the tool would impact the world (e.g. showing an outline
     * of terrain being drawn).
     */
    virtual void renderEditingPreview() const = 0;
    
    /**
     * Render UI elements of this tool.  This includes things like property
     * configuration that should appear independent of the world, rather than
     * within it.
     */
    virtual void renderUI(float aspectRatio) const = 0;
    
    /**
     * Update animated UI elements of the tool.
     *
     * @param milliseconds Amount of time to animate.
     */
    virtual void updateUI(unsigned int milliseconds) = 0;
    
    virtual bool inputTool(int id, double yaw) = 0;
    
    /**
     * Process editor cursor movement.  This is useful for tools that perform
     * object selection based on cursor movement.  The end position may be
     * modified if the tool wishes to restrict or modify cursor movement.
     *
     * @param start Position from which the cursor has moved.
     * @param end Position to which the cursor has moved.     
     */
    virtual void processCursorMovement(LiteralVertex* start, LiteralVertex* end) = 0;
    
    /**
     * Get the cursor snap interval for the X/Y axis when using this tool.
     * Most tools snap to the nearest cell (1), but a some objects may be
     * placed more precisely.
     *
     * @return Cursor snap interval when using this tool.
     */
    virtual double getSnapInterval() const = 0;
    
    virtual ~IWorldEditorToolInstance() {}
  };
}
