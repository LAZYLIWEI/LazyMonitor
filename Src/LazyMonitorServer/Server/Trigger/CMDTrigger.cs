using Core;
using System;
using System.Collections.Generic;
using System.Text;

namespace Server
{
    public class CMDTrigger : IMonitorTrigger
    {
        public IMonitorContext Context { get; set; }
        public MonitorTriggerParameter Parameter { get; set; }

        public void Handle()
        {
            Console.WriteLine(Parameter.Parameter);
        }
    }
}
