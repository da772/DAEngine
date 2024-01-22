#pragma once

#include "daengine/core/containers.h"
#include "graphics_pipeline.h"
#include "graphics_vertex.h"
#include "graphics_renderable.h"
#include "graphics_material_data.h"

namespace da::graphics
{

	struct FMesh
	{
		std::vector<FVertexBase> Vertices;
		std::vector<uint32_t> Indices;
		size_t MaterialIndex;
	};

	class CStaticMesh
	{
	public:
		CStaticMesh(const std::string& path);
		inline CStaticMesh() {};
		virtual ~CStaticMesh();

		inline const std::vector<FMesh>& getMeshes() const { return m_meshes; };
		inline const std::vector<FMaterialData>& getMaterials() const { return m_materials; };
		
		inline virtual const void* getNativeVBIndex(size_t index) const { return nullptr; };
		inline virtual const void* getNativeIBIndex(size_t index) const { return nullptr; };

		inline const std::string& getPath() const { return m_path; }

	private:
		std::string m_path;

	protected:
		std::vector<FMesh> m_meshes;
		std::vector<FMaterialData> m_materials;

	};
}
