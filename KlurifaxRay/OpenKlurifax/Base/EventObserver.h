#pragma once
#include "Event.h"
#include <string>
#include <queue>
using std::string;
using std::queue;

class EventManager;
class EventObserver{
public:
	EventObserver();
	EventObserver(EventManager* EventManagerPtr);
	~EventObserver();
	 Event* GetEvent();
	 bool HasEvents();
	 void Subscripe(string EventName);
	 void RecieveEvent(Event* e);
private:
	queue<Event*> m_Events;
	EventManager* m_EventManagerPtr;
};