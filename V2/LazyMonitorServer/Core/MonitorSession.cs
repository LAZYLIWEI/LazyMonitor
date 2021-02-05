using DotNetty.Transport.Channels;
using System;
using System.Collections.Generic;
using System.Text;

namespace Core
{
    public class MonitorSession
    {
        /// <summary>
        /// id
        /// </summary>
        public int Id { get; set; }
        /// <summary>
        /// ctx
        /// </summary>
        public IChannelHandlerContext Context { get; }
        /// <summary>
        /// 别名
        /// </summary>
        public string Alias { get; set; }

        public MonitorSession(IChannelHandlerContext ctx)
            : this(ctx, string.Empty)
        {
        }

        public MonitorSession(IChannelHandlerContext ctx, string alias)
        {
            this.Context = ctx;
            this.Alias = alias;
        }

        public override string ToString()
        {
            return $"[{Id}] [{Alias}] {Context.Channel.RemoteAddress}";
        }

    }
}
