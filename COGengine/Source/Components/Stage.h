#pragma once

#include "Component.h"

namespace Cog {

	class Node;
	class Behavior;
	class Scene;
	class Tween;
	enum class TweenDirection;

	/**
	* Node context
	*/
	class Stage : public Component{
	
		OBJECT(Stage)
	
	private:
		// message listeners
		map<StringHash, vector<MsgListener*>> msgListeners;

		// actual scene
		Scene* actualScene = nullptr;

		Scene* loadingScene = nullptr;
		// list of all scenes
		vector<Scene*> scenes = vector<Scene*>();

		// root object, should be created only once
		Node* rootObject = nullptr;

		stack<Scene*> sceneStack = stack<Scene*>();

	public:

		void Init();

		void OnMessage(Msg& msg);

		/**
		* Gets the root object
		*/
		Node* GetRootObject() {
			return rootObject;
		}

		/**
		* Gets actual scene
		*/
		Scene* GetActualScene() {
			return actualScene;
		}

		/**
		* Adds a new scene
		* @param setAsActual if true, scene will be set as the actual scene
		*/
		void AddScene(Scene* scene, bool setAsActual);

		/**
		* Registers global behavior listener for selected action
		* @param action action to register
		* @param listener listener that will be called when selected action is invoked
		*/
		void RegisterGlobalListener(StringHash action, MsgListener* listener);

		/**
		* Unregisters global message listener for selected action
		* @return true if listener has been found and erased
		*/
		bool UnregisterGlobalListener(StringHash action, MsgListener* listener);

		/**
		* Finds scene by its name
		*/
		Scene* FindSceneByName(string name) const;
	
		void SwitchToScene(Scene* scene, TweenDirection tweenDir);

		bool SwitchBackToScene(TweenDirection tweenDir);

		/**
		* Loads scene from xml
		*/
		void LoadScenesFromXml(spt<ofxXml> xml);

		void WriteInfo(int logLevel = 0);

	};

}// namespace