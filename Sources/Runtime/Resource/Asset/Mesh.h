/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "array"
#include "vector"
#include "memory"

#include "Modules/Rendering/Buffers/VertexArray.h"
#include "Modules/Rendering/Buffers/IndexBuffer.h"
#include "Resource/Asset/IMesh.h"
#include "Modules/Rendering/Geometry/Vertex.h"
#include "Modules/Rendering/Geometry/BoundingSphere.h"

namespace LittleEngine
{

	
	/**
	* Standard mesh of LittleEngine::Rendering
	*/
	class Mesh : public IMesh
	{
	public:
		/**
		* Create a mesh with the given vertices, indices and material index
		* @param p_vertices
		* @param p_indices
		* @param p_materialIndex
		*/
		Mesh(Rendering::Geometry::VertexDataBuffer& p_vertices, const std::vector<uint32_t>& p_indices,
			uint32_t p_materialIndex,
			Rendering::Geometry::EVertexDataFlags p_dataFlag ,
			bool p_isReadWrite);
		~Mesh();
/*
		Mesh(const Mesh& mesh);
		Mesh& operator=(const Mesh& r);
		
		*/
		virtual void Rebind(bool p_recreateBound = false);
		/**
		* Bind the mesh (Actually bind its VAO)
		*/
		virtual void Bind() override;

		/**
		* Unbind the mesh (Actually unbind its VAO)
		*/
		virtual void Unbind() override;

		/**
		* Returns the number of vertices
		*/
		virtual uint32_t GetVertexCount() override;

		/**
		* Returns the number of indices
		*/
		virtual uint32_t GetIndexCount() override;

		/**
		* Returns the material index of the mesh
		*/
		uint32_t GetMaterialIndex() const;

		/**
		* Returns the bounding sphere of the mesh
		*/
		const LittleEngine::Rendering::Geometry::BoundingSphere& GetBoundingSphere() const;
		
		float* GetAnimatedPositions()const;
		float* GetPositions() const;
		float* GetBoneWeights() const;
		int* GetBoneIds() const;
		int GetPositionBufferSize();
		Rendering::Geometry::EVertexDataFlags GetDataFlags(){ return m_dataFlags;} 
	private:
		void ComputeBoundingSphere();

	private:
		const uint32_t m_vertexCount;
		const uint32_t m_indicesCount;
		const uint32_t m_materialIndex;

		Rendering::Buffers::VertexArray							m_vertexArray;
		std::unique_ptr<Rendering::Buffers::IndexBuffer>			m_indexBuffer;
		
		std::unique_ptr<Rendering::Buffers::VertexBuffer<float>>	m_vertexBuffer[VERTEX_DATA_FLAGS_INDEX_COUNT];
		// malloc datas ,should be released correct
		void*  m_dataPtrs[VERTEX_DATA_FLAGS_INDEX_COUNT];
		float* m_animatedPositions;
		Rendering::Geometry::EVertexDataFlags m_dataFlags;
		
		Rendering::Geometry::BoundingSphere m_boundingSphere;

	public:

		bool isSkinMesh;
		bool isReadWriteable;
		
		void* GetDataPtrs(Rendering::Geometry::EVertexDataFlagsIndex typeIdx);
	};
}
