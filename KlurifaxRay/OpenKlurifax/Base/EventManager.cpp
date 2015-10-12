#include "EventManager.h"

EventManager::EventManager(){
}

EventManager::~EventManager(){
	//Delete observers
	for (vector<EventObserver*>::iterator it = m_Observers.begin(); it != m_Observers.end(); it++){
		delete *it;
	}
}

EventManager* EventManager::GetInstance(){
	static EventManager m_Manager;
	return &m_Manager;
}

EventObserver* EventManager::CreateObserver(){
	EventObserver* observer = new EventObserver(this);
	m_Observers.push_back(observer);
	return observer;
}
void EventManager::SubscribeToEvent(string eventName, EventObserver* observer){
	m_SubscribedObservers[static_cast<int>(m_Hasher(eventName))].push_back(observer);
}

void EventManager::PushEvent(Event* e){
	m_EventQueue.push_back(e);
}

void EventManager::HandleEvents(){
	Event* e;
	//for each event
	for (int i = 0; i < m_EventQueue.size(); i++){
		e = m_EventQueue.at(i);
		//for each subscribed observer
		vector<EventObserver*> observers = m_SubscribedObservers[e->HashedName];
		for (int k = 0; k < observers.size(); k++){
			observers.at(k)->RecieveEvent(e);
		}
	}
}

void EventManager::ClearEvents(){
	m_EventQueue.clear();
}