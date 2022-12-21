/*
 * GameCanvas.cpp
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */


#include "GameCanvas.h"
#include "MenuCanvas.h"


const int GameCanvas::ENEMYANIMATION_WALK = 0;
const int GameCanvas::ENEMYANIMATION_ATTACK = 1;
const int GameCanvas::ENEMYANIMATION_DEATH = 2;


GameCanvas::GameCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
	levelno = 0;
	characterno = 0;
	score = 0;
}

GameCanvas::~GameCanvas() {
}

void GameCanvas::setup() {
//	gLogi("GameCanvas") << "setup";
	loadcounterlimit = 100;
	loadcounter = 0;
	gamestate = GAMESTATE_LOADING;
	loadInitialAssets();
}

void GameCanvas::update() {
//	gLogi("GameCanvas") << "update";
	if(gamestate == GAMESTATE_LOADING) {
		loadAssets();
		return;
	}
	if(pausedialogueshown) return;
	moveBullets();
	if(gameoverdialogueshown) return;
	moveEnemies();
	if(youwindialogueshown) return;
	moveCharacter();
	moveCamera();
	moveObjects();
}

void GameCanvas::draw() {
//	gLogi("GameCanvas") << "draw";
	if(gamestate == GAMESTATE_LOADING) {
		drawLoadingScreen();
		return;
	}
	drawBackground();
	drawObjects();
	drawEnemies();
	drawCharacter();
	drawBullets();
	drawLevelMap();
	drawGUI();
//	fpsfont.drawText("FPS:" + gToStr(root->getFramerate()), 10, 22);
}

void GameCanvas::moveCharacter() {
	if((keystate & KEY_W) != 0) {
		cdx = std::sin(gDegToRad(crot)) * cspeed;
		cdy = -std::cos(gDegToRad(crot)) * cspeed;
	} else if((keystate & KEY_S) != 0) {
		cdx = -std::sin(gDegToRad(crot)) * cspeed;
		cdy = std::cos(gDegToRad(crot)) * cspeed;
	}

	if((keystate & KEY_D) != 0) {
		cdx = std::cos(gDegToRad(crot)) * cspeed;
		cdy = std::sin(gDegToRad(crot)) * cspeed;
	} else if((keystate & KEY_A) != 0) {
		cdx = -std::cos(gDegToRad(crot)) * cspeed;
		cdy = -std::sin(gDegToRad(crot)) * cspeed;
	}

	cx += cdx;
	cy += cdy;

	if(cdx != 0.0f || cdy != 0.0f) {
		characterframecounter++;
		if(characterframecounter >= framecounterlimit) {
			cframeno++;
			if(cframeno >= animationframenum) cframeno = 0;
			characterframecounter = 0;
		}
	}

	if(!healthboxtaken && checkCollision(cx + camx, cy + camy, cx + camx + c0gw, cy + camy + c0gh,
			healthboxx, healthboxy, mchhbxw, mchhbxh)) {
		healthboxtaken = true;
		clive += 20;
		if(clive > cinitiallive) clive = cinitiallive;
	}
}

void GameCanvas::moveCamera() {
	camleftmargin = mcagw4;
	if(camx <= 0) camleftmargin = 0;
	camtopmargin = mcagh4;
	if(camy <= 0) camtopmargin = 0;
	camrightmargin = mcagw34;
	if(camx >= mcabggw) camrightmargin = gw;
	cambottommargin = mcagh34;
	if(camy >= mcabggh) cambottommargin = gh;

	if(cx <= camleftmargin || cx + c0gw >= camrightmargin) {
		cx -= cdx;
		camx += cdx;
		if(camx <= 0) {
			camx = 0;
		}
		if(camx >= mcabggw) {
			camx = mcabggw;
		}
	}
	if(cy <= camtopmargin || cy + c0gh >= cambottommargin) {
		cy -= cdy;
		camy += cdy;
		if(camy <= 0) {
			camy = 0;
		}
		if(camy >= mcabggh) {
			camy = mcabggh;
		}
	}

	cdx = 0.0f;
	cdy = 0.0f;
}

void GameCanvas::moveObjects() {
	if(healthboxtaken) return;
	if(std::fabs(healthboxy - healthboxinitialy) >= healthboxdylimit) {
		healthboxdy = -healthboxdy;
	}
	healthboxy += healthboxdy;
}

