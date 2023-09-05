/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once



namespace LittleEngine::Rendering::Settings
{
    /**
    * Projection modes, mostly used for cameras
    */
    enum class EProjectionMode
    {
        ORTHOGRAPHIC,
        PERSPECTIVE
    };
    enum class ECameraType
    {
        Game,
        Scene,
        Asset,
        Other
    };
}
