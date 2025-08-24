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
#include <iostream>
#include <vector>

#include "IsoRealms/Application.h"
#include "IsoRealms/Exception/InitException.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/Options/Options.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/System.h"

int main(int argc, char** argv) {
  int mReturnCode = 0;
  try {

    // Prefer fixed notation output for numbers.
    std::cout.setf(std::ios::fixed, std::ios::floatfield);

    // Construct and initialise application and project.
    IsoRealms::Options mOptions(argc, argv);
    IsoRealms::Application mApplication;
    bool mProjectFinished = false;
    IsoRealms::Project mProject(mApplication, [&mProjectFinished](bool forceQuit) {
      mProjectFinished = true;
    }, mOptions.getOption("file"), mOptions.getOption("type") == "user");
    mProject.reset();
    mProject.initMainThread();

    // Events might have accumulated during project construction, so lets flush it before starting.
    sf::Event mEvent;
    while (mApplication.pollEvent(mEvent));

    // Project execution main loop.  Continues until project finish callback.
    unsigned int mLeftoverMilliseconds = 0;
    sf::Clock mClock;
    int mPreviousTime = mClock.getElapsedTime().asMilliseconds() - 10;
    float mAspectRatio = mApplication.getScreenAspectRatio();

    // Disable antialiasing TODO: This was added to make sequence editing look nicer... Probably should be moved elsewhere for Spindizzy
    glDisable(GL_DITHER);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glHint(GL_POINT_SMOOTH, GL_DONT_CARE);
    glHint(GL_LINE_SMOOTH, GL_DONT_CARE);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
    #define GL_MULTISAMPLE_ARB 0x809D
    glDisable( GL_MULTISAMPLE_ARB);

    while (!mProjectFinished) {
      int mCurrentTime = mClock.getElapsedTime().asMilliseconds();
      int mMillisecondsPassed = mCurrentTime - mPreviousTime;
      if (mMillisecondsPassed > 0) {

        // Process all pending input events.
        while (mApplication.pollEvent(mEvent)) {
          mApplication.getHatHandler().input(mEvent);
          if (mEvent.type == sf::Event::Closed) {
            mProject.requestQuit();
          }
          mProject.input(mEvent);
        }

        // Update in fixed increments to ensure consistent behaviour.
        mMillisecondsPassed += mLeftoverMilliseconds;
        while (mMillisecondsPassed >= 10) {
          mProject.updateRuntime(10);
          mMillisecondsPassed -= 10;
        }
        mProject.updateRuntimeComplete();
        mLeftoverMilliseconds = mMillisecondsPassed;

        // Render the project.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glPopAttrib();
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, 0);
        glLoadIdentity();
        glScalef(mAspectRatio, 1.0f, 1.0f);
        mProject.render(1.0f / mAspectRatio);
        glLoadIdentity();
        glEnable(GL_DEPTH_TEST);
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glPopAttrib();
        mApplication.display();
      }
      mPreviousTime = mCurrentTime;
      mApplication.cleanUp();
    };

    // Save settings on completion. TODO: Make save scriptable instead of program doing it.
    mApplication.saveDefaultSettings();
  } catch (IsoRealms::InitException& e) {
    std::cout << "Fatal: " << std::endl << e.getMessage() << std::endl;
    mReturnCode = 1;
  } catch (std::exception& e) {
    std::cout << "Fatal: " << std::endl << "Unhandled exception thrown: " << e.what() << std::endl;
    mReturnCode = 1;
  } catch (IsoRealms::ParseException& e) {
    std::cout << "Fatal: " << std::endl << e.getMessage() << std::endl;
    mReturnCode = 1;
  } catch (...) {
    std::exception_ptr eptr = std::current_exception();
    try {
      if (eptr) {
        std::rethrow_exception(eptr);
      }
    } catch(const std::exception& e) {
      std::cout << "Fatal:  Unhandled exception \"" << e.what() << "\"" << std::endl;
    }
  }

  // Clean up.
  return mReturnCode;
}

