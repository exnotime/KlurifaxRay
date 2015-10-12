#include "EventObserver.h"
#include "EventManager.h"
EventObserver::EventObserver(){

}

EventObserver::EventObserver(EventManager* EventManagerPtr){
	m_EventManagerPtr = EventManagerPtr;
}

EventObserver::~EventObserver(){

}

Event* EventObserver::GetEvent(){
	Event* e = m_Events.front();
	m_Events.pop();
	return e;
}

bool EventObserver::HasEvents(){
	return !m_Events.empty();
}

void EventObserver::Subscripe(string EventName){
	m_EventManagerPtr->SubscribeToEvent(EventName, this);
}

void EventObserver::RecieveEvent(Event* e){
	m_Events.push(e);
}