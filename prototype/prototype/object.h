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

	Vector2 last_position{ 0, 0 };

	Vector2 size{ 64, 120 };

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
	float x_speed = 800;
	State state = State::Idle;
	bool flipped = false;
	virtual void TakeDamage(float damage) {};

	

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
	Punch() {
		damage = 15;
		cooldown = 0.2f;
	}

	void Use(Character* character) override;
};

class Player : public Character {
public:
	float offsetX = -85;  
	float offsetY = -140; 
	int playerID; //1,2
	float displayHP = 100;

	Player(int id, Vector2 position);
	Skill* skills[4] = { nullptr, nullptr, nullptr, nullptr };

	void ApplyGravity(float dt);
	void MoveX(float dt);


	void Update(float dt) override;
	void CheckCollisionBlock(float dt);
	void Draw() override;	
	
	void Move();
	void TakeDamage(float damage) override;

	void UseSkill(int index);

	void HPBar();

	Character* target = nullptr;

	// 임시
	Texture2D texture = LoadTexture("player.png");
	Texture2D attackTexture = LoadTexture("player_punch.png");
	Texture2D kickTexture = LoadTexture("player_kick.png");

	Texture2D playerNow = attackTexture;


	int attackStat = -1;

	Map* map = nullptr;

	int currentFrame = 0;
	float frameTime = 0.0f;
	float frameSpeed = 0.02f; // FRAME

	bool isAttacking = false;
private:
	// 1번 G,1 || 2번 T,4 || 3번 H,2 || 4번 J,3
	float player_cool[4] = { 0 };
	char skillKeys[4] = { 0 };
	bool passPlatform = false;
	double passTimer = 0;

	bool hasHit = false;

	class State {
	public:
		virtual void Enter(Player* player) = 0;
		virtual void Update(Player* player, double dt) = 0;
		virtual void CheckExit(Player* player) = 0;
		virtual std::string GetName() = 0;
	};

	class State_Idle : public State {
	public:
		virtual void Enter(Player* player) override;
		virtual void Update(Player* player,double dt) override;
		virtual void CheckExit(Player* player) override;
		std::string GetName() override { return "Idle"; }
	};

	State_Idle state_idle;

	class State_Jumping : public State {
	public:
		virtual void Enter(Player* player) override;
		virtual void Update(Player* player,double dt) override;
		virtual void CheckExit(Player* player) override;
		std::string GetName() override { return "Jumping"; }
	};

	State_Jumping state_jumping;

	class State_Running : public State {
	public:
		virtual void Enter(Player* player) override;
		virtual void Update(Player* player,double dt) override;
		virtual void CheckExit(Player* player) override;
		std::string GetName() override { return "Running"; }
	};

	State_Running state_running;

	class State_Attack : public State {
	public:
		virtual void Enter(Player* player) override;
		virtual void Update(Player* player, double dt) override;
		virtual void CheckExit(Player* player) override;
		std::string GetName() override { return "Attacking"; }
	};

	State_Attack state_attacking;

	void change_state(State* new_state);
	State* current_state = nullptr;

};