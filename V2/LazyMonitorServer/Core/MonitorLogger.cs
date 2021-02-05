using log4net;
using log4net.Config;
using System;
using System.IO;

namespace Core
{
    /// <summary>
    /// 日志
    /// </summary>
    public class MonitorLogger
    {
        private readonly ILog logger;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="config"></param>
        public MonitorLogger(IMonitorConfig config) 
        {
            var repository = LogManager.CreateRepository(config.LogRepositoryName);
            XmlConfigurator.Configure(repository, new FileInfo(config.LogConfigFilename));
            this.logger = LogManager.GetLogger(repository.Name, config.LogName);
        }

        /// <summary>
        /// 普通日志
        /// </summary>
        /// <param name="message"></param>
        /// <param name="exception"></param>
        public void Info(string message, Exception exception = null)
        {
            if (exception == null)
                logger.Info(message);
            else
                logger.Info(message, exception);
        }

        /// <summary>
        /// 告警日志
        /// </summary>
        /// <param name="message"></param>
        /// <param name="exception"></param>
        public void Warn(string message, Exception exception = null)
        {
            if (exception == null)
                logger.Warn(message);
            else
                logger.Warn(message, exception);
        }

        /// <summary>
        /// 错误日志
        /// </summary>
        /// <param name="message"></param>
        /// <param name="exception"></param>
        public void Error(string message, Exception exception = null)
        {
            if (exception == null)
                logger.Error(message);
            else
                logger.Error(message, exception);
        }
    }
}
