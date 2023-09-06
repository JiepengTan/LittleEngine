/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once
#include <array>
#include <cstdint>
#include "Core/CoreInclude.h"
#include "Modules/Rendering/Buffers/VertexArray.h"

#define VERTEX_DATA_FLAGS_INDEX_COUNT ((int)(LittleEngine::Rendering::Geometry::EVertexDataFlagsIndex::count))
namespace LittleEngine::Rendering::Geometry
{
	enum EVertexDataFlags
	{
		vdf_none		= 0x00,
		
		vdf_position	= 0x01,
		vdf_texCoord	= 0x02,
		vdf_color		= 0x04,
		vdf_normal		= 0x08,
		vdf_tangent		= 0x10,
		vdf_bitangent	= 0x20,
		vdf_boneWeight	= 0x40,
		vdf_boneId		= 0x80,
		
		vdf_all			= 0xffff,
		vdf_allNoBone = vdf_all&(~(vdf_boneWeight|vdf_boneId))
	};

	enum class EVertexDataFlagsIndex
	{
		position	= 0,
		texCoord	= 1,
		color		= 2,
		normal		= 3,
		tangent		= 4,
		bitangent	= 5,
		boneWeight	= 6,
		boneId		= 7,
		
		count		= 8,
	};

	struct VertexStructInfo
	{
		EVertexDataFlags flag;
		EVertexDataFlagsIndex pos;
		short elemCount;
		short elemTypeSize;
		Buffers::EType elemType;
		int MemorySize()
		{
			return elemCount * elemTypeSize;
		}
		bool HasFlag(EVertexDataFlags p_flag)
		{
			return (p_flag & flag)!= 0;
		}
	};
	static VertexStructInfo VertexStructInfos[(int)EVertexDataFlagsIndex::count]
	{
		{vdf_position,	EVertexDataFlagsIndex::position,	3 ,sizeof(float)       ,Buffers::EType::FLOAT},
		{vdf_texCoord,	EVertexDataFlagsIndex::texCoord,	4 ,sizeof(float)       ,Buffers::EType::FLOAT},
		{vdf_color,		EVertexDataFlagsIndex::color,		4 ,sizeof(float)       ,Buffers::EType::FLOAT},
		{vdf_normal,		EVertexDataFlagsIndex::normal,		3 ,sizeof(float)       ,Buffers::EType::FLOAT},
		{vdf_tangent,	EVertexDataFlagsIndex::tangent,		3 ,sizeof(float)       ,Buffers::EType::FLOAT},
		{vdf_bitangent,	EVertexDataFlagsIndex::bitangent,	3 ,sizeof(float)       ,Buffers::EType::FLOAT},
		{vdf_boneWeight,	EVertexDataFlagsIndex::boneWeight,	4 ,sizeof(float)       ,Buffers::EType::FLOAT},
		{vdf_boneId,		EVertexDataFlagsIndex::boneId,		4 ,sizeof(int32_t)       ,Buffers::EType::INT},
	};
	
	
	struct VertexDataBuffer
	{
	public:
		Geometry::EVertexDataFlags dataFlags;
		int count;
		//[VERTEX_DATA_FLAGS_INDEX_COUNT]
		void* dataPtrs[VERTEX_DATA_FLAGS_INDEX_COUNT];
		int dataLen[VERTEX_DATA_FLAGS_INDEX_COUNT];
	public:
		VertexDataBuffer(int p_size,Geometry::EVertexDataFlags p_dataFlag);
		~VertexDataBuffer();
		int Size(){return count;}
		void* GetDataPtr(Geometry::EVertexDataFlagsIndex type);
		void* MoveDataPtr(Geometry::EVertexDataFlagsIndex type);
		
		void SetPosition(int idx,float x,float y, float z)			    	{ SetData<float>	(EVertexDataFlagsIndex::position,idx,x,y,z);}
		void SetTexCoord(int idx,float x,float y)					    	{ SetData<float>	(EVertexDataFlagsIndex::texCoord,idx,x,y,0,0);}
		void SetColor(int idx,float x,float y, float z,float w)		    	{ SetData<float>	(EVertexDataFlagsIndex::color,idx,x,y,z,w);}
		void SetNormal(int idx,float x,float y, float z)			    	{ SetData<float>	(EVertexDataFlagsIndex::normal,idx,x,y,z);}
		void SetTangent(int idx,float x,float y, float z)			    	{ SetData<float>	(EVertexDataFlagsIndex::tangent,idx,x,y,z);}
		void SetBitangent(int idx,float x,float y, float z)			    	{ SetData<float>	(EVertexDataFlagsIndex::bitangent,idx,x,y,z);}
		void SetBoneId(int idx,int32_t x,int32_t y, int32_t z,int32_t w)	{ SetData<int32_t>	(EVertexDataFlagsIndex::boneId,idx,x,y,z,w);}
		void SetBoneWeight(int idx,float x,float y, float z,float w)    	{ SetData<float>	(EVertexDataFlagsIndex::boneWeight,idx,x,y,z,w);}
		// full uv
		//void SetTexCoord(int idx,float x,float y,float z,float w)		    { SetData<float>	(EVertexDataFlagsIndex::texCoord,idx,x,y,z,w);}
	private:
		template<typename T>
		void SetData(EVertexDataFlagsIndex TFlag,int idx, T _1,T _2)
		{
			T* ptr  = static_cast<T*>(dataPtrs[(int)TFlag]);
			int len = dataLen[(int)TFlag];
			int offset  = idx *2;
			ptr[offset+0] = _1;
			ptr[offset+1] = _2;
			LE_ASSERT(len > offset + 1,"SetData outOfRange ");
		}
		template<typename T>
		void SetData(EVertexDataFlagsIndex TFlag,int idx,  T _1,T _2,T _3)
		{
			T* ptr  = static_cast<T*>(dataPtrs[(int)TFlag]);
			int len = dataLen[(int)TFlag];
			int offset  = idx *3;
			ptr[offset+0] = _1;
			ptr[offset+1] = _2;
			ptr[offset+2] = _3;
			LE_ASSERT(len > offset + 2,"SetData outOfRange ");
		}
		template<typename T>
		void SetData(EVertexDataFlagsIndex TFlag,int idx,  T _1,T _2,T _3,T _4)
		{
			T* ptr  = static_cast<T*>(dataPtrs[(int)TFlag]);
			int len = dataLen[(int)TFlag];
			int offset  = idx *4;
			ptr[offset+0] = _1;
			ptr[offset+1] = _2;
			ptr[offset+2] = _3;
			ptr[offset+3] = _4;
			LE_ASSERT(len > offset + 3,"SetData outOfRange ");
		}
	};
}
