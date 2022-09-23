using System;
using System.Collections.Generic;
using System.Text;

namespace myCSharpApp
{
    class DeviceDatabase
    {
        public int current_index = 0;
        public const int DEVICE_LIST_SIZE = 50;
        public int totalNoOfDevices = 0;
        public string[] listOfDevices = new string[DEVICE_LIST_SIZE];
        public double[] priceList = new double[DEVICE_LIST_SIZE];

         public void PopulateArrays()
        {
            Array.Fill(listOfDevices, "");
            Array.Fill(priceList, 0.0);
        }
    }
}
