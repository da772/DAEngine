#pragma once

#include "dastd.h"
#include "core/factory.h"
#include "graphics_pipeline.h"
#include "graphics_skeletal_vertex.h"
#include "graphics_renderable.h"
#include "graphics_material_data.h"
#include "dastd.h"
#include "daglm.h"
#include "dacommon.h"

namespace da
{

#define MAX_BONE_INFLUENCE 4

	struct FBoneInfo
	{
		int id;
		glm::mat4 offset;

		bool operator==(const FBoneInfo& rhs) const
		{
			return id == rhs.id && offset == rhs.offset;
		}
	};

	struct FSkeletalMesh
	{
		std::vector<FSkeletalVertexBase> Vertices;
		std::vector<uint32_t> Indices;
		size_t MaterialIndex;
	};

	class CSkeletalMesh
	{
	public:

		inline const std::vector<FSkeletalMesh>& getMeshes() const { return m_meshes; };
		FMaterialData& getMaterial(size_t index);
		inline size_t getMaterialCount() const { return m_materials.size(); }
		inline const std::vector<FMaterialData>& getMaterials() const { return m_materials; };

		inline virtual const void* getNativeVBIndex(size_t index) const { return nullptr; };
		inline virtual const void* getNativeIBIndex(size_t index) const { return nullptr; };
		inline bool getCastShadows() const { return m_castShadows; };
		inline void castShadows(bool b) { m_castShadows = b; }
		inline const std::unordered_map<CHashString, FBoneInfo>& getBoneMap() const { return m_boneMap; }
		const FBoneInfo& addBone(CHashString name);

		inline bool getHidden() const { return m_hidden; };
		inline void hide(bool b) { m_hidden = b; }

		inline const std::string& getPath() const { return m_path; }
		inline virtual void setBuffers(size_t index, uint8_t stream) {};

	protected:
		CSkeletalMesh(const std::string& path, bool binary, bool inverseNormals = false);
		inline CSkeletalMesh() {};
		virtual ~CSkeletalMesh();

	private:
		void setVertexBoneData(FSkeletalVertexBase& vertex, int boneID, float weight) const;

	private:
		std::string m_path;

	protected:
		int m_boneCount = 0;
		std::vector<FSkeletalMesh> m_meshes;
		std::vector<FMaterialData> m_materials;
		std::unordered_map<CHashString, FBoneInfo> m_boneMap;
		bool m_castShadows = true;
		bool m_hidden = false;

		friend class da::CFactory<CSkeletalMesh>;

	};
}
