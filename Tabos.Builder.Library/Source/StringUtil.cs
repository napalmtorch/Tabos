namespace Tabos.Builder.Library;

public static class StringUtil
{
    public static string FormatPath(string path, bool ending_slash = true)
    {
        path = path.Replace("\\", "/");
        if (!path.EndsWith("/") && ending_slash) { path += "/"; }
        return path;
    }
}