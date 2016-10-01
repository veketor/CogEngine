#include "Scene.h"
#include "ofxCogEngine.h"
#include "NodeBuilder.h"
#include "SpriteSheet.h"
#include "Renderer.h"
#include "Stage.h"
#include "ComponentStorage.h"

namespace Cog {

	Scene::Scene(string name, bool isLazyLoaded)
		: name(name), lazyLoad(isLazyLoaded) {
		this->CreateSceneNode();
	}

	Scene::~Scene() {
		delete sceneNode;
	}


	void Scene::Init() {
		auto renderer = GETCOMPONENT(Renderer);
		auto cache = GETCOMPONENT(Resources);

		// notify renderer that we will use specific layers
		for (auto layer : layers) {
			auto spriteSheet = cache->GetSpriteSheet(layer.spriteSheetName);
			renderer->AddTileLayer(spriteSheet->GetSpriteImage(), layer.name, layer.bufferSize, layer.zIndex);
		}
	}

	void Scene::Dispose() {
		auto renderer = GETCOMPONENT(Renderer);
		auto cache = GETCOMPONENT(Resources);

		// remove layers from renderer
		for (auto layer : layers) {
			auto spriteSheet = cache->GetSpriteSheet(layer.spriteSheetName);
			renderer->RemoveTileLayer(layer.name);
		}
	}

	void Scene::Finish() {
		loaded = false;
		initialized = false;
		this->allBehaviors.clear();
		this->allNodes.clear();
		this->allNodes_id.clear();
		this->allNodes_tag.clear();

		// delete listeners but copy global listeners to the stage
		this->msgListeners.clear();
		this->msgListenerActions.clear();
		auto stage = GETCOMPONENT(Stage);
		stage->CopyGlobalListenersToScene(this);

		// delete scene node
		auto sceneNodePtr = this->sceneNode;
		this->sceneNode->RemoveFromParent(true);
		CreateSceneNode();

		// stop this scene
		sceneNode->SetRunningMode(RunningMode::DISABLED);
		// add new scene node into root object
		stage->GetRootObject()->AddChild(sceneNode);
	}

	void Scene::SetSceneSettings(Settings& settings) {

		this->settings = Settings();

		auto cache = GETCOMPONENT(Resources);
		// always merge from default settings that is stored in the resource cache
		auto defaultSettings = cache->GetDefaultSettings();
		this->settings.MergeSettings(defaultSettings);
		this->settings.MergeSettings(settings);
	}

	LayerEnt Scene::FindLayerSettings(string name) {
		for (LayerEnt& entity : this->layers) {
			if (entity.name.compare(name) == 0) return entity;
		}

		return LayerEnt();
	}

	void Scene::RegisterListener(StrId action, BaseComponent* listener) {

		if (msgListeners.count(action) == 0) {
			msgListeners[action] = vector <BaseComponent*>();
		}

		vector<BaseComponent*>& listeners = msgListeners[action];
		
		if (find(listeners.begin(), listeners.end(), listener) == listeners.end()) {
			listeners.push_back(listener);
		}
		
		auto& msgAction = msgListenerActions[listener->GetId()];

		if (find(msgAction.begin(), msgAction.end(), action) == msgAction.end()) {
			msgAction.push_back(action);
		}
	}

	bool Scene::UnregisterListener(StrId action, BaseComponent* listener) {
		if (msgListeners.count(action) != 0) {
			vector<BaseComponent*>& listeners = msgListeners[action];

			for (auto it = listeners.begin(); it != listeners.end(); ++it) {
				if ((*it)->GetId() == listener->GetId()) {
					listeners.erase(it);
					return true;
				}
			}
		}
		return false;
	}

	void Scene::UnregisterListener(BaseComponent* listener) {

		auto found = msgListenerActions.find(listener->GetId());

		if (found != msgListenerActions.end()) {

			vector<StrId> actions = found->second;

			// unregister all actions
			for (auto action : actions) {
				UnregisterListener(action, listener);
			}

			// remove from the second collection
			msgListenerActions.erase(listener->GetId());
		}
	}

