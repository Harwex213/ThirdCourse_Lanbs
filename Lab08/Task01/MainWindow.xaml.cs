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

namespace Task01
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

        private bool _firstTime = true;
        
        private void Calculate(object sender, RoutedEventArgs e)
        {
            if (!_firstTime)
            {
                TextX.Text = TextY.Text;
            }
            _firstTime = false;
            var x = int.Parse(TextX.Text);
            var a = int.Parse(TextA.Text);
            var b = Convert.ToString(x, 2).Length;
            var c = Math.Pow(2, b);
            var n = double.Parse(TextN.Text);
            var stopwatch = new Stopwatch();
            stopwatch.Start();
            var y = Math.Abs(((a * x) + c) % n);
            stopwatch.Stop();
            TextY.Text = y.ToString();
            TextTime.Text = stopwatch.ElapsedTicks + " ms";
        }

    }
}