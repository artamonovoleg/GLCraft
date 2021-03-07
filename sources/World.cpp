#include "World.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

World::World(const Camera& camera)
    : m_Camera(camera)
{
    m_Shader = std::make_shared<Shader>("../shaders/vert.glsl", "../shaders/frag.glsl");
    m_Texture = std::make_shared<Texture>("../assets/textures/atlas.png");
    m_Shader->SetInt("texture1", 0);

    for (int iz = 0; iz < 3; iz++)
    {
        for (int ix = 0; ix < 3; ix++)
            m_Data.insert({{ ix, 0, iz }, Chunk({ ix * 16, 0, iz * 16 })});
    }
}

Chunk* World::GetChunk(const glm::vec3& position)
{
    if (m_Data.find(GlobalToChunkPosition(position)) == m_Data.end()) return nullptr;
    return &m_Data.at(GlobalToChunkPosition(position));
}

Voxel* World::Get(int ix, int iy, int iz)
{
    auto* ch = GetChunk({ix, iy, iz});
    if (ch == nullptr) return nullptr;
    return &ch->m_ChunkData.At(GlobalVoxelToLocal({ ix, iy, iz }));
}

void World::Draw()
{
    m_Texture->Bind();
    m_Shader->Bind();
    m_Shader->SetMat4("u_Model", glm::mat4(1.0));
    m_Shader->SetMat4("u_View", m_Camera.GetViewMatrix());
    m_Shader->SetMat4("u_Projection", m_Camera.GetProjectionMatrix());

    for (auto& p : m_Data)
        p.second.Draw();
}

Voxel* World::Raycast(const glm::vec3& orig, const glm::vec3& dir, float maxDist,  glm::vec3& norm, glm::vec3& iend)
{
    float px = orig.x;
    float py = orig.y;
    float pz = orig.z;

    float dx = dir.x;
    float dy = dir.y;
    float dz = dir.z;

    float t = 0.0f;
    int ix = floor(px);
    int iy = floor(py);
    int iz = floor(pz);

    float stepx = (dx > 0.0f) ? 1.0f : -1.0f;
    float stepy = (dy > 0.0f) ? 1.0f : -1.0f;
    float stepz = (dz > 0.0f) ? 1.0f : -1.0f;

    float infinity = std::numeric_limits<float>::infinity();

    float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
    float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
    float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);

    float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
    float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
    float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);

    float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
    float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
    float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;

    int steppedIndex = -1;

    while (t <= maxDist)
    {
        
        Voxel* voxel = Get(ix, iy, iz);

        if (voxel == nullptr || voxel->id)
        {
            iend.x = ix;
            iend.y = iy;
            iend.z = iz;

            norm.x = norm.y = norm.z = 0.0f;
            if (steppedIndex == 0) norm.x = -stepx;
            if (steppedIndex == 1) norm.y = -stepy;
            if (steppedIndex == 2) norm.z = -stepz;
            return voxel;
        }

        if (txMax < tyMax) 
        {
            if (txMax < tzMax) 
            {
                ix += stepx;
                t = txMax;
                txMax += txDelta;
                steppedIndex = 0;
            } 
            else 
            {
                iz += stepz;
                t = tzMax;
                tzMax += tzDelta;
                steppedIndex = 2;
            }
        } 
        else 
        {
            if (tyMax < tzMax) 
            {
                iy += stepy;
                t = tyMax;
                tyMax += tyDelta;
                steppedIndex = 1;
            } 
            else 
            {
                iz += stepz;
                t = tzMax;
                tzMax += tzDelta;
                steppedIndex = 2;
            }
        }
    }
    iend.x = ix;
    iend.y = iy;
    iend.z = iz;

    norm.x = norm.y = norm.z = 0.0f;

    return nullptr;
}