using System;
using System.Collections.Generic;
using System.IO;
using GamesTan;

namespace ReshapeProject
{
    static class StringExt
    {
        static bool StartWithExt(this string str, string tag)
        {
            if (str.Length < tag.Length) return false;
            var len = tag.Length;
            for (int i = 0; i < len; i++)
            {
                if (str[i] != tag[i]) return false;
            }

            return true;
        }
    }

    internal class Program
    {
        public static void Main(string[] args)
        {
            var paths = new string[]
            {
                @"../../Sources/Game",
                @"../../Sources/Runtime",
                @"../../Sources/Editor",
            };
            var namespaceMap = new Dictionary<string, string>()
            {
                { "OvAnalytics", "Core/Analytics" },
                { "OvAudio", "Modules/Audio" },
                { "OvDebug", "Core/Debug" },
                { "OvEditor", "../Editor" },
                { "OvGame", "../Game" },
                { "OvMaths", "Core/Maths" },
                { "OvPhysics", "Modules/Physics" },
                { "OvRendering", "Resource/Rendering" },
                { "OvTools", "Core/Tools" },
                { "OvUI", "Modules/UI" },
                { "OvWindowing", "Platform/Windowing" },
                { "OvAnim", "Modules/Anim" },
                { "OvCore/ResourceManagement", "Resource/Rendering" },
                { "OvCore/Resources", "Resource/Resources" },
                { "OvCore/GlobalState.h", "Core/GlobalState.h" },
                { "OvCore/", "GamePlay/" },
            };


            ReplaceCodeIncludes(paths, namespaceMap);
        }

        
        private static void ReplaceCodeIncludes(string[] paths, Dictionary<string, string> namespaceMap)
        {
            List<string> allCppFiles = new List<string>();
            foreach (var dir in paths)
            {
                PathUtil.Walk(dir, "*.h|*.inl", (p) => { allCppFiles.Add(p); });
                PathUtil.Walk(dir, "*.h", (p) => { allCppFiles.Add(p); });
                PathUtil.Walk(dir, "*.cpp", (p) => { allCppFiles.Add(p); });
            }

            var headTag = "#include";
            var oldIncludeTag = "Ov";
            foreach (var file in allCppFiles)
            {
                Console.WriteLine(file);
                var lines = File.ReadAllLines(file);
                var resultLines = new List<string>(lines.Length);
                foreach (var line in lines)
                {
                    var result = line;
                    var trimLine = line.Trim();
                    var isHead = trimLine.StartsWith(headTag);
                    if (isHead)
                    {
                        var head = trimLine.Replace(headTag, "").Trim()
                            .Replace("\"", "")
                            .Replace("<", "")
                            .Replace(">", "")
                            .Replace("\\", "/")
                            .Trim();
                        if (head.StartsWith(oldIncludeTag)) // old namespace
                        {
                            foreach (var item in namespaceMap)
                            {
                                if (head.StartsWith(item.Key))
                                {
                                    head = head.Replace(item.Key, item.Value);
                                    break;
                                }
                            }

                            result = $"{headTag} \"{head}\"";
                        }
                    }

                    resultLines.Add(result);
                }

                File.WriteAllLines(file, resultLines);
            }
        }
    }
}