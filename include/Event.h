#pragma once

class EventPimpl;

class Event
{
public:
	bool isHandled() const { return m_handled; }
	void setIsMouseEvent( bool value ) { m_isMouseEvent = value; }
	void setIsKeyboardEvent( bool value ) { m_isKeyboardEvent = value; }
private:
	bool m_handled = false;
	const EventPimpl* m_parent;
	bool m_isMouseEvent = false;
	bool m_isKeyboardEvent = false;
};

class Event2 : public Event
{
public:
	Event2( bool isMouseEvent, bool isKeyboardEvent = false )
	{
		setIsMouseEvent( isMouseEvent );
		setIsKeyboardEvent( isKeyboardEvent );
	}
};
