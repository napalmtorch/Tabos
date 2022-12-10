namespace Tabos.Builder.Library;

public class Command
{
    public string Name  { get; private set; }
    public Action<string, List<string>> Handler { get; private set; }

    public Command(string name, Action<string, List<string>> handler)
    {
        this.Name    = name;
        this.Handler = handler;
    }
}