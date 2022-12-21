/*
 * gApp.cpp
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#include "gApp.h"
#include "MenuCanvas.h"


gApp::gApp() {
}

gApp::~gApp() {
	for(int i = 0; i < soundeffectnum; i++) soundeffect[i].close();
	for(int i = 0; i < musicnum; i++) music[i].close();
	settingsdb.close();
}

void gApp::setup() {
	setFramerate(64);

	settingsdb.loadDatabase("settings.db");
	settingsdb.execute("SELECT * FROM SETTINGS");
	std::string settingsdata = "";
	while(settingsdb.hasSelectData()) {
		settingsdata = settingsdb.getSelectData();
	}
	std::vector<std::string> data = gSplitString(settingsdata, "|");

	musicon = gToInt(data[1]);
	soundon = gToInt(data[2]);
	selectedcharacterno = gToInt(data[3]);


	soundeffect[SOUNDEFFECT_NEXT].loadSound("sound_next5.wav");
	soundeffect[SOUNDEFFECT_CLICK].loadSound("sound_click2.wav");
	soundeffect[SOUNDEFFECT_GUNSHOT].loadSound("gun_shot_2.wav");
	soundeffect[SOUNDEFFECT_ENEMYDEATH].loadSound("monster_death.wav");
	soundeffect[SOUNDEFFECT_ENEMYFOOTSTEP].loadSound("monster_footsteps.wav");
	music[MUSIC_MENU].loadSound("birthofahero.mp3");
	music[MUSIC_GAME].loadSound("savage-law.mp3");
	music[MUSIC_MENU].setLoopType(gBaseSound::LOOPTYPE_NORMAL);
	music[MUSIC_MENU].play();
	music[MUSIC_MENU].setPaused(!musicon);
	music[MUSIC_GAME].setLoopType(gBaseSound::LOOPTYPE_NORMAL);
	music[MUSIC_GAME].play();
	music[MUSIC_GAME].setPaused(true);


	MenuCanvas *cnv = new MenuCanvas(this);
	appmanager->setCurrentCanvas(cnv);

}

void gApp::update() {
}

void gApp::playSound(int soundNo) {
	if(!soundon) return;
	soundeffect[soundNo].play();
}

bool gApp::isSoundPlaying(int soundNo) {
	return soundeffect[soundNo].isPlaying();
}

void gApp::playMusic(int musicNo) {
	music[musicNo].play();
}


void gApp::stopMusic(int musicNo) {
	music[musicNo].stop();
}


void gApp::setMusicPaused(int musicNo, bool isPaused) {
	music[musicNo].setPaused(isPaused);
}

bool gApp::isMusicPlaying(int musicNo) {
	return music[musicNo].isPlaying();
}

void gApp::saveSettings() {
	settingsdb.execute("UPDATE SETTINGS SET musicon=" + gToStr(musicon) + ", soundon=" + gToStr(soundon) + ", choosecharacter=" + gToStr(selectedcharacterno));
}