void GameCanvas::moveEnemies() {
	for(meni = 0; meni < enemynum; meni++) {
		if(enemy[meni].animationno != ENEMYANIMATION_DEATH) enemy[meni].rotation = gRadToDeg(std::atan2((cy + camy + c0gh2) - (enemy[meni].y + ei00gh2), (cx + camx + c0gw2) - (enemy[meni].x + ei00gw2))) - 90.0f;

		menedx = 0.0f;
		menedy = 0.0f;
		if(enemy[meni].animationno == ENEMYANIMATION_WALK && enemy[meni].x + ei00gw > camx && enemy[meni].x < camx + gw && enemy[meni].y + ei00gh > camy && enemy[meni].y < camy + gh) {
			menedx = -std::sin(gDegToRad(enemy[meni].rotation));
			menedy = std::cos(gDegToRad(enemy[meni].rotation));
			if(!root->isSoundPlaying(gApp::SOUNDEFFECT_ENEMYFOOTSTEP)) root->playSound(gApp::SOUNDEFFECT_ENEMYFOOTSTEP);
		}

		enemy[meni].x += menedx;
		enemy[meni].y += menedy;

		if(enemy[meni].animationno != ENEMYANIMATION_DEATH && checkCollision(enemy[meni].x + 49, enemy[meni].y + 25, enemy[meni].x + 114, enemy[meni].y + 70,
				cx + camx, cy + camy + 30, cx + camx + c0gw, cy + camy + c0gh)) {
			if(enemy[meni].animationno != ENEMYANIMATION_ATTACK) enemy[meni].frameno = 0;
			enemy[meni].animationno = ENEMYANIMATION_ATTACK;
			if(enemy[meni].frameno == 2 || enemy[meni].frameno == 9) {
				clive -= 1;
				score = score - 5;
				if(score < 0) score = 0;
				if(clive <= 0 && !gameoverdialogueshown) {
					clive = 0;
					scorex = (pausedialogue.getWidth() - scorefont.getStringWidth(gToStr(score))) / 2;
					gameoverdialogueshown = true;
				}
			}

		} else {
			if(enemy[meni].animationno != ENEMYANIMATION_DEATH) {
				if(enemy[meni].animationno != ENEMYANIMATION_WALK) enemy[meni].frameno = 0;
				enemy[meni].animationno = ENEMYANIMATION_WALK;
			}
		}

		if((menedx != 0.0f || menedy != 0.0f)  || enemy[meni].animationno != ENEMYANIMATION_WALK) {
			enemy[meni].framecounter++;
			if(enemy[meni].framecounter >= enemyframecounterlimit) {
				enemy[meni].frameno++;
				if(enemy[meni].frameno >= enemyframenum[enemy[meni].animationno]) {
					if(enemy[meni].animationno == ENEMYANIMATION_DEATH) {
						enemy[meni].frameno = enemyframenum[enemy[meni].animationno] - 1;
					} else enemy[meni].frameno = 0;
				}
				enemy[meni].framecounter = 0;

				if(enemy[meni].animationno == ENEMYANIMATION_WALK && (enemy[meni].frameno == 3 || enemy[meni].frameno == 7)) {
					menbx = 0.0f;
					menby = 0.0f;
					if(enemy[meni].frameno == 3) {
						menbx = enemy[meni].x + ((ei00gw - bigw) / 2) - (std::sin(gDegToRad(enemy[meni].rotation + enemyrighthandangle)) * enemyrighthanddistance);
						menby = enemy[meni].y + ((ei00gh - bigh) / 2) + (std::cos(gDegToRad(enemy[meni].rotation + enemyrighthandangle)) * enemyrighthanddistance);
					} else {
						menbx = enemy[meni].x + ((ei00gw - bigw) / 2) - (std::sin(gDegToRad(enemy[meni].rotation + enemylefthandangle)) * enemylefthanddistance);
						menby = enemy[meni].y + ((ei00gh - bigh) / 2) + (std::cos(gDegToRad(enemy[meni].rotation + enemylefthandangle)) * enemylefthanddistance);
					}
					menbdx = -std::sin(gDegToRad(enemy[meni].rotation)) * bulletspeed;
					menbdy = std::cos(gDegToRad(enemy[meni].rotation)) * bulletspeed;
					generateBullet(menbx, menby, menbdx, menbdy, enemy[meni].rotation, BULLETSENDER_ENEMY);
				}
			}
		}
	}
}

