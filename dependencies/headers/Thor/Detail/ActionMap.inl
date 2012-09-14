/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2012 Jan Haller
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////

namespace thor
{

template <typename ActionIdentifier>
ActionMap<ActionIdentifier>::ActionMap(sf::Window& window)
: mActionMap()
, mEventBuffer(window)
{
}

template <typename ActionIdentifier>
void ActionMap<ActionIdentifier>::Update()
{
	mEventBuffer.ClearEvents();
	mEventBuffer.PollEvents();
}

template <typename ActionIdentifier>
void ActionMap<ActionIdentifier>::PushEvent(const sf::Event& event)
{
	mEventBuffer.PushEvent(event);
}

template <typename ActionIdentifier>
void ActionMap<ActionIdentifier>::ClearEvents()
{
	mEventBuffer.ClearEvents();
}

template <typename ActionIdentifier>
Action& ActionMap<ActionIdentifier>::operator[] (const ActionIdentifier& id)
{
	return mActionMap[id];
}

template <typename ActionIdentifier>
void ActionMap<ActionIdentifier>::RemoveAction(const ActionIdentifier& id)
{
	mActionMap.erase(id);
}

template <typename ActionIdentifier>
void ActionMap<ActionIdentifier>::ClearActions()
{
	mActionMap.clear();
}

template <typename ActionIdentifier>
bool ActionMap<ActionIdentifier>::IsActive(const ActionIdentifier& id) const
{
	typename Map::const_iterator action = mActionMap.find(id);
	if (action == mActionMap.end())
		return false;

	return action->second.IsActive(mEventBuffer);
}

template <typename ActionIdentifier>
void ActionMap<ActionIdentifier>::InvokeCallbacks(CallbackSystem& system) const
{
	sf::Window& window = mEventBuffer.GetWindow();

	THOR_CONST_FOREACH(typename Map, mActionMap, actionItr)
	{
		// Check if current action is active, collect additional information in result
		detail::ActionResult result = {};
		if (!actionItr->second.IsActive(mEventBuffer, result))
			continue;

		// If at least one realtime constellation triggers this action, invoke callback for it
		if (result.nbRealtimeTriggers > 0)
			system.TriggerEvent( ActionContext<ActionIdentifier>(window, NULL, actionItr->first) );

		// Additionally, invoke callback once for every sf::Event
		THOR_CONST_FOREACH(std::vector<sf::Event>, result.eventContainer, eventItr)
			system.TriggerEvent( ActionContext<ActionIdentifier>(window, &*eventItr, actionItr->first) );
	}
}

} // namespace thor
