#pragma once


namespace Cog {

	// ==========================================================================
	// This section contains actions, attribute names and state names
	// that are used as StrId objects in the engine
	
	/** Action events */
	#define ACT_BEHAVIOR_ADDED "BEHAVIOR_ADDED"
	#define ACT_BEHAVIOR_REMOVED "BEHAVIOR_REMOVED"
	#define ACT_OBJECT_CREATED "OBJECT_CREATED"
	#define ACT_OBJECT_REMOVED "OBJECT_REMOVED"
	#define ACT_USER_INPUT "USER_INPUT"
	#define ACT_COLLISION_OCURRED "COLLISION_OCURRED"
	#define ACT_NODE_DESTROYED "NODE_DESTROYED"
	#define ACT_ATTRIBUTE_CHANGED "ATTRIBUTE_CHANGED"
	#define ACT_OBJECT_HIT_STARTED "OBJECT_HIT_STARTED"
	#define ACT_OBJECT_HIT_OVER "OBJECT_HIT_OVER"
	#define ACT_OBJECT_HIT_LOST "OBJECT_HIT_LOST"
	#define ACT_OBJECT_HIT_ENDED "OBJECT_HIT_ENDED"
	#define ACT_APP_FINISHED "APP_FINISHED"
	#define ACT_TWEEN_ENDED "TWEEN_ENDED"
	#define ACT_FLIP_ENDED "FLIP_ENDED"
	#define ACT_DELAY_ACTION "DELAY_ACTION"
	#define ACT_SCENE_SWITCHED "SCENE_SWITCHED"
	#define ACT_PROCESS_ENDED "PROCESS_ENDED"
	#define ACT_BEHAVIOR_FINISHED "BEHAVIOR_FINISHED"
	#define ACT_STATE_CHANGED "STATE_CHANGED"
	#define ACT_TRANSFORM_ENDED  "TRANSFORM_ENDED"
    #define ACT_KEY_PRESSED "KEY_PRESSED"
    #define ACT_SCREEN_CHANGED "SCREEN_CHANGED"
	#define ACT_NET_MESSAGE_RECEIVED "NET_MESSAGE_RECEIVED"
	#define ACT_NET_CLIENT_CONNECTED "NET_CLIENT_CONNECTED"
	#define ACT_NET_CALLBACK_RECEIVED "NET_CALLBACK_RECEIVED"
	#define ACT_DELTA_MESSAGE_RECEIVED "DELTA_MESSAGE_RECEIVED"
	#define ACT_OBJECT_SELECTED "OBJECT_SELECTED"
	#define ACT_BUTTON_CLICKED "BUTTON_CLICKED"
	#define ACT_ATTR_CHANGED "ATTR_CHANGED"
	
	/** Attribute keys */
	#define ATTR_MOVEMENT "MOVEMENT"
	#define ATTR_STEERING_BEH_SEEK_DEST "STEERING_BEH_SEEK_DEST"
	#define ATTR_STEERING_BEH_WANDER "STEERING_BEH_WANDER"
	#define ATTR_ACTIONS "ACTIONS"
	#define ATTR_ALLOWED_ACTIONS "ALLOWED_ACTIONS"
	#define ATTR_BOUNDS "BOUNDS"
	#define ATTR_SIZE "SIZE"
	#define ATTR_POLYGON "POLYGON"
	#define ATTR_FRAME "FRAME"
	#define ATTR_COLLISIONS "COLLISIONS"
	#define ATTR_IMGBOUNDS "IMGBOUNDS"
	#define ATTR_SLIDER_VALUE "SLIDER_VALUE"

	/** States*/
	#define STATES_HITTABLE "HITTABLE"
	#define STATES_TOLEFT "TO_LEFT"
	#define STATES_TORIGHT "TO_RIGHT"
	#define STATES_HIT "HIT"
	#define STATES_COLLID_SOURCE "COLLID_SOURCE"
	#define STATES_COLLID_TARGET "COLLID_TARGET"
    #define STATES_DISABLED "DISABLED"
	#define STATES_ENABLED "ENABLED"
	#define STATES_LOCKED "LOCKED"
    #define STATES_SELECTED "SELECTED"

	/** Net messages */
	#define NET_MSG_DELTA_UPDATE "DELTA_UPDATE"


}// namespace