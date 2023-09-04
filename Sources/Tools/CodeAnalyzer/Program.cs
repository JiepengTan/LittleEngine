using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using GamesTan;

namespace CodeAnalyzer
{
    internal class Program
    {
        public static void Main(string[] args)
        {
            var paths = new string[]
            {
                @"../../Sources/Runtime",
            };
            string targetPath = "CodeAnalyzerOutput.txt";
            if (args.Length > 1)
            {
                targetPath = args[1];
            }

            ReplaceCodeIncludes(paths,targetPath);
        }

        
        private static void ReplaceCodeIncludes(string[] paths,string targetPath)
        {
            HashSet<string> allCppFiles = new HashSet<string>();
            foreach (var dir in paths)
            {
                PathUtil.Walk(dir, "*.h|*.inl|*.hpp", (p) =>
                {
                    allCppFiles.Add(Path.GetFullPath(p).Replace("\\","/"));
                });
            }

            var lst = allCppFiles.ToList();
            lst.Sort();
            StringBuilder sb = new StringBuilder();
            foreach (var file in lst)
            {
                sb.Append(file+ ";");
            }
            File.WriteAllText(targetPath,sb.ToString());
        }
    }
}