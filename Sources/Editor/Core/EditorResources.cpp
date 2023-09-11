/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Utils/GUIUtil.h"

#include "Modules/Rendering/Settings/ETextureFilteringMode.h"

#include "Core/Tools/Utils/PathParser.h"

#include "../Editor/Core/EditorResources.h"
#include "../Editor/Resources/RawTextures.h"

LittleEngine::Editor::Core::EditorResources::EditorResources(const std::string& p_editorAssetsPath)
{
	using namespace LittleEngine::Resources;

	std::string iconsFolder		= p_editorAssetsPath + "Textures\\Icons\\";
	std::string modelsFolder	= p_editorAssetsPath + "Models\\";
	std::string shadersFolder	= p_editorAssetsPath + "Shaders\\";

	LittleEngine::Resources::EModelParserFlags modelParserFlags = LittleEngine::Resources::EModelParserFlags::NONE;

	modelParserFlags |= LittleEngine::Resources::EModelParserFlags::TRIANGULATE;
	modelParserFlags |= LittleEngine::Resources::EModelParserFlags::GEN_SMOOTH_NORMALS;
	modelParserFlags |= LittleEngine::Resources::EModelParserFlags::OPTIMIZE_MESHES;
	modelParserFlags |= LittleEngine::Resources::EModelParserFlags::OPTIMIZE_GRAPH;
	modelParserFlags |= LittleEngine::Resources::EModelParserFlags::FIND_INSTANCES;
	modelParserFlags |= LittleEngine::Resources::EModelParserFlags::CALC_TANGENT_SPACE;
	modelParserFlags |= LittleEngine::Resources::EModelParserFlags::JOIN_IDENTICAL_VERTICES;
	modelParserFlags |= LittleEngine::Resources::EModelParserFlags::DEBONE;
	modelParserFlags |= LittleEngine::Resources::EModelParserFlags::FIND_INVALID_DATA;
	modelParserFlags |= LittleEngine::Resources::EModelParserFlags::IMPROVE_CACHE_LOCALITY;
	modelParserFlags |= LittleEngine::Resources::EModelParserFlags::GEN_UV_COORDS;
	modelParserFlags |= LittleEngine::Resources::EModelParserFlags::PRE_TRANSFORM_VERTICES;
	modelParserFlags |= LittleEngine::Resources::EModelParserFlags::GLOBAL_SCALE;

	/* Buttons */
	auto CreateTex = [this,iconsFolder ](const std::string& p_name,bool isLinear){
		auto mode = isLinear?
			LittleEngine::Rendering::Settings::ETextureFilteringMode::LINEAR:
			LittleEngine::Rendering::Settings::ETextureFilteringMode::NEAREST;
		m_textures[p_name]= TextureLoader::Create(iconsFolder + p_name + ".png",mode, mode, false);
	};
	CreateTex("Empty_Texture",true);
	CreateTex("Button_Play",true);
	CreateTex("Button_Pause",true);
	CreateTex("Button_Stop",true);
	CreateTex("Button_Next",true);
	CreateTex("Button_Refresh",true);
	/* Icons */
	CreateTex("Icon_Unknown",true);
	CreateTex("Icon_Folder",true);
	CreateTex("Icon_Texture",true);
	CreateTex("Icon_Model",true);
	CreateTex("Icon_Shader",true);
	CreateTex("Icon_Material",true);
	CreateTex("Icon_Scene",true);
	CreateTex("Icon_Sound",true);
	CreateTex("Icon_Script",true);
	CreateTex("Icon_Font",true);
	
	CreateTex("Bill_Point_Light",false);
	CreateTex("Bill_Spot_Light",false);
	CreateTex("Bill_Directional_Light",false);
	CreateTex("Bill_Ambient_Box_Light",false);
	CreateTex("Bill_Ambient_Sphere_Light",false);

	GUIUtil::ProvideEmptyTexture(*m_textures["Empty_Texture"]);
	
	/* Models */
	m_models["Cube"]				= ModelLoader::Create(modelsFolder + "Cube.fbx", modelParserFlags);
	m_models["Cylinder"]			= ModelLoader::Create(modelsFolder + "Cylinder.fbx", modelParserFlags);
	m_models["Plane"]				= ModelLoader::Create(modelsFolder + "Plane.fbx", modelParserFlags);
	m_models["Vertical_Plane"]	= ModelLoader::Create(modelsFolder + "Vertical_Plane.fbx", modelParserFlags);
	m_models["Roll"]				= ModelLoader::Create(modelsFolder + "Roll.fbx", modelParserFlags);
	m_models["Sphere"]			= ModelLoader::Create(modelsFolder + "Sphere.fbx", modelParserFlags);
	m_models["Arrow_Translate"]	= ModelLoader::Create(modelsFolder + "Arrow_Translate.fbx", modelParserFlags);
	m_models["Arrow_Rotate"]		= ModelLoader::Create(modelsFolder + "Arrow_Rotate.fbx", modelParserFlags);
	m_models["Arrow_Scale"]		= ModelLoader::Create(modelsFolder + "Arrow_Scale.fbx", modelParserFlags);
	m_models["Arrow_Picking"]		= ModelLoader::Create(modelsFolder + "Arrow_Picking.fbx", modelParserFlags);
	m_models["Camera"]			= ModelLoader::Create(modelsFolder + "Camera.fbx", modelParserFlags);

	/* Shaders */
	m_shaders["Grid"]			= ResourcesUtils::LoadShader(":Shaders\\EditorGrid.glsl");
	m_shaders["Gizmo"]		= ResourcesUtils::LoadShader(":Shaders\\EditorGizmo.glsl");
	m_shaders["Billboard"]	= ResourcesUtils::LoadShader(":Shaders\\EditorBillboard.glsl");
	
}

LittleEngine::Editor::Core::EditorResources::~EditorResources()
{
	for (auto[id, texture] : m_textures)
		LittleEngine::Resources::TextureLoader::Destroy(texture);

	for (auto [id, mesh] : m_models)
		LittleEngine::Resources::ModelLoader::Destroy(mesh);

}

LittleEngine::Texture* LittleEngine::Editor::Core::EditorResources::GetFileIcon(const std::string& p_filename)
{
	using namespace LittleEngine::Utils;
	return GetTexture("Icon_" + PathParser::FileTypeToString(PathParser::GetFileType(p_filename)));
}

LittleEngine::Texture* LittleEngine::Editor::Core::EditorResources::GetTexture(const std::string& p_id)
{
	if (m_textures.find(p_id) != m_textures.end())
		return m_textures.at(p_id);

	return nullptr;
}

LittleEngine::Model* LittleEngine::Editor::Core::EditorResources::GetModel(const std::string& p_id)
{
	if (m_models.find(p_id) != m_models.end())
		return m_models.at(p_id);

	return nullptr;
}

LittleEngine::Shader* LittleEngine::Editor::Core::EditorResources::GetShader(const std::string& p_id)
{
	if (m_shaders.find(p_id) != m_shaders.end())
		return m_shaders.at(p_id);

	return nullptr;
}
