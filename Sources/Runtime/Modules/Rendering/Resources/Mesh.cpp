/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <algorithm>

#include "Modules/Rendering/Resources/Mesh.h"

LittleEngine::Rendering::Resources::Mesh::Mesh(Geometry::VertexDataBuffer& p_vertices, const std::vector<uint32_t>& p_indices,
			uint32_t p_materialIndex,
			Geometry::EVertexDataFlags p_dataFlag,
			bool p_isReadWrite ):
	m_vertexCount(static_cast<uint32_t>(p_vertices.Size())),
	m_indicesCount(static_cast<uint32_t>(p_indices.size())),
	m_materialIndex(p_materialIndex),
	m_dataFlags(p_dataFlag)
{
	m_vertexArray.Bind();
	std::cout<< "Create Mesh ====================   " <<std::endl;
	isSkinMesh = (m_dataFlags & (Geometry::EVertexDataFlags::vdf_boneId |  Geometry::EVertexDataFlags::vdf_boneWeight) ) != 0;
	isReadWriteable = p_isReadWrite;
	for (int i=0;i<VERTEX_DATA_FLAGS_INDEX_COUNT;i++)
	{
		m_dataPtrs[i] = nullptr;
		m_vertexBuffer[i] = nullptr;
	}
	m_animatedPositions = nullptr;

	for (int i =0;i< VERTEX_DATA_FLAGS_INDEX_COUNT;i++)
	{
		auto& info = Geometry::VertexStructInfos[i];
		auto ptr  = p_vertices.GetDataPtr(info.pos);
		if(ptr != nullptr)
		{
			std::cout<< " create vertexBuffer ctor  ptr= "<<(long)ptr<<"  idx " << i << " info.pos= " << (int)info.pos << std::endl;
			m_vertexBuffer[(int)info.pos] = std::make_unique<Buffers::VertexBuffer<float>>(ptr, m_vertexCount * info.elemCount,  info.elemTypeSize);
		}
	}
	
	if(isReadWriteable)
	{
		for (auto element : Geometry::VertexStructInfos)
		{
			if (element.HasFlag(p_dataFlag)){
				m_dataPtrs[(int)element.pos] = p_vertices.MoveDataPtr(element.pos);
			}
		}
	}
	
	if(!isReadWriteable &&isSkinMesh){
		// anim need raw position data
		m_dataPtrs[(int)Geometry::EVertexDataFlagsIndex::position] = p_vertices.MoveDataPtr(Geometry::EVertexDataFlagsIndex::position);
	}
	
	if(isSkinMesh)
	{
		auto totalSize= m_vertexCount * Geometry::VertexStructInfos[(int)Geometry::EVertexDataFlagsIndex::position].MemorySize();
		m_animatedPositions = (float*)malloc( totalSize);
		memcpy(m_animatedPositions,m_dataPtrs[(int)Geometry::EVertexDataFlagsIndex::position] ,totalSize);
	}
	m_indexBuffer= std::make_unique<Buffers::IndexBuffer>(const_cast<uint32_t*>(p_indices.data()), p_indices.size());

	// bind
	for (int i =0;i< VERTEX_DATA_FLAGS_INDEX_COUNT;i++)
	{
		auto& info = Geometry::VertexStructInfos[i];
		if(m_vertexBuffer[(int)info.pos] != nullptr)
		{
			Buffers::VertexBuffer<float>& ptr = *m_vertexBuffer[(int)info.pos];
			std::cout<< " BindAttribute  VertexBufferId= "<<ptr.GetID() <<"  idx " << i << " info.pos= " << (int)info.pos << std::endl;
			m_vertexArray.BindAttribute(i, ptr,  info.elemType, info.elemCount, info.MemorySize(), 0);
		}
	}
	m_vertexArray.Unbind();
	ComputeBoundingSphere();
}




