using DotNetty.Transport.Channels;
using System;
using System.Collections.Generic;
using System.Text;

namespace Core
{
    public interface IMonitorTrigger
    {
        IMonitorContext Context { get; set; }
        MonitorTriggerParameter Parameter { get; set; }
        void Handle();
    }
}