void GameCanvas::moveBullets() {
	for(mbui = bullets.size() - 1; mbui >= 0; mbui--) {
		bullets[mbui][0] += bullets[mbui][2];
		bullets[mbui][1] += bullets[mbui][3];
		bullets[mbui][5]++;
		if(bullets[mbui][5] >= 30) {
			bullets.erase(bullets.begin() + mbui);
			continue;
		}

		if(bullets[mbui][0] + bigw < camx || bullets[mbui][0] >= camx + gw || bullets[mbui][1] + bigh < camy || bullets[mbui][1] >= camy + gh) {
			bullets.erase(bullets.begin() + mbui);
			continue;
		}

		if(gameoverdialogueshown || youwindialogueshown) continue;

		if(bullets[mbui][6] == BULLETSENDER_CHARACTER) {
			for(mbuj = 0; mbuj < enemynum; mbuj++) {
				if(enemy[mbuj].animationno != ENEMYANIMATION_DEATH && checkCollision(bullets[mbui][0], bullets[mbui][1], bullets[mbui][0] + bigw, bullets[mbui][1] + bigh,
						enemy[mbuj].x, enemy[mbuj].y, enemy[mbuj].x + ei00gw, enemy[mbuj].y + ei00gh)) {
					enemy[mbuj].animationno = ENEMYANIMATION_DEATH;
					enemy[mbuj].frameno = 0;
					root->playSound(gApp::SOUNDEFFECT_ENEMYDEATH);
					deadenemynum++;
					score = score + 50;
					if(deadenemynum >= enemynum && !youwindialogueshown) {
						scorex = (pausedialogue.getWidth() - scorefont.getStringWidth(gToStr(score))) / 2;
						youwindialogueshown = true;
					}
					//TODO olen enemy vektorun basina kaydedilecek
					bullets.erase(bullets.begin() + mbui);
					break;
				}
			}
		} else if(bullets[mbui][6] == BULLETSENDER_ENEMY) {
			if(checkCollision(bullets[mbui][0], bullets[mbui][1], bullets[mbui][0] + bigw, bullets[mbui][1] + bigh,
						cx + camx, cy + camy, cx + camx + c0gw, cy + camy + c0gh)) {
				clive--;
				score = score - 5;
				if(score < 0) score = 0;
				if(clive <= 0 && !gameoverdialogueshown) {
					clive = 0;
					scorex = (pausedialogue.getWidth() - scorefont.getStringWidth(gToStr(score))) / 2;
					gameoverdialogueshown = true;
				}
				bullets.erase(bullets.begin() + mbui);
				break;
			}
		}
	}
}

void GameCanvas::drawBackground() {
	background.drawSub(0, 0, gw, gh, camx, camy, gw, gh);
}

void GameCanvas::drawObjects() {
	if(healthboxtaken) return;
	healthbox.draw(healthboxx - camx, healthboxy - camy);
}

void GameCanvas::drawCharacter() {
	character[cframeno].draw(cx, cy, c0gw, c0gh, crot);
}

void GameCanvas::drawEnemies() {
	for(deni = 0; deni < enemynum; deni++) {
		enemyimage[enemy[deni].animationno][enemy[deni].frameno].draw(enemy[deni].x - camx, enemy[deni].y - camy, ei00gw, ei00gh, enemy[deni].rotation);
	}
}

void GameCanvas::drawBullets() {
	for(dbui = 0; dbui < bullets.size(); dbui++) {
		bulletimage[(int)bullets[dbui][6]].draw(bullets[dbui][0] - camx, bullets[dbui][1] - camy, bigw, bigh, bullets[dbui][4]);
	}
}

void GameCanvas::drawLevelMap() {
	levelmapimage.draw(levelmapx, levelmapy);
	for(dlmi = 0; dlmi < enemynum; dlmi++) {
		if(enemy[dlmi].animationno != ENEMYANIMATION_DEATH) levelmapsign2.draw(dlmx2 + enemy[dlmi].x / 32, dlmy2 + enemy[dlmi].y / 32);
	}
	levelmapsign1.draw(dlmx2 + (cx + camx) / 32, dlmy2 + (cy + camy) / 32);
	if(!healthboxtaken) levelmapsign3.draw(dlmx2 + healthboxx / 32, dlmy2 + healthboxy / 32);
}

