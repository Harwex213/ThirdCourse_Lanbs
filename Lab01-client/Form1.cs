using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab01_client
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private async void buttonCalc_Click(object sender, EventArgs e)
        {
            var client = new HttpClient
            {
                BaseAddress = new Uri("http://localhost:8000/")
            };
            var response = await client.PostAsync("task4",  new FormUrlEncodedContent(new[]
            {
                new KeyValuePair<string, string>("X", textBoxX.Text),
                new KeyValuePair<string, string>("Y", textBoxY.Text),
            }));
            
            textBoxResult.Text = await response.Content.ReadAsStringAsync();
        }
    }
}