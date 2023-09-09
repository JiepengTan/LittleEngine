/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Core/Maths/FVector3.h"
#include "Core/Maths/FMatrix4.h"
#include "Core/Maths/FTransform.h"
#include "Resource/Data/ResIncludeRendering.h"

namespace LittleEngine::Rendering::Entities
{
	/**
	* Data structure that can represent any type of light
	*/
	struct Light
	{
		/**
		* Light types
		*/
		enum class Type { POINT, DIRECTIONAL, SPOT, AMBIENT_BOX, AMBIENT_SPHERE };
		Light();
		/**
		* Create the light with the given transform and type
		* @param p_transform
		* @param p_type
		*/
		Light(LittleEngine::FTransform& p_tranform, Type p_type);
		void DoInit(LittleEngine::FTransform& p_tranform, Type p_type) ;
		/**
		* Generate the light matrix, ready to send to the GPU
		*/
		LittleEngine::FMatrix4 GenerateMatrix() const;

		/**
		* Calculate the light effect range from the quadratic falloff equation
		*/
		float GetEffectRange() const;

		/**
		* Returns the light transform
		*/
		const LittleEngine::FTransform* GetTransform() const;

		FVector3			color		= { 1.f, 1.f, 1.f };
		float				intensity	= 1.f;
		float				constant	= 0.0f;
		float				linear		= 0.0f;
		float				quadratic	= 1.0f;
		float				cutoff		= 12.f;
		float				outerCutoff = 15.f;
		float				type		= 0.0f;

	protected:
		LittleEngine::FTransform* m_transform;
	};
}
