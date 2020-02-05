using DotNetty.Transport.Channels;
using System;
using System.Collections.Generic;
using System.Text;

namespace Core
{
    public interface IMonitorContext
    {
        IEventLoopGroup BossGroup { get; }
        IEventLoopGroup WorkerGroup { get; }
        IChannel BindResult { get; }
        IMonitorConfig Config { get; }
        MonitorSessionGroup SessionGroup { get; set; }
        Dictionary<MonitorTriggerType, IMonitorTrigger> TriggerGroup { get; }

        void LogInfo(string message, Exception exception = null);
        void LogWarn(string message, Exception exception = null);
        void LogError(string message, Exception exception = null);
        void SendEmail(int id, string address);
    }
}
