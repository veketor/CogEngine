#pragma once

#include "ofxSmartPointer.h"

class AIState{
protected:
	int agentOnTurn = 0;

public:
	int GetAgentOnTurn() const {
		return agentOnTurn;
	}

	void SetAgentOnTurn(int agentOnTurn) {
		this->agentOnTurn = agentOnTurn;
	}
};