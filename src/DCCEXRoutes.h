/* -*- c++ -*-
 *
 * DCCEXProtocol
 *
 * This package implements a DCCEX native protocol connection,
 * allow a device to communicate with a DCC-EX EX-CommandStation.
 *
 * Copyright © 2023 Peter Akers
 * Copyright © 2023 Peter Cole
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike
 * 4.0 International License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to
 * Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
 *
 * Attribution — You must give appropriate credit, provide a link to the
 * license, and indicate if changes were made. You may do so in any
 * reasonable manner, but not in any way that suggests the licensor
 * endorses you or your use.
 *
 * ShareAlike — If you remix, transform, or build upon the material, you
 * must distribute your contributions under the same license as the
 * original.
 *
 * All other rights reserved.
 *
 */

#ifndef DCCEXROUTES_H
#define DCCEXROUTES_H

#include <Arduino.h>

enum RouteType {
    RouteTypeRoute = 'R',
    RouteTypeAutomation = 'A',
};

class Route {
public:
  /// @brief Constructor
  /// @param id Route ID
  Route(int id);
  
  /// @brief Get route ID
  /// @return 
  int getId();

  /// @brief Set route name
  /// @param name 
  void setName(char* name);

  /// @brief Get route name
  /// @return 
  char* getName();

  /// @brief Set route type (A automation, R route)
  /// @param type RouteType - RouteTypeAutomation or RouteTypeRoute
  /// @return 
  void setType(RouteType type);

  /// @brief Get route type (A automation, R route)
  /// @return 
  RouteType getType();

  /// @brief Get count of routes
  /// @return 
  int getCount();

  /// @brief Get first Route object
  /// @return 
  static Route* getFirst();

  /// @brief Get next Route object
  /// @return 
  Route* getNext();

  /// @brief Get route object by its ID
  /// @return 
  Route* getById(int id);
  
private:
  int _id;
  char* _name;
  char _type;
  static Route* _first;
  Route* _next;
  int _count=0;

};

#endif