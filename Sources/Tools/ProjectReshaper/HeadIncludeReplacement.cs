﻿using System;
using System.Collections.Generic;
using System.IO;

namespace GamesTan.ProjectReshaper
{
    public class HeadIncludeReplacement
    {
        public static void ReplaceFiles(string oldIncludeTag, string[] paths, Dictionary<string, string> namespaceMap)
        {
            List<string> allCppFiles = new List<string>();
            foreach (var dir in paths)
            {
                PathUtil.Walk(dir, "*.h|*.inl|*.cpp", (p) => { allCppFiles.Add(p); });
            }

            var headTag = "#include";
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
                        if (oldIncludeTag== "" || head.StartsWith(oldIncludeTag)) // old namespace
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