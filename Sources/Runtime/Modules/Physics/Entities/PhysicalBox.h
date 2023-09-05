/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Physics/Entities/PhysicalObject.h"

namespace LittleEngine::Physics::Entities
{
	/**
	* PhysicalObject with a box shape
	*/
	class PhysicalBox : public PhysicalObject
	{
	public:
		/**
		* PhysicalBox constructor (Internal transform management)
		* @param p_radius
		* @param p_height
		*/
		PhysicalBox(const LittleEngine::FVector3& p_size = {0.5f, 0.5f, 0.5f});

		/**
		* PhysicalBox constructor (External transform management)
		* @param p_transform
		* @param p_radius
		* @param p_height
		*/
		PhysicalBox(LittleEngine::FTransform& p_transform, const LittleEngine::FVector3& p_size = { 0.5f, 0.5f, 0.5f });

		/**
		* Defines the size of the physical box
		* @param p_size
		*/
		void SetSize(const LittleEngine::FVector3& p_size);

		/**
		* Returns the size of the physical box
		*/
		LittleEngine::FVector3 GetSize() const;

	private:
		void CreateCollisionShape(const LittleEngine::FVector3& p_size);
		void RecreateCollisionShape(const LittleEngine::FVector3& p_size);
		virtual void SetLocalScaling(const LittleEngine::FVector3& p_scaling) override;

	private:
		LittleEngine::FVector3 m_size;
	};
}
