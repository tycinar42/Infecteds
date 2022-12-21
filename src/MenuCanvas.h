/*
 * MenuCanvas.h
 *
 *  Created on: 25 Kas 2021
 *      Author: user
 */

#ifndef SRC_MENUCANVAS_H_
#define SRC_MENUCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include "gFont.h"

class MenuCanvas: public gBaseCanvas {
public:
	MenuCanvas(gApp* root);
	virtual ~MenuCanvas();

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered();
	void mouseExited();

	void showNotify();
	void hideNotify();

private:
	gApp* root;
	gImage background;
	gImage mainmenu;
	gImage tick;
	gImage ban;
	gImage glistenginelogo;
	gFont versiontextfont;
	std::string versiontext;

	int menux, menuy;
	int playbuttonleftx, playbuttonrightx, playbuttontopy, playbuttonbottomy;
	int versiontextx, versiontexty, versiontextw, direction;
};

#endif /* SRC_MENUCANVAS_H_ */
