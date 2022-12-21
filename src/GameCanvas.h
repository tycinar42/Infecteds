/*
 * GameCanvas.h
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#ifndef GAMECANVAS_H_
#define GAMECANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include "Enemy.h"
#include "gFont.h"

class GameCanvas : public gBaseCanvas {
public:
	GameCanvas(gApp* root);
	virtual ~GameCanvas();

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
	void setLevelNo(int levelNo);
	int getLevelNo();
	void setCharacterNo(int characterNo);
	void setScore(int score);

private:
	static const int KEY_NONE = 0, KEY_W = 1, KEY_S = 2, KEY_D = 4, KEY_A = 8;
	static const int ENEMYANIMATION_WALK, ENEMYANIMATION_ATTACK, ENEMYANIMATION_DEATH;
	enum {
		BULLETSENDER_CHARACTER, BULLETSENDER_ENEMY
	};
	enum {
		GAMESTATE_LOADING, GAMESTATE_PLAYING
	};

	static const int animationframenum = 6;
	static const int enemyanimationnum = 3;
	static const int enemymaxframenum = 16;

	gApp* root;
	gImage background;
	gImage character[animationframenum];
	gImage enemyimage[enemyanimationnum][enemymaxframenum];
	gImage levelmapimage;
	gImage levelmapsign1, levelmapsign2;
	gImage bulletimage[2];
	gImage charactericon;
	gImage healthicon;
	gImage progressbarbackground, progressbarframe, healthprogressbar;
	gImage shieldicon;
	gImage shieldprogressbar;
	gImage pausedialogue;
	gImage buttoncontinue, buttonmainmenu;
	gImage gameoverdialogue;
	gImage buttonreplay;
	gImage youwindialogue;
	gImage buttonnextlevel;
	gFont fpsfont;
	gFont scoretextfont;
	gFont scorefont;
	gImage loadingbackground, loadingprogressbar, loadingtext;
	gImage healthbox;
	gImage levelmapsign3;
	float cx, cy;
	float crot;
	int keystate;
	float cdx, cdy;
	float cspeed;
	void moveCharacter();
	void moveCamera();
	void moveObjects();
	void moveEnemies();
	void moveBullets();
	void drawBackground();
	void drawObjects();
	void drawCharacter();
	void drawEnemies();
	void drawBullets();
	void drawLevelMap();
	void drawGUI();
	bool checkCollision(int xLeft1, int yUp1, int xRight1, int yBottom1, int xLeft2, int yUp2, int xRight2, int yBottom2);
	void generateBullet(float bulletx, float bullety, float bulletdx, float bulletdy, float bulletrot, int bulletSender);
	void loadInitialAssets();
	void loadAssets();
	void loadAsset(int assetNo);
	void loadVariables();
	void drawLoadingScreen();
	int camx, camy;
	int camleftmargin, camtopmargin, camrightmargin, cambottommargin;
	int cframeno;
	int characterframecounter, framecounterlimit;
	int enemynum;
	std::vector<Enemy> enemy;
	int levelmapx, levelmapy;
	int enemyframecounterlimit;
	int enemyframenum[enemyanimationnum];
	std::vector<std::vector<float>> bullets;
	float bulletspeed;
	float muzzleangle, muzzledistance;
	int deadenemynum;
	float enemyrighthandangle, enemyrighthanddistance;
	float enemylefthandangle, enemylefthanddistance;
	int clive, cinitiallive;
	int charactericonx, charactericony;
	int healthiconx, healthicony;
	int healthprogressbarx, healthprogressbary;
	int shieldiconx, shieldicony;
	int shieldprogressbarx, shieldprogressbary;
	int dialoguex, dialoguey;
	int leftbuttonx, rightbuttonx, buttony;
	bool pausedialogueshown;
	bool gameoverdialogueshown;
	bool youwindialogueshown;
	int levelno;
	int characterno;
	int scoretextx, scoretexty;
	int score;
	int scorex, scorey;
	int gamestate;
	int loadcounter, loadcounterlimit;
	std::string characterpath, charactericonpath;
	int loadingprogressbarx, loadingprogressbary;
	int loadingtextx, loadingtexty;
	int healthboxx, healthboxy;
	int healthboxdy;
	int healthboxdylimit;
	int healthboxinitialy;
	bool healthboxtaken;

	//OPTIMIZATION VARIABLES
	int gw, gh;
	int mchhbxw, mchhbxh;
	int mcagw4, mcagh4, mcagw34, mcagh34;
	int mcabggw, mcabggh;
	int meni;
	float menedx, menedy;
	int c0gw2, c0gh2;
	int ei00gw2, ei00gh2;
	int c0gw, c0gh, ei00gw, ei00gh;
	float menbx, menby;
	int bigw, bigh;
	float menbdx, menbdy;
	int mbui, mbuj;
	int deni;
	int dbui;
	int dlmi;
	int dlmx2, dlmy2;
	int dguhx4, dguhy8;
};

#endif /* GAMECANVAS_H_ */
