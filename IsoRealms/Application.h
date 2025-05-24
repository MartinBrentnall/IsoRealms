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

#include <condition_variable>
#include <iostream>
#include <functional>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
 
#include "DisplayResolution.h"
#include "Exception/ApplicationException.h"
#include "IApplication.h"
#include "Input/HatHandler.h"
#include "Persistence/JSONDocument.h"
#include "System.h"
 
namespace IsoRealms {
  
  /**
   * Thread pool allows tasks to be performed in parallel, and/or large tasks to
   * be divided into smaller chunks that can be performed independently.  Each
   * tasks consists of a vector of functions that take no arguments and return
   * no value, where all functions can be performed simultaneously.
   */ 
  class Application : public IApplication {
    private:
    static const std::string JSON_FULL_SCREEN;
    static const std::string JSON_HEIGHT;
    static const std::string JSON_SETTINGS;
    static const std::string JSON_WIDTH;

    static const std::string FILENAME_SETTINGS;
    
    /**
     * Task wrapper contains data regarding number of workers assigned to a task
     * and the progress of the task.
     */
    class Task {
      private:
      
      /**
       * Collection of task functions to be performed in parallel.
       */
      std::vector<std::function<void()>> cTask;
      
      /**
       * Number of worker threads assigned to this task.
       */
      unsigned int cWorkers;
      
      /**
       * Index of next task function to be performed.
       */
      unsigned int cFunctionIndex;
      
      /**
       * Mutex controls access to functions of the internal task.  Only one
       * thread may be obtaining a function at any given time.
       */
      std::mutex cFunctionMutex;
      
      /**
       * Flag to indicate that all functions of the internal task have been
       * completed.
       */
      bool cComplete;

      /**
       * Mutex controls access to fields that control completion of the task.
       */
      std::mutex cCompleteMutex;
      
      /**
       * Condition to be notified when the task has completed.  This may be used
       * to wait for a task to complete.
       */
      std::condition_variable cCompleteCond;
      
      public:
      
      /**
       * Construct a task with the specified internal task.
       *
       * @param task The internal task to execute.
       */
      Task(const std::vector<std::function<void()>> task);
      
      /**
       * Await completion of this task.  This function does not return until
       * after all functions of the task have been completed.
       */
      void wait();

      /**
       * Increment the worker count of this task.  This should be called once by
       * each thread that will execute functions of this task.
       */
      void addWorker();
      
      /**
       * Executes the next function of the internal task.
       *
       * @return true if more functions are available, otherwise false.
       */
      bool executeNextFunction();
      
      /**
       * Decrement the worker count of this task.  This should be called once by
       * each thread that added a worker after no more functions are available
       * within this task.  When the worker count reaches zero, this indicates
       * that all functions have been completed, thus any threads awaiting
       * completion of this task will resume.
       */
      void removeWorker();
    };
    
    // Application window fields.
    sf::Window cWindow;                                   /// The application window.
    std::vector<DisplayResolution> cAvailableResolutions; /// List of available screen resolutions supported by the system.
    unsigned int cSelectedResolution;                     /// Index of application windows current screen resolution.
    bool cFullScreen;                                     /// Whether the application is fullscreen or not.
    
    // Threadpool fields.
    std::queue<std::shared_ptr<Task>> cTasks;      /// Tasks to be performed by this thread pool.
    std::vector<std::thread> cThreads;             /// Worker threads.
    unsigned int cWorkingThreads;                  /// Count for the number of working threads.
    std::condition_variable cTriggerCondition;     /// Condition to be triggered to begin execution of all tasks within the the task queue.
    std::mutex cTaskMutex;                         /// Mutex controls access to the task queue.
    bool cReleaseThreads;                          /// Set when the application is terminating to release all threads.

    // Input.
    HatHandler cHatHandler;

    std::mutex cCleanUpTaskMutex;
    std::queue<std::function<void()>> cMainThreadCleanUpTasks; /// Clean-up tasks to be performed on the main thread.

    /**
     * Set the application window and OpenGL view port dimensions according to
     * the current screen mode.
     */
    void resizeScreen();
    
    /**
     * Add the specified task to the task queue, then immediately trigger the
     * thread pool to begin working, in case it is not already working.  The
     * internal task object is returned.
     *
     * @param The task to start.
     * @return Internal task object representing the specified task.
     */
    Task* startTask(const std::vector<std::function<void()>> task);
    
    public:
    
    /**
     * Construct an IsoRealms application.  Specifically, this constructs the
     * window in which an IsoRealms application is shown within, and performs
     * other global level initialisation such as setting up the thread-pool.
     */
    Application();
    ~Application();

    bool pollEvent(sf::Event& event);
    void display();

    /**
     * Main loop function for threads in the pool.  This should not be called
     * by the main thread.
     */
    void loop(int threadID);
    void cleanUp();

    /***************************\
     * Implements IApplication * 
    \***************************/   
    std::vector<DisplayResolution> getAvailableDisplayResolutions() const override;
    DisplayResolution getDisplayResolution() const override;
    bool isFullScreen() const override;
    void setDisplayResolution(DisplayResolution resolution, bool fullscreen) override;
    float getScreenAspectRatio() const override;
    Point2D normalise(const int x, const int y) const override;
    float normalise(const int pixels) const override;
    ScreenArea crop(const ScreenArea& area) const override;
    bool isCropped(const ScreenArea& area) const override;
    void setViewPort() const override;
    void saveDefaultSettings() const override;
    void executeAndWait(const std::vector<std::function<void()>> task) override;
    void executeAndReturn(const std::vector<std::function<void()>> task) override;
    void executeAndReturn(const std::function<void()> task) override;
    HatHandler& getHatHandler() override;
    void mainThreadCleanUp(std::function<void()> function) override;
  };
}
