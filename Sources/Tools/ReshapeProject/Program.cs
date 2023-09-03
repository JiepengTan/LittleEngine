// See https://aka.ms/new-console-template for more information

using GamesTan;


class Application
{
    static int Main()
    {
        var paths = new string[]
        {
            @"I:\Projects\OverloadEngine\Overload\Sources\Runtime",
            @"I:\Projects\OverloadEngine\Overload\Sources\Game",
            @"I:\Projects\OverloadEngine\Overload\Sources\Editor",
        };
        List<string> allCppFiles = new List<string>();
        foreach (var dir in paths)
        {
            PathUtil.Walk(dir,"*.h|*.inl", (p) =>
            {
                allCppFiles.Add(p);
            });
            PathUtil.Walk(dir,"*.h", (p) =>
            {
                allCppFiles.Add(p);
            });
            PathUtil.Walk(dir,"*.cpp", (p) =>
            {
                allCppFiles.Add(p);
            });
        }

        foreach (var file in allCppFiles)
        {
            Console.WriteLine(file);
        }
        return 1;
    }
}