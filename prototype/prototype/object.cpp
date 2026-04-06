#include "Object.h"

void Skill::Use(Character* character)
{
    if (character == nullptr) return;
}



void Punch::Use(Character* character)
{
    if (character == nullptr) return;

    Player* player = static_cast<Player*>(character);
    if (player == nullptr) return;

    Character* target = player->target;
    if (target == nullptr) return;

    float range = 60.0f;
    float height = 50.0f;

    Rectangle hitbox;

    float punch_offset_X = 10;
    float punch_offset_Y = 90;

    if (!player->flipped) {
        hitbox = {
            player->position.x + punch_offset_X,
            player->position.y - height / 2 - punch_offset_Y,
            range,
            height
        };
    }
    else {
        hitbox = {
            player->position.x - range - punch_offset_X,
            player->position.y - height / 2 - punch_offset_Y,
            range,
            height
        };
    }

    Rectangle targetBox = {
        target->position.x - target->size.x / 2,
        target->position.y - target->size.y,
        target->size.x,
        target->size.y
    };

    if (CheckCollisionRecs(hitbox, targetBox)) {
        target->TakeDamage(damage);
        std::cout << "Hit" << std::endl;
    }
    DrawRectangle(targetBox.x, targetBox.y, targetBox.width, targetBox.height, GREEN);
    DrawRectangle(hitbox.x, hitbox.y, hitbox.width, hitbox.height, RED);
}

void Player::TakeDamage(float damage) {
    hp -= damage;
    std::cout << hp << std::endl;
}

void Player::Update(float dt)
{
    last_position = position;

    if (current_state != nullptr) {
        current_state->Update(this, dt);
        current_state->CheckExit(this);
    }

    MoveX(dt);
    if (velocity.x != 0)
        position.x += velocity.x * (flipped ? -1 : 1) * dt;

    for (int i = 0; i < 4; i++) {
        player_cool[i] += dt;
    }

    CheckCollisionBlock(dt);



    float speed = 150.0f;



    if (displayHP > hp) {
        displayHP -= speed * dt;
        if (displayHP < hp) displayHP = hp;
    }

}


void Player::UseSkill(int index) {
    if (skills[index] == nullptr) return;
    if (player_cool[index] >= skills[index]->cooldown) {
        skills[index]->Use(this);
        player_cool[index] = 0;
    }
}

void Player::ApplyGravity(float dt)
{
    if (map == nullptr) return;

    velocity.y += gravity * dt;

    float nextY = position.y + velocity.y * dt;

    int tileSize = map->GetTileSize();

    float footXLeft = position.x - size.x / 2 + 2;
    float footXRight = position.x + size.x / 2 - 2;

    float footY = nextY;

    int tileY = footY / tileSize;

    int tileXLeft = footXLeft / tileSize;
    int tileXRight = footXRight / tileSize;

    bool grounded = false;

    for (int tx : { tileXLeft, tileXRight }) {

        if (map->IsPlatform(tx, tileY)) {

            if (velocity.y >= 0) {
                position.y = tileY * tileSize;
                velocity.y = 0;
                grounded = true;
                break;
            }

        }
        if (map->IspassPlatform(tx, tileY) && !passPlatform){
            if (velocity.y >= 0) {
                position.y = tileY * tileSize;
                velocity.y = 0;
                grounded = true;
                
                break;
            }
        }
        if (passPlatform) {
            passTimer -= dt;
            if (passTimer <= 0) {
                passPlatform = false;
            }
        }

    }

    if (!grounded) {
        position.y = nextY;
    }

    IsGround = grounded;
}

Player::Player(int id, Vector2 pos) {
    playerID = id;
    position = pos;
    skills[0] = new Punch();

    if (playerID == 1) {
        skillKeys[1] = KEY_G;
    }
    else if (playerID == 2) {
        skillKeys[1] = KEY_KP_1; 
    }

    current_state = &state_idle;
}

