/*
 * MenuCanvas.cpp
 *
 *  Created on: 25 Kas 2021
 *      Author: user
 */

#include "MenuCanvas.h"
#include "GameCanvas.h"
#include "AboutCanvas.h"


MenuCanvas::MenuCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
}

MenuCanvas::~MenuCanvas() {
}

void MenuCanvas::setup() {
	background.loadImage("menu/arkaplan.jpg");
	mainmenu.loadImage("menu/anamenu.png");
	tick.loadImage("menu/tikler.png");
	ban.loadImage("menu/ban.png");
	glistenginelogo.loadImage("glistengine_logo.png");
	versiontextfont.loadFont("BlackOpsOne-Regular.ttf", 20);
	versiontext = "INFECTEDS v.1.0";
	menux = (getWidth() - mainmenu.getWidth()) / 2;
	menuy = (getHeight() - mainmenu.getHeight()) / 2;
	playbuttonleftx = menux + 88;
	playbuttonrightx = menux + 202;
	playbuttontopy = menuy + 295;
	playbuttonbottomy = menuy + 354;
//	selectedcharacterno = 0;
	versiontextx = 0;
	versiontexty = getHeight() - (versiontextfont.getSize() / 2);
	versiontextw = versiontextfont.getStringWidth(versiontext);
	direction = 1;
	root->setMusicPaused(gApp::MUSIC_MENU, !root->musicon);
}

void MenuCanvas::update() {
//	gLogi("MenuCanvas") << "update";
	versiontextx += direction;
	if(versiontextx <= 0 || versiontextx + versiontextw >= getWidth()) direction = -direction;
}

void MenuCanvas::draw() {
	background.draw(0, 0, getWidth(), getHeight());
	setColor(255, 255, 255, 32);
	glistenginelogo.draw(0, getHeight() - glistenginelogo.getHeight());
	setColor(255, 255, 255, 255);
	mainmenu.draw(menux, menuy);
	tick.drawSub(menux + 385 + tick.getHeight() / 2, menuy + 404 - tick.getHeight() * 3 / 2, tick.getWidth() / 2, tick.getHeight(),
			(tick.getWidth() / 2) * root->selectedcharacterno, 0, tick.getWidth() / 2, tick.getHeight());
	tick.drawSub(menux + 385 - tick.getWidth() / 2 - tick.getHeight() / 2, menuy + 404 - tick.getHeight() * 3 / 2, tick.getWidth() / 2, tick.getHeight(),
			(tick.getWidth() / 2) * (1 - root->selectedcharacterno), 0, tick.getWidth() / 2, tick.getHeight());
	if(!root->soundon) ban.draw(menux + 97 + (94 - ban.getWidth()) / 2, menuy + 167 + (23 - ban.getHeight()) / 2);
	if(!root->musicon) ban.draw(menux + 97 + (94 - ban.getWidth()) / 2, menuy + 141 + (23 - ban.getHeight()) / 2);
	setColor(144, 238, 144);
	versiontextfont.drawText(versiontext, versiontextx, versiontexty);
}

void MenuCanvas::keyPressed(int key) {
}

void MenuCanvas::keyReleased(int key) {
}

void MenuCanvas::mouseMoved(int x, int y) {
}

void MenuCanvas::mouseDragged(int x, int y, int button) {
}

void MenuCanvas::mousePressed(int x, int y, int button) {
}

void MenuCanvas::mouseReleased(int x, int y, int button) {
	if(x >= playbuttonleftx && x < playbuttonrightx && y >= playbuttontopy && y < playbuttonbottomy) {
//		gLogi("MenuCanvas");
		root->playSound(gApp::SOUNDEFFECT_NEXT);
		root->setMusicPaused(gApp::MUSIC_MENU, true);
		GameCanvas* cnv = new GameCanvas(root);
		cnv->setLevelNo(0);
		cnv->setCharacterNo(root->selectedcharacterno);
		root->getAppManager()->setCurrentCanvas(cnv);
	}

	if(x >= menux + 209 && x < menux + 383 && y >= menuy + 65 && y < menuy + 405) {
		root->playSound(gApp::SOUNDEFFECT_CLICK);
		root->selectedcharacterno = 0;
		root->saveSettings();
	}
	if(x >= menux + 389 && x < menux + 559 && y >= menuy + 65 && y < menuy + 405) {
		root->playSound(gApp::SOUNDEFFECT_CLICK);
		root->selectedcharacterno = 1;
		root->saveSettings();
	}

	if(x >= menux + 97 && x < menux + 191 && y >= menuy + 141 && y < menuy + 164) {
		root->playSound(gApp::SOUNDEFFECT_CLICK);
		root->musicon = !root->musicon;
		root->setMusicPaused(gApp::MUSIC_MENU, !root->musicon);
		root->saveSettings();
	}
	if(x >= menux + 97 && x < menux + 191 && y >= menuy + 167 && y < menuy + 190) {
		root->playSound(gApp::SOUNDEFFECT_CLICK);
		root->soundon = !root->soundon;
		root->saveSettings();
	}
	if(x >= menux + 97 && x < menux + 191 && y >= menuy + 194 && y < menuy + 216) {
		root->playSound(gApp::SOUNDEFFECT_CLICK);
		AboutCanvas* cnv = new AboutCanvas(root);
		root->getAppManager()->setCurrentCanvas(cnv);
	}
}

void MenuCanvas::mouseEntered() {
}

void MenuCanvas::mouseExited() {
}

void MenuCanvas::showNotify() {
}

void MenuCanvas::hideNotify() {
}
