/*
 * Copyright 2009 Martin Brentnall
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
#ifndef TERMINATE_ATTRACT_SCENE_COMMAND_H
#define TERMINATE_ATTRACT_SCENE_COMMAND_H

#include "AttractSceneManager.h"

#include <IsoRealms/ICommand.h>

class TerminateAttractSceneCommand:public ICommand {
  private:

  /**
   * The manager used to terminate the attract scene.
   */
  AttractSceneManager* cAttractSceneManager;

  /**
   * The attract scene to terminate on event execution.
   */
  IAttract* cAttractScene;

  public:

  /**
   * Construct an event that will terminate the specified attract scene using
   * the specified manager.
   *
   * @param AttractSceneManager&  The manager in which to run the scene.
   * @param IAttract&             The scene to start.
   */
  TerminateAttractSceneCommand(AttractSceneManager&, IAttract*);

  /**************************************************************************\
   * Implemented methods of ICommand.h                                      *
  \**************************************************************************/
  void execute();
};

#endif
