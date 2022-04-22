using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Task02
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }
        
        private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);
        }

        public static byte[] EncryptData(byte[] pwd, byte[] data)
        {
            int a, i, j, k, tmp;
            int[] key, box;
            byte[] cipher;

            key = new int[64];
            box = new int[64];
            cipher = new byte[data.Length];

            for (i = 0; i < 64; i++)
            {
                key[i] = pwd[i % pwd.Length];
                box[i] = i;
            }
            for (j = i = 0; i < 64; i++)
            {
                j = (j + box[i] + key[i]) % 64;
                tmp = box[i];
                box[i] = box[j];
                box[j] = tmp;
            }
            for (a = j = i = 0; i < data.Length; i++)
            {
                a++;
                a %= 64;
                j += box[a];
                j %= 64;
                tmp = box[a];
                box[a] = box[j];
                box[j] = tmp;
                k = box[((box[a] + box[j]) % 64)];
                cipher[i] = (byte)(data[i] ^ k);
            }
            return cipher;
        }

        public static byte[] DecryptData(byte[] pwd, byte[] data)
        {
            int a, i, j, k, tmp;
            int[] key, box;
            byte[] cipher;

            key = new int[64];
            box = new int[64];
            cipher = new byte[data.Length];

            for (i = 0; i < 64; i++)
            {
                key[i] = pwd[i % pwd.Length];
                box[i] = i;
            }
            for (j = i = 0; i < 64; i++)
            {
                j = (j + box[i] + key[i]) % 64;
                tmp = box[i];
                box[i] = box[j];
                box[j] = tmp;
            }
            for (a = j = i = 0; i < data.Length; i++)
            {
                a++;
                a %= 64;
                j += box[a];
                j %= 64;
                tmp = box[a];
                box[a] = box[j];
                box[j] = tmp;
                k = box[((box[a] + box[j]) % 64)];
                cipher[i] = (byte)(data[i] ^ k);
            }
            return cipher;
        }

        private void Encrypt(object sender, RoutedEventArgs e)
        {
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();

            TextEnc.Clear();
            string text = TextOrig.Text;
            byte[] data = Encoding.ASCII.GetBytes(text);

            string[] temp = Key.Text.Split(' ');
            byte[] key = new byte[temp.Count()];
            for (int i = 0; i < temp.Count(); i++)
            {
                key[i] = Convert.ToByte(temp[i]);
            }
            byte[] enc = EncryptData(key, data);

            string encText = "";
            for (int i = 0; i < enc.Count(); i++)
            {
                encText += enc[i].ToString() + " ";
            }

            stopwatch.Stop();
            encText += $"\nTime: {stopwatch.ElapsedMilliseconds} milliseconds";

            TextEnc.Text = encText;

        }

        private void Decrypt(object sender, RoutedEventArgs e)
        {
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();

            string[] text = TextOrig.Text.Substring(0, TextOrig.Text.Length-1).Split(' ');
            byte[] data = new byte[text.Count()];
            for (int i = 0; i < text.Count(); i++)
            {
                if (text[i].Length != 0)
                { 
                    data[i] = Convert.ToByte(text[i]); 
                }
            }

            string[] temp = Key.Text.Split(' ');
            byte[] key = new byte[temp.Count()];
            for (int i = 0; i < temp.Count(); i++)
            {
                key[i] = Convert.ToByte(temp[i]);
            }
            byte[] decr = DecryptData(key, data);

            string decrText = "";
            for (int i = 0; i < decr.Count(); i++)
            {

                decrText += ((char)decr[i]).ToString();
            }

            stopwatch.Stop();
            decrText += $"\nTime: {stopwatch.ElapsedMilliseconds} milliseconds";

            TextEnc.Text = decrText;
        }
    }
}