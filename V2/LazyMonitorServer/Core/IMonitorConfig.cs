using System;
using System.Collections.Generic;
using System.Text;

namespace Core
{
    public interface IMonitorConfig
    {
        string Version { get; }
        int Port { get; }
        int Heartbeat { get; }
        string UploadDirectory { get; }
        bool EnableEmailNotify { get; }
        bool EnableLog { get; }

        string LogName { get; }
        string LogRepositoryName { get; }
        string LogConfigFilename { get; }

        string EmailName { get; }
        string EmailAddress { get; }
        string EmailPassword { get; }
        string EmailHost { get; set; }
        int EmailPort { get; set; }
    }
}
