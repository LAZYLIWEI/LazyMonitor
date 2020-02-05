using Core;
using DotNetty.Transport.Channels;
using System;
using System.Collections.Generic;
using System.Text;

namespace Server
{
    /// <summary>
    /// 心跳包触发器
    /// </summary>
    public class HeartbeatTrigger : IMonitorTrigger
    {
        public IMonitorContext Context { get; set; }
        public MonitorTriggerParameter Parameter { get; set; }
        public void Handle()
        {
        }
    }
}
