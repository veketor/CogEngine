#pragma once

#include "MEnums.h"
#include "EnFlags.h"



class GMsg;
class GNode;

/**
* Configuration class that holds information how the message should bubble through the scene tre
*/
class BubblingType{

public:

	/**
	* Creates a new bubbling configuration
	* @param scopeType type of object bubbling should start from
	* @param deep if true, message will be sent to near nodes as well
	* @param bubbleDown if true, bubbling will go from top to bottom; otherwise it will go from bottom 
	*/
	BubblingType(ScopeType scopeType, bool deep, bool bubbleDown) : scope(scopeType), deep(deep), bubbleDown(bubbleDown){

	}
	// scope object
	ScopeType scope;
	// if true, message will be sent to near nodes as well
	bool deep;
	// if true, bubbling will go from top to bottom; otherwise it will go from bottom 
	bool bubbleDown;
};

/**
* Message that is used to send information between nodes
* and their behaviors
*/
class GMsg{
private:
	// identifier counter
	static int idCounter;
	// id of this message
	int id;
	// id of action that has been invoked
	int action;
	// id of subaction that has been invoked
	int subAction;
	// bubbling settings
	BubblingType bubblingType;
	// data payload
	void* data;
	// id of behavior that sent this message
	int behaviorId;
	// source node that is connected with this message
	GNode* sourceObj;

public:

	/**
	* Creates a new message
	* @param bubblingType bubbling settings
	* @param action id of action that has been invoked
	* @param subAction id of subaction that has been invoked
	* @param behaviorId id of behavior that sent this message
	* @param sourceObj source object
	* @param data payload
	*/
	GMsg(BubblingType bubblingType, int action, int subAction, int behaviorId, GNode* sourceObj, void* data);

	~GMsg(){

	}

	/**
	* Gets id of action; see Actions for common action ids
	*/
	const int GetAction() const{
		return action;
	}

	/**
	* Gets id of subaction; see Actions for common action ids
	*/
	const int GetSubaction() const{
		return subAction;
	}

	/**
	* Gets bubbling configuration
	*/
	BubblingType& GetBubblingType(){
		return bubblingType;
	}

	/**
	* Gets data payload
	*/
	const void* GetData() const{
		return data;
	}
	
	/**
	* Gets data payload
	*/
	void* GetData(){
		return data;
	}

	/**
	* Gets identifier of the behavior that sent this message
	*/
	const int GetBehaviorId() const{
		return behaviorId;
	}

	/**
	* Gets source object that is connected with this message
	*/
	GNode* GetSourceObject(){
		return sourceObj;
	}

};

