using DotNetty.Buffers;
using DotNetty.Handlers.Timeout;
using DotNetty.Transport.Channels;
using System;
using System.Text;

namespace Core
{
    public class MonitorServerHandler : SimpleChannelInboundHandler<string>
    {
        public IMonitorContext Context { get; set; }
        public MonitorServerHandler(IMonitorContext ctx)
        {
            this.Context = ctx;
        }

        protected override void ChannelRead0(IChannelHandlerContext ctx, string msg)
        {
            var entity = MonitorSerializer.Deserialize<MonitorEntity>(msg);
            MonitorTriggerType triggerType = (MonitorTriggerType)entity.CMD;
            TriggeHandle(ctx, triggerType, entity.Body);
        }

        private void TriggeHandle(IChannelHandlerContext ctx, MonitorTriggerType type, string body)
        {
            var trigger = this.Context.TriggerGroup[type];
            if (trigger != null)
            {
                trigger.Parameter = new MonitorTriggerParameter(type, body, ctx);
                trigger.Context = this.Context;
                trigger.Handle();
            }
            else
            {
                this.Context.LogError(string.Format("当前类型[{0}]触发器不存在,请认真检查是否具有当前类型触发器", (int)type));
            }
       }

        public override void UserEventTriggered(IChannelHandlerContext ctx, object evt)
        {
            if (evt is IdleStateEvent)
            {
                ctx.Channel.CloseAsync();
            }
        }

        public override void ChannelActive(IChannelHandlerContext ctx)
        {
            var session = this.Context.SessionGroup.Add(new MonitorSession(ctx));
            this.Context.LogInfo($"客户端{ctx.Channel.RemoteAddress}上线");
            this.Context.SendEmail(session.Id, $"{ ctx.Channel.RemoteAddress }");
        }

        public override void ChannelInactive(IChannelHandlerContext ctx)
        {
            this.Context.SessionGroup.Remove(ctx);
            this.Context.LogInfo($"客户端{ctx.Channel.RemoteAddress}离线了");
        }

        public override void ExceptionCaught(IChannelHandlerContext ctx, Exception exception)
        {
            ctx.CloseAsync();
        }
    }
}