void GameCanvas::drawGUI() {
	charactericon.draw(charactericonx, charactericony);
	healthicon.draw(healthiconx, healthicony);
	progressbarbackground.draw(dguhx4, dguhy8); // buradan sonrasina bak
	healthprogressbar.drawSub(dguhx4, dguhy8, healthprogressbar.getWidth() * clive / cinitiallive, healthprogressbar.getHeight(),
			0, 0, healthprogressbar.getWidth() * clive / cinitiallive, healthprogressbar.getHeight());
	progressbarframe.draw(healthprogressbarx, healthprogressbary);
	shieldicon.draw(shieldiconx, shieldicony);
	progressbarbackground.draw(shieldprogressbarx + 4, shieldprogressbary + 8);
	shieldprogressbar.drawSub(shieldprogressbarx + 4, shieldprogressbary + 8, shieldprogressbar.getWidth() * (enemynum - deadenemynum) / enemynum, shieldprogressbar.getHeight(),
			0, 0, shieldprogressbar.getWidth() * (enemynum - deadenemynum) / enemynum, shieldprogressbar.getHeight());
	progressbarframe.draw(shieldprogressbarx, shieldprogressbary);

	if(youwindialogueshown) {
		youwindialogue.draw(dialoguex, dialoguey);
		buttonnextlevel.draw(leftbuttonx, buttony);
		buttonmainmenu.draw(rightbuttonx, buttony);
		setColor(0, 0, 0);
		scoretextfont.drawText("SCORE", dialoguex + scoretextx + 4, dialoguey + scoretexty + 4);
		scorefont.drawText(gToStr(score), dialoguex + scorex + 4, dialoguey + scorey + 4);
		setColor(213, 232, 109);
		scoretextfont.drawText("SCORE", dialoguex + scoretextx, dialoguey + scoretexty);
		scorefont.drawText(gToStr(score), dialoguex + scorex, dialoguey + scorey);
	}

	if(gameoverdialogueshown) {
		gameoverdialogue.draw(dialoguex, dialoguey);
		buttonreplay.draw(leftbuttonx, buttony);
		buttonmainmenu.draw(rightbuttonx, buttony);
		setColor(0, 0, 0);
		scoretextfont.drawText("SCORE", dialoguex + scoretextx + 4, dialoguey + scoretexty + 4);
		scorefont.drawText(gToStr(score), dialoguex + scorex + 4, dialoguey + scorey + 4);
		setColor(213, 232, 109);
		scoretextfont.drawText("SCORE", dialoguex + scoretextx, dialoguey + scoretexty);
		scorefont.drawText(gToStr(score), dialoguex + scorex, dialoguey + scorey);
	}

	if(pausedialogueshown) {
		pausedialogue.draw(dialoguex, dialoguey);
		buttoncontinue.draw(leftbuttonx, buttony);
		buttonmainmenu.draw(rightbuttonx, buttony);
		setColor(0, 0, 0);
		scoretextfont.drawText("SCORE", dialoguex + scoretextx + 4, dialoguey + scoretexty + 4);
		scorefont.drawText(gToStr(score), dialoguex + scorex + 4, dialoguey + scorey + 4);
		setColor(213, 232, 109);
		scoretextfont.drawText("SCORE", dialoguex + scoretextx, dialoguey + scoretexty);
		scorefont.drawText(gToStr(score), dialoguex + scorex, dialoguey + scorey);
	}
}

void GameCanvas::keyPressed(int key) { // w:87 s:83 d:68 a:65
//	gLogi("GameCanvas") << "keyPressed:" << key;
	if(key == 256 && !gameoverdialogueshown && !youwindialogueshown) {
		pausedialogueshown = !pausedialogueshown;
		scorex = (pausedialogue.getWidth() - scorefont.getStringWidth(gToStr(score))) / 2;
		return;
	}

	int pressedkey = KEY_NONE;
	switch(key) {
	case 87:
		pressedkey = KEY_W;
		break;
	case 83:
		pressedkey = KEY_S;
		break;
	case 68:
		pressedkey = KEY_D;
		break;
	case 65:
		pressedkey = KEY_A;
		break;
	default:
		break;
	}
	keystate |= pressedkey;
}