void Player::CheckCollisionBlock(float dt) {
    if (map == nullptr) return;
    
    Rectangle PreHitbox = {
        last_position.x - size.x/2,
        last_position.y - size.y,
        size.x,
        size.y
    };
	Rectangle NowHitbox = GetHitBox();


    int tileSize = map->GetTileSize();

    for (int y = 0; y < map->GetMap().size(); y++) {
        int startX = -1;
        for (int x = 0; x < map->GetMap()[y].size(); x++) {
            char tile = map->GetMap()[y][x];

            if (tile == 'L') {
                startX = x;
            }
            if (tile == 'R' && startX != -1) {
                int endX = x;

                Rectangle platform = {
                    startX * tileSize,
                    y * tileSize,
                    (endX - startX + 1) * tileSize,
                    (float)tileSize
                };

                if (CheckCollisionRecs(NowHitbox, platform)) {

                    float overlapX = 0;
                    float overlapY = 0;

                    if (NowHitbox.x < platform.x) {
                        overlapX = (NowHitbox.x + NowHitbox.width) - platform.x;
                    }
                    else {
                        overlapX = NowHitbox.x - (platform.x + platform.width);
                    }

                    if (NowHitbox.y < platform.y) {
                        overlapY = (NowHitbox.y + NowHitbox.height) - platform.y;
                    }
                    else {
                        overlapY = NowHitbox.y - (platform.y + platform.height);
                    }
                    if (abs(overlapX) < abs(overlapY)) {
                        position.x -= overlapX;
                        velocity.x = 0;
                    }
                    else {
                        position.y -= overlapY;
                        velocity.y = 0;
                    }
                    NowHitbox = GetHitBox();
                }

                startX = -1;
            }
            
        }
    }
    ApplyGravity(dt);

}

void Object::Draw() {
	
}

void Player::Draw() {
    float frameWidth = 1600 / 8.0f; // 200
    float frameHeight = 150;

    Rectangle src;

    if (isAttacking) {
        src = {
            frameWidth * currentFrame + (flipped ? - size.x / 2 + 2 : 0),
            0,
            frameWidth * (flipped ? -1 : 1),
            frameHeight
        };
        DrawTextureRec(
            attackTexture,
            src,
            { position.x + offsetX, position.y + offsetY },
            WHITE
        );
    }
    else {
        src = {
            0,
            0,
            (float)texture.width * (flipped ? -1 : 1),
            frameHeight
        };
        DrawTextureRec(
            attackTexture,
            src,
            { position.x + offsetX, position.y + offsetY },
            WHITE
        );
    }

    //Hitbox
    Rectangle body = GetHitBox();
    DrawRectangleLines(body.x, body.y, body.width, body.height, GREEN);

    DrawCircle(position.x, position.y, 3, RED);

    HPBar();
}


void Player::MoveX(float dt) {

    switch (playerID) {

    case 1:
        if (IsKeyDown(KEY_A)) {
            velocity.x = x_speed;
            flipped = true;
        }
        else if (IsKeyDown(KEY_D)) {
            velocity.x = x_speed;
            flipped = false;
        }
        else {
            velocity.x = 0;
        }
        break;

    case 2:
        if (IsKeyDown(KEY_LEFT)) {
            velocity.x = x_speed;
            flipped = true;
        }
        else if (IsKeyDown(KEY_RIGHT)) {
            velocity.x = x_speed;
            flipped = false;
        }
        else {
            velocity.x = 0;
        }
        break;
    }
}

void Player::HPBar() {
    float barWidth = 750;
    float barHeight = 50;
    float ratio = displayHP / 100.0f;

    if (ratio < 0) ratio = 0;

    float x;
    float y = 30;

    if (playerID == 1) {
        float x = 30;

        DrawRectangle(x, y, barWidth, barHeight, GRAY);
        DrawRectangle(x, y, barWidth * ratio, barHeight, RED);
        DrawRectangleLines(x, y, barWidth, barHeight, BLACK);
    }
    else if(playerID == 2){
        float x = GetScreenWidth() - barWidth - 30;

        DrawRectangle(x, y, barWidth, barHeight, GRAY);

        float currentWidth = barWidth * ratio;
        float drawX = x + (barWidth - currentWidth);

        DrawRectangle(drawX, y, currentWidth, barHeight, RED);
        DrawRectangleLines(x, y, barWidth, barHeight, BLACK);
    }
}

void Player::change_state(State* new_state) {
    current_state = new_state;
    current_state->Enter(this);
}



