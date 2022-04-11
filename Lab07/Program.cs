using System.Diagnostics;
using System.Net;
using System.Security.Cryptography;
using System.Text;

namespace Lab07
{
    static class Program
{
    public static byte[] KeyOne { get; set; }
    public static byte[] KeyTwo { get; set; }
    
    private static void GenerateKeys()
    {
        var desEncryptor = DES.Create();
        
        desEncryptor.GenerateKey();
        KeyOne = desEncryptor.Key;

        desEncryptor.GenerateKey();
        KeyTwo = desEncryptor.Key;

        Console.WriteLine($"Key one: {Encoding.ASCII.GetString(KeyOne)}. Length - {KeyOne.Length}");
        Console.WriteLine($"Key two: {Encoding.ASCII.GetString(KeyTwo)} Length - {KeyTwo.Length}");
    }

    private static async Task<string> GetData()
    {
        var client = new HttpClient();
        return await client.GetStringAsync("https://loripsum.net/api/plaintext");
    }
    
    private static (byte[] data, byte[] iv) Encrypt(string data)
    {
        var stopwatch = new Stopwatch();
        stopwatch.Start();

        var des = DES.Create();
        des.GenerateIV();
        
        des.Key = KeyOne;
        var encrypted = des.EncryptCbc(Encoding.Default.GetBytes(data), des.IV, PaddingMode.Zeros);

        des.Key = KeyTwo;
        encrypted = des.DecryptCbc(encrypted, des.IV, PaddingMode.Zeros);
        
        des.Key = KeyOne;
        encrypted = des.EncryptCbc(encrypted, des.IV, PaddingMode.Zeros);

        stopwatch.Stop();
        Console.WriteLine($"Encrypting elapsed milliseconds: " + stopwatch.ElapsedMilliseconds);
        return (encrypted, des.IV);
    }

    private static byte[] Decrypt(byte[] encrypted, byte[] initializationVector)
    {
        var stopwatch = new Stopwatch();
        stopwatch.Start();
        
        var des = DES.Create();
        des.IV = initializationVector;
        
        des.Key = KeyOne;
        var decrypted = des.DecryptCbc(encrypted, des.IV, PaddingMode.Zeros);

        des.Key = KeyTwo;
        decrypted = des.EncryptCbc(decrypted, des.IV, PaddingMode.Zeros);
        
        des.Key = KeyOne;
        decrypted = des.DecryptCbc(decrypted, des.IV, PaddingMode.Zeros);
        
        stopwatch.Stop();
        Console.WriteLine($"Decrypting elapsed milliseconds: " + stopwatch.ElapsedMilliseconds);
        return decrypted;
    }
    
    public static async Task Main()
    {
        GenerateKeys();
        
        using var reader = new StreamReader("../../../input/data.txt");
        var data = await reader.ReadToEndAsync();
        Console.WriteLine("Plain text (150): " + data[..150]);
        Console.WriteLine("Plain text all length: " + data.Length);

        var encrypted = Encrypt(data);
        Console.WriteLine("Encrypted length: " + Encoding.Default.GetString(encrypted.data).Length);
        
        var decrypted = Decrypt(encrypted.data, encrypted.iv);
        Console.WriteLine("Decrypted (150): " + Encoding.Default.GetString(decrypted)[..150]);
        Console.WriteLine("Decrypted length: " + Encoding.Default.GetString(decrypted).Length);
        
        await using (var writer = new StreamWriter("../../../input/encrypted.txt", false))
        {
            await writer.WriteAsync(Encoding.Default.GetString(encrypted.data));
        }
        await using (var writer = new StreamWriter("../../../input/decrypted.txt", false))
        {
            await writer.WriteAsync(Encoding.Default.GetString(decrypted));
        }
    }
}
}
