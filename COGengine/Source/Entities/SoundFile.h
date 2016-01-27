#pragma once

#include "ofxCogCommon.h"

namespace Cog {

	/**
	* Entity for sound attachment
	*/
	class Sound {
	private:
		ofSoundPlayer soundPlayer;
	public:
		string filename;
		bool started;
		float volume;

		Sound(string filename) : filename(filename) {
			started = false;
			// music must be streamed; otherwise it won't work on android
			soundPlayer.load(filename, true);
			SetVolume(1.0f);
		}

		bool Ended() {
			return started && !soundPlayer.isPlaying();
		}

		void Play() {
			soundPlayer.play();
			started = true;
		}

		void Stop() {
			soundPlayer.stop();
		}

		void SetLoop(bool loop) {
			soundPlayer.setLoop(loop);
		}

		void SetVolume(float volume) {
			this->volume = volume;
			soundPlayer.setVolume(volume);
		}

		float GetVolume() {
			return volume;
		}
	};

}// namespace


