#include "Player.h"

Player::Player(Vector3 startPos) {
    camera.position = startPos;
    camera.target = { startPos.x, startPos.y, startPos.z - 1 };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

void Player::Update(float dt) {
    UpdateCamera(&camera, CAMERA_FIRST_PERSON);
}