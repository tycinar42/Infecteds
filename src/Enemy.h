/*
 * Enemy.h
 *
 *  Created on: 28 Eki 2021
 *      Author: user
 */

#ifndef SRC_ENEMY_H_
#define SRC_ENEMY_H_

class Enemy {
public:
	Enemy();
	virtual ~Enemy();

	void setPosition(float x, float y);
	void setRotation(float rotation);
	void setFrameNo(int frameNo);
	void setFrameCounter(int frameCounter);
	void setAnimationNo(int animationNo);
	int getAnimationNo();
	int getFrameNo();
	int getFrameCounter();
	float getRotation();
	float getX();
	float getY();

	float x, y;
	float rotation;
	int frameno, framecounter;
	int animationno;

private:

};

#endif /* SRC_ENEMY_H_ */
