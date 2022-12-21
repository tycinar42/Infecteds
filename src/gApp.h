/*
 * gApp.h
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#ifndef EXAMPLES_TESTEXAMPLE_GAPP_H_
#define EXAMPLES_TESTEXAMPLE_GAPP_H_

#include "gBaseApp.h"
#include "gFmodSound.h"
#include "gDatabase.h"


class gApp : public gBaseApp {
public:
	static const int SOUNDEFFECT_NEXT = 0, SOUNDEFFECT_CLICK = 1, SOUNDEFFECT_GUNSHOT = 2, SOUNDEFFECT_ENEMYDEATH = 3, SOUNDEFFECT_ENEMYFOOTSTEP = 4;
	static const int MUSIC_MENU = 0, MUSIC_GAME = 1;

	gApp();
	~gApp();

	void setup();
	void update();

	void playSound(int soundNo);
	bool isSoundPlaying(int soundNo);

	void playMusic(int musicNo);
	void stopMusic(int musicNo);
	void setMusicPaused(int musicNo, bool isPaused);
	bool isMusicPlaying(int musicNo);

	void saveSettings();

	bool soundon, musicon;
	int selectedcharacterno;

private:
	static const int soundeffectnum = 5;
	static const int musicnum = 2;

	gFmodSound soundeffect[soundeffectnum];
	gFmodSound music[musicnum];

	gDatabase settingsdb;
};

#endif /* EXAMPLES_TESTEXAMPLE_GAPP_H_ */
