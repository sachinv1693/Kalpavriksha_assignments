using myCSharpApp;
using System;
using System.Collections.Generic;
using System.Text;

namespace myCustomer
{
    class Customer
    {
        public string customerId;
        public string password;
        public string firstName;
        public string lastName;
        public string phoneNumber;

        public const int CUSTOMER_LIST_SIZE = 50;

        public Customer(string id, string pswd)
        {
            customerId = id;
            password = pswd;
        }

        public void AddCustomer(int index)
        {
            SetName();
            SetPhoneNumber();
            CustomerDatabase customerDb = new CustomerDatabase();
            customerDb.customerIds[index] = customerId;
            customerDb.customerPasswords[index] = password;
            customerDb.customerNames[index] = firstName + " " + lastName;
            customerDb.customerPhoneNummbers[index] = phoneNumber;
        }

        public string inputName(string whichName)
        {
            Console.Write("Please enter the " + whichName + "  name: ");
            string name = Console.ReadLine();
            if (string.IsNullOrEmpty(name))
            {
                Console.WriteLine("Invalid input! A valid string is expected!");
                return inputName(whichName);
            }
            return name;
        }

        public void SetName()
        {
            firstName = inputName("first");
            lastName = inputName("last");
        }

        public void SetPhoneNumber()
        {
            Console.Write("Please enter a 10-digit phone number: ");
            phoneNumber = Console.ReadLine();
            if (int.TryParse(phoneNumber, out _) || phoneNumber.Length != 10)
            {
                Console.WriteLine("Invalid phone number! Try again.");
                SetPhoneNumber();
            }
        }
    }
}
