using DotNetty.Transport.Channels;
using System;
using System.Collections.Generic;
using System.Text;

namespace Core
{
    /// <summary>
    /// 触发器参数
    /// </summary>
    public class MonitorTriggerParameter
    {
        public MonitorTriggerType Type { get; }
        public string Parameter { get; }
        public IChannelHandlerContext ChannelHandlerContext { get; }

        public MonitorTriggerParameter(MonitorTriggerType type, string parameter, IChannelHandlerContext ctx)
        {
            this.Type = type;
            this.Parameter = parameter;
            this.ChannelHandlerContext = ctx;
        }
    }
}
