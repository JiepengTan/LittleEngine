﻿using System;
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
            //ReplaceContext();
        }

        private static void ReplaceContext()
        {
            var namespaceMap = new Dictionary<string, string>()
            {
                { "std::weak_ptr", "WeakPtr"},
                { "std::shared_ptr", "SharedPtr"},
                { "std::unique_ptr", "UniquePtr"},
                { "std::vector", "TVector"},
                { "std::array", "TArray"},
                { "std::map", "TMap"},
                { "std::list", "TList"},
                { "std::set", "TSet"},
                { "std::unordered_map", "TUnorderedMap"},
                { "std::queue", "TQueue"},
            };
            ContentReplacement.ReplaceFiles(sourceDirs, namespaceMap);
        }

        private static void ReplaceHeadInclude()
        {
            var namespaceMap = new Dictionary<string, string>()
            {
                { "Modules/Rendering/ResourceManagement/", "Resource/Manager/" },
                { "Modules/Resources/Loaders/", "Resource/Loader/" },
                { "Modules/Resources/Parsers/", "Resource/Parser/" },
                { "Modules/Resources/", "Resource/Asset/" },
                { "Resource/ResInclude", "Resource/Data/ResInclude" },
                { "Resource/TextAsset", "Resource/Asset/TextAsset" },
            };
            var oldIncludeTag = "Resource/";
            HeadIncludeReplacement.ReplaceFiles(oldIncludeTag, sourceDirs, namespaceMap);
        }
    }
}