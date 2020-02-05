using DotNetty.Handlers.Logging;
using DotNetty.Transport.Bootstrapping;
using DotNetty.Transport.Channels;
using DotNetty.Transport.Channels.Sockets;
using System.Collections.Generic;

namespace Core
{
    /// <summary>
    /// server
    /// </summary>
    public class MonitorServer
    {
        public MonitorContext Context { get; set; }
        public IMonitorConfig Config { get; }
        public MonitorServer(IMonitorConfig config)
        {
            this.Config = config;
            this.Context = new MonitorContext {
                BossGroup = new MultithreadEventLoopGroup(),
                WorkerGroup = new MultithreadEventLoopGroup(),
                SessionGroup = new MonitorSessionGroup(),
                TriggerGroup = new Dictionary<MonitorTriggerType, IMonitorTrigger>(),
                Config = this.Config,
                Logger = null,
                Email = null
            };

            if (this.Config.EnableLog)
            {
                this.Context.Logger = new MonitorLogger(this.Config);
            }
            if (this.Config.EnableEmailNotify)
            {
                this.Context.Email = new MonitorEmail(this.Context);
            }
        }

        public void SetTrigger(MonitorTriggerType type, IMonitorTrigger trigger)
        {
            this.Context.TriggerGroup.Add(type, trigger);
        }

        public void Run()
        {
            var bootstrap = new ServerBootstrap();
            bootstrap.Group(this.Context.BossGroup, this.Context.WorkerGroup)
                .Channel<TcpServerSocketChannel>()
                .Handler(new LoggingHandler(LogLevel.INFO))
                .ChildHandler(new MonitorChannelInitializer(this.Context));
            var boundChannelTask = bootstrap.BindAsync(this.Config.Port);
            this.Context.BindResult = boundChannelTask.Result;
        }

        public void Close()
        {
            this.Context.BindResult?.CloseAsync();
            this.Context.WorkerGroup.ShutdownGracefullyAsync();
            this.Context.WorkerGroup.ShutdownGracefullyAsync();
        }

    }
}
