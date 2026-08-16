#include "Room.h"

Room::~Room() {
    for (auto* w : walls) delete w;
    for (auto* d : doors) delete d;
    for (auto* p : pickups) delete p;
}

void Room::BuildRoomWithDoor(Vector3 center, float width, float depth, float height,
                              float doorWidth, float doorHeight, bool doorLocked, int requiredKeyId,
                              Texture2D* wallTexture, Texture2D* doorTexture,
                              Texture2D* floorTexture, Texture2D* ceilingTexture,
                              const char* doorOpenSoundPath,
                              bool doorOnNearSide,
                              bool buildFarWall,
                              float wallThickness)
{
    float wallCenterY  = center.y + height / 2.0f;
    float halfW         = width / 2.0f;
    float halfD         = depth / 2.0f;
    float sideWallWidth = (width - doorWidth) / 2.0f;
    float topWallHeight = height - doorHeight;
    float doorCenterY   = center.y + doorHeight / 2.0f;

    walls.push_back(new Wall({center.x - halfW, wallCenterY, center.z},
                              {wallThickness, height, depth}, WHITE, wallTexture));
    walls.push_back(new Wall({center.x + halfW, wallCenterY, center.z},
                              {wallThickness, height, depth}, WHITE, wallTexture));

    float doorWallZ  = doorOnNearSide ? (center.z + halfD) : (center.z - halfD);
    float otherWallZ = doorOnNearSide ? (center.z - halfD) : (center.z + halfD);

    walls.push_back(new Wall({center.x - (doorWidth + sideWallWidth) / 2.0f, wallCenterY, doorWallZ},
                              {sideWallWidth, height, wallThickness}, WHITE, wallTexture));
    walls.push_back(new Wall({center.x + (doorWidth + sideWallWidth) / 2.0f, wallCenterY, doorWallZ},
                              {sideWallWidth, height, wallThickness}, WHITE, wallTexture));

    if (topWallHeight > 0.01f) {
        walls.push_back(new Wall({center.x, doorHeight + topWallHeight / 2.0f, doorWallZ},
                                  {doorWidth, topWallHeight, wallThickness}, WHITE, wallTexture));
    }

    doors.push_back(new Door({center.x, doorCenterY, doorWallZ},
                              {doorWidth, doorHeight, 0.18f}, doorLocked, requiredKeyId,
                              doorTexture, doorOpenSoundPath));

    if (buildFarWall) {
        walls.push_back(new Wall({center.x, wallCenterY, otherWallZ},
                                  {width, height, wallThickness}, WHITE, wallTexture));
    }

    Wall* floor = new Wall({center.x, center.y, center.z},
                            {width, wallThickness, depth}, WHITE, floorTexture);
    floor->isFloorOrCeiling = true;
    walls.push_back(floor);

    Wall* roof = new Wall({center.x, center.y + height, center.z},
                           {width, wallThickness, depth}, WHITE, ceilingTexture);
    roof->isFloorOrCeiling = true;
    walls.push_back(roof);
}

void Room::BuildHallway(Vector3 center, float width, float depth, float height,
                         Texture2D* wallTexture, Texture2D* floorTexture, Texture2D* ceilingTexture,
                         float wallThickness)
{
    float wallCenterY = center.y + height / 2.0f;
    float halfW = width / 2.0f;

    walls.push_back(new Wall({center.x - halfW, wallCenterY, center.z},
                              {wallThickness, height, depth}, WHITE, wallTexture));
    walls.push_back(new Wall({center.x + halfW, wallCenterY, center.z},
                              {wallThickness, height, depth}, WHITE, wallTexture));

    Wall* floor = new Wall({center.x, center.y, center.z},
                            {width, wallThickness, depth}, WHITE, floorTexture);
    floor->isFloorOrCeiling = true;
    walls.push_back(floor);

    Wall* roof = new Wall({center.x, center.y + height, center.z},
                           {width, wallThickness, depth}, WHITE, ceilingTexture);
    roof->isFloorOrCeiling = true;
    walls.push_back(roof);
}

void Room::DrawRoomWithDoor() const {
    for (auto* w : walls) w->Draw();
    for (auto* d : doors) d->Draw();
    for (auto* p : pickups) p->Draw();
}

void Room::DrawHallway() const {
    for (auto* w : walls) w->Draw();
    for (auto* p : pickups) p->Draw();
}

void Room::AddPickup(Pickup* pickup) {
    pickups.push_back(pickup);
}

std::vector<BoundingBox> Room::GetSolidObstacles() const {
    std::vector<BoundingBox> obstacles;
    obstacles.reserve(walls.size() + doors.size());
    for (auto* w : walls) {
        if (w->isFloorOrCeiling) continue;
        obstacles.push_back(w->GetBoundingBox());
    }
    for (auto* d : doors) {
        if (d->IsBlocking()) obstacles.push_back(d->GetBoundingBox());
    }
    return obstacles;
}