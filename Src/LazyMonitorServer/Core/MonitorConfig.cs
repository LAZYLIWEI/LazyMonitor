using Microsoft.Extensions.Configuration;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace Core
{
    public class MonitorConfig : IMonitorConfig
    {
        /// <summary>
        /// 版本
        /// </summary>
        public string Version { get; set; }
        /// <summary>
        /// 心跳检测时间(s)
        /// 为0则不开启
        /// </summary>
        public int Heartbeat { get; set; }
        /// <summary>
        /// 端口
        /// </summary>
        public int Port { get; set; }
        /// <summary>
        /// 上传文件目录
        /// </summary>
        public string UploadDirectory { get; set; }
        /// <summary>
        /// 启用邮件通知
        /// </summary>
        public bool EnableEmailNotify { get; set; }
        /// <summary>
        /// 启用日志
        /// </summary>
        public bool EnableLog { get; set; }

        /// <summary>
        /// 日志名
        /// </summary>
        public string LogName { get; set; }
        /// <summary>
        /// 日志的仓储名
        /// </summary>
        public string LogRepositoryName { get; set; }
        /// <summary>
        /// 日志的文件名
        /// </summary>
        public string LogConfigFilename { get; set; }

        public string EmailName { get; set; }
        public string EmailAddress { get; set; }
        public string EmailPassword { get; set; }
        public string EmailHost { get; set; }
        public int EmailPort { get; set; }

        public MonitorConfig()
        {
            this.Heartbeat = 0;
            this.Port = 30000;
            this.EnableLog = false;
            this.EnableEmailNotify = false;
        }

        public MonitorConfig LoadFromConfigFile()
        {
            IConfigurationBuilder builder = new ConfigurationBuilder()
                .SetBasePath(Directory.GetCurrentDirectory())
                .AddJsonFile("config.json");
            IConfigurationRoot configuration = builder.Build();

            var baseConfiguration = configuration.GetSection("Base");
            this.Version = baseConfiguration["Version"];
            this.Port = Convert.ToInt32(baseConfiguration["Port"]);
            this.Heartbeat = Convert.ToInt32(baseConfiguration["Heartbeat"]);
            this.UploadDirectory = baseConfiguration["UploadDirectory"];
            this.EnableLog = Convert.ToBoolean(baseConfiguration["EnableLog"]);
            this.EnableEmailNotify = Convert.ToBoolean(baseConfiguration["EnableEmailNotify"]);

            if (this.EnableLog)
            {
                var logConfiguration = configuration.GetSection("Log");
                this.LogName = logConfiguration["Name"];
                this.LogConfigFilename = logConfiguration["ConfigFilename"];
                this.LogRepositoryName = logConfiguration["RepositoryName"];
            }

            if (this.EnableEmailNotify)
            {
                var emailConfiguration = configuration.GetSection("EmailNotify");
                this.EmailName = emailConfiguration["Name"];
                this.EmailAddress = emailConfiguration["Address"];
                this.EmailPassword = emailConfiguration["Password"];
                this.EmailHost = emailConfiguration["Host"];
                this.EmailPort = Convert.ToInt32(emailConfiguration["Port"]);
            }
            return this;
        }

    }
}
