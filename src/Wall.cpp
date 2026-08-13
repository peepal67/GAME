#include "Wall.h"

Wall::Wall(Vector3 pos, Vector3 sz, Color col, Texture2D* texture)
    : position(pos), size(sz)
{
    Mesh m = GenMeshCube(size.x, size.y, size.z);
    model = LoadModelFromMesh(m);

    if (texture != nullptr) {
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = *texture;
    } else {
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = col;
    }
}

Wall::~Wall() {
    UnloadModel(model);
}

void Wall::Draw() const {
    DrawModel(model, position, 1.0f, WHITE);
}

BoundingBox Wall::GetBoundingBox() const {
    return {
        { position.x - size.x/2, position.y - size.y/2, position.z - size.z/2 },
        { position.x + size.x/2, position.y + size.y/2, position.z + size.z/2 }
    };
}