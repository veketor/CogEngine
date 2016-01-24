#include "Environment.h"
#include "InputAct.h"
#include "CogEngine.h"
#include "Utils.h"

namespace Cog {

	void Environment::Init() {
		virtualAspectRatio = ((float)ofGetWindowSize().x) / ofGetWindowSize().y;
		OnScreenSizeChanged(ofGetWindowSize().x, ofGetWindowSize().y);
		screenSizeChanged = false;
	}

	void Environment::Init(spt<ofxXml> xml) {
		this->Init();

		auto resCache = GETCOMPONENT(ResourceCache);
		Setting set = resCache->GetGlobalSettings("display");

		if (!set.name.empty()) {
			string aspectRatio = set.GetItemVal("aspect_ratio");

			if (!aspectRatio.empty()) {
				int dividerIndex = aspectRatio.find("/");

				if (dividerIndex == -1) {
					throw IllegalArgumentException(string_format("Error while parsing aspect ratio for environment; expected format xx/yy, found %s", aspectRatio.c_str()));
				}

				float firstVal = ofToFloat(aspectRatio.substr(0, dividerIndex));
				float secondVal = ofToFloat(aspectRatio.substr(dividerIndex + 1));

				this->SetVirtualAspectRatio(firstVal / secondVal);
				CogLogInfo("Environment", "Aspect ratio set as %s",aspectRatio.c_str());
			}
		}

	}

	void Environment::OnScreenSizeChanged(int newWidth, int newHeight) {
		screenSizeChanged = true;
		screenWidth = virtualWidth = ofGetWindowSize().x;
		screenHeight = virtualHeight = ofGetWindowSize().y;
		screenOrient = screenWidth > screenHeight ? ScreenOrient::LANDSCAPE : ScreenOrient::PORTRAIT;

		aspectRatio = ((float)screenWidth) / screenHeight;
		RecalcVirtualSize();
	}


	void Environment::OnKeyAction(int key, bool pressed) {

		if (pressed) {
			// key down

			// if pressed keys contains this key, remove it
			for (auto pKey : pressedKeys) {
				// key is already in the collection of pressed keys
				if (pKey->key == key) return;
			}

			pressedKeys.push_back(new InputAct(key));
		}
		else {
			// key up
			for (auto it = pressedKeys.begin(); it != pressedKeys.end(); ++it) {
				if ((*it)->key == key) {
					(*it)->ended = true;
					break;
				}
			}
		}
	}

	void Environment::OnMultiTouchButton(int x, int y, int button, bool pressed) {
		// user touches the screen with more fingers
		x -= ofGetCurrentViewport().x;
		y -= ofGetCurrentViewport().y;

		if (pressed) {
			pressedPoints.push_back(new InputAct(button, ofVec3f(x, y)));
		}
		else {
			for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
				if ((*it)->inputType == InputType::TOUCH && (*it)->touchId == button) {
					// change position
					(*it)->position = ofVec3f(x, y);
					(*it)->ended = true;
					return;
				}
			}
		}
	}

	void Environment::OnMultiTouchMotion(int x, int y, int button) {
		// user moves fingers

		x -= ofGetCurrentViewport().x;
		y -= ofGetCurrentViewport().y;

		for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
			if ((*it)->touchId == button && (*it)->inputType == InputType::TOUCH) {
				(*it)->position = ofVec3f(x, y);
			}
		}
	}

	void Environment::OnSingleTouchButton(int x, int y, int button, bool pressed) {
		// user touches the screen

		x -= ofGetCurrentViewport().x;
		y -= ofGetCurrentViewport().y;

		if (pressed) {
			pressedPoints.push_back(new InputAct(button, ofVec3f(x, y)));
		}
		else {
			for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
				if ((*it)->inputType == InputType::TOUCH) {
					// change position
					(*it)->position = ofVec3f(x, y);
					(*it)->ended = true;
					return;
				}
			}
		}
	}

	void Environment::OnSingleTouchMotion(int x, int y, int button) {

		x -= ofGetCurrentViewport().x;
		y -= ofGetCurrentViewport().y;

		// user moves finger
		for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
			if ((*it)->touchId == button && (*it)->inputType == InputType::TOUCH) {
				(*it)->position = ofVec3f(x, y);
			}
		}
	}

	void Environment::RemoveEndedProcesses() {
		screenSizeChanged = false;

		// remove released keys
		for (auto it = pressedKeys.begin(); it != pressedKeys.end();) {
			if ((*it)->ended) {
				InputAct* act = (*it);
				it = pressedKeys.erase(it);
				delete act;
				continue;
			}
			else if ((*it)->started) {
				(*it)->started = false;
			}
			// increment only if item hasn't been erased
			++it;
		}

		// remove released touches
		for (auto it = pressedPoints.begin(); it != pressedPoints.end();) {
			if ((*it)->ended) {
				InputAct* act = (*it);
				it = pressedPoints.erase(it);
				delete act;
				continue;
			}
			else if ((*it)->started) {
				(*it)->started = false;
			}
			// increment only if item hasn't been erased
			++it;
		}

		// remove ended sounds
		for (auto it = playedSounds.begin(); it != playedSounds.end();) {
			if ((*it)->Ended()) {
				it = playedSounds.erase(it);

				continue;
			}
			// increment only if item hasn't been erased
			++it;
		}

		// remove ended threads
		for (auto it = runningThreads.begin(); it != runningThreads.end();) {
			if (!(*it)->isThreadRunning()) {
				delete (*it);
				it = runningThreads.erase(it);
				continue;
			}
			// increment only if item hasn't been erased
			++it;
		}

	}

	void Environment::RunThread(ofThread* thread) {
		runningThreads.push_back(thread);
		thread->startThread();
	}

}// namespace