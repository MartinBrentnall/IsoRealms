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
#include "Application.h"

#include "IsoRealms/Exception/InitException.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/Metadata.h"
#include "IsoRealms/Persistence/ParseException.h"
#include "IsoRealms/Persistence/JSONDocument.h"
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Project/Options.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/System.h"

namespace IsoRealms {
  const std::string Application::JSON_FULL_SCREEN = "fullScreen";
  const std::string Application::JSON_HEIGHT      = "height";
  const std::string Application::JSON_PROPERTIES  = "properties";
  const std::string Application::JSON_SETTINGS    = "settings";
  const std::string Application::JSON_WIDTH       = "width";

  const std::string Application::FILENAME_SETTINGS     = "settings.json";

  Application::Application() :
            cReleaseThreads(false) {

    // Construct the application thread pool.
    cWorkingThreads = 0;
//    for (unsigned int i = 0; i < 1; i++) {
    for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++) {
      cThreads.push_back(std::thread([this, i]() {
        loop(i);
      }));
    }

    // Initialise available display resolutions.
    std::vector<sf::VideoMode> mVideoModes = sf::VideoMode::getFullscreenModes();
    sf::VideoMode mDesktopMode = sf::VideoMode::getDesktopMode();
    for (sf::VideoMode& mVideoMode : mVideoModes) {
      if (mVideoMode.bitsPerPixel == mDesktopMode.bitsPerPixel) {
        cAvailableResolutions.push_back(DisplayResolution(mVideoMode.width, mVideoMode.height));
      }
    }

    // Read application settings file.
    if (System::fileExists(FILENAME_SETTINGS, true)) {
      JSONDocument mSettingsDocument(FILENAME_SETTINGS, true);
      JSONObject mSettingsObject = mSettingsDocument.getObject(JSON_SETTINGS);
      cFullScreen = mSettingsObject.getBoolean(JSON_FULL_SCREEN);
      unsigned int mWidth = mSettingsObject.getInteger(JSON_WIDTH);
      unsigned int mHeight = mSettingsObject.getInteger(JSON_HEIGHT);
      
      // Find the matching resolution
      bool mResolutionFound = false;
      for (unsigned int i = 0; i < cAvailableResolutions.size(); i++) {
        if (cAvailableResolutions[i].getWidth() == mWidth && cAvailableResolutions[i].getHeight() == mHeight) {
          cSelectedResolution = i;
          mResolutionFound = true;
          break;
        }
      }
      
      // Warning, and set to first available
      if (!mResolutionFound) {
        std::cout << "Warning: Configured resolution is not available" << std::endl;
        cSelectedResolution = 0;
      }
    } else {
      cFullScreen         = false;
      cSelectedResolution = 0;
    }


    // Load application metadata.
    std::locale mLocale("");
    std::string mMetadataPath = "Metadata/IsoRealms." + mLocale.name();
    std::string::size_type mLastExtensionIndex = mMetadataPath.find_last_of('.');
    std::string::size_type mLastDashIndex = mMetadataPath.find_last_of('_');
    std::string::size_type mLastSeparatorIndex = (mLastExtensionIndex != std::string::npos && (mLastDashIndex == std::string::npos || mLastExtensionIndex > mLastDashIndex))
                                               ? mLastExtensionIndex
                                               : mLastDashIndex;
    while (!System::fileExists(mMetadataPath + ".json", false) && mLastSeparatorIndex != std::string::npos) {
      mMetadataPath = mMetadataPath.substr(0, mLastSeparatorIndex);
      mLastExtensionIndex = mMetadataPath.find_last_of('.');
      mLastDashIndex = mMetadataPath.find_last_of('_');
      mLastSeparatorIndex = (mLastExtensionIndex != std::string::npos && (mLastDashIndex == std::string::npos || mLastExtensionIndex > mLastDashIndex))
                          ? mLastExtensionIndex
                          : mLastDashIndex;
    }