	void Scene::SendMessage(Msg& msg) {

		COGLOGDEBUG("Messaging", "Message %s:%s", msg.GetAction().GetStringValue().c_str(), msg.GetContextNode()  ? msg.GetContextNode()->GetTag().c_str() : "");

		// skip if there is no such subscriber
		if (!IsRegisteredListener(msg.GetAction())) return;
		
		auto recType = msg.GetRecipientType();

		if (recType == MsgObjectType::SUBSCRIBERS) {
			SendDirectMessage(msg);
		}
		else if (recType == MsgObjectType::BEHAVIOR) {
			auto beh = FindBehaviorById(msg.GetRecipientId());
			if (beh != nullptr) beh->OnMessage(msg);
		}
		else if (recType == MsgObjectType::COMPONENT) {
			auto cmp = CogGetComponentStorage()->GetComponentById(msg.GetRecipientId());
			if (cmp != nullptr) cmp->OnMessage(msg);
		}
		else if (recType == MsgObjectType::NODE_CHILDREN ||
			recType == MsgObjectType::NODE_ACTUAL ||
			recType == MsgObjectType::NODE_ROOT ||
			recType == MsgObjectType::NODE_SCENE) {
			if (msg.GetRecipientId() == -1) {
				auto node = this->FindNodeById(msg.GetRecipientId());
				if (node != nullptr) SendTunnelingMessage(msg, node);
			}
			else {
				SendTunnelingMessage(msg, msg.GetContextNode());
			}
		}
		else {
			throw IllegalArgumentException("Message recipient mustn't be of type OTHER!");
		}
	}


	bool Scene::IsRegisteredListener(StrId action) const {
		return msgListeners.count(action) != 0;
	}

	bool Scene::IsRegisteredListener(int action, BaseComponent* listener) {
		if (msgListenerActions.count(listener->GetId()) == 0) return false;

		vector<StrId>& actions = msgListenerActions[listener->GetId()];
		return (std::find(actions.begin(), actions.end(), action) != actions.end());
	}

	Node* Scene::FindNodeById(int id) const {
		auto found = allNodes_id.find(id);
		if (found == allNodes_id.end()) return nullptr;
		else return found->second;
	}

	Behavior* Scene::FindBehaviorById(int id) const {
		for (auto it = allBehaviors.begin(); it != allBehaviors.end(); ++it) {
			if ((*it)->GetId() == id) return (*it);
		}
		return nullptr;
	}

