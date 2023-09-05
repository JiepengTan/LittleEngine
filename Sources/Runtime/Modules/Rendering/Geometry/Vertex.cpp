#include "Modules/Rendering/Geometry/Vertex.h"
#include <cstdlib>
#include <iostream>
#include <ostream>


OvRendering::Geometry::VertexDataBuffer::VertexDataBuffer(int p_size, Geometry::EVertexDataFlags p_dataFlag):
    dataFlags(p_dataFlag)
{
    count = p_size;
    std::cout << "Mesh vertexCount " << count<< std::endl;
    for (int i = 0; i < VERTEX_DATA_FLAGS_INDEX_COUNT; i++)
    {
        dataPtrs[i] = nullptr;
        dataLen[i] = 0;
    }
    for (auto element : Geometry::VertexStructInfos)
    {
        if (element.HasFlag(p_dataFlag))
        {
            dataPtrs[(int)element.pos] = malloc(count * element.MemorySize());
            dataLen[(int)element.pos] =count * element.elemCount;
            
            std::cout<<(int)element.pos << " alloc " << (count * element.MemorySize())  <<" ptr= " << (long)(dataPtrs[(int)element.pos])<< std::endl;
        }
    }
}

OvRendering::Geometry::VertexDataBuffer::~VertexDataBuffer()
{
    for (int i = 0; i < VERTEX_DATA_FLAGS_INDEX_COUNT; i++)
    {
        if (dataPtrs[i] != nullptr)
        {
            std::cout<<i << " free ptr " << (long)dataPtrs[i]<< std::endl;
            free(dataPtrs[i]);
        }
        dataPtrs[i] = nullptr;
    }
}


void* OvRendering::Geometry::VertexDataBuffer::GetDataPtr(Geometry::EVertexDataFlagsIndex type)
{
    if (dataPtrs[(int)type] != nullptr)
    {
        return dataPtrs[(int)type];
    }
    return nullptr;
}

void* OvRendering::Geometry::VertexDataBuffer::MoveDataPtr(Geometry::EVertexDataFlagsIndex type)
{
    void* retPtr = dataPtrs[(int)type];
    dataPtrs[(int)type] = nullptr;
    return retPtr;
}
