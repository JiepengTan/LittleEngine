using System;
using System.Collections.Generic;
using System.IO;
using GamesTan;

namespace GamesTan.ProjectReshaper
{
    internal class Program
    {
        static string[] sourceDirs = new string[]
        {
            @"../../Sources/Game",
            @"../../Sources/Runtime",
            @"../../Sources/Editor",
        };
        public static void Main(string[] args)
        {
            //ReplaceHeadInclude();
            ReplaceContext();
        }

        private static void ReplaceContext()
        {
            var namespaceMap = new Dictionary<string, string>()
            {
                { "LittleEngine::Rendering::Resources::Shader", "LittleEngine::Shader"},
                { "Rendering::Resources::Shader", "Shader"},
                { "Resources::Shader", "Shader"},
                
                { "LittleEngine::Rendering::Resources::Texture", "LittleEngine::Texture"},
                { "Rendering::Resources::Texture", "Texture"},
                { "Resources::Texture", "Texture"},
                
                { "LittleEngine::Rendering::Resources::Model", "LittleEngine::Model"},
                { "Rendering::Resources::Model", "Model"},
                { "Resources::Model", "Model"},
                
                { "LittleEngine::Rendering::Resources::Mesh", "LittleEngine::Mesh"},
                { "Rendering::Resources::Mesh", "Mesh"},
                { "Resources::Mesh", "Mesh"},
                
                { "LittleEngine::Rendering::Resources::IMesh", "LittleEngine::IMesh"},
                { "Rendering::Resources::IMesh", "IMesh"},
                { "Resources::IMesh", "IMesh"},
                
                { "LittleEngine::Resources::Material", "LittleEngine::Material"},
                { "Resources::Material", "Material"},
                
                { "LittleEngine::Audio::Resources::Sound", "LittleEngine::Sound"},
                { "Audio::Resources::Sound", "Sound"},
                { "Resources::Sound", "Sound"},
                
                { "LittleEngine::Audio::Resources::Loaders", "LittleEngine::Resources"},
                { "LittleEngine::Rendering::Resources::Loaders", "LittleEngine::Resources"},
                { "Rendering::Resources::Loaders", "Resources"},
                { "Rendering::Resources", "Resources"},
                { "Audio::Resources::Loaders", "Resources"},
                { "Audio::Resources", "Resources"},
            };
            ContentReplacement.ReplaceFiles(sourceDirs, namespaceMap);
        }

        private static void ReplaceHeadInclude()
        {
            var namespaceMap = new Dictionary<string, string>()
            {
                { "Modules/Audio/Resources/Loaders", "Resource/Loader/" },
                { "Modules/Audio/Resources/", "Resource/Asset/" },
            };
            var oldIncludeTag = "";
            HeadIncludeReplacement.ReplaceFiles(oldIncludeTag, sourceDirs, namespaceMap);
        }
    }
}