void GameCanvas::keyReleased(int key) {
//	gLogi("GameCanvas") << "keyReleased:" << key;
	int pressedkey = KEY_NONE;
	switch(key) {
	case 87:
		pressedkey = KEY_W;
		break;
	case 83:
		pressedkey = KEY_S;
		break;
	case 68:
		pressedkey = KEY_D;
		break;
	case 65:
		pressedkey = KEY_A;
		break;
	default:
		break;
	}
	keystate &= ~pressedkey;
}

void GameCanvas::mouseMoved(int x, int y) {
//	gLogi("GameCanvas") << "mouseMoved" << ", x:" << x << ", y:" << y;
	if(pausedialogueshown || gameoverdialogueshown || youwindialogueshown) return;
	crot = gRadToDeg(std::atan2(y - (cy + (character[0].getHeight() / 2)), x - (cx + (character[0].getWidth() / 2)))) + 90.0f;
}

void GameCanvas::mouseDragged(int x, int y, int button) {
//	gLogi("GameCanvas") << "mouseDragged" << ", x:" << x << ", y:" << y << ", b:" << button;
}

void GameCanvas::mousePressed(int x, int y, int button) {
//	gLogi("GameCanvas") << "mousePressed" << ", x:" << x << ", y:" << y << ", b:" << button << ", crot:" << crot;
}

void GameCanvas::mouseReleased(int x, int y, int button) {
//	gLogi("GameCanvas") << "mouseReleased" << ", button:" << button;
	//if(leftbuttonx < )
	if(pausedialogueshown && x >= leftbuttonx && x < leftbuttonx + buttoncontinue.getWidth() && y >= buttony && y < buttony + buttoncontinue.getHeight()) {
		pausedialogueshown = false;
		return;
	}
	if(gameoverdialogueshown && x >= leftbuttonx && x < leftbuttonx + buttoncontinue.getWidth() && y >= buttony && y < buttony + buttoncontinue.getHeight()) {
		GameCanvas* cnv = new GameCanvas(root);
		cnv->setLevelNo(0);
		cnv->setCharacterNo(characterno);
		root->getAppManager()->setCurrentCanvas(cnv);
		root->setMusicPaused(gApp::MUSIC_GAME, true);
		return;
	}
	if(youwindialogueshown && x >= leftbuttonx && x < leftbuttonx + buttoncontinue.getWidth() && y >= buttony && y < buttony + buttoncontinue.getHeight()) {
		GameCanvas* cnv = new GameCanvas(root);
		cnv->setLevelNo(levelno + 1);
		cnv->setCharacterNo(characterno);
		cnv->setScore(score);
		root->getAppManager()->setCurrentCanvas(cnv);
		root->setMusicPaused(gApp::MUSIC_GAME, true);
		return;
	}
	if((pausedialogueshown || gameoverdialogueshown || youwindialogueshown) && x >= rightbuttonx && x < rightbuttonx + buttoncontinue.getWidth() && y >= buttony && y < buttony + buttoncontinue.getHeight()) {
		MenuCanvas* cnv = new MenuCanvas(root);
		root->getAppManager()->setCurrentCanvas(cnv);
		root->setMusicPaused(gApp::MUSIC_GAME, true);
		return;
	}

	if(pausedialogueshown || gameoverdialogueshown || youwindialogueshown) return;
	float bx = cx + camx + ((character[0].getWidth() - bulletimage[0].getWidth()) / 2) + (std::sin(gDegToRad(crot + muzzleangle)) * muzzledistance);
	float by = cy + camy + ((character[0].getHeight() - bulletimage[0].getHeight()) / 2) - (std::cos(gDegToRad(crot + muzzleangle)) * muzzledistance);
	float bdx = std::sin(gDegToRad(crot)) * bulletspeed;
	float bdy = -std::cos(gDegToRad(crot)) * bulletspeed;
	generateBullet(bx, by, bdx, bdy, crot, BULLETSENDER_CHARACTER);
	root->playSound(gApp::SOUNDEFFECT_GUNSHOT);
}

void GameCanvas::mouseEntered() {
}

void GameCanvas::mouseExited() {
}

void GameCanvas::showNotify() {

}

void GameCanvas::hideNotify() {

}

bool GameCanvas::checkCollision(int xLeft1, int yUp1, int xRight1, int yBottom1, int xLeft2, int yUp2, int xRight2, int yBottom2) {
	if(xLeft1 < xRight2 && xRight1 > xLeft2 && yBottom1 > yUp2 && yUp1 < yBottom2) {
		return true;
	}
	return false;
}

