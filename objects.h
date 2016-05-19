#pragma once

#include <allegro5/allegro5.h>
#include "Const.h"

class GameObject
{
private:
	int ID;
	bool alive;
	bool movable;

protected:
	float x;
	float y;
	float velX;
	float velY;
	int dirX;
	int dirY;
	int object_type;

	//int maxFrame;
	//int curFrame;
	//int frameCount;
	//int frameDelay;
	//int frameWidth;
	//int frameHeight;
	//int animationColumns;
	//int animationDirection;

	ALLEGRO_BITMAP *image;

public:
	GameObject();
	void virtual Destroy();

	void Init(float x, float y, float velX, int dirX, float VelY, int dirY);
	void virtual Update();
	void virtual Render();

	//Creature Methods
	int virtual CheckAttack();
	void virtual GotHit(int dam);

	float GetX() {return x;}
	float GetY() {return y;}
	float GetDX() {return dirX;}
	float GetVX() {return velX;}
	float GetDY() {return dirY;}
	float GetVY() {return velY;}

	void SetX(float x) {GameObject::x = x;}
	void SetY(float y) {GameObject::y = y;}
	void SetVX(float velX) {GameObject::velX = velX;}
	void SetDX(float dirX) {GameObject::dirX = dirX;}
	void SetVY(float velY) {GameObject::velY = velY;}
	void SetDY(float dirY) {GameObject::dirY = dirY;}

	void SetImage(ALLEGRO_BITMAP *image) {GameObject::image = image;}

	int GetID() {return ID;}
	void SetID(int ID) {GameObject::ID = ID;}

	int GetType() {return object_type;}

	bool GetAlive() {return alive;}
	void SetAlive(bool alive) {GameObject::alive = alive;}

	bool CanMove() {return movable;}
	void SetMove(bool movable) {GameObject::movable = movable;}

	bool ObstacleCheck(GameObject *otherObject);
};
