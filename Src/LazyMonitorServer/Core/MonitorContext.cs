using DotNetty.Transport.Channels;
using System;
using System.Collections.Generic;
using System.Text;

namespace Core
{
    /// <summary>
    /// 监控上下文
    /// </summary>
    public class MonitorContext : IMonitorContext
    {
        public IMonitorConfig Config { get; set; }
        public MonitorLogger Logger {get; set; }
        public MonitorSessionGroup SessionGroup { get; set; }
        public Dictionary<MonitorTriggerType, IMonitorTrigger> TriggerGroup { get; set; }
        public IEventLoopGroup BossGroup { get; set; }
        public IEventLoopGroup WorkerGroup { get; set; }
        public IChannel BindResult { get; set; }
        public MonitorEmail Email { get; set; }


        /// <summary>
        /// 普通日志
        /// </summary>
        /// <param name="message"></param>
        /// <param name="exception"></param>
        public void LogInfo(string message, Exception exception = null)
        {
            if (exception == null)
                this.Logger?.Info(message);
            else
                this.Logger?.Info(message, exception);
        }

        /// <summary>
        /// 告警日志
        /// </summary>
        /// <param name="message"></param>
        /// <param name="exception"></param>
        public void LogWarn(string message, Exception exception = null)
        {
            if (exception == null)
                this.Logger?.Warn(message);
            else
                this.Logger?.Warn(message, exception);
        }

        /// <summary>
        /// 错误日志
        /// </summary>
        /// <param name="message"></param>
        /// <param name="exception"></param>
        public void LogError(string message, Exception exception = null)
        {
            if (exception == null)
                this.Logger?.Error(message);
            else
                this.Logger?.Error(message, exception);
        }


        /// <summary>
        /// 发送邮件
        /// </summary>
        /// <param name="id"></param>
        /// <param name="address"></param>
        public void SendEmail(int id, string address)
        {
            this.Email?.Send(id, address);
        }

    }
}
