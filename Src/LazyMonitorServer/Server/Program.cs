using Core;
using DotNetty.Transport.Channels;
using System;
using System.Linq;
using System.Threading.Tasks;

namespace Server
{
    public class Program
    {
        static void Main(string[] args)
        {
            MonitorServer server = new MonitorServer(new MonitorConfig().LoadFromConfigFile());

            try
            {
                server.SetTrigger(MonitorTriggerType.HEARTBEAT, new HeartbeatTrigger());
                server.SetTrigger(MonitorTriggerType.EXECUTECMD, new CMDTrigger());
                server.SetTrigger(MonitorTriggerType.GETFULLSCREEN, new ScreenTrigger());
                server.SetTrigger(MonitorTriggerType.COMPLETECAPTURE, new CompleteCaptureTrigger());
                server.Run();

                // 循环
                MonitorUI.Loop(server.Context);
            }
            catch(Exception ex)
            {
                server.Context.Logger.Error(ex.ToString());
                server.Close();
            }

            Console.ReadKey();
        }
    }
}