	int Scene::GetNodesCountByTag(string tag) const {
		int counter = 0;
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) counter++;
		}
		return counter;
	}

	Node* Scene::FindNodeByTag(string tag) const {
		auto found = allNodes_tag.find(StrId(tag));
		if (found == allNodes_tag.end()) return nullptr;
		else return found->second;
	}

	void Scene::FindNodesByTag(char* tag, vector<Node*>& output) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) output.push_back(*it);
		}
	}

	int Scene::GetNodesCountBySecondaryId(int secondaryId) const {
		int counter = 0;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSecondaryId() == secondaryId) counter++;
		}
		return counter;
	}

	Node* Scene::FindNodeBySecondaryId(int secondaryId) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSecondaryId() == secondaryId) return (*it);
		}
		return nullptr;
	}

	void Scene::FindNodesBySecondaryId(int subtype, vector<Node*>&output) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSecondaryId() == subtype) output.push_back(*it);
		}
	}

	void Scene::FindNodesByGroup(StrId group, vector<Node*>& output) const {
		
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			Node* nd = (*it);

			if (nd->IsInGroup(group)) {
				output.push_back(nd);
			}
		}
	}

	void Scene::LoadFromXml(spt<ofxXml> xml) {

		COGLOGDEBUG("Scene", "Loading scene %s from xml", this->name.c_str());

		string type = xml->getAttributex("type", "scene");

		// load scene type
		if (type.compare("scene") == 0) this->sceneType = SceneType::SCENE;
		else if (type.compare("dialog") == 0) this->sceneType = SceneType::DIALOG;

		// load settings
		if (xml->pushTagIfExists("scene_settings")) {
			Settings set = Settings();
			set.LoadFromXml(xml);
			this->SetSceneSettings(set);
			xml->popTag();
		}
		
		// load layers
		if (xml->pushTagIfExists("scene_layers")) {
			int layersNum = xml->getNumTags("layer");

			for (int i = 0; i < layersNum; i++) {
				xml->pushTag("layer", i);
				LayerEnt layer = LayerEnt();
				layer.LoadFromXml(xml);
				layers.push_back(layer);
				xml->popTag();
			}

			xml->popTag();
		}

		int nodes = xml->getNumTags("node");
		NodeBuilder bld = NodeBuilder();

		// load nodes
		for (int i = 0; i < nodes; i++) {
			xml->pushTag("node", i);
			Node* node = bld.LoadNodeFromXml(xml, sceneNode, this);
			sceneNode->AddChild(node);
			xml->popTag();
		}

		loaded = true;
	}

	void Scene::CreateSceneNode() {
		sceneNode = new Node(NodeType::SCENE, 0, name);
		sceneNode->SetScene(this);
		// default mesh is a rectangle with the size of the window
		sceneNode->SetMesh(spt<Mesh>(new Rectangle((float)CogGetScreenWidth(), (float)CogGetScreenHeight())));
	}

	void Scene::SendMessageToBehaviors(Msg& msg, Node* actualNode) {
		for (auto it = actualNode->GetBehaviors().begin(); it != actualNode->GetBehaviors().end(); ++it) {
			Behavior* beh = (*it);
			if ((beh->GetComponentState() == ComponentState::ACTIVE_MESSAGES || beh->GetComponentState() == ComponentState::ACTIVE_ALL) &&
				(msg.GetSenderType() != MsgObjectType::BEHAVIOR || beh->GetId() != msg.GetSenderId())) { // don't send to the same behavior!
				if (IsRegisteredListener(msg.GetAction(), beh)) {
					COGLOGDEBUG("Messaging", "Sending msg  %s; to behavior %s with id %d", msg.GetAction().GetStringValue().c_str(), typeid(beh).name(), beh->GetId());
					beh->OnMessage(msg);
				}
			}
		}
	}

	void Scene::SendTunnelingMessageToChildren(Msg& msg, Node* actualNode) {
		for (auto it = actualNode->GetChildren().begin(); it != actualNode->GetChildren().end(); ++it) {
			SendTunnelingMessage(msg, (*it));
		}
	}

	void Scene::SendTunnelingMessage(Msg& msg, Node* actualNode) {

		if (msg.GetRecipientType() == MsgObjectType::NODE_ROOT) {
			msg.SetRecipientType(MsgObjectType::NODE_ACTUAL);
			// find root and call recursively
			Node* root = actualNode->GetRoot();
			if (root != nullptr) {
				// call this method again from the root
				if (msg.SendToWholeTree() && msg.GetTunnelingMode() == TunnelingMode::BUBBLING) SendTunnelingMessageToChildren(msg, root);
				SendMessageToBehaviors(msg, root);
				if (msg.SendToWholeTree() && msg.GetTunnelingMode() == TunnelingMode::TUNNELING) SendTunnelingMessageToChildren(msg, root);
			}
			return;
		}
		else if (msg.GetRecipientType() == MsgObjectType::NODE_SCENE) {
			msg.SetRecipientType(MsgObjectType::NODE_ACTUAL);
			// find scene and call recursively
			Node* scRoot = actualNode->GetSceneRoot();
			if (scRoot != nullptr) {
				if (msg.SendToWholeTree() && msg.GetTunnelingMode() == TunnelingMode::BUBBLING) SendTunnelingMessageToChildren(msg, scRoot);
				SendMessageToBehaviors(msg, scRoot);
				if (msg.SendToWholeTree() && msg.GetTunnelingMode() == TunnelingMode::TUNNELING) SendTunnelingMessageToChildren(msg, scRoot);
			}
			return;
		}

		if (msg.GetRecipientType() == MsgObjectType::NODE_ACTUAL) {
			// call children and itself
			if (msg.SendToWholeTree() && msg.GetTunnelingMode() == TunnelingMode::BUBBLING) SendTunnelingMessageToChildren(msg, actualNode);
			SendMessageToBehaviors(msg, actualNode);
			if (msg.SendToWholeTree() && msg.GetTunnelingMode() == TunnelingMode::TUNNELING) SendTunnelingMessageToChildren(msg, actualNode);
		}
		else if (msg.GetRecipientType() == MsgObjectType::NODE_CHILDREN) {
			msg.SetRecipientType(MsgObjectType::NODE_ACTUAL);
			// call children only
			SendTunnelingMessageToChildren(msg, actualNode);
		}
	}

	void Scene::SendDirectMessage(Msg& msg) {
		// find all listeners
		auto listeners = msgListeners.find(msg.GetAction());

		if (listeners != msgListeners.end()) {
			vector<BaseComponent*>& lsts = listeners->second;

			for (auto it = lsts.begin(); it != lsts.end(); ++it) {
				if (((*it)->GetComponentState() == ComponentState::ACTIVE_MESSAGES || (*it)->GetComponentState() == ComponentState::ACTIVE_ALL)) {
					(*it)->OnMessage(msg);
				}
			}
		}
	}

	bool Scene::AddNode(Node* node) {
		COGLOGDEBUG("Scene", "Adding node %s to scene %s", node->GetTag().c_str(), this->name.c_str());
	
		auto found = find(allNodes.begin(), allNodes.end(), node);
		if (found == allNodes.end()) {
			allNodes.push_back(node);
			allNodes_id[node->GetId()] = node;

			if (!node->GetTag().empty()) allNodes_tag[StrId(node->GetTag())] = node;
			
			node->SetScene(this);
			return true;
		}
		else return false;
	}

	void Scene::RemoveNode(Node* node) {
		COGLOGDEBUG("Scene", "Removing node %s from scene %s", node->GetTag().c_str(), this->name.c_str());
		auto found = find(allNodes.begin(), allNodes.end(), node);
		if (found != allNodes.end()) allNodes.erase(found);

		if (allNodes_id.count(node->GetId()) != 0) allNodes_id.erase(node->GetId());
		if (!node->GetTag().empty() && allNodes_tag.count(StrId(node->GetTag())) != 0) allNodes_tag.erase(StrId(node->GetTag()));
	}

	bool Scene::AddBehavior(Behavior* beh) {
		COGASSERT(beh->GetOwner() != nullptr, "Scene", "Behavior %s has no node assigned", typeid(beh).name());
		COGLOGDEBUG("Scene", "Adding behavior %s to node %s", typeid(beh).name(), beh->GetOwner()->GetTag().c_str());
		auto found = find(allBehaviors.begin(), allBehaviors.end(), beh);
		if (found == allBehaviors.end()) {
			allBehaviors.push_back(beh);

			return true;
		}
		else return false;
	}

	void Scene::RemoveBehavior(Behavior* beh) {
		COGASSERT(beh->GetOwner() != nullptr, "Scene", "Behavior %s has no node assigned", typeid(beh).name());
		COGLOGDEBUG("Scene", "Removing behavior %s from node %s", typeid(beh).name(), beh->GetOwner()->GetTag().c_str());

		auto found = find(allBehaviors.begin(), allBehaviors.end(), beh);
		if (found != allBehaviors.end()) allBehaviors.erase(found);

		UnregisterListener(beh);
	}

	void Scene::WriteInfo(int logLevel) {
		
		CogLogTree("INFO_SCENE", logLevel, "Scene %s info:", this->name.c_str());

#if DEBUG

		if(msgListeners.size() > 0) CogLogTree("INFO_SCENE", logLevel+1, "Message listeners: %d",msgListeners.size());

		for (auto it = msgListeners.begin(); it != msgListeners.end(); ++it) {
			StrId key = (*it).first;
			int listeners = (*it).second.size();

			if (listeners > 0) {
				string str = key.GetStringValue ()+":"+ofToString(listeners);
				CogLogTree("INFO_SCENE", logLevel+2, str.c_str());
			}
		}
#endif

		CogLogTree("INFO_SCENE", logLevel+1, "Nodes: %d",allNodes.size());
		CogLogTree("INFO_SCENE", logLevel+1, "Behaviors: %d", allBehaviors.size());

		CogLogTree("INFO_SCENE_NODES", logLevel+1, "Nodes::");

		this->sceneNode->WriteInfo(logLevel + 2);
	}


}// namespace