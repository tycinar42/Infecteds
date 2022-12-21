/*
 * Enemy.cpp
 *
 *  Created on: 28 Eki 2021
 *      Author: user
 */

#include "Enemy.h"

Enemy::Enemy() {
	x = 0.0f;
	y = 0.0f;
	rotation = 0.0f;
	frameno = 0;
	framecounter = 0;
	animationno = 0;
}

Enemy::~Enemy() {

}

void Enemy::setPosition(float x, float y) {
	this->x = x;
	this->y = y;
}

float Enemy::getX() {
	return x;
}

float Enemy::getY() {
	return y;
}

void Enemy::setRotation(float rotation) {
	this->rotation = rotation;
}

float Enemy::getRotation() {
	return rotation;
}

void Enemy::setFrameNo(int frameNo) {
	frameno = frameNo;
}

void Enemy::setFrameCounter(int frameCounter) {
	framecounter = frameCounter;
}

int Enemy::getFrameNo() {
	return frameno;
}

int Enemy::getFrameCounter() {
	return framecounter;
}

void Enemy::setAnimationNo(int animationNo) {
	animationno = animationNo;
}

int Enemy::getAnimationNo() {
	return animationno;
}

