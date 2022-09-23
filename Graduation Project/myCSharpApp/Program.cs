using System;
using myCustomer;
using System.Collections.Generic;

namespace myCSharpApp
{
    class Devices
    {
        static void Main(string[] args)
        {
            CustomerDatabase customerDb = new CustomerDatabase();
            DeviceDatabase deviceDb = new DeviceDatabase();
            
            customerDb.PopulateArrays();
            deviceDb.PopulateArrays();

            while (true)
            {
                int menuOption;
                LoginMenu();
                Console.Write("Please select an option: ");
                if (!int.TryParse(Console.ReadLine(), out menuOption))
                {
                    Console.WriteLine("Invalid input! Please enter an appropriate integer choice.");
                }
                else if (menuOption == 1)
                {
                    Customer customer = NewCustomerSignUp(customerDb);
                    if (customer != null)
                    {
                        customerDb.db_index += 1;
                        CustomerMenu(customer, deviceDb);
                    }
                }
                else if (menuOption == 2)
                {
                    Customer customer = ExistingCustomerLogin(customerDb);
                    if (customer != null)
                        CustomerMenu(customer, deviceDb);
                }
                else if (menuOption == 3)
                {
                    Console.WriteLine("Thank you! See you again.");
                    return;
                }
                else
                {
                    Console.WriteLine("Invalid option");
                }
            }
        }

        static void LoginMenu()
        {
            Console.WriteLine("----------------Login Menu------------------");
            Console.WriteLine("1. New Customer Sign Up");
            Console.WriteLine("2. Customer Login");
            Console.WriteLine("3. Exit");
        }

        static void DisplayMenu()
        {
            Console.WriteLine("---------------Menu-----------------");
            Console.WriteLine("1. Display list of all devices");
            Console.WriteLine("2. Add devices into the list");
            Console.WriteLine("3. Search for a device");
            Console.WriteLine("4. Log out");
            Console.Write("Please select an option: ");
        }

        static void CustomerMenu(Customer customer, DeviceDatabase deviceDb)
        {
            while (true)
            {
                int menuOption;
                Console.WriteLine("Welcome, " + customer.firstName);
                DisplayMenu();
                Console.Write("Please select an option: ");
                if (!int.TryParse(Console.ReadLine(), out menuOption))
                {
                    Console.WriteLine("Invalid input! Please enter an appropriate integer choice.");
                }
                switch (menuOption)
                {
                    case 1://Display all devices
                        if (deviceDb.priceList[0] == 0.0)
                        {
                            Console.WriteLine("There are no devices in the database!");
                            break;
                        }
                        DisplayDevices(deviceDb.listOfDevices, deviceDb.priceList);
                        break;
                    case 2://Add devices into the list
                        if (!isAdmin(customer.customerId, customer.password))
                        {
                            Console.WriteLine("Sorry! Only admin can add devices");
                            break;
                        }
                        Console.Write("Please enter the no. of devices you want to add: ");
                        int noOfDevices;
                        while (!int.TryParse(Console.ReadLine(), out noOfDevices))
                        {
                            Console.WriteLine("Invalid input! An integer value expected!");
                            Console.Write("Please enter the no. of devices you want to add: ");
                        }
                        if (deviceDb.totalNoOfDevices + noOfDevices <= DeviceDatabase.DEVICE_LIST_SIZE)
                        {
                            AddDevicesIntoList(ref deviceDb.listOfDevices, ref deviceDb.priceList,
                                noOfDevices, deviceDb.totalNoOfDevices);
                            deviceDb.totalNoOfDevices += 1;
                        }    
                        else
                            Console.WriteLine($"Unable to add {noOfDevices} devices into the list!");
                        break;
                    case 3://Search for a device
                        if (deviceDb.priceList[0] == 0.0)
                        {
                            Console.WriteLine("There are no devices in the database!");
                            break;
                        }
                        Console.Write("Please enter the name of the device you want to search: ");
                        string deviceName = Console.ReadLine();
                        SearchForADevice(deviceDb.listOfDevices, deviceDb.priceList, deviceName);
                        break;
                    case 4://Log out
                        return;
                    default://Invalid choice
                        Console.WriteLine("Invalid option!");
                        break;
                }
            }
        }

