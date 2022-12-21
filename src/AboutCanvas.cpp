/*
 * AboutCanvas.cpp
 *
 *  Created on: 9 Ara 2021
 *      Author: okand
 */

#include "AboutCanvas.h"
#include "MenuCanvas.h"

AboutCanvas::AboutCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
}

AboutCanvas::~AboutCanvas() {

}

void AboutCanvas::setup() {
	background.loadImage("menu/arkaplan.jpg");
	textfont.loadFont("FreeSans.ttf", 20);
	text[0] = "GELISTIRENLER";
	text[1] = "Ramazan Noyan Culum";
	text[2] = "Oguzhan Altintas";
	text[3] = "Emircan Celik";
	text[4] = "Taha Yasin Cinar";
	text[5] = "Halit Dagacan";
	text[6] = "Okan Demirtas";
	text[7] = "Goktug Kucukdereli";
	text[8] = "Melisa Ronay Seven";
	text[9] = " ";
	text[10] = "Bu oyun Gamelab Istanbul tarafindan duzenlenen";
	text[11] = "C++ ile Oyun Programlama Egitiminde gelistirilmistir. (c)2021";
	for(int i = 0; i < linenum; i++) textx[i] = (getWidth() - textfont.getStringWidth(text[i])) / 2;
	lineh = textfont.getSize() * 15 / 10;
	firstliney = (getHeight() - (linenum * lineh)) / 2;
}

void AboutCanvas::update() {
	firstliney--;
	if(firstliney < -lineh * linenum) firstliney = getHeight() + lineh;
}

void AboutCanvas::draw() {
	background.draw(0, 0, getWidth(), getHeight());
	for(int i = 0; i < linenum; i++) {
		setColor(0, 0, 0);
		textfont.drawText(text[i], textx[i] + 2, firstliney + 2 + i * lineh);
		setColor(214, 255, 175);
		textfont.drawText(text[i], textx[i], firstliney + i * lineh);
	}
}

void AboutCanvas::keyPressed(int key) {
}

void AboutCanvas::keyReleased(int key) {

}

void AboutCanvas::mouseMoved(int x, int y) {
}

void AboutCanvas::mouseDragged(int x, int y, int button) {
}

void AboutCanvas::mousePressed(int x, int y, int button) {
}

void AboutCanvas::mouseReleased(int x, int y, int button) {
	MenuCanvas* cnv = new MenuCanvas(root);
	root->getAppManager()->setCurrentCanvas(cnv);
}

void AboutCanvas::mouseEntered() {
}

void AboutCanvas::mouseExited() {
}

void AboutCanvas::showNotify() {
}

void AboutCanvas::hideNotify() {
}
