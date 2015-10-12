#pragma once
#include <string>
#include <functional>
using std::string;
using std::hash;

struct EventArg{
	enum ARG_TYPE
	{
		TYPE_FLOAT,
		TYPE_INT,
		TYPE_BOOL
	};

	ARG_TYPE Type;

	union{
		float AsFloat;
		int AsInterger;
		bool AsBool;
	}ArgData;
};
#define MAX_ARGS 24
struct Event
{
	string EventName;
	int HashedName;
	unsigned int NumArgs = 0;
	EventArg Args[MAX_ARGS];

	void PushArg(int val){
		Args[NumArgs].ArgData.AsInterger = val;
		Args[NumArgs].Type = EventArg::TYPE_INT;
		NumArgs++;
	}
	void PushArg(float val){
		Args[NumArgs].ArgData.AsFloat = val;
		Args[NumArgs].Type = EventArg::TYPE_FLOAT;
		NumArgs++;
	}
	void PushArg(bool val){
		Args[NumArgs].ArgData.AsBool = val;
		Args[NumArgs].Type = EventArg::TYPE_BOOL;
		NumArgs++;
	}
	int HashName(){
		hash<string> hash;
		HashedName = static_cast<int>(hash(EventName));
		return HashedName;
	}

	Event(string name){
		EventName = name;
		HashName();
	}
};
