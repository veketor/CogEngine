#pragma once

#include "Behavior.h"

#ifdef ANDROID
#include "ofxAndroidVibrator.h"
#endif

namespace Cog {

/**
* Behavior for hit testing
*/
class HitEvent : public Behavior{
protected:
	// if true, device will vibrate when object is hit
	bool vibrate;
	// indicator, if hit has started over this object
	bool hitStarted;
	// indicator, if hit has been lost
	bool hitLost;
	// id of button who started the hit
	int hitStartedTouchId;
	// id of handler behavior (-1 for all behaviors)
	int handlerBehId;

public:

	/**
	* Creates a new behavior for hit testing
	* @param handlerBehId id of handler behavior (set -1 for all handlers that listens HIT events)
	* @param vibrate if true, device will vibrate when object is hit
	*/
	HitEvent(int handlerBehId, bool vibrate) : 
		handlerBehId(handlerBehId), vibrate(vibrate), hitStarted(false), hitLost(false), hitStartedTouchId(-1){

	}

	void Init(){
		owner->SetState(StringHash(STATES_HITTABLE));
	}

	/**
	* Tests if the image has been hit
	* @param image image to test
	* @param testPos test position
	*/
	bool ImageHitTest(spt<ofImage> image, ofVec3f testPos){
		// move the test position into "local" coordinate space
		ofVec3f localPos = testPos + ofVec3f(image->getWidth() / 2, image->getHeight()/ 2);
		// test for location outside the image rectangle
		if (localPos.x < 0
			|| localPos.y < 0
			|| localPos.x >(float)image->getWidth()
			|| localPos.y >(float)image->getHeight())
			return false;

		ofColor col = image->getColor(localPos.x, localPos.y);
		// return a hit if the specified local alpha value is greater than half
		return col.a > 0x80;
	}

	virtual void Update(const uint64 delta, const uint64 absolute){

		if (owner->HasState(StringHash(STATES_HITTABLE))){

			// get inverse matrix
			ofMatrix4x4 inverse = owner->GetTransform().GetAbsMatrix().getInverse();

			if (owner->HasRenderType(RenderType::IMAGE)){
				spt<ofImage> hitImage = owner->GetShape<spt<Image>>()->GetImage();

				bool atLeastOneTouch = false;

				for (auto touch : CogGetPressedPoints()){

					// calculate vector in image space
					ofVec3f touchVector = touch.position;
					ofVec3f touchTrans = touchVector*inverse;

					if (ImageHitTest(hitImage, touchTrans)){
						// image has been hit
						if (touch.started){
#ifdef ANDROID
							if(vibrate) ofxAndroidVibrator::vibrate(50);
#endif
							atLeastOneTouch = true;
							hitStarted = true;
							hitStartedTouchId = touch.touchId;

							owner->SetState(StringHash(STATES_HIT));
							if (handlerBehId == -1) SendMessageNoBubbling(ACT_OBJECT_HIT_STARTED, 0, nullptr, owner);
							else SendDirectMessage(ACT_OBJECT_HIT_STARTED, 0, nullptr, owner, handlerBehId);
						}
						else if (touch.ended){
							

							owner->ResetState(StringHash(STATES_HIT));
							if (hitStarted){
								if (handlerBehId == -1) SendMessageNoBubbling(ACT_OBJECT_HIT_ENDED, 0, nullptr, owner);
								else SendDirectMessage(ACT_OBJECT_HIT_ENDED, 0, nullptr, owner, handlerBehId);
							}
							else{
								// hit has been lost
								hitLost = true;
								if (handlerBehId == -1) SendMessageNoBubbling(ACT_OBJECT_HIT_LOST, 0, nullptr, owner);
								else SendDirectMessage(ACT_OBJECT_HIT_LOST, 0, nullptr, owner, handlerBehId);
							}

							if (hitStartedTouchId == touch.touchId && hitStarted){
								hitStarted = false;
							}
						}
						else{
							atLeastOneTouch = true;
							if (!owner->HasState(StringHash(STATES_HIT))){
#ifdef ANDROID
								if(vibrate) ofxAndroidVibrator::vibrate(50);
#endif
								// touch hasn't started but this object hasn't been hit
								owner->SetState(StringHash(STATES_HIT));
							}
							// hit started and continues
							if (hitLost){
								hitLost = false;
								// hit started, lost and started again
								if (handlerBehId == -1) SendMessageNoBubbling(ACT_OBJECT_HIT_STARTED, 0, nullptr, owner);
								else SendDirectMessage(ACT_OBJECT_HIT_STARTED, 0, nullptr, owner, handlerBehId);
							}
							else{
								// hit started but not on first touch
								if (handlerBehId == -1) SendMessageNoBubbling(ACT_OBJECT_HIT_OVER, 0, nullptr, owner);
								else SendDirectMessage(ACT_OBJECT_HIT_OVER, 0, nullptr, owner, handlerBehId);
							}
						}
					}
					else{
						if (touch.ended && touch.touchId == hitStartedTouchId){
							// object isn't hit and this hit has already ended
							hitStarted = false;
						}
					}
				}

				if (!atLeastOneTouch){
					// object could lost its hit
					if (owner->HasState(StringHash(STATES_HIT))){
						owner->ResetState(StringHash(STATES_HIT));
						hitLost = true;
						if (handlerBehId == -1) SendMessageNoBubbling(ACT_OBJECT_HIT_LOST, 0, nullptr, owner);
						else SendDirectMessage(ACT_OBJECT_HIT_LOST, 0, nullptr, owner, handlerBehId);
					}
				}

			}
			else{
				// TODO
				/*for (auto touch : GetPressedPoints()){
					if (touch.started){
						ofVec3f touchVector = touch.position;
						ofVec3f touchTrans = inverse*(touchVector);
						ofVec3f size = owner->GetAttr<ofVec3f>(ATTR_SIZE);
						ofVec3f sizeTrans = owner->GetTransform().Scale*size;

						if (touchTrans.x+sizeTrans.x/2 <= sizeTrans.x && touchTrans.y+sizeTrans.y/2 <= sizeTrans.y &&
							touchTrans.x+sizeTrans.x/2 >= 0 && touchTrans.y+sizeTrans.y/2>=0){
							// is hit

						}
					}
				}*/
			}
		}
	}

};
}// namespace