/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "tinyxml2/tinyxml2.h"


#include "Core/Serialization/ISerializer.h"

namespace LittleEngine::API
{
	/**
	* ISerializable is an interface for any class that can be serialized
	*/
	class ISerializable
	{
	public:
		/**
		* Called when the serialization is asked
		* @param p_doc
		* @param p_node
		*/
		virtual void OnSerialize(ISerializer p_serializer) = 0;

		/**
		* Called when the deserialization is asked
		* @param p_doc
		* @param p_node
		*/
		virtual void OnDeserialize(ISerializer p_serializer) = 0;

		/**
		* Default polymorphic destructor
		*/
		virtual ~ISerializable() = default;
	};
}
