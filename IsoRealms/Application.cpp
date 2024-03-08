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
#include "Application.h"
 
namespace IsoRealms {
  const std::string Application::TAG_SETTINGS          = "IsoRealmsSettings";
  
  const std::string Application::ATTRIBUTE_FULL_SCREEN = "fullScreen";
  const std::string Application::ATTRIBUTE_HEIGHT      = "height";
  const std::string Application::ATTRIBUTE_WIDTH       = "width";
  
  const std::string Application::FILENAME_SETTINGS     = "settings.xml";

  Application::Application() :
            cReleaseThreads(false) {

    // Enable game controller support
//     SDL_JoystickEventState(SDL_ENABLE);
//     for (int i = 0; i < SDL_NumJoysticks(); i++) {
//       SDL_JoystickOpen(i);
//     }
    
    // Initialise Apache Xerces-C
    try {
      xercesc::XMLPlatformUtils::Initialize();
    } catch (const xercesc::XMLException &mXMLException) {
      throw ApplicationException("Failed to initialise Xerces-C: " + std::string(xercesc::XMLString::transcode(mXMLException.getMessage())));
    }

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
      DOMNode mConfigurationRootNode(FILENAME_SETTINGS, DOMNode::Type::USER);
      DOMNode& mSettingsNode = mConfigurationRootNode.getNode(TAG_SETTINGS);
      cFullScreen          = mSettingsNode.getBooleanAttribute(ATTRIBUTE_FULL_SCREEN);
      unsigned int mWidth  = mSettingsNode.getIntegerAttribute(ATTRIBUTE_WIDTH);
      unsigned int mHeight = mSettingsNode.getIntegerAttribute(ATTRIBUTE_HEIGHT);
      
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

  ScreenLocation Application::normalise(const int x, const int y) const {
    float mHalfScreenWidth  = cAvailableResolutions[cSelectedResolution].getWidth()  / 2.0f;
    float mHalfScreenHeight = cAvailableResolutions[cSelectedResolution].getHeight() / 2.0f;
    return ScreenLocation((x - mHalfScreenWidth) / mHalfScreenWidth, -(y - mHalfScreenHeight) / mHalfScreenHeight);
  }

  float Application::normalise(int pixels) const {
    return static_cast<float>(pixels * 2) / cAvailableResolutions[cSelectedResolution].getHeight();
  }

  ScreenArea Application::crop(const ScreenArea& area) const {
    float mHalfScreenWidth  = cAvailableResolutions[cSelectedResolution].getWidth()  / 2.0f;
    float mHalfScreenHeight = cAvailableResolutions[cSelectedResolution].getHeight() / 2.0f;
    
    int mLeft   = static_cast<int>(area.getLeft()   * mHalfScreenWidth  + mHalfScreenWidth);
    int mRight  = static_cast<int>(area.getRight()  * mHalfScreenWidth  + mHalfScreenWidth);
    int mBottom = static_cast<int>(area.getBottom() * mHalfScreenHeight + mHalfScreenHeight);
    int mTop    = static_cast<int>(area.getTop()    * mHalfScreenHeight + mHalfScreenHeight);
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
    DOMNodeWriter mConfigurationNode(TAG_SETTINGS);
    mConfigurationNode.addAttribute(ATTRIBUTE_FULL_SCREEN, cFullScreen);
    mConfigurationNode.addAttribute(ATTRIBUTE_WIDTH,       cAvailableResolutions[cSelectedResolution].getWidth());
    mConfigurationNode.addAttribute(ATTRIBUTE_HEIGHT,      cAvailableResolutions[cSelectedResolution].getHeight());
    mConfigurationNode.save(FILENAME_SETTINGS);
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
                   "IsoRealms Spindizzy",
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
}