    if (!System::fileExists(mMetadataPath + ".json", false)) {
      mMetadataPath = "Metadata/IsoRealms.en";
    }
    JSONDocument mMetadataDocument(mMetadataPath + ".json", false);
    JSONObject mPropertiesObject = mMetadataDocument.getObject(JSON_PROPERTIES);
    cMetadata.load(mPropertiesObject);
    resizeScreen();
  }

  Application::~Application() {
    cWindow.close();
    cTaskMutex.lock();
    cReleaseThreads = true;
    cTaskMutex.unlock();
    cTriggerCondition.notify_all();
    for (std::thread& mThread : cThreads) {
      mThread.join();
    }
  }

  int Application::run(Options& options) {
    int mReturnCode = 0;
    try {

      // Prefer fixed notation output for numbers.
      std::cout.setf(std::ios::fixed, std::ios::floatfield);

      // Construct and initialise application and project.
      bool mProjectFinished = false;
      Project mProject(*this, [&mProjectFinished](bool forceQuit) {
        mProjectFinished = true;
      }, options.getOption("file"), options.getOption("type") == "user");
      mProject.reset();
      initMainThread();

      // Events might have accumulated during project construction, so lets flush it before starting.
      sf::Event mEvent;
      while (pollEvent(mEvent));

      // Project execution main loop.  Continues until project finish callback.
      unsigned int mLeftoverMilliseconds = 0;
      sf::Clock mClock;
      int mPreviousTime = mClock.getElapsedTime().asMilliseconds() - 10;
      float mAspectRatio = getScreenAspectRatio();

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
          while (pollEvent(mEvent)) {
            getHatHandler().input(mEvent);
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
          display();
        }
        mPreviousTime = mCurrentTime;
        cleanUp();
      };

      // Save settings on completion. TODO: Make save scriptable instead of program doing it.
      saveDefaultSettings();
    } catch (InitException& e) {
      std::cout << "Fatal: " << std::endl << e.getMessage() << std::endl;
      mReturnCode = 1;
    } catch (std::exception& e) {
      std::cout << "Fatal: " << std::endl << "Unhandled exception thrown: " << e.what() << std::endl;
      mReturnCode = 1;
    } catch (ParseException& e) {
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

  JSONDocument Application::createDocument() {
    return JSONDocument();
  }

  JSONDocument Application::openDocument(const std::string& name) {
    return JSONDocument(name, true);
  }

  bool Application::pollEvent(sf::Event& event) {
    return cWindow.pollEvent(event);
  }

  void Application::display() {
    cWindow.display();
  }

  void Application::loop(int threadID) {
    
    // Count thread as working initially.
    cTaskMutex.lock();
    cWorkingThreads++;
    cTaskMutex.unlock();

    // Main thread loop.
    std::shared_ptr<Task> mCurrentTask;
    while (true) {

      // Await tasks.
      {
        std::unique_lock mLock(cTaskMutex);
        while (cTasks.empty()) {

          // No more tasks, thread is no longer working.
          cWorkingThreads--;

          // Wait for tasks.
          cTriggerCondition.wait(mLock);

          // Check if threads should terminate.
          if (cReleaseThreads) {
            return;
          }

          // Thread is now working!
          cWorkingThreads++;
        }

        // Obtain next task from queue.
        mCurrentTask = cTasks.empty() ? nullptr : cTasks.front();
      }

      // For multiple tasks, loop until none are left.
      while (mCurrentTask != nullptr) {

        // Add ourself as a worker on this task.
        mCurrentTask->addWorker();

        // Execute task functions.
        while (mCurrentTask->executeNextFunction());

        // Check if we can remove ourself from this task.
        mCurrentTask->removeWorker();

        // If we're the first to finish, pop complete task from the queue.
        cTaskMutex.lock();
        if (!cTasks.empty() && mCurrentTask == cTasks.front()) {
          cTasks.pop();
        }
        
        // Get next task.
        mCurrentTask = cTasks.empty() ? nullptr : cTasks.front();
        cTaskMutex.unlock();
      }
    }
  }

  void Application::mainThreadAlloc(std::function<void()> task) {
    cMainThreadAllocTasks.push(task);
  }

  void Application::mainThreadInit(std::function<void()> task) {
    cMainThreadInitTasks.push(task);
  }

  void Application::initMainThread() {
    while (!cMainThreadAllocTasks.empty()) {
      std::function<void()> mTask = cMainThreadAllocTasks.front();
      mTask();
      cMainThreadAllocTasks.pop();
    }
    while (!cMainThreadInitTasks.empty()) {
      std::function<void()> mTask = cMainThreadInitTasks.front();
      mTask();
      cMainThreadInitTasks.pop();
    }
  }

  std::vector<DisplayResolution> Application::getAvailableDisplayResolutions() const {
    return cAvailableResolutions;
  }

  DisplayResolution Application::getDisplayResolution() const {
    return cAvailableResolutions[cSelectedResolution];
  }

  bool Application::isFullScreen() const {
    return cFullScreen;
  }

  void Application::setDisplayResolution(DisplayResolution resolution, bool fullscreen) {
    unsigned int mResolutionIndex = cSelectedResolution;
    for (unsigned int i = 0; i < cAvailableResolutions.size(); i++) {
      if (cAvailableResolutions[i] == resolution) {
        cSelectedResolution = i;
        break;
      }
    }
    if (mResolutionIndex != cSelectedResolution || cFullScreen != fullscreen) {
      cFullScreen = fullscreen;
      resizeScreen();
    }
  }

  float Application::getScreenAspectRatio() const {
    return (float) cAvailableResolutions[cSelectedResolution].getHeight() / (float) cAvailableResolutions[cSelectedResolution].getWidth();
  }

  Point2D Application::normalise(const int x, const int y) const {
    float mHalfScreenWidth  = cAvailableResolutions[cSelectedResolution].getWidth()  / 2.0f;
    float mHalfScreenHeight = cAvailableResolutions[cSelectedResolution].getHeight() / 2.0f;
    return Point2D((x - mHalfScreenWidth) / mHalfScreenWidth / getScreenAspectRatio(), -(y - mHalfScreenHeight) / mHalfScreenHeight);
  }

  float Application::normalise(int pixels) const {
    return static_cast<float>(pixels * 2) / cAvailableResolutions[cSelectedResolution].getHeight();
  }

  ScreenArea Application::crop(const ScreenArea& area) const {
    float mHalfScreenWidth  = cAvailableResolutions[cSelectedResolution].getWidth()  / 2.0f;
    float mHalfScreenHeight = cAvailableResolutions[cSelectedResolution].getHeight() / 2.0f;
    float mAspectRatio = 1.0f / getScreenAspectRatio();
    int mLeft   = static_cast<int>(area.getLeft()  / mAspectRatio * mHalfScreenWidth  + mHalfScreenWidth);
    int mRight  = static_cast<int>(area.getRight() / mAspectRatio * mHalfScreenWidth  + mHalfScreenWidth);
    int mBottom = static_cast<int>(area.getBottom()               * mHalfScreenHeight + mHalfScreenHeight);
    int mTop    = static_cast<int>(area.getTop()                  * mHalfScreenHeight + mHalfScreenHeight);
    int mOldScissorBoxValues[4];
    glGetIntegerv(GL_SCISSOR_BOX, mOldScissorBoxValues);

    if (mLeft > 0 && mTop > 0 && mRight < static_cast<int>(cAvailableResolutions[cSelectedResolution].getWidth()) && mBottom < static_cast<int>(cAvailableResolutions[cSelectedResolution].getHeight())) {
      glEnable(GL_SCISSOR_TEST);
      glScissor(mLeft + 1, mBottom + 1, (mRight - mLeft) - 1, (mTop - mBottom) - 1);
    } else {
      glDisable(GL_SCISSOR_TEST);
    }
    return ScreenArea(static_cast<float>(mOldScissorBoxValues[0]), static_cast<float>(mOldScissorBoxValues[2]), static_cast<float>(mOldScissorBoxValues[1]), static_cast<float>(mOldScissorBoxValues[3]));
  }
  
  bool Application::isCropped(const ScreenArea& area) const {
    float mHalfScreenWidth  = cAvailableResolutions[cSelectedResolution].getWidth()  / 2.0f;
    float mHalfScreenHeight = cAvailableResolutions[cSelectedResolution].getHeight() / 2.0f;
    
    int mLeft   = static_cast<int>(area.getLeft()   * mHalfScreenWidth  + mHalfScreenWidth);
    int mRight  = static_cast<int>(area.getRight()  * mHalfScreenWidth  + mHalfScreenWidth);
    int mBottom = static_cast<int>(area.getBottom() * mHalfScreenHeight + mHalfScreenHeight);
    int mTop    = static_cast<int>(area.getTop()    * mHalfScreenHeight + mHalfScreenHeight);
    int mOldScissorBoxValues[4];
    glGetIntegerv(GL_SCISSOR_BOX, mOldScissorBoxValues);
    int mMaxLeft   = std::max(mLeft,   mOldScissorBoxValues[0]);
    int mMinRight  = std::min(mRight,  mOldScissorBoxValues[0] + mOldScissorBoxValues[2]);
    int mMaxBottom = std::max(mBottom, mOldScissorBoxValues[1]);
    int mMinTop    = std::min(mTop,    mOldScissorBoxValues[1] + mOldScissorBoxValues[3]);
    
    return mMaxLeft > mMinRight || mMaxBottom > mMinTop;
  }
  
  
  void Application::setViewPort() const {
    glViewport(0, 0, cAvailableResolutions[cSelectedResolution].getWidth(), cAvailableResolutions[cSelectedResolution].getHeight());
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  }
  
  void Application::saveDefaultSettings() const {
    JSONDocument mJSONDocument;
    JSONObject mSettingsObject = mJSONDocument.addObject(JSON_SETTINGS);
    mSettingsObject.addBoolean(JSON_FULL_SCREEN, cFullScreen);
    mSettingsObject.addInteger(JSON_WIDTH, cAvailableResolutions[cSelectedResolution].getWidth());
    mSettingsObject.addInteger(JSON_HEIGHT, cAvailableResolutions[cSelectedResolution].getHeight());
    mJSONDocument.save(FILENAME_SETTINGS);
  }

  void Application::executeAndWait(const std::vector<std::function<void()>> task) {
    startTask(task)->wait();
  }

  void Application::executeAndReturn(const std::vector<std::function<void()>> task) {
    startTask(task);
  }

  void Application::executeAndReturn(const std::function<void()> task) {
    std::vector<std::function<void()>> mTask;
    mTask.push_back(task);
    startTask(mTask);
  }
  
  void Application::resizeScreen() {
    cWindow.create(sf::VideoMode(cAvailableResolutions[cSelectedResolution].getWidth(), cAvailableResolutions[cSelectedResolution].getHeight()),
                   "IsoRealms",
                   sf::Style::Titlebar | sf::Style::Close | (cFullScreen ? sf::Style::Fullscreen : 0),
                   sf::ContextSettings(24, 0, 4, 4, 6));

    // OpenGL extensions
    GLenum err = glewInit();
    if (GLEW_OK != err) {
      std::stringstream mStringStream;
      mStringStream << glewGetErrorString(err);
      throw ApplicationException("Failed to initialise GLEW: " + mStringStream.str());
    }

    // Setup OpenGL options
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // Enable a nice alpha blending effect.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, cAvailableResolutions[cSelectedResolution].getWidth(), cAvailableResolutions[cSelectedResolution].getHeight());
  }

  void Application::setName(const std::string& name) {
    cWindow.setTitle(name);
  }

  Application::Task* Application::startTask(const std::vector<std::function<void()>> task) {
    std::shared_ptr<Task> mTask = std::make_shared<Task>(task);
    cTaskMutex.lock();
    cTasks.emplace(mTask);
    Task* mReturnedTask = cTasks.back().get();
    cTaskMutex.unlock();
    cTriggerCondition.notify_all();
    return mReturnedTask;
  }
  
  Application::Task::Task(const std::vector<std::function<void()>> task) {
    cTask          = task;
    cWorkers       = 0;
    cFunctionIndex = 0;
    cComplete      = false;
  }

  void Application::Task::wait() {
    cCompleteMutex.lock();
    while (!cComplete) {
      std::unique_lock mLock(cCompleteMutex, std::adopt_lock);
      cCompleteCond.wait(mLock);
    }
    cCompleteMutex.unlock();
  }

  void Application::Task::addWorker() {
    cCompleteMutex.lock();
    if (!cComplete) {
      cWorkers++;
    }
    cCompleteMutex.unlock();
  }

  bool Application::Task::executeNextFunction() {
    std::function<void()> mTaskFunction;
    bool mGotFunction = false;
    cFunctionMutex.lock();
    if (cFunctionIndex < cTask.size()) {
      mTaskFunction = cTask[cFunctionIndex++];
      mGotFunction = true;
    }
    bool mFunctionsAvailable = cFunctionIndex < cTask.size();
    cFunctionMutex.unlock();
    if (mGotFunction) {
      mTaskFunction();
    }
    return mFunctionsAvailable;
  }

  void Application::Task::removeWorker() {
    cCompleteMutex.lock();
    if (!cComplete) {
      cWorkers--;
      if (cWorkers == 0) {
        cComplete = true;
        cCompleteCond.notify_one();
      }
    }
    cCompleteMutex.unlock();
  }

  HatHandler& Application::getHatHandler() {
    return cHatHandler;
  }

  void Application::cleanUp() {
    std::lock_guard<std::mutex> mLockGuard(cCleanUpTaskMutex);
    while (!cMainThreadCleanUpTasks.empty()) {
      std::function<void()> mTask = cMainThreadCleanUpTasks.front();
      mTask();
      cMainThreadCleanUpTasks.pop();
    }
  }

  void Application::mainThreadCleanUp(std::function<void()> function) {
    std::lock_guard<std::mutex> mLockGuard(cCleanUpTaskMutex);
    cMainThreadCleanUpTasks.push(function);
  }

  const Metadata& Application::getMetadata(const std::string& key) const {
    return cMetadata;
  }
}
