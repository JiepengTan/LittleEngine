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
            foreach (var arg in args)
            {
            }
            var dir = "../../Sources/Runtime";
            string targetPath = "CodeAnalyzerOutput.txt";
            if (args.Length >= 2)
            {
                targetPath = args[0];
                dir= args[1];
            }
            Console.WriteLine("CodeAnalyzer Param targetPath=" +targetPath  + " sourceDir = " +dir );
            ReplaceCodeIncludes(dir,targetPath);
        }

        
        private static void ReplaceCodeIncludes(string dir,string targetPath)
        {
            HashSet<string> allCppFiles = new HashSet<string>();
            PathUtil.Walk(dir, "*.h|*.inl|*.hpp", (p) =>
            {
                allCppFiles.Add(Path.GetFullPath(p).Replace("\\","/"));
            });

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