        static void AddDevicesIntoList(ref string[] listOfDevices, ref double[] priceList, int noOfDevices, int totalNoOfDevices)
        {
            for (int index = totalNoOfDevices; index < totalNoOfDevices + noOfDevices; index++)
            {
                Console.Write($"Please enter the name of the device {index + 1}: ");
                string devName = Console.ReadLine();
                listOfDevices[index] = devName;
                Console.Write($"Please enter the price of the device {index + 1}: ");
                double price;
                if (!double.TryParse(Console.ReadLine(), out price))
                {
                    Console.WriteLine("Invalid input! Please enter a valid integer choice.");
                }
                priceList[index] = price;
            }
        }

        static void DisplayDevices(string[] listOfDevices, double[] priceList)
        {
            Console.WriteLine("Following is the list of devices:");
            Console.WriteLine("Sr. No.\tDevice Name\t\t\t\t\tPrice");
            Console.WriteLine("-------------------------------------------------------------");
            for (int index = 0; index < listOfDevices.Length; index++)
            {
                if (priceList[index] == 0.0)
                    break;
                Console.Write(Convert.ToString(index + 1) + "\t" + listOfDevices[index] + "\t\t\t\t\t");
                Console.WriteLine(priceList[index]);
            }
            Console.WriteLine("------------------------------------------------------------");
        }

        static void SearchForADevice(string[] listOfDevices, double[] priceList, string deviceName)
        {
            for (int index = 0; index < listOfDevices.Length; index++)
            {
                if (deviceName == listOfDevices[index])
                {
                    Console.WriteLine("This device is available!");
                    Console.WriteLine($"Device Price: {priceList[index]}");
                    return;
                }
                index++;
            }
            Console.WriteLine("This device is not available!");
        }

        static Customer NewCustomerSignUp(CustomerDatabase customerDb)
        {
            if (customerDb.db_index >= CustomerDatabase.CUSTOMER_LIST_SIZE)
            {
                Console.WriteLine("Sorry! Cannot add more customers into the database!");
                return null;
            }
            string customerId, password;
            customerId = SetId();
            // Check if cutomer ID is already present, else create one
            if (idExistsInDatabase(customerId, customerDb))
            {
                Console.WriteLine("This customer ID already exists! Kindly try with 'Customer Login' option.");
                return null;
            }
            password = SetPassword();
            Customer customer = new Customer(customerId, password);
            customer.AddCustomer(customerDb.db_index);
            return customer;
        }

        private static bool idExistsInDatabase(string customerId, CustomerDatabase customerDb)
        {
            for (int index = 0; index < Customer.CUSTOMER_LIST_SIZE; index++)
            {
                if (String.Equals(customerDb.customerIds[index], customerId))
                    return true;
            }
            return false;
        }

        static Customer ExistingCustomerLogin(CustomerDatabase customerDb)
        {
            Console.Write("Please enter the Customer ID: ");
            string customerId = Console.ReadLine();
            Console.Write("Please enter the password: ");
            string password = Console.ReadLine();
            // Verify if cutomer ID and password exists in database
            for (int index = 0; index < Customer.CUSTOMER_LIST_SIZE; index++)
            {
                if (String.Equals(customerDb.customerIds[index], customerId) &&
                        String.Equals(customerDb.customerPasswords[index], password))
                {
                    Customer customer = new Customer(customerId, password);
                    return customer;
                }
            }
            Console.WriteLine("Invalid customer ID or password!");
            return null;
        }

        static string SetId()
        {
            Console.Write("Please set an Id: ");
            string customerId = Console.ReadLine();
            if (string.IsNullOrEmpty(customerId))
            {
                Console.WriteLine("Invalid input! A valid string is expected!");
                return SetId();
            }
            return customerId;
        }

        static string SetPassword()
        {
            Console.Write("Please set a password: ");
            string password1 = Console.ReadLine();
            if (string.IsNullOrEmpty(password1))
            {
                Console.WriteLine("Invalid input! A valid string is expected!");
                return SetPassword();
            }
            Console.Write("Please re-enter the password: ");
            string password2 = Console.ReadLine();
            if (password1 == password2)
            {
                Console.WriteLine("Passwords matched!");
                return password1;
            }
            Console.WriteLine("Passwords do not match!Try again.");
            return SetPassword();
        }

        private static bool isAdmin(string id, string pswd)
        {
            if (id == "admin" && pswd == "admin123")
                return true;
            return false;
        }
    }
}
