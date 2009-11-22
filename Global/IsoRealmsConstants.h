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
#ifndef ISO_REALMS_CONSTANTS_H
#define ISO_REALMS_CONSTANTS_H

/**
 * This class contains constants that can be used throughout the Iso-Realms
 * library.
 */
class IsoRealmsConstants {
  public:

  // Dimensions
  static const double BLOCK_SIZE;
  static const double BLOCK_RADIUS;
  static const double BLOCK_HEIGHT;
  static const double LINE_WIDTH;  
  static const double STEP_HEIGHT;  // 1 = One block high
//   static const double CRAFT_RADIUS = 0.4;
//   static const double CRAFT_HEIGHT = 1.7;
//   static const double WALL_IMPACT_ABSORTION = 0.4;
// 
// /*  static const double GRAVITY_STRENGTH = 0.00000193;
//   static const double CRAFT_ACCELERATION = 0.0000015;*/
// //  static const double GRAVITY_STRENGTH = 0.0001;
//   static const double GRAVITY_STRENGTH = 0.0003;
//   static const double CRAFT_ACCELERATION = 0.00003;

  /**
   * When the craft hits a trampoline, we simulate a fixed number of ticks
   * within which the player can adjust direction of movement.
   */
//   static const float SIMULATED_TRAMPOLINE_TICKS = 10.0f; // TODO: Research and correct

  // Directions
//   static const int NO_DIRECTION = 0;
//   static const int NORTHWEST = 1;
//   static const int NORTHEAST = 2;
//   static const int SOUTHWEST = 3;
//   static const int SOUTHEAST = 4;
//   static const int NORTH = 5;
//   static const int SOUTH = 6;
//   static const int EAST = 7;
//   static const int WEST = 8;
// 
//   // Viewpoints
//   static const int MAP = 9;
// 
//   // Content types
//   static const int NORMAL = 0;
//   static const int SPECIAL = 1;
//   static const int JEWEL = 2;
//   static const int ENEMY = 3;
//   static const int LIFT = 4;

/*
  static const int LIFT_CIRCLE_BOTH = 0;
  static const int LIFT_CIRCLE_LEFT = 1;
  static const int LIFT_CIRCLE_RIGHT = 2;
  static const int LIFT_CIRCLE_NONE = 3;
  static const int LIFT_SQUARE_BOTH = 4;
  static const int LIFT_SQUARE_LEFT = 5;
  static const int LIFT_SQUARE_RIGHT = 6;
  static const int LIFT_SQUARE_NONE = 7;
  static const int LIFT_DIAMOND_BOTH = 8;
  static const int LIFT_DIAMOND_LEFT = 9;
  static const int LIFT_DIAMOND_RIGHT = 10;
  static const int LIFT_DIAMOND_NONE = 11;

  static const int JEWEL_COLLECT = 26;
  static const int LIFT_CLICK_1 = 27;
  static const int LIFT_CLICK_2 = 28;
  static const int LIFT_CLICK_3 = 29;
  static const int TRAMPOLINE_BOUNCE = 30;
  static const int IMPACT_SOFT = 31;
  static const int IMPACT_MEDIUM = 32;
  static const int IMPACT_HARD = 33;
  static const int RESPAWN_HARD = 34;
  static const int LIFT_CLICKS_COUNT = 3;
  static const int LIFT_CLICKS_START = LIFT_CLICK_1;

  static const int MUSIC = 40;

  static const float TRANSITION_SPEED = 0.0025;
  static const float LIFT_SPEED = 0.001;
    
  // Events
  static const int EVENT_HIT_NORTH_WALL = 1;
  static const int EVENT_HIT_SOUTH_WALL = 2;
  static const int EVENT_HIT_EAST_WALL = 3;
  static const int EVENT_HIT_WEST_WALL = 4;
  static const int EVENT_HIT_FLOOR = 5;
  static const int EVENT_LEAVE_FLOOR = 6;
  static const int EVENT_LEAVE_WALL = 7;

  // Orientations
  static const bool HORIZONTAL = true;
  static const bool VERTICAL = false;  

  // Block constants
  static const bool FLOOR = true;
  static const bool CEILING = false;*/
};

#endif
