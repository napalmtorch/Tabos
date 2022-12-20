using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Linq;
using System.Threading;
using Tabos.Builder.Library;

namespace Tabos.RAMFS;

public struct RAMFile
{
    public const int EntrySz       = 73;
    public const int MaxNameLength = 64;

    public char[] Name;
    public bool   Hidden;
    public uint   Offset;
    public uint   Size;

    public RAMFile(string name, bool hidden, uint offset, uint size)
    {
        this.Name   = new char[MaxNameLength];
        this.Hidden = hidden;
        this.Offset = offset;
        this.Size   = size;
        this.SetName(name);
    }

    public void Read(byte[] data, int offset)
    {
        BinaryReader reader = new BinaryReader(new MemoryStream(data, offset, EntrySz, false), Encoding.ASCII);
        Name                = reader.ReadChars(64);
        Hidden              = reader.ReadBoolean();
        Offset              = reader.ReadUInt32();
        Size                = reader.ReadUInt32();
    }

    public byte[] Write()
    {
        byte[] data = new byte[EntrySz];
        BinaryWriter writer = new BinaryWriter(new MemoryStream(data), Encoding.ASCII);
        writer.Write(Name, 0, RAMFile.MaxNameLength);
        writer.Write(Hidden);
        writer.Write(Offset);
        writer.Write(Size);
        return data;
    }

    public void SetName(string name)
    {
        if (name.Length >= MaxNameLength) { Debug.Error("File name excceeds 80 characters - '%s'\n", name); }
        Array.Fill(Name, (char)0);
        Array.Copy(name.ToCharArray(), Name, name.Length);
    }

    public string GetName() 
    { 
        string name = String.Empty;
        int i = 0;
        while (Name[i] != 0) { name += Name[i++]; }
        return name;
    }
}

public struct RAMFSHeader
{
    public uint TableOffset;
    public uint TableSize;
    public uint DataOffset;
    public uint DataSize;
    public uint DataPosition;
    public uint Count;
    public uint CountMax;

    public RAMFSHeader(uint taddr, uint tsz, uint daddr, uint dsz, uint dp, uint c, uint max)
    {
        this.TableOffset    = taddr;
        this.TableSize      = tsz;
        this.DataOffset     = daddr;
        this.DataSize       = dsz;
        this.DataPosition   = dp;
        this.Count          = c;
        this.CountMax       = max;
    }

    public void Read(byte[] data, int offset)
    {
        BinaryReader reader = new BinaryReader(new MemoryStream(data, offset, 28, false), Encoding.ASCII);
        TableOffset         = reader.ReadUInt32();
        TableSize           = reader.ReadUInt32();
        DataOffset          = reader.ReadUInt32();
        DataSize            = reader.ReadUInt32();
        DataPosition        = reader.ReadUInt32();
        Count               = reader.ReadUInt32();
        CountMax            = reader.ReadUInt32();
    }

    public byte[] Write()
    {
        byte[] data = new byte[28];
        BinaryWriter writer = new BinaryWriter(new MemoryStream(data), Encoding.ASCII);
        writer.Write(TableOffset);
        writer.Write(TableSize);
        writer.Write(DataOffset);
        writer.Write(DataSize);
        writer.Write(DataPosition);
        writer.Write(Count);
        writer.Write(CountMax);
        return data;
    }
}

public class RAMFileSystem
{
    public byte[]      Data;
    public RAMFSHeader Header;

    public RAMFileSystem()
    {
        this.Data = new byte[1];
        this.Header = new RAMFSHeader();
    }

    public RAMFileSystem(byte[] data)
    {
        Data = new byte[data.Length];
        Array.Copy(Data, data, data.Length);
        Header = new RAMFSHeader();
        Header.Read(Data, 0);
        Debug.Log("Initialized RAMFS\n");
    }

    public RAMFileSystem(uint max_files, uint sz)
    {
        Data = new byte[(max_files * RAMFile.EntrySz) + sz];
        Header = new RAMFSHeader(28, (max_files * RAMFile.EntrySz), 28 + (max_files * RAMFile.EntrySz), sz, 0, 0, max_files);
        Array.Copy(Header.Write(), 0, Data, 0, 28);
        Debug.Log("Created RAMFS - Max:%u Size:%u bytes\n", max_files, sz);
    }

    public RAMFile AddFile(string name, byte[] data, bool hidden = false)
    {
        if (data == null || data.Length == 0) { Debug.Log("Attempt to create blank file"); return new RAMFile(); }

        int i = GetNextIndex();
        if (i == -1) { Debug.Error("Maximum amount of files reached"); return new RAMFile(); }

        RAMFile file = new RAMFile(name, hidden, Header.DataPosition, (uint)data.Length);
        Array.Copy(file.Write(), 0, Data, (Header.TableOffset + (i * RAMFile.EntrySz)), RAMFile.EntrySz);
        Array.Copy(data, 0, Data, Header.DataOffset + Header.DataPosition, data.Length);
        Header.DataPosition += (uint)data.Length;
        Header.Count++;
        Array.Copy(Header.Write(), 0, Data, 0, 28);
        Debug.Log("Added file - Name:%s Hidden:%d Size:%d bytes\n", name, (hidden ? 1 : 0), data.Length);
        return GetFile(i);
    }

    private RAMFile GetFile(int index, bool rd_hdr = true)
    {
        if (rd_hdr) { Header.Read(Data, 0); }
        RAMFile file = new RAMFile();
        file.Read(Data, (int)(Header.TableOffset + (index * RAMFile.EntrySz)));
        return file;
    }

    public RAMFile GetFile(string name, bool rd_hdr = true)
    {
        if (rd_hdr) { Header.Read(Data, 0); }
        for (int i = 0; i < Header.CountMax; i++)
        {
            RAMFile file = GetFile(i, false);
            if (new string(file.Name) == name) { return file; }
        }
        return new RAMFile();
    }

    public byte[] GetFileData(RAMFile file)
    {
        byte[] data = new byte[file.Size];
        Array.Copy(Data, Header.DataOffset + file.Offset, data, 0, file.Size);
        return data;
    }

    public List<RAMFile> GetFiles()
    {
        List<RAMFile> files = new List<RAMFile>();
        Header.Read(Data, 0);
        for (int i = 0; i < Header.CountMax; i++)
        {
            RAMFile file = GetFile(i, false);
            if (file.Name[0] == 0) { continue; }
            files.Add(file);
        }
        return files;
    }

    private int GetNextIndex()
    {
        Header.Read(Data, 0);
        for (int i = 0; i < Header.CountMax; i++)
        {
            RAMFile file = GetFile(i, false);
            if (file.Name[0] == 0) { return i; }
        }
        return -1;
    }
}
