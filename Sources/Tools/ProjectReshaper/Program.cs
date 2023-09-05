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
            ReplaceHeadInclude();
            ReplaceNameSpace();
        }

        private static void ReplaceNameSpace()
        {
            var namespaceMap = new Dictionary<string, string>()
            {
                { "OvCore", "LittleEngine" },
                { "OvEditor", "LittleEditor" },
            };
            ContentReplacement.ReplaceFiles(sourceDirs, namespaceMap);
        }

        private static void ReplaceHeadInclude()
        {
            var namespaceMap = new Dictionary<string, string>()
            {
                { "Resource/ResourceManagement/", "Modules/Rendering/ResourceManagement/" },
                { "Resource/Resources/", "Modules/Rendering/Resources/" },
                { "Resource/Rendering/", "Modules/Rendering/" },
            };
            var oldIncludeTag = "Resource/";
            HeadIncludeReplacement.ReplaceFiles(oldIncludeTag, sourceDirs, namespaceMap);
        }
    }
}