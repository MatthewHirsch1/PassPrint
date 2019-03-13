using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace LEDTestForm
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            serialPort1.Open();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            serialPort1.Write("a");
            serialPort1.Write("ask.com,bob123@ask.com,12345pass");
            String incData = serialPort1.ReadLine();
            MessageBox.Show(incData, "Test Response from BT");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            serialPort1.Write("b");
            serialPort1.Write("yes.com,bob123@yes.com,54321pass");
            String incData = serialPort1.ReadLine();
            MessageBox.Show(incData, "Test Response from BT");
        }
    }
}

