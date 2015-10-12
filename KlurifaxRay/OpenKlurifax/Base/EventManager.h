#pragma once
#define MAX_EVENTS 512
#include "Event.h"
#include "EventObserver.h"
#include "Timer.h"
#include <map>
#include <vector>
#include <queue>
#include <functional>
using std::map;
using std::queue;
using std::vector;
using std::hash;

class EventManager{
public:
	EventManager();
	~EventManager();
	 EventObserver* CreateObserver();
	void PushEvent(Event* e);
	static EventManager* GetInstance();
	void SubscribeToEvent(string EventName, EventObserver* observer);
	void HandleEvents();
	void ClearEvents();
private:
	map<int, vector<EventObserver*>> m_SubscribedObservers;
	vector<Event*> m_EventQueue;
	vector<EventObserver*> m_Observers;
	hash<string> m_Hasher;
};