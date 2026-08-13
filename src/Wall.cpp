#include "Wall.h"
#include <cmath>

Wall::Wall(Vector3 pos, Vector3 sz, Color col, Texture2D* texture)
    : position(pos), size(sz)
{
    Mesh m = GenMeshCube(size.x, size.y, size.z);

    // Repeat the texture across long walls instead of stretching one giant
    // copy from end to end.
    const float tilesAcross = fmaxf(size.x, size.z) / 2.0f;
    const float tilesHigh = fmaxf(size.y, 1.0f) / 2.0f;
    for (int vertex = 0; vertex < m.vertexCount; vertex++) {
        m.texcoords[vertex * 2] *= tilesAcross;
        m.texcoords[vertex * 2 + 1] *= tilesHigh;
    }

    model = LoadModelFromMesh(m);

    if (texture != nullptr) {
        SetMaterialTexture(&model.materials[0], MATERIAL_MAP_ALBEDO, *texture);
        // Textures are multiplied by this material colour. Explicitly using
        // white preserves the colours from the jpg instead of turning them
        // black.
        model.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
    } else {
        model.materials[0].maps[MATERIAL_MAP_ALBEDO].color = col;
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