LittleEngine::Rendering::Resources::Mesh::~Mesh()
{
	for (int i = 0; i<(int) Geometry::EVertexDataFlagsIndex::count; i++)
	{
		if(m_dataPtrs[i] != nullptr)
		{
			//std::cout<<"~MeshFree ptr "<<i <<" " <<(long)m_dataPtrs[i] <<std::endl;
			free(m_dataPtrs[i]);
		}
		m_dataPtrs[i] = nullptr;
	}
	if(m_animatedPositions != nullptr)
	{
		//std::cout<<"~MeshFree ptr m_animatedPositions " <<(long)m_animatedPositions <<std::endl;
		free(m_animatedPositions);
	}
	m_animatedPositions = nullptr;
		/**/
}
void* LittleEngine::Rendering::Resources::Mesh::GetDataPtrs(Geometry::EVertexDataFlagsIndex typeIdx)
{
	if(m_dataPtrs[(int)typeIdx] != nullptr)
	{
		return m_dataPtrs[(int)typeIdx];
	}
}
void LittleEngine::Rendering::Resources::Mesh::Rebind(bool p_recreateBound)
{
	Bind();
	auto& info =  Geometry::VertexStructInfos[(int)Geometry::EVertexDataFlagsIndex::position];
	auto& posBuffer = m_vertexBuffer[(int)Geometry::EVertexDataFlagsIndex::position];
	posBuffer->Rebind(GetAnimatedPositions(),m_vertexCount * info.elemCount,  info.elemTypeSize);
	if(p_recreateBound)	ComputeBoundingSphere();
	Unbind();
}
void LittleEngine::Rendering::Resources::Mesh::Bind()
{
	m_vertexArray.Bind();
}

void LittleEngine::Rendering::Resources::Mesh::Unbind()
{
	m_vertexArray.Unbind();
}

uint32_t LittleEngine::Rendering::Resources::Mesh::GetVertexCount()
{
	return m_vertexCount;
}

uint32_t LittleEngine::Rendering::Resources::Mesh::GetIndexCount()
{
	return m_indicesCount;
}

uint32_t LittleEngine::Rendering::Resources::Mesh::GetMaterialIndex() const
{
	return m_materialIndex;
}

const LittleEngine::Rendering::Geometry::BoundingSphere& LittleEngine::Rendering::Resources::Mesh::GetBoundingSphere() const
{
	return m_boundingSphere;
}

float* LittleEngine::Rendering::Resources::Mesh::GetPositions() const{return (float*)(m_dataPtrs[(int)Geometry::EVertexDataFlagsIndex::position]);}

float* LittleEngine::Rendering::Resources::Mesh::GetBoneWeights()const{return (float*)(m_dataPtrs[(int)Geometry::EVertexDataFlagsIndex::boneWeight]);}

int* LittleEngine::Rendering::Resources::Mesh::GetBoneIds() const{return (int*)(m_dataPtrs[(int)Geometry::EVertexDataFlagsIndex::boneId]);}

float* LittleEngine::Rendering::Resources::Mesh::GetAnimatedPositions()const{return m_animatedPositions;}

int LittleEngine::Rendering::Resources::Mesh:: GetPositionBufferSize(){
	return m_vertexCount * Geometry::VertexStructInfos[(int)Geometry::EVertexDataFlagsIndex::position].MemorySize();
}

void LittleEngine::Rendering::Resources::Mesh::ComputeBoundingSphere()
{
	m_boundingSphere.position = LittleEngine::FVector3::Zero;
	m_boundingSphere.radius = 0.0f;
	auto p_vertices = (float*)m_dataPtrs[(int)Geometry::EVertexDataFlagsIndex::position] ;
	if (p_vertices != nullptr) 
	{
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float minZ = std::numeric_limits<float>::max();

		float maxX = std::numeric_limits<float>::min();
		float maxY = std::numeric_limits<float>::min();
		float maxZ = std::numeric_limits<float>::min();

		for (uint32_t i =0;i< m_vertexCount;i++)
		{
			auto position = &p_vertices[i*3];
			minX = std::min(minX, position[0]);
			minY = std::min(minY, position[1]);
			minZ = std::min(minZ, position[2]);

			maxX = std::max(maxX, position[0]);
			maxY = std::max(maxY, position[1]);
			maxZ = std::max(maxZ, position[2]);
		}

		m_boundingSphere.position = LittleEngine::FVector3{ minX + maxX, minY + maxY, minZ + maxZ } / 2.0f;

		for (uint32_t i =0;i< m_vertexCount;i++)
		{
			LittleEngine::FVector3 position = *(reinterpret_cast<const LittleEngine::FVector3*> (&p_vertices[i*3]));
			m_boundingSphere.radius = std::max(m_boundingSphere.radius, LittleEngine::FVector3::Distance(m_boundingSphere.position, position));
		}
	}
}
