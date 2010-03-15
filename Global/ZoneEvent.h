#ifndef ZONE_EVENT_H
#define ZONE_EVENT_H

#include <iostream>

class IZone;

#include "Vertex.h"

class ZoneEvent {
  public:
  enum Type {

    /**
     * Something entered a zone.
     */
    ENTERED,
    
    /**
     * Something left a zone.
     */
    EXITED
  };

  private:
  /**
   * The zone on which this event occurred.
   */
  IZone* cZone;

  /**
   * The type of this event.
   */
  Type cType;

  /**
   * The time at which this event occurred within a known interval, from 0.0f
   * (start) until 1.0f (end).
   */
  float cTime;

  /**
   * The location at which the event occurred.
   */
  Vertex* cLocation;

  public:

  /**
   * Construct a new ZoneEvent.
   * 
   * @param IZone*  The Zone in which the event occurred.
   * @param Type  The type of event.
   * @param float  The time at which this event occurred within a known
   *        interval from 0.0f (start) until 1.0f (end).
   * @param Vertex*  The location of the event.
   */
  ZoneEvent(IZone*, Type, float, Vertex*);

  /**
   * Get the zone on which this event occurred.
   * 
   * @return  The zone on which this event occurred.
   */
  IZone* getZone();

  /**
   * Get the type of this event.
   * 
   * @return  The type of this event.
   */
  Type getType();

  /**
   * Get The time at which this event occurred within a known interval, from
   * 0.0f (start) until 1.0f (end).
   */
  float getTime();

  /**
   * Get the location at which this event occurred.
   */
  Vertex* getLocation();

  bool operator==(const ZoneEvent&) const;
  bool operator!=(const ZoneEvent&) const;
  bool operator<(const ZoneEvent&) const;
  bool operator>(const ZoneEvent&) const;
  bool operator>=(const ZoneEvent&) const;
  bool operator<=(const ZoneEvent&) const;
};

#endif