void GameCanvas::generateBullet(float bulletx, float bullety, float bulletdx, float bulletdy, float bulletrot, int bulletSender) {
	std::vector<float> newbullet;
	newbullet.push_back(bulletx);
	newbullet.push_back(bullety);
	newbullet.push_back(bulletdx);
	newbullet.push_back(bulletdy);
	newbullet.push_back(bulletrot);
	newbullet.push_back(0);
	newbullet.push_back(bulletSender);
	bullets.push_back(newbullet);
}

void GameCanvas::setLevelNo(int levelNo) {
	levelno = levelNo;
}

int GameCanvas::getLevelNo() {
	return levelno;
}

void GameCanvas::setCharacterNo(int characterNo) {
	characterno = characterNo;
}

void GameCanvas::setScore(int score) {
	this->score = score;
}

void GameCanvas::loadInitialAssets() {
	loadingbackground.loadImage("menu/arkaplan.jpg");
	loadingprogressbar.loadImage("oyun/gui/gui_progressbar.png");
	loadingtext.loadImage("oyun/gui/gui_loadingtext2.png");
	loadingprogressbarx = (getWidth() - loadingprogressbar.getWidth()) / 2;
	loadingprogressbary = getHeight() * 3 / 4;
	loadingtextx = (getWidth() - loadingtext.getWidth()) / 2;
	loadingtexty = loadingprogressbary - loadingtext.getHeight() * 2;
}

void GameCanvas::loadAssets() {
	loadAsset(loadcounter);
	loadcounter++;
	if(loadcounter >= loadcounterlimit) gamestate = GAMESTATE_PLAYING;
}

void GameCanvas::loadAsset(int assetNo) {
	switch(assetNo) {
	case 0:
		background.loadImage("oyun/haritalar/arkaplan" + gToStr((levelno % 3) + 1) + ".jpg");
		break;
	case 1:
		characterpath = "erkek/erkek";
		if(characterno == 1) characterpath = "kadin/kadin";
		for(int i = 0; i < animationframenum; i++) character[i].loadImage("oyun/karakterler/" + characterpath + "_tufek_0" + gToStr(i) + ".png");
		break;
	case 2:
		enemyframenum[ENEMYANIMATION_WALK] = 8;
		enemyframenum[ENEMYANIMATION_ATTACK] = 16;
		enemyframenum[ENEMYANIMATION_DEATH] = 14;
		for(int i = 0; i < enemyanimationnum; i++) {
			std::string animpath = "walk/Walk";
			if(i == ENEMYANIMATION_ATTACK) animpath = "attack/attack1";
			else if(i == ENEMYANIMATION_DEATH) animpath = "death/Death";
			for(int j = 0; j < enemyframenum[i]; j++) {
				std::string fno = gToStr(j);
				if(j < 10) fno = "0" + fno;
				enemyimage[i][j].loadImage("oyun/dusmanlar/" + animpath + "_0" + fno + ".png");
			}
		}
		break;
	case 3:
		levelmapimage.loadImage("oyun/haritalar/radar" + gToStr((levelno % 3) + 1) + ".jpg");
		break;
	case 4:
		levelmapsign1.loadImage("oyun/haritalar/radarisaret1.png");
		break;
	case 5:
		levelmapsign2.loadImage("oyun/haritalar/radarisaret2.png");
		break;
	case 6:
		bulletimage[BULLETSENDER_CHARACTER].loadImage("oyun/objeler/mermi.png");
		break;
	case 7:
		bulletimage[BULLETSENDER_ENEMY].loadImage("oyun/objeler/mermi2.png");
		break;
	case 8:
		charactericonpath = "erkekikon";
		if(characterno == 1) charactericonpath = "kadinikon";
		charactericon.loadImage("oyun/gui/" + charactericonpath + ".png");
		break;
	case 9:
		healthicon.loadImage("oyun/gui/element_0098_Layer-100.png");
		break;
	case 10:
		progressbarbackground.loadImage("oyun/gui/element_0077_Layer-79.png");
		break;
	case 11:
		progressbarframe.loadImage("oyun/gui/element_0092_Layer-94.png");
		break;
	case 12:
		healthprogressbar.loadImage("oyun/gui/element_0076_Layer-78.png");
		break;
	case 13:
		shieldicon.loadImage("oyun/gui/element_0100_Layer-102.png");
		break;
	case 14:
		shieldprogressbar.loadImage("oyun/gui/element_0076_Layer-78b.png");
		break;
	case 15:
		pausedialogue.loadImage("oyun/gui/dialogue_pause.png");
		break;
	case 16:
		buttoncontinue.loadImage("oyun/gui/button_continue.png");
		break;
	case 17:
		buttonmainmenu.loadImage("oyun/gui/button_mainmenu.png");
		break;
	case 18:
		gameoverdialogue.loadImage("oyun/gui/dialogue_gameover.png");
		break;
	case 19:
		buttonreplay.loadImage("oyun/gui/button_replay.png");
		break;
	case 20:
		youwindialogue.loadImage("oyun/gui/dialogue_youwin.png");
		break;
	case 21:
		buttonnextlevel.loadImage("oyun/gui/button_nextlevel.png");
		break;
	case 22:
		fpsfont.loadFont("FreeSans.ttf", 12);
		break;
	case 23:
		scoretextfont.loadFont("BlackOpsOne-Regular.ttf", 30);
		break;
	case 24:
		scorefont.loadFont("BlackOpsOne-Regular.ttf", 80);
		break;
	case 25:
		healthbox.loadImage("oyun/objeler/healthbox.png");
		break;
	case 26:
		levelmapsign3.loadImage("oyun/haritalar/radarisaret3.png");
		break;
	case 99:
		loadVariables();
		break;
	}
}

