using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace CMSI_lr6
{
    public partial class MainWindow : Window
    {
        static string alphabet = "abcdefghijklmnopqrstuvwxyz";

        static Dictionary<char, string> rotors = new Dictionary<char, string>(2)
        {
            {'1', "ekmflgdqvzntowyhxuspaibrcj"}, //rotor 1
            {'2', "ajdksiruxblhwtmcqgznpyfvoe"}, //rotor 2
            {'3', "bdfhjlcprtxvznyeiwgakmusqo"}, //rotor 3
            {'4', "esovpzjayquirhxlnftgkdcmwb"}, //rotor 4
            {'5', "vzbrgityupsdnhlxawmjqofeck"}, //rotor 5
            {'6', "jpgvoumfyqbenhzrdkasxlictw"}, //rotor 6
            {'7', "nzjhgrcxmyswboufaivlpekqdt"}, //rotor 7
            {'8', "fkqhtlxocbjspdzramewniuygv"}, //rotor 8
            {'B', "leyjvcnixwpbqmdrtakzgfuhos"}, //rotor Beta
            {'G', "fsokanuerhmbtiycwlqpzxvgjd"}, //rotor Gamma
        };

        int rotorR = 9, rotorM = 8, rotorL = 0;
        int rotorRValue = 0, rotorMValue = 0, rotorLValue = 0;
        int rotorRStep = 4, rotorMStep = 2, rotorLStep = 4;

        static List<char> reflectorLetter = new List<char>(26)
        {
            'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z', ' '
        };

        static int[] reflectors = new int[26]
        {
            0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 19, 21, 24, 17, 20, 7, 16, 18, 11, 15, 23, 13, 14, 25, 22
        };

        public MainWindow()
        {
            InitializeComponent();
        }

        private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            RotorRValue.Text = rotorRValue.ToString();
            RotorMValue.Text = rotorMValue.ToString();
            RotorLValue.Text = rotorLValue.ToString();

            var reflectorStr = "AY BR CU DH EQ FS GL IP JX KN MO TZ VW";
            var alphabetTransformed = string.Join(" ", alphabet.ToUpper().Select(item => item));
            
            RotorL.Text = string.Join(" ", rotors['1'].ToUpper().Select(item => item));
            AlphabetRotorL.Text = alphabetTransformed;
            RotorM.Text = string.Join(" ", rotors['B'].ToUpper().Select(item => item));
            AlphabetRotorM.Text = alphabetTransformed;
            RotorR.Text = string.Join(" ", rotors['G'].ToUpper().Select(item => item));
            AlphabetRotorR.Text = alphabetTransformed;
            Reflector.Text = string.Join(" ", reflectorStr.ToUpper().Split(' '));
                
            RotorLValue.Text = "0";
            RotorMValue.Text = "0";
            RotorRValue.Text = "0";
        }

        async private void Button_Click(object sender, RoutedEventArgs e)
        {
            string reflector = "abcdefghijklmnopqrstuvwxyz";
            rotorRValue = Int32.Parse(RotorRValue.Text);
            rotorMValue = Int32.Parse(RotorMValue.Text);
            rotorLValue = Int32.Parse(RotorLValue.Text);

            for (int i = 0; i < 13; i++)
            {
                string first = reflector.Substring(reflectors[i], 1);
                string second = reflector.Substring(reflectors[i + 13], 1);
                reflector = reflector.Replace(Convert.ToChar(first), '@');
                reflector = reflector.Replace(Convert.ToChar(second), '#');
                reflector = reflector.Replace('@', Convert.ToChar(second));
                reflector = reflector.Replace('#', Convert.ToChar(first));
            }

            int letterPosition = -1;
            string letter = ((Button)sender).Content.ToString().ToLower();
            //Rotor R
            for (int i = 0; i < alphabet.Length; i++)
            {
                if (letter == alphabet[i].ToString())
                {
                    letterPosition = ((i + rotorRValue) % 26);
                }
            }
            letter = rotors.ElementAt(rotorR).Value[letterPosition].ToString();
            //RotorM
            for (int i = 0; i < alphabet.Length; i++)
            {
                if (letter == alphabet[i].ToString())
                {
                    letterPosition = ((i + rotorMValue) % 26);
                }
            }
            letter = rotors.ElementAt(rotorM).Value[letterPosition].ToString();
            //RotorL
            for (int i = 0; i < alphabet.Length; i++)
            {
                if (letter == alphabet[i].ToString())
                {
                    letterPosition = ((i + rotorLValue) % 26);
                }
            }
            letter = rotors.ElementAt(rotorL).Value[letterPosition].ToString();
            //Reflector
            for (int i = 0; i < alphabet.Length; i++)
            {
                if (letter == alphabet[i].ToString())
                {
                    letterPosition = i;
                }
            }
            letter = reflector[letterPosition].ToString();
            //RotorL
            for (int i = 0; i < rotors.ElementAt(rotorL).Value.Length; i++)
            {
                if (letter == rotors.ElementAt(rotorL).Value[i].ToString())
                {
                    if (i - rotorLValue >= 0)
                    {
                        letterPosition = ((i - rotorLValue) % 26);
                    }
                    else
                    {
                        letterPosition = 26 + i - rotorLValue;
                    }
                }
            }
            letter = alphabet[letterPosition].ToString();
            //RotorM
            for (int i = 0; i < rotors.ElementAt(rotorM).Value.Length; i++)
            {
                if (letter == rotors.ElementAt(rotorM).Value[i].ToString())
                {
                    if (i - rotorMValue >= 0)
                    {
                        letterPosition = ((i - rotorMValue) % 26);
                    }
                    else
                    {
                        letterPosition = 26 + i - rotorMValue;
                    }
                }
            }
            letter = alphabet[letterPosition].ToString();
            //Rotor R
            for (int i = 0; i < rotors.ElementAt(rotorR).Value.Length; i++)
            {
                if (letter == rotors.ElementAt(rotorR).Value[i].ToString())
                {
                    if (i - rotorRValue >= 0)
                    {
                        letterPosition = ((i - rotorRValue) % 26);
                    }
                    else
                    {
                        letterPosition = 26 + i - rotorRValue;
                    }
                }
            }
            letter = alphabet[letterPosition].ToString();

            rotorRValue = rotorRValue + rotorRStep;
            if (rotorRValue > alphabet.Length - 1)
            {
                rotorRValue = (rotorRValue % alphabet.Length);
                rotorMValue = rotorMValue + rotorMStep;
            }
            if (rotorMValue > alphabet.Length - 1)
            {
                rotorMValue = (rotorMValue % alphabet.Length);
                rotorLValue = rotorLValue + rotorLStep;
            }
            if (rotorLValue > alphabet.Length - 1)
            {
                rotorLValue = 0;
                rotorMValue = 0;
            }

            RotorRValue.Text = rotorRValue.ToString();
            RotorMValue.Text = rotorMValue.ToString();
            RotorLValue.Text = rotorLValue.ToString();

            if (letter == "a") TextA.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "b") TextB.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "c") TextC.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "d") TextD.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "e") TextE.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "f") TextF.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "g") TextG.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "h") TextH.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "i") TextI.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "j") TextJ.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "k") TextK.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "l") TextL.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "m") TextM.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "n") TextN.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "o") TextO.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "p") TextP.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "q") TextQ.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "r") TextR.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "s") TextS.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "t") TextT.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "u") TextU.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "v") TextV.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "w") TextW.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "x") TextX.Background = new SolidColorBrush(Colors.Gold);
            else if (letter == "y") TextY.Background = new SolidColorBrush(Colors.Gold);
            else TextZ.Background = new SolidColorBrush(Colors.Gold);

            CancellationTokenSource cts = new CancellationTokenSource();
            await Task.Delay(TimeSpan.FromMilliseconds(300), cts.Token);

            if (letter == "a") TextA.Background = new SolidColorBrush(Colors.White);
            else if (letter == "b") TextB.Background = new SolidColorBrush(Colors.White);
            else if (letter == "c") TextC.Background = new SolidColorBrush(Colors.White);
            else if (letter == "d") TextD.Background = new SolidColorBrush(Colors.White);
            else if (letter == "e") TextE.Background = new SolidColorBrush(Colors.White);
            else if (letter == "f") TextF.Background = new SolidColorBrush(Colors.White);
            else if (letter == "g") TextG.Background = new SolidColorBrush(Colors.White);
            else if (letter == "h") TextH.Background = new SolidColorBrush(Colors.White);
            else if (letter == "i") TextI.Background = new SolidColorBrush(Colors.White);
            else if (letter == "j") TextJ.Background = new SolidColorBrush(Colors.White);
            else if (letter == "k") TextK.Background = new SolidColorBrush(Colors.White);
            else if (letter == "l") TextL.Background = new SolidColorBrush(Colors.White);
            else if (letter == "m") TextM.Background = new SolidColorBrush(Colors.White);
            else if (letter == "n") TextN.Background = new SolidColorBrush(Colors.White);
            else if (letter == "o") TextO.Background = new SolidColorBrush(Colors.White);
            else if (letter == "p") TextP.Background = new SolidColorBrush(Colors.White);
            else if (letter == "q") TextQ.Background = new SolidColorBrush(Colors.White);
            else if (letter == "r") TextR.Background = new SolidColorBrush(Colors.White);
            else if (letter == "s") TextS.Background = new SolidColorBrush(Colors.White);
            else if (letter == "t") TextT.Background = new SolidColorBrush(Colors.White);
            else if (letter == "u") TextU.Background = new SolidColorBrush(Colors.White);
            else if (letter == "v") TextV.Background = new SolidColorBrush(Colors.White);
            else if (letter == "w") TextW.Background = new SolidColorBrush(Colors.White);
            else if (letter == "x") TextX.Background = new SolidColorBrush(Colors.White);
            else if (letter == "y") TextY.Background = new SolidColorBrush(Colors.White);
            else TextZ.Background = new SolidColorBrush(Colors.White);

            cts.Cancel();
        }
    }
}
