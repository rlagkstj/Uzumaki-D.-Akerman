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

    float range = 80.0f;
    float height = 60.0f;

    Rectangle hitbox;

    float punch_offset = 100;

    if (!player->flipped) {
        hitbox = {
            player->position.x,
            player->position.y - height / 2 - punch_offset,
            range,
            height
        };
    }
    else {
        hitbox = {
            player->position.x - range,
            player->position.y - height / 2 - punch_offset,
            range,
            height
        };
    }

    Rectangle targetBox = {
        target->position.x - target->size.x / 2,
        target->position.y - target->size.y / 2,
        target->size.x,
        target->size.y
    };

    if (CheckCollisionRecs(hitbox, targetBox)) {
        target->TakeDamage(damage);
        std::cout << "Hit" << std::endl;
    }
    DrawRectangle(hitbox.x, hitbox.y, hitbox.width, hitbox.height, RED);
}

void Player::TakeDamage(float damage) {
    hp -= damage;
    std::cout << hp << std::endl;
}

void Player::Update(float dt)
{
    last_position = position;
    switch (playerID) {
    case 1: //WASD
        if (IsKeyDown(KEY_A)) {
            velocity.x = x_speed;
            flipped = true;
        }
        if (IsKeyDown(KEY_D)) {
            velocity.x = x_speed;
            flipped = false;
        }
        if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D))
            velocity.x = 0;
        if (IsKeyDown(KEY_W)) jump(dt);
        break;
    case 2: //Arrow
        if (IsKeyDown(KEY_LEFT)) {
            velocity.x = x_speed;
            flipped = true;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            velocity.x = x_speed;
            flipped = false;
        }
        if (!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))
            velocity.x = 0;
        if (IsKeyDown(KEY_UP)) jump(dt);
        break;
    }
    for (int i = 0; i < 4; i++) {
        if (skills[i] != nullptr) {
            if (IsKeyPressed(skillKeys[i])) {
                skills[i]->Use(this);
            }
        }
    }

    if (velocity.x != 0)
        position.x += velocity.x * (flipped ? -1 : 1) * dt;
    
    if (IsKeyPressed(KEY_U)) {
        position.y -= 100;
    }
    if (IsKeyPressed(KEY_H)) {
        position.x -= 100;
    }
    if (IsKeyPressed(KEY_J)) {
        position.y += 100;
    }
    if (IsKeyPressed(KEY_K)) {
        position.x += 100;
    }


    CheckCollisionBlock(dt);
   
    
    
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

    }

    if (!grounded) {
        position.y = nextY;
    }

    IsGround = grounded;
}

Player::Player(int id, Vector2 pos) {
    playerID = id;
    position = pos;
    skills[1] = new Punch();

    if (playerID == 1) {
        skillKeys[1] = KEY_G;
    }
    else if (playerID == 2) {
        skillKeys[1] = KEY_KP_1; 
    }
    
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

            /*Rectangle tileRect = {
                (float)x * tileSize,
                (float)y * tileSize,
                (float)tileSize,
                (float)tileSize
            };

            if (CheckCollisionRecs(NowHitbox, tileRect) && (tile == '=' || tile == 'L' || tile == 'R')) {

                float overlapX = 0;
                float overlapY = 0;

                if (NowHitbox.x < tileRect.x) {
                    overlapX = (NowHitbox.x + NowHitbox.width) - tileRect.x;
                }
                else {
                    overlapX = NowHitbox.x - (tileRect.x + tileRect.width);
                }

                if (NowHitbox.y < tileRect.y) {
                    overlapY = (NowHitbox.y + NowHitbox.height) - tileRect.y;
                }
                else {
                    overlapY = NowHitbox.y - (tileRect.y + tileRect.height);
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
            */
            

            
        }
    }
    ApplyGravity(dt);

}

void Object::Draw() {
	
}

void Player::Draw() {

    Rectangle src = {
        0, 0,
        (float)texture.width * (flipped ? -1 : 1),
        (float)texture.height
    };

    DrawTextureRec(
        texture,
        src,
        { position.x + offsetX, position.y + offsetY },
        WHITE
    );

    Rectangle body = GetHitBox();
    DrawRectangleLines(body.x, body.y, body.width, body.height, GREEN);

    DrawCircle(position.x, position.y, 3, RED);
}

void Player::jump(float dt)
{
    if (IsGround) {
        state = State::Jump;
        velocity.y = jumpPower;
        IsGround = false;
    }
}