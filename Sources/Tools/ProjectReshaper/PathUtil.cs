using System;
using System.Collections.Generic;
using System.IO;

namespace GamesTan
{
    public static partial class PathUtil
    {
        private static string _relPath;

        private static string RelPath
        {
            get
            {
                if (_relPath == null)
                {
                    _relPath = "";
                }

                return _relPath;
            }
        }

        /// 删除目录下面的内容，但是会保留目录本身
        public static void DeleteDirectoryContents(string path)
        {
            path = GetFullPath(path);
            if (Directory.Exists(path))
            {
                var files = PathUtil.GetAllPath(path, "*.*");
                foreach (var file in files)
                {
                    File.Delete(file);
                }
            }
        }

        /// 删除目录
        public static void DeleteDirectory(string path, bool recv = true)
        {
            path = GetFullPath(path);
            if (Directory.Exists(path))
            {
                Directory.Delete(path, recv);
            }
        }

        /// 创建目录
        public static void CreateDirectoryIfNecessary(string path)
        {
            path = GetFullPath(path);
            var dir = Path.GetDirectoryName(path);
            if (string.IsNullOrEmpty(dir) || Directory.Exists(dir))
            {
                return;
            }

            Directory.CreateDirectory(dir);
        }

        public static string GetAbsPath(string assetPath)
        {
            assetPath = assetPath.Replace("\\", "/");
            var paths = assetPath.Split('/');
            List<string> strs = new List<string>();
            foreach (var path in paths)
            {
                if (path == ".")
                {
                    continue;
                }

                if (path == "..")
                {
                    try
                    {
                        strs.RemoveAt(strs.Count - 1);
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine(e);
                        throw;
                    }

                    continue;
                }

                strs.Add(path);
            }

            var finalPath = string.Join("/", strs);
            return finalPath;
        }

        public static string GetFullPath(string assetPath)
        {
            if (assetPath.StartsWith("Assets")) return Path.Combine(RelPath, assetPath);
            return assetPath;
        }

        public static string GetParentDir(string dir)
        {
            dir = dir.Replace("\\", "/");
            if (dir.EndsWith("/"))
            {
                dir = dir.Substring(0, dir.Length - 1);
            }

            dir = dir.Substring(0, dir.LastIndexOf("/"));
            return dir;
        }

        public static string GetAssetPath(string fullPath)
        {
            fullPath = fullPath.Replace("\\", "/");
            if (fullPath.StartsWith(RelPath))
            {
                return fullPath.Substring(RelPath.Length + 1);
            }

            return fullPath;
        }

        public static List<string> GetAllPath(string path, string exts)
        {
            var lst = new List<string>();
            Walk(path, exts, (p) => { lst.Add(p); });
            return lst;
        }

        public static void Walk(string path, string exts, System.Action<string> callback, bool isIncludeDir = false,
            bool isSaveAssets = false, bool isAllDirs = true)
        {
            bool isAll = string.IsNullOrEmpty(exts) || exts == "*" || exts == "*.*";
            string[] extList = exts.Replace("*", "").Split('|');
            if (Directory.Exists(path))
            {
                var searchOption = isAllDirs ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly;
                List<string> paths = new List<string>();
                string[] lst = exts.Split('|');
                foreach (var ext in lst)
                {
                    // 如果选择的是文件夹
                    string[] files = Directory.GetFiles(path, ext, searchOption);
                    paths.AddRange(files);
                    if (isIncludeDir)
                    {
                        string[] directories = Directory.GetDirectories(path, ext, searchOption);
                        paths.AddRange(directories);
                    }
                }

                foreach (var item in paths)
                {
                    if (callback != null)
                    {
                        callback(item);
                    }
                }

                if (isSaveAssets)
                {
                }
            }
            else
            {
                if (isAll)
                {
                    if (callback != null)
                    {
                        callback(path);
                    }
                }
                else
                {
                    // 如果选择的是文件
                    foreach (var ext in extList)
                    {
                        if (path.EndsWith(ext, StringComparison.OrdinalIgnoreCase))
                        {
                            if (callback != null)
                            {
                                callback(path);
                            }
                        }
                    }
                }

                if (isSaveAssets)
                {
                }
            }
        }
    }
}