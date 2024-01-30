#pragma once

#include "daengine/core/containers.h"
#include "graphics_pipeline.h"
#include "graphics_skeletal_vertex.h"
#include "graphics_renderable.h"
#include "graphics_material_data.h"

namespace da::graphics
{

#define MAX_BONE_INFLUENCE 4

	struct FBoneInfo
	{
		int id;
		glm::mat4 offset;
	};

	struct FSkeletalMesh
	{
		std::vector<FSkeletalVertexBase> Vertices;
		std::vector<uint32_t> Indices;
		size_t MaterialIndex;
		std::unordered_map<CHashString, FBoneInfo> BoneMap;
		int BoneCounter;
	};

	class CSkeletalMesh
	{
	public:
		CSkeletalMesh(const std::string& path, bool inverseNormals = false);
		inline CSkeletalMesh() {};
		virtual ~CSkeletalMesh();

		inline const std::vector<FSkeletalMesh>& getMeshes() const { return m_meshes; };
		FMaterialData& getMaterial(size_t index);
		inline size_t getMaterialCount() const { return m_materials.size(); }
		inline const std::vector<FMaterialData>& getMaterials() const { return m_materials; };

		inline virtual const void* getNativeVBIndex(size_t index) const { return nullptr; };
		inline virtual const void* getNativeIBIndex(size_t index) const { return nullptr; };
		inline bool getCastShadows() const { return m_castShadows; };
		inline void castShadows(bool b) { m_castShadows = b; }

		inline bool getHidden() const { return m_hidden; };
		inline void hide(bool b) { m_hidden = b; }

		inline const std::string& getPath() const { return m_path; }

	private:
		void setVertexBoneData(FSkeletalVertexBase& vertex, int boneID, float weight) const;
		
	private:
		std::string m_path;

	protected:
		std::vector<FSkeletalMesh> m_meshes;
		std::vector<FMaterialData> m_materials;
		bool m_castShadows = true;
		bool m_hidden = false;

	};
}
