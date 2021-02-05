using System;
using System.Collections.Generic;
using System.Text;

namespace Core
{
    /// <summary>
    /// 触发器类型
    /// </summary>
    public enum MonitorTriggerType
    {
        /// <summary>
        /// 心跳
        /// </summary>
        HEARTBEAT = 100,
        /// <summary>
        /// 执行cmd命令
        /// </summary>
        EXECUTECMD,
        /// <summary>
        /// 获取屏幕
        /// </summary>
        GETFULLSCREEN,
        /// <summary>
        /// 从摄像头获取图片
        /// </summary>
        STARTCAPTURE,
        /// <summary>
        /// 完成获取
        /// </summary>
        COMPLETECAPTURE
    }
}
