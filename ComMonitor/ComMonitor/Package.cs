using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ComMonitor
{
    public enum Message
    {
        I2C_StartSet = 1,
        I2C_Busy,
        I2C_StartSucc,
        I2C_NoAck,
        I2C_Transmitter,
        I2C_Receiver,
        I2C_CommandTrans,
        I2C_ByteRec
    };

    public class Package
    {
        public const int PackageSize = 5;
        //private static const int 

        public byte[] Bytes;

        public Package()
        {
            Bytes = new byte[PackageSize];
        }

        public string[] Decompose(ref int messagesAmount)
        {
            //BitConverter.ToInt16(
            string[] result;// = new string
            switch (Bytes[0])
            {
                case 1:
                    messagesAmount = PackageSize - 1;
                    result = new string[messagesAmount];
                    for (int i = 1; i < PackageSize; i++)
                    {
                        result[i-1] = Bytes[i].ToString();
                    }
                    break;
                case 2:
                    messagesAmount = (PackageSize - 1) / 2;
                    result = new string[messagesAmount];
                    for (int i = 0; i < messagesAmount; i++)
                    {
                        result[i] = (BitConverter.ToInt16(Bytes, 1+i*2)).ToString();
                    }
                    break;
                case 3:
                    messagesAmount = 1;
                    result = new string[1];
                    result[0] = BitConverter.ToInt32(Bytes, 1).ToString(); 
                    break;
                default:
                    messagesAmount = 1;
                    result = new string[1];
                    result[0] = "";
                    break;
            }
            return result;
        }

        private string[] DecodeMessage(Byte[] bytes)
        {
            int arraySize = bytes.GetLength(0);
            string[] messages = new string[arraySize];
            for (int i = 0; i < arraySize; i++)
            {
                switch (bytes[i])
                {
                    case (byte)Message.I2C_StartSet:
                        messages[i] = "I2C start set";
                        break;
                    case (byte)Message.I2C_Busy:
                        messages[i] = "I2C is busy";
                        break;
                    case (byte)Message.I2C_StartSucc:
                        messages[i] = "I2C start successful";
                        break;
                    case (byte)Message.I2C_NoAck:
                        messages[i] = "I2C no ack. Stop is set";
                        break;
                    case (byte)Message.I2C_Transmitter:
                        messages[i] = "I2C transmitter";
                        break;
                    case (byte)Message.I2C_Receiver:
                        messages[i] = "I2C receiver";
                        break;
                    case (byte)Message.I2C_CommandTrans:
                        messages[i] = "I2C command transmitted";
                        break;
                    case (byte)Message.I2C_ByteRec:
                        messages[i] = "I2C byte received";
                        break;
                    default:
                        messages[i] = "Unknown command";
                        break;
                }
            }
            return messages;
        }
    }


}
