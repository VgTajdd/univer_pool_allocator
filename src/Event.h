#pragma once

class EventPimpl;

class Event
{
private:
	bool m_handled = false;
	const EventPimpl* m_parent;
	bool m_isMouseEvent = false;
	bool m_isKeyboardEvent = false;
};
