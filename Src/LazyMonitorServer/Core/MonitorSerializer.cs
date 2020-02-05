using System;
using System.Collections.Generic;
using System.Text;
using Newtonsoft.Json;

namespace Core
{
    /// <summary>
    /// 序列化类
    /// </summary>
    public class MonitorSerializer
    {
        public static string Serialize(object entity)
        {
            return JsonConvert.SerializeObject(entity);
        }

        public static T Deserialize<T>(string value)
        {
            return JsonConvert.DeserializeObject<T>(value);
        }
    }
}
