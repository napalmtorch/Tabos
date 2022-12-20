using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Linq;
using System.Threading;
using Tabos.Builder.Library;

namespace Tabos.RAMFS;

public class CommandDeclarations
{
    public static Command NEW   { get; private set; } = new Command("NEW",  CommandHandlers.NEW);
    public static Command SAVE  { get; private set; } = new Command("SAVE", CommandHandlers.SAVE);
    public static Command LOAD  { get; private set; } = new Command("LOAD", CommandHandlers.LOAD);
    public static Command ADD   { get; private set; } = new Command("ADD", CommandHandlers.ADD);
    public static Command ADDN  { get; private set; } = new Command("ADDN", CommandHandlers.ADDN);
    public static Command LIST  { get; private set; } = new Command("LIST", CommandHandlers.LIST);

    public static List<Command> List = new List<Command>()
    {
        NEW, SAVE, LOAD, ADD, ADDN, LIST,
    };
}

public static class CommandHandlers
{
    public static void NEW(string input, List<string> args)
    {
        if (args.Count == 1) { Debug.Error("Expected size of image"); }
        if (args.Count == 2) { Debug.Error("Expected maximum amount of files"); }

        int max_ents = 0, sz = 0;
        if (!int.TryParse(args[1], out max_ents)) { Debug.Error("Invalid value for max entries - '%s'", args[1]); }
        if (!int.TryParse(args[2], out sz))       { Debug.Error("Invalid value for size - '%s'", args[2]); }

        Program.RAMFS = new RAMFileSystem((uint)max_ents, (uint)sz);
    }

    public static void SAVE(string input, List<string> args)
    {
        if (args.Count < 2) { Debug.Error("Expected filename"); }

        string fname = input.Substring(5);
        File.WriteAllBytes(fname, Program.RAMFS.Data);
        Debug.Log("Saved image file to '%s'\n", fname);
    }

    public static void LOAD(string input, List<string> args)
    {
        string fname = input.Substring(5);
        if (!File.Exists(fname)) { Debug.Error("Unable to locate image file at '%s'", fname); }
        Program.RAMFS = new RAMFileSystem(File.ReadAllBytes(fname));
        Debug.Log("Saved image file to '%s'\n", fname);
    }

    public static void ADD(string input, List<string> args)
    {
        if (args.Count == 1) { Debug.Error("Expected hidden flag for new file"); }
        if (args.Count == 2) { Debug.Error("Expected name for new file"); }
        if (args.Count == 3) { Debug.Error("Expected source path for new file"); }
        if (args[1] != "1" && args[1] != "0") { Debug.Error("Invalid value for hidden flag - '%s'", args[1]); }

        if (!File.Exists(args[3])) { Debug.Error("Unable to locate source file '%s'", args[3]); }
        byte[] data = File.ReadAllBytes(args[3]);
        Program.RAMFS.AddFile(args[2], data, (args[1] == "1" ? true : false));
    }

    public static void ADDN(string input, List<string> args)
    {
        if (args.Count == 1) { Debug.Error("Expected directory to import"); }

        string dir = input.Substring(5).Replace("\\", "/");
        if (!Directory.Exists(dir)) { Debug.Error("Unable to locate source directory '%s'", dir); }
        if (!dir.EndsWith("/")) { dir += "/"; }

        string[] files = Directory.GetFiles(dir);
        foreach (string file in files) { CommandParser.Execute("ADD 0 " + Path.GetFileName(file) + " " + file); }        
    }

    public static void LIST(string input, List<string> args)
    {
        List<RAMFile> files = Program.RAMFS.GetFiles();

        Debug.Log("NAME                    HIDDEN  SIZE\n");
        foreach (RAMFile file in files)
        {
            Debug.Log(file.GetName().PadRight(24, ' '));
            Debug.Log((file.Hidden ? "1" : "0") + "       ");
            Debug.Log(file.Size + " bytes\n");
        }
    }
}