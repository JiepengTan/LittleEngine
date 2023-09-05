using System;
using System.Collections.Generic;
using System.IO;

namespace GamesTan.ProjectReshaper
{
    public class ContentReplacement
    {  
        public static void ReplaceFiles(string[] paths, Dictionary<string, string> namespaceMap)
        {
            List<string> allCppFiles = new List<string>();
            foreach (var dir in paths)
            {
                PathUtil.Walk(dir, "*.h|*.inl|*.cpp|*.mustache", (p) => { allCppFiles.Add(p); });
            }
            foreach (var file in allCppFiles)
            {
                Console.WriteLine(file);
                var lines = File.ReadAllText(file);
                foreach (var item in namespaceMap)
                {
                    lines = lines.Replace(item.Key, item.Value);
                }
                File.WriteAllText(file, lines);
            }
        }


    }
}