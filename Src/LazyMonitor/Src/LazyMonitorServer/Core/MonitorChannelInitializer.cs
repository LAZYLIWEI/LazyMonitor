using DotNetty.Codecs;
using DotNetty.Handlers.Timeout;
using DotNetty.Transport.Channels;
using System.Text;

namespace Core
{
    public class MonitorChannelInitializer : ChannelInitializer<IChannel>
    {
        public IMonitorContext Context { get; set; }
        public MonitorChannelInitializer(IMonitorContext ctx)
        {
            this.Context = ctx;
        }

        protected override void InitChannel(IChannel channel)
        {
            var pipeline = channel.Pipeline;
            pipeline.AddLast(new IdleStateHandler(this.Context.Config.Heartbeat, 0, 0));
            pipeline.AddLast(new LengthFieldBasedFrameDecoder(int.MaxValue, 0, 4, 0, 4));
            pipeline.AddLast(new LengthFieldPrepender(4));
            pipeline.AddLast(new StringDecoder(Encoding.UTF8));
            pipeline.AddLast(new StringEncoder(Encoding.UTF8));
            pipeline.AddLast(new MonitorServerHandler(this.Context));
        }
    }
}
