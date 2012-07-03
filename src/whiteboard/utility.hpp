/* $Id$ */
/*
 Copyright (C) 2010 - 2012 by Gabriel Morin <gabrielmorin (at) gmail (dot) com>
 Part of the Battle for Wesnoth Project http://www.wesnoth.org

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY.

 See the COPYING file for more details.
 */

/**
 * @file
 */

#ifndef WB_UTILITY_HPP_
#define WB_UTILITY_HPP_

#include <vector>

#include <boost/function.hpp>

#include "typedefs.hpp"

class unit;
class team;

namespace wb {

/// @return The current viewing team's index
size_t viewer_team();

/// @return The current viewing side's number (i.e. team index + 1)
int viewer_side();

/// @return The side_actions instance belonging to the current viewing team
side_actions_ptr viewer_actions();

/// @return The side_actions instance belonging to the current playing team
side_actions_ptr current_side_actions();

/**
 * For a given leader on a keep, find another leader on another keep in the same castle.
 * @retval NULL if no such leader has been found
 */
unit const* find_backup_leader(unit const& leader);

/**
 * @return a leader from the specified team who can recruit on the specified hex
 * @retval NULL if no such leader has been found
 */
unit* find_recruiter(size_t team_index, map_location const&);

/// Applies the future unit map and @return a pointer to the unit at hex
/// @retval NULL if none is visible to the specified viewer side
unit* future_visible_unit(map_location hex, int viewer_side = wb::viewer_side());

/// Applies the future unit map and @return a pointer to the unit at hex
/// @retval NULL if none is visible to the specified viewer side
/// @param on_side Only search for units of this side.
unit* future_visible_unit(int on_side, map_location hex, int viewer_side = wb::viewer_side());

/// Computes the MP cost for u to travel path
int path_cost(std::vector<map_location> const& path, unit const& u);

struct temporary_unit_hider {
	temporary_unit_hider(unit& u);
	~temporary_unit_hider();
	unit* const unit_;
};

/**
  * Finalizer class to help with exception safety
  * sets variable to value on destruction
  */
template <typename T>
class variable_finalizer
{
public:
	variable_finalizer(T & variable, T value):
		variable_(&variable),
		value_(value)
	{}
	~variable_finalizer()
	{
		if(variable_ != NULL) {
			*variable_ = value_;
		}
	}
	/** Stop tracking the variable, i.e. this object won't do anything on destruction. */
	void clear()
	{
		variable_ = NULL;
	}
private:
	T * variable_;
	T value_;
};

void ghost_owner_unit(unit* unit);
void unghost_owner_unit(unit* unit);

/** Return whether the whiteboard has actions. */
bool has_actions();

/** Returns whether a given team's plan is visible. */
bool team_has_visible_plan(size_t,team&);

/** Always returns true. */
inline bool default_team_filter(size_t,team&){ return true; }

/**
 * Callable object class to filter teams.
 *
 * The first argument is the relative turn number.
 * The second argument is the team to consider.
 */
typedef boost::function<bool(size_t,team&)> team_filter;

/**
 * Apply a function to all the actions of the whiteboard.
 *
 * The actions are processed chronologically.
 * The second parameter is a @ref team_filter, it is called for each team, if it returns false, the actions of this team won't be processed.
 * The third parameter is also a @ref team_filter, it is called for each team after visiting its action, if it returns false, for_each_action returns.
 *
 * @param function the function to execute.
 * @param pre_team_filter select whether a team is visited (default to @ref team_has_visible_plan).
 * @param post_team_filter stop condition, called after visiting each team (default to @ref default_team_filter).
 */
void for_each_action(boost::function<void(action_ptr)> function,
                     team_filter pre_team_filter = team_has_visible_plan,
                     team_filter post_team_filter = default_team_filter);

} //end namespace wb

#endif /* WB_UTILITY_HPP_ */
