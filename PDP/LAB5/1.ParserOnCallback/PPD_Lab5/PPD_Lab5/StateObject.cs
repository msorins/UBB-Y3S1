using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace PPD_Lab5
{
    // State object for receiving data from remote device.  
    public class StateObject
    {
        public const int BUFFER_SIZE = 256;

        // Client socket.  
        public Socket WorkSocket { get; set; }
        // Size of receive buffer.  
        
        // Receive buffer.  
        public byte[] Buffer { get; set; }
        // Received data string.  
        public StringBuilder SB { get; set; }

        public int Index { get; set; }
        public String Host { get; set; }

        public StateObject()
        {
            SB = new StringBuilder();
            Buffer = new byte[BUFFER_SIZE];
        }

        public StateObject(String host, int index)
        {
            Host = host;
            Index = index;
            SB = new StringBuilder();
            Buffer = new byte[BUFFER_SIZE];
        }
    }
}
