/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

namespace OvRendering::Geometry
{
	/**
	* Data structure that defines the geometry of a vertex
	*/
	struct Vertex
	{
		float position[3];//0
		float texCoords[2];//3
		float normals[3];//5
		float tangent[3];//8
		float bitangent[3];//11
		float boneWeights[4];//14
		int32_t boneIds[4];
	};
}