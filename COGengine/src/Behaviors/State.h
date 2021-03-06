#pragma once

#include "Behavior.h"

namespace Cog {

	class StateMachine;

	/**
	* Base behavior for all behavioral states of the finite state machine
	*/
	class State : public Behavior {
	private:
		StrId state = 0;
		StateMachine* parent = nullptr;

	public:

		State() {

		}

		State(StrId state) : state(state){

		}

		/**
		* Gets state identifier
		*/
		StrId GetState() const {
			return state;
		}

		/**
		* Sets the state identifier
		*/
		void SetState(StrId state) {
			this->state = state;
		}

		StateMachine* GetParent() const {
			return parent;
		}

		void SetParent(StateMachine* parent) {
			this->parent = parent;
		}

	};

}// namespace