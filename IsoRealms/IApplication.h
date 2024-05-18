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

#include <iostream>
#include <functional>
#include <memory>
#include <queue>
#include <vector>

#include "Common/ScreenArea.h"
#include "Common/ScreenLocation.h"
#include "DisplayResolution.h"
 
namespace IsoRealms {
  class HatHandler;
  
  /**
   * The application interface provides several application-level
   * functionalities.  Current functionalities provided by the application
   * include window/screen-related functions, Lua state management, and the
   * thread pool.
   * 
   * Thread pool allows tasks to be performed in parallel, and/or large tasks to
   * be divided into smaller chunks that can be performed independently.  Each
   * tasks consists of a vector of functions that take no arguments and return
   * no value, where all functions can be performed simultaneously.
   */ 
  class IApplication {
    public:

    /**
     * Retrieve a vector containing screen modes available on the current
     * hardware.
     * 
     * @returns vector containing available screen modes.
     */
    virtual std::vector<DisplayResolution> getAvailableDisplayResolutions() const = 0;
    
    /**
     * Retrieve the current screen mode.
     * 
     * @returns The current screen mode.
     */
    virtual DisplayResolution getDisplayResolution() const = 0;
    
    /**
     * Determine if the application is currently fullScreen.
     *
     * @returns true if the application is fullscreen, otherwise false.
     */
    virtual bool isFullScreen() const = 0;
    
    /**
     * Set the screen mode.
     * 
     * @param mode The resolution to set.
     * @param fullscreen true to set fullscreen, false to set windowed.
     */
    virtual void setDisplayResolution(DisplayResolution mode, bool fullscreen) = 0;

    /**
     * Retrieve the aspect ratio of the current screen mode.  The aspect ratio
     * assumes a screen height of 1.0, so a standard widescreen display would
     * return 1.77777777, and a portrait display would return a value less
     * than zero.
     * 
     * @return The aspect ratio of the current screen mode.
     */
    virtual float getScreenAspectRatio() const = 0;
    
    /**
     * Take the specified pixel co-ordinates and convert them to a normalised
     * resolution independent screen location (x and y values between -1.0 and
     * 1.0).
     * 
     * @param x Pixel location across the screen.
     * @param y Pixel location down the screen.
     * @return Normalised screen location.
     */
    virtual ScreenLocation normalise(const int x, const int y) const = 0;

    /**
     * Return the vertical distance in GL coordinates for the specified number
     * of pixels assuming a normalised screen height of -1.0 to 1.0.
     * 
     * @param pixels The number of pixels
     * @return Normalised distance.
     */
    virtual float normalise(const int pixels) const = 0;

    /**
     * Set the specified area of the screen to be cropped for rendering.
     * The previous crop area is returned.
     * 
     * @param area The screen area to crop.
     * @return The previous screen area crop.
     */
    virtual ScreenArea crop(const ScreenArea& area) const = 0;

    /**
     * Determine whether the specified area of the screen is fully cropped
     * out of current rendering.
     * 
     * @param area The area to test.
     * @return true if the area is fully cropped.
     */
    virtual bool isCropped(const ScreenArea& area) const = 0;
    
    /**
     * Set the GL view port according to the screen settings.  This is useful
     * to reset the view port after it has been set for other purposes (e.g.
     * render-to-texture).
     */
    virtual void setViewPort() const = 0;

    /**
     * Save the default settings of the application.  The default settings can
     * be used when constructing an application.
     */
    virtual void saveDefaultSettings() const = 0;

    /**
     * Add the specified task to the task queue, then immediately trigger the
     * thread pool to begin working, in case it is not already working.  Does not
     * return until the specified task has been completed.
     *
     * @param The task to perform.
     */
    virtual void executeAndWait(const std::vector<std::function<void()>> task) = 0;
    
    /**
     * Add the specified task to the task queue, then immediately trigger the
     * thread pool to begin working, in case it is not already working.  Returns
     * immediately without waiting for the task to complete.
     *
     * @param The task to perform.
     */
    virtual void executeAndReturn(const std::vector<std::function<void()>> task) = 0;

    /**
     * Add the specified task to the task queue, then immediately trigger the
     * thread pool to begin working, in case it is not already working.  Returns
     * immediately without waiting for the task to complete.
     *
     * @param The task to perform.
     */
    virtual void executeAndReturn(const std::function<void()> task) = 0;

    virtual HatHandler& getHatHandler() = 0;
  };
}
