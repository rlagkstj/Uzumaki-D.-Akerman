#pragma once
#include<iostream>
#include<vector>
#include "raylib.h"
#include "map.h"


#define FLOOR_Y 500

enum class State {
	Idle,
	Move,
	Attack,
	Hit,
	Jump
};

class Object {
public:

	Vector2 position{ 0,0 };
	Vector2 velocity{ 0,0 };
	bool active = true;


	virtual void Update(float dt) {};
	virtual void Draw() ;

	virtual ~Object() {};

private:
};

class Character : public Object {
public:

	Vector2 size{ 64, 160 };

	Vector2 hotspot{ size.x / 2,size.y / 2 };

	Rectangle GetHitBox() {
		return {
			position.x - size.x / 2,
			position.y - size.y,
			size.x,
			size.y
		};
	}

	float hp = 100;
	float speed = 200;

	State state = State::Idle;
	bool flipped = false;
	virtual void TakeDamage(float damage) {};

	//jump
	virtual void jump(float dt) {};
	float gravity = 1800;
	float jumpPower = -1300;
	bool IsGround = true;

	virtual void Update(float dt) override {};

private:
};

class Item : public Object {
public:
	virtual void Effect(Character* character);
};

class Skill{
public: 
	float damage;
	float cooldown;

	virtual void Use(Character* character);
private:
};

class Punch : public Skill {
public:
	float damage = 30;
	void Use(Character* character) override;
};

class Player : public Character {
public:
	float offsetX = -85;  
	float offsetY = -180; 
	int playerID; //1,2

	Player(int id, Vector2 position);
	Skill* skills[4] = { nullptr, nullptr, nullptr, nullptr };

	void jump(float dt) override;
	void ApplyGravity(float dt);

	void Update(float dt) override;

	void Draw() override;	
	
	void Move();
	void TakeDamage(float damage) override;

	Character* target = nullptr;

	// юс╫ц
	Texture2D texture = LoadTexture("player.png");
	Map* map = nullptr;
private:
	int skillKeys[4];
};