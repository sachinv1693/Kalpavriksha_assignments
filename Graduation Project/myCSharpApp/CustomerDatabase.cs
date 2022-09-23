using System;
using System.Collections.Generic;
using System.Text;

namespace myCSharpApp
{
    class CustomerDatabase
    {
        public const int CUSTOMER_LIST_SIZE = 50;
        public int db_index = 0;

        public string[] customerIds = new string[CUSTOMER_LIST_SIZE];
        public string[] customerPasswords = new string[CUSTOMER_LIST_SIZE];
        public string[] customerNames = new string[CUSTOMER_LIST_SIZE];
        public string[] customerPhoneNummbers = new string[CUSTOMER_LIST_SIZE];

        public void PopulateArrays()
        {
            Array.Fill(customerIds, "");
            Array.Fill(customerPasswords, "");
            Array.Fill(customerNames, "");
            Array.Fill(customerPhoneNummbers, "");
        }
    }
}