void GameCanvas::loadVariables() {
	cx = (getWidth() - character[0].getWidth()) / 2;
	cy = (getHeight() - character[0].getHeight()) / 2;
	crot = 0.0f;
	keystate = KEY_NONE;
	cdx = 0.0f;
	cdy = 0.0f;
	cspeed = 4.0f;
	camx = 0;
	camy = 0;
	camleftmargin = getWidth() / 4;
	camtopmargin = getHeight() / 4;
	camrightmargin = (getWidth() * 3) / 4;
	cambottommargin = (getHeight() * 3) / 4;
	cframeno = 0;
	characterframecounter = 0;
	framecounterlimit = 6;
	enemynum = 20;
	enemyframecounterlimit = 3;
	for(int i = 0; i < enemynum; i++) {
		Enemy e;
		enemy.push_back(e);
		int ex, ey;
		do {
			ex = gRandom(background.getWidth() - enemyimage[0][0].getWidth());
			ey = gRandom(background.getHeight() - enemyimage[0][0].getHeight());
		} while(ex < getWidth() && ey < getHeight());
		enemy[i].setPosition(ex, ey);

		enemy[i].setRotation(0.0f);
		enemy[i].setFrameNo(gRandom(enemyframenum[ENEMYANIMATION_WALK]));
		enemy[i].setFrameCounter(gRandom(enemyframecounterlimit));
		enemy[i].setAnimationNo(ENEMYANIMATION_WALK);
	}
	levelmapx = getWidth() - levelmapimage.getWidth() - (levelmapimage.getWidth() / 2);
	levelmapy = levelmapimage.getWidth() / 2;
	bulletspeed = cspeed * 6;
	muzzleangle = gRadToDeg(std::atan2(2 - (character[0].getHeight() / 2), 43 - (character[0].getWidth() / 2))) + 90.0f;
	muzzledistance = std::sqrt(std::pow(2 - (character[0].getHeight() / 2), 2) + std::pow(43 - (character[0].getWidth() / 2), 2));
	deadenemynum = 0;
	enemyrighthandangle = gRadToDeg(std::atan2(109 - (enemyimage[0][0].getHeight() / 2), 58 - (enemyimage[0][0].getWidth() / 2))) - 90.0f;
	enemyrighthanddistance = std::sqrt(std::pow(109 - (enemyimage[0][0].getHeight() / 2), 2) + std::pow(58 - (enemyimage[0][0].getWidth() / 2), 2));
	enemylefthandangle = gRadToDeg(std::atan2(106 - (enemyimage[0][0].getHeight() / 2), 91 - (enemyimage[0][0].getWidth() / 2))) - 90.0f;
	enemylefthanddistance = std::sqrt(std::pow(106 - (enemyimage[0][0].getHeight() / 2), 2) + std::pow(91 - (enemyimage[0][0].getWidth() / 2), 2));
	cinitiallive = 100;
	clive = cinitiallive;
	charactericonx = charactericon.getHeight() / 2;
	charactericony = charactericonx;
	healthiconx = charactericonx + charactericon.getWidth() + 5;
	healthicony = charactericony;
	healthprogressbarx = healthiconx + healthicon.getWidth() + 5;
	healthprogressbary = healthicony + ((healthicon.getHeight() - progressbarframe.getHeight()) / 2);
	shieldiconx = healthiconx;
	shieldicony = charactericony + healthicon.getHeight() + 10;
	shieldprogressbarx = healthprogressbarx;
	shieldprogressbary = shieldicony + ((shieldicon.getHeight() - progressbarframe.getHeight()) / 2);
	dialoguex = (getWidth() - pausedialogue.getWidth()) / 2;
	dialoguey = (getHeight() - pausedialogue.getHeight()) / 2;
	leftbuttonx = dialoguex + (pausedialogue.getWidth() / 2) - 10 - buttoncontinue.getWidth();
	rightbuttonx = dialoguex + (pausedialogue.getWidth() / 2) + 10;
	buttony = dialoguey + (pausedialogue.getHeight() * 11 / 16);
	pausedialogueshown = false;
	gameoverdialogueshown = false;
	youwindialogueshown = false;
	scoretextx = (pausedialogue.getWidth() - scoretextfont.getStringWidth("SCORE")) / 2;
	scoretexty = pausedialogue.getHeight() * 2 / 7;
	scorex = 0;
	scorey = pausedialogue.getHeight() * 4 / 7;
	healthboxdylimit = 10;
	healthboxx = gRandom(background.getWidth() - healthbox.getWidth());
	healthboxy = gRandom(background.getHeight() - healthbox.getHeight() - healthboxdylimit);
	healthboxdy = 1;
	healthboxinitialy = healthboxy;
	healthboxtaken = false;

	root->setMusicPaused(gApp::MUSIC_GAME, !root->musicon);

	//OPTIMIZATION VARIABLES
	gw = getWidth();
	gh = getHeight();
	mchhbxw = healthboxx + healthbox.getWidth();
	mchhbxh = healthboxy + healthbox.getHeight();
	mcagw4 = getWidth() / 4;
	mcagh4 = getHeight() / 4;
	mcagw34 = getWidth() * 3 / 4;
	mcagh34 = getHeight() * 3 / 4;
	mcabggw = background.getWidth() - getWidth();
	mcabggh = background.getHeight() - getHeight();
	c0gw2 = character[0].getWidth() / 2;
	c0gh2 = character[0].getHeight() / 2;
	ei00gw2 = enemyimage[0][0].getWidth() / 2;
	ei00gh2 = enemyimage[0][0].getHeight() / 2;
	c0gw = character[0].getWidth();
	c0gh = character[0].getHeight();
	ei00gw = enemyimage[0][0].getWidth();
	ei00gh = enemyimage[0][0].getHeight();
	bigw = bulletimage[0].getWidth();
	bigh = bulletimage[0].getHeight();
	dlmx2 = levelmapx + 2;
	dlmy2 = levelmapy + 2;
	dguhx4 = healthprogressbarx + 4;
	dguhy8 = healthprogressbary + 8;
}

void GameCanvas::drawLoadingScreen() {
	loadingbackground.draw(0, 0, getWidth(), getHeight());
	loadingtext.draw(loadingtextx, loadingtexty);
	loadingprogressbar.drawSub(loadingprogressbarx, loadingprogressbary, loadingprogressbar.getWidth(), loadingprogressbar.getHeight() / 2,
			0, 0, loadingprogressbar.getWidth(), loadingprogressbar.getHeight() / 2);
	loadingprogressbar.drawSub(loadingprogressbarx, loadingprogressbary, loadcounter * loadingprogressbar.getWidth() / (loadcounterlimit - 1), loadingprogressbar.getHeight() / 2,
			loadingprogressbar.getWidth() - (loadcounter * loadingprogressbar.getWidth() / (loadcounterlimit - 1)), loadingprogressbar.getHeight() / 2, loadcounter * loadingprogressbar.getWidth() / (loadcounterlimit - 1), loadingprogressbar.getHeight() / 2);
}
