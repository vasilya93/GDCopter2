using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;

namespace ComMonitor
{
    class Program
    {
        static Package _package;
        static bool _continue;
        static SerialPort _serialPort;

        static void Main(string[] args)
        {
            string message;
            _package = new Package();

            StringComparer stringComparer = StringComparer.OrdinalIgnoreCase;

            Thread readThread = new Thread(Read);

            _serialPort = new SerialPort();
            _serialPort.PortName = SetPortName(_serialPort.PortName);
            _serialPort.BaudRate = 56000;

            _serialPort.ReadTimeout = 500;
            _serialPort.WriteTimeout = 500;

            _serialPort.Open();
            _continue = true;
            readThread.Start();

            Console.WriteLine("Type QUIT to exit");

            while (_continue)
            {
                message = Console.ReadLine();

                if (stringComparer.Equals("quit", message))
                {
                    _continue = false;
                }
                else
                {
                    _serialPort.WriteLine(message);
                }
            }

            readThread.Join();
            _serialPort.Close();
        }

        public static void Read()
        {
            while (_continue)
            {
                try
                {
                    int packagesToRead = _serialPort.BytesToRead;
                    packagesToRead -= packagesToRead % Package.PackageSize;
                    packagesToRead /= Package.PackageSize;
                    int messagesAmount = 0;
                    for (int i = 0; i < packagesToRead; i++)
                    {
                        _serialPort.Read(_package.Bytes, 0, Package.PackageSize - 1);
                        string[] messages = _package.Decompose(ref messagesAmount);
                        for (int j = 0; j < messagesAmount; j++)
                        {
                            Console.WriteLine(messages[j]);
                        }

                    }
                }
                catch (TimeoutException) { }
            }
        }
        //пакеты, нужно соглашение; мк не начинает слать, пока не получил запрос на посылку
        //причем нужно, чтоб в буффер вообще ничего не засовывалось до этого момента
        //то, что буффер ждет освобождения, чтоб запихнуть следующие данные - неправильно
        //должен переписывать сразу, но целый пакет
        public static string SetPortName(string defaultPortName)
        {
            string portName;

            Console.WriteLine("Available Ports:");
            foreach (string s in SerialPort.GetPortNames())
            {
                Console.WriteLine("   {0}", s);
            }

            Console.Write("COM port({0}): ", defaultPortName);
            portName = Console.ReadLine();

            if (portName == "")
            {
                portName = defaultPortName;
            }
            return portName;
        }
    }
}
