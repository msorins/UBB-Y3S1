using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace PPD_Lab5
{
    public class AsynchronousClient
    {
        // The port number for the remote device.  
        private const int port = 80;

        // ManualResetEvent instances signal completion.  
        public static List<ManualResetEvent> connectDone = new List<ManualResetEvent>();
        public static List<ManualResetEvent> sendDone = new List<ManualResetEvent>();
        public static List<ManualResetEvent> receiveDone = new List<ManualResetEvent>();

        // The response from the remote device.  
        public static List<String> responses = new List<String>();

        public static String StartClient(StateObject stateObj)
        {
            // Connect to a remote device.  
            try
            {
                // Establish the remote endpoint for the socket.   
                IPHostEntry ipHostInfo = Dns.GetHostEntry(stateObj.Host);
                IPAddress ipAddress = ipHostInfo.AddressList[0];
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, port);

                // Create a TCP/IP socket.  
                Socket client = new Socket(ipAddress.AddressFamily,
                    SocketType.Stream, ProtocolType.Tcp);
                stateObj.WorkSocket = client;

                // Connect to the remote endpoint.  
                client.BeginConnect(remoteEP,new AsyncCallback(ConnectCallback), stateObj);
                connectDone[stateObj.Index].WaitOne();

                // Send test data to the remote device.  
                Send(stateObj, "GET / index.html HTTP/1.1\r\nHost: " + stateObj.Host + "\r\n\r\n");
                sendDone[stateObj.Index].WaitOne();

                // Receive the response from the remote device.  
                Receive(stateObj);
                receiveDone[stateObj.Index].WaitOne();

                // Release the socket.  
                client.Shutdown(SocketShutdown.Both);
                client.Close();
            }
            catch (Exception e)
            {
                //Console.WriteLine(e.ToString());
            }

            return FormatResult(stateObj);
        }

        private static void ConnectCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the socket from the state object.  
                StateObject stateObj = (StateObject)ar.AsyncState;
                Socket client = stateObj.WorkSocket;

                // Complete the connection.  
                client.EndConnect(ar);

                // Signal that the connection has been made.  
                connectDone[stateObj.Index].Set();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        private static void Receive(StateObject stateObj)
        {
            try
            {
                // Begin receiving the data from the remote device.  
                stateObj.WorkSocket.BeginReceive(stateObj.Buffer, 0, StateObject.BUFFER_SIZE, 0,
                    new AsyncCallback(ReceiveCallback), stateObj);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        private static void ReceiveCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the state object and the client socket   
                // from the asynchronous state object.  
                StateObject state = (StateObject)ar.AsyncState;
                Socket client = state.WorkSocket;

                // Read data from the remote device.  
                int bytesRead = client.EndReceive(ar);

                if (bytesRead > 0)
                {
                    // There might be more data, so store the data received so far.  
                    state.SB.Append(Encoding.ASCII.GetString(state.Buffer, 0, bytesRead));

                    // Get the rest of the data.  
                    client.BeginReceive(state.Buffer, 0, StateObject.BUFFER_SIZE, 0,
                        new AsyncCallback(ReceiveCallback), state);
                }
                else
                {
                    // All the data has arrived; put it in response.  
                    if (state.SB.Length > 1)
                    {
                        responses[state.Index] = state.SB.ToString();
                    }
                    // Signal that all bytes have been received.  
                    receiveDone[state.Index].Set();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        private static void Send(StateObject stateObj, String data)
        {
            // Convert the string data to byte data using ASCII encoding.  
            byte[] byteData = Encoding.ASCII.GetBytes(data);

            // Begin sending the data to the remote device.  
            stateObj.WorkSocket.BeginSend(byteData, 0, byteData.Length, 0,
                new AsyncCallback(SendCallback), stateObj);
        }

        private static void SendCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the socket from the state object.  
                StateObject stateObj = (StateObject)ar.AsyncState;
                Socket client = stateObj.WorkSocket;

                // Complete sending the data to the remote device.  
                int bytesSent = client.EndSend(ar);

                // Signal that all bytes have been sent.  
                sendDone[stateObj.Index].Set();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        private static String FormatResult(StateObject stateObj)
        {
            StringReader reader = new StringReader(responses[stateObj.Index]);
            String line;

            while ((line = reader.ReadLine()) != null)
            {
                if(line.StartsWith("Content-Length"))
                {
                    return String.Format("Host: {0} \n{1} \n", stateObj.Host, line);
                }
            }

            return String.Format("Host: {0} \nNo content. \n", stateObj.Host);
        }
    }
}
