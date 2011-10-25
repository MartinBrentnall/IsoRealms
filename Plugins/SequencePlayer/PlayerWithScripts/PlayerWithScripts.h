/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef PLAYER_WITH_SCRIPTS_H
#define PLAYER_WITH_SCRIPTS_H

#include <set>

#include <IsoRealms/Script.h>

#include "../ISequencePlayer.h"

/**
 * This Player implementation has two major features.  Firstly, it has 
 * configurable sequence duration.  Secondly, it triggers user-definable scripts
 * on certain playback events.
 */
class PlayerWithScripts:public ISequencePlayer,
                        public IDynamicElement {
  private:
  
  ICommandRegistry* cCommandRegistry;
    
  // Definition
  /**
   * The sequences will last for this long when played by this player in
   * milliseconds.
   */
  unsigned int cLength;
  
  /**
   * The sequences under control of this player.
   */
  std::set<ISequence*> cSequences;
  
  /**
   * This script is triggered when playback moves from 0.0 to a number higher
   * than 0.0.  For example, when the play() function is invoked for the first
   * time.
   */
  Script* cLeftStartScript;
  
  /**
   * This script is triggered when playback moves from 1.0 to a number less than
   * 1.0.  This occurs when the rewind() function is invoked after playback has
   * run forward until the end.
   */
  Script* cLeftEndScript;
  
  /**
   * This script is triggered when playback moves to 1.0 from a number less than
   * 1.0.  This occurs when playback runs until the end.
   */
  Script* cReachedEndScript;
  
  /**
   * This script is triggered when playback moves to 0.0 from a number higher
   * than 0.0.  This occurs when the player is rewound to the beginning.
   */
  Script* cReachedStartScript;
  
  // Runtime data
  /**
   * True when the player is moving forward, False when the player is moving
   * backward (rewinding).
   */
  bool cForward;
  
  /**
   * True when the player is paused.
   */
  bool cPaused;
  
  /**
   * The position of the player in milliseconds.
   */
  unsigned int cPosition;
    
  public:
  
  /**
   * Constructor.  Sets a default length of one second.  The player is rewound
   * to 0.0 and does not begin playback until the play() function is invoked.
   */
  PlayerWithScripts();
  
  /**********************\
   * Implements IPlayer *
  \**********************/
  void addSequence(ISequence*);
  void removeSequence(ISequence*);
  void play();
  void rewind();
  void pause();
  void unpause();
  
  /******************************\
   * Implements IDynamicElement *
  \******************************/
  void update(int);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper* node);
  void save(DOMNodeWriter* node);
  void setEditingContext(BlockLocation*, IComponentContainer*, ICommandRegistry*);
  std::vector<IDynamicElement*> getPreLoopCommands();
};

#endif
