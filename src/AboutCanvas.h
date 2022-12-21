/*
 * AboutCanvas.h
 *
 *  Created on: 9 Ara 2021
 *      Author: okand
 */

#ifndef SRC_ABOUTCANVAS_H_
#define SRC_ABOUTCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include "gFont.h"

class AboutCanvas: public gBaseCanvas {
public:
	AboutCanvas(gApp* root);
	virtual ~AboutCanvas();

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
	static const int linenum = 12;

	gApp* root;
	gImage background;
	gFont textfont;
	std::string text[linenum];
	int textx[linenum];
	int firstliney, lineh;
};

#endif /* SRC_ABOUTCANVAS_H_ */
