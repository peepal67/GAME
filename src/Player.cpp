#include "Player.h"
#include "raymath.h"

Player::Player(Vector3 startPos) {
    camera.position = startPos;
    camera.target = { startPos.x, startPos.y, startPos.z - 1 };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

bool Player::WouldCollide(Vector3 candidatePosition,
                          const std::vector<BoundingBox>& solidObstacles) const {
    for (const BoundingBox& obstacle : solidObstacles) {
        bool overlapsX = candidatePosition.x + collisionRadius > obstacle.min.x &&
                         candidatePosition.x - collisionRadius < obstacle.max.x;
        bool overlapsY = candidatePosition.y > obstacle.min.y &&
                         candidatePosition.y - collisionHeight < obstacle.max.y;
        bool overlapsZ = candidatePosition.z + collisionRadius > obstacle.min.z &&
                         candidatePosition.z - collisionRadius < obstacle.max.z;

        if (overlapsX && overlapsY && overlapsZ) return true;
    }

    return false;
}

void Player::Update(float dt, const std::vector<BoundingBox>& solidObstacles) {
    (void)dt;

    Vector3 previousPosition = camera.position;
    UpdateCamera(&camera, CAMERA_FIRST_PERSON);

    // Keep the new look direction, but resolve walking along one horizontal
    // axis at a time. This blocks walls while still allowing the player to
    // slide naturally along their side.
    Vector3 desiredPosition = camera.position;
    Vector3 viewOffset = Vector3Subtract(camera.target, camera.position);
    Vector3 resolvedPosition = previousPosition;

    Vector3 xMove = { desiredPosition.x, desiredPosition.y, previousPosition.z };
    if (!WouldCollide(xMove, solidObstacles)) {
        resolvedPosition.x = xMove.x;
    }

    Vector3 zMove = { resolvedPosition.x, desiredPosition.y, desiredPosition.z };
    if (!WouldCollide(zMove, solidObstacles)) {
        resolvedPosition.z = zMove.z;
    }

    resolvedPosition.y = desiredPosition.y;
    camera.position = resolvedPosition;
    camera.target = Vector3Add(resolvedPosition, viewOffset);
}
