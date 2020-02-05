using System;
using System.Collections.Generic;
using System.Text;

namespace Core
{
    /// <summary>
    /// base64
    /// </summary>
    public class MonitorBase64
    {
        /// <summary>
        /// 编码
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static string Encode(byte[] value)
        {
            return Convert.ToBase64String(value);
        }

        /// <summary>
        /// 解码
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static byte[] Decode(string value)
        {
            return Convert.FromBase64String(value);
        }
    }
}
