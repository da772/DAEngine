#pragma once

#if 0
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"
#include "LightList.h"
#include <glm/matrix.hpp>
#include <bgfx/bgfx.h>
#include <bx/allocator.h>

struct aiMesh;
struct aiMaterial;
struct aiCamera;

class Scene
{
public:
    Scene();

    static void init();

    // load meshes, materials, camera from .gltf file
    bool load(const char* file);
    void clear();

    bool loaded = false;
    glm::vec3 minBounds;
    glm::vec3 maxBounds;
    glm::vec3 center;
    float diagonal;
    Camera camera;
    std::vector<Mesh> meshes;
    std::vector<Material> materials;

    // these are not populated by load
    glm::vec3 skyColor;
    AmbientLight ambientLight;
    PointLightList pointLights;

private:
    static bx::DefaultAllocator allocator;

    Mesh loadMesh(const aiMesh* mesh); // not static because it changes minBounds and maxBounds
    static Material loadMaterial(const aiMaterial* material, const char* dir);
    static Camera loadCamera(const aiCamera* camera);

    static bgfx::TextureHandle loadTexture(const char* file, bool sRGB = false);
};
#endif