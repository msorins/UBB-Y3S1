using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace PPD_Lab5
{
    public class Program
    {
        public static List<string> hosts = new List<string> { "www.google.com", "www.youtube.com", "www.facebook.com", "www.linkedin.com",
                                                             "www.google.com", "www.youtube.com", "www.facebook.com","www.linkedin.com"};
        public static DateTime start;
        static void Main(string[] args)
        {
            EventDrivenDownload();
            TaskDownload();
            AsyncAwaitDownload().GetAwaiter().GetResult();
            Console.WriteLine("\n\n");
        }

        public static void EmptyAllLists()
        {
            AsynchronousClient.connectDone = new List<ManualResetEvent>();
            AsynchronousClient.sendDone = new List<ManualResetEvent>();
            AsynchronousClient.receiveDone = new List<ManualResetEvent>();
            AsynchronousClient.responses = new List<String>();
        }

        public static void AddNewObjects()
        {
            AsynchronousClient.connectDone.Add(new ManualResetEvent(false));
            AsynchronousClient.sendDone.Add(new ManualResetEvent(false));
            AsynchronousClient.receiveDone.Add(new ManualResetEvent(false));
            AsynchronousClient.responses.Add(String.Empty);

        }
        public static void EventDrivenDownload()
        { 
            EmptyAllLists();
            Console.WriteLine("_______________ Event driven download _______________\n");
            start = DateTime.Now;
            for (int i=0;i<hosts.Count;i++)
            {
                AddNewObjects();
                Console.WriteLine(AsynchronousClient.StartClient(new StateObject(hosts[i], i)));
            }
            Console.WriteLine(String.Format("\tDuration: {0}\n", DateTime.Now - start));
        }
       
        public static void TaskDownload()
        {
            Console.WriteLine("_______________ Task download _______________\n");
            List<Task> tasks = new List<Task>();
            EmptyAllLists();
            start = DateTime.Now;
            for (int i = 0; i < hosts.Count; i++)
            {
                int index = 0 + i;
                AddNewObjects();
                tasks.Add(Task.Run(() => {
                    Console.WriteLine(AsynchronousClient.StartClient(new StateObject(hosts[index], index)));
                }));
            }

            Task.WaitAll(tasks.ToArray());
            Console.WriteLine(String.Format("\tDuration: {0}\n",DateTime.Now-start));
        }

        public static async Task AsyncAwaitDownload()
        {
            Console.WriteLine("_______________ Async/Await download _______________\n");
            List < Task> tasks = new List<Task>();
            EmptyAllLists();
            start = DateTime.Now;
            for (int i = 0; i < hosts.Count; i++)
            {
                int index = 0 + i;
                AddNewObjects();
                String result =  await Task.Run(() => {
                    return AsynchronousClient.StartClient(new StateObject(hosts[index], index));
                });
                Console.WriteLine(result);
            }
            Console.WriteLine(String.Format("\tDuration: {0}\n", DateTime.Now - start));
        }
    }
}