void Player::State_Idle::Enter(Player* player){}
void Player::State_Idle::Update(Player* player, double dt){}
void Player::State_Idle::CheckExit(Player* player) {
    switch (player->playerID) {
    case 1: {
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
            player->change_state(&player->state_running);
        }
        else if (IsKeyPressed(KEY_W) && player->IsGround) {
            player->change_state(&player->state_jumping);
        }
        else if (IsKeyDown(KEY_S)) {
            player->passPlatform = true;
            player->passTimer = 0.5;
		}
        else if (IsKeyPressed(KEY_G)) {
            player->change_state(&player->state_attacking);
        }
        break;
    }
    case 2: {
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) {
            player->change_state(&player->state_running);
        }
        else if (IsKeyPressed(KEY_UP) && player->IsGround) {
            player->change_state(&player->state_jumping);
        }
        else if (IsKeyDown(KEY_DOWN)) {
            player->passPlatform = true;
            player->passTimer = 0.5;
        }
        else if (IsKeyPressed(KEY_KP_1)) {
            player->change_state(&player->state_attacking);
        }
        break;
    }
    }
}
void Player::State_Jumping::Enter(Player* player) {
    player-> velocity.y = player->jumpPower;
    player->IsGround = false;
}
void Player::State_Jumping::Update(Player* player, double dt) {

}
void Player::State_Jumping::CheckExit(Player* player) {
    if (player->IsGround) {
        switch (player->playerID) {
        case 1:
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
                player->change_state(&player->state_running);
            }
            else {
                player->change_state(&player->state_idle);
            }
            break;
        case 2:
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) {
                player->change_state(&player->state_running);
            }
            else {
                player->change_state(&player->state_idle);
            }
            break;
        }
    }
}
void Player::State_Running::Enter(Player* player) {

}
void Player::State_Running::Update(Player* player, double dt) {
    switch (player->playerID) {

    case 1: {
        if (IsKeyDown(KEY_A)) {
            player->velocity.x = player->x_speed;
            player->flipped = true;
        }
        else if (IsKeyDown(KEY_D)) {
            player->velocity.x = player->x_speed;
            player->flipped = false;
        }
        else if (IsKeyDown(KEY_S)) {
            player->passPlatform = true;
            player->passTimer = 0.5;
        }
        else {
            player->velocity.x = 0;
        }
        break;
    }

    case 2: {
        if (IsKeyDown(KEY_LEFT)) {
            player->velocity.x = player->x_speed;
            player->flipped = true;
        }
        else if (IsKeyDown(KEY_RIGHT)) {
            player->velocity.x = player->x_speed;
            player->flipped = false;
        }
        else if (IsKeyDown(KEY_DOWN)) {
            player->passPlatform = true;
            player->passTimer = 0.5;
        }
        else {
            player->velocity.x = 0;
        }
        break;
    }

    }
}
void Player::State_Running::CheckExit(Player* player) {
    switch (player->playerID) {
    case 1:
        if (IsKeyPressed(KEY_W) && player->IsGround) {
            player->change_state(&player->state_jumping);
        }
        else if (IsKeyPressed(KEY_G)) {
            player->change_state(&player->state_attacking);
        }
        else if (player->velocity.x == 0) {
            player->change_state(&player->state_idle);
        }
        break;
    case 2:
        if (IsKeyPressed(KEY_UP) && player->IsGround) {
            player->change_state(&player->state_jumping);
        }
        else if (IsKeyPressed(KEY_KP_1)) {
            player->change_state(&player->state_attacking);
        }
        else if (player->velocity.x == 0) {
            player->change_state(&player->state_idle);
        }
        break;
    }
}
void Player::State_Attack::Enter(Player* player) {
    player->isAttacking = true;
    player->currentFrame = 0;
    player->frameTime = 0;
    player->hasHit = false;

    player->velocity.x = 0;
}
void Player::State_Attack::Update(Player* player, double dt) {
    player->frameTime += dt;

    if (player->frameTime >= player->frameSpeed) {
        player->frameTime = 0;
        player->currentFrame++;

        if (player->currentFrame >= 3 && !player->hasHit) {
            player->UseSkill(0); 
            player->hasHit = true;
        }
    }


    //player->MoveX(dt);
}
void Player::State_Attack::CheckExit(Player* player) {
    if (player->currentFrame >= 8) {
        player->isAttacking = false;

        switch (player->playerID) {
        case 1:
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
                player->change_state(&player->state_running);
            }
            else {
                player->change_state(&player->state_idle);
            }
            break;

        case 2:
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) {
                player->change_state(&player->state_running);
            }
            else {
                player->change_state(&player->state_idle);
            }
            break;
        }
    }
}