#pragma once

#include <allegro5/allegro5.h>
#include "Const.h"

class GameObject
{
private:
	int ID;
	bool alive;
	bool movable;
	bool solid;

protected:
	float x;
	float y;
	float velX;
	float velY;
	int dirX;
	int dirY;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;
	int ANIMATION;

	ALLEGRO_BITMAP *image;

public:
	GameObject();
	/* */
	void virtual Destroy();
	void virtual Update();
	void virtual Render();

    /* Wyrzuciæ jako wspólny interface - combat? */
    void virtual GotHit(int dam);
    int virtual CheckAttack();
    float PositionY();
	/* */
	void Init(float x, float y, float velX, int dirX, float VelY, int dirY);
	/* */
	float GetX() {return x;}
	float GetY() {return y;}
	float GetDX() {return dirX;}
	float GetVX() {return velX;}
	float GetDY() {return dirY;}
	float GetVY() {return velY;}
	/* */
	void SetX(float x) {GameObject::x = x;}
	void SetY(float y) {GameObject::y = y;}
	void SetVX(float velX) {GameObject::velX = velX;}
	void SetDX(float dirX) {GameObject::dirX = dirX;}
	void SetVY(float velY) {GameObject::velY = velY;}
	void SetDY(float dirY) {GameObject::dirY = dirY;}
	/* */
	int GetID() {return ID;}
	bool GetAlive() {return alive;}
	bool GetSolid() {return solid;}
	bool CanMove() {return movable;}
	int GetAnimation() {return ANIMATION;}

	/* Wyrzuciæ jako wspólny interface - Range/Collisions? */
	bool CollisionCheck(GameObject *otherObject);
	bool IsInRange(GameObject *otherObject, float attack_range);
	bool IsOppositeTeam(GameObject *otherObject);

	/* */
	void SetID(int ID) {GameObject::ID = ID;}
	void SetAlive(bool alive) {GameObject::alive = alive;}
	void SetSolid(bool solid) {GameObject::solid = solid;}
	void SetMove(bool movable) {GameObject::movable = movable;}
	void SetImage(ALLEGRO_BITMAP *image) {GameObject::image = image;}
	void SetAnim(int ANIMATION) {GameObject::ANIMATION = ANIMATION;}
};
