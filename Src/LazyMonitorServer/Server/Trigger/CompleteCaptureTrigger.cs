using Core;
using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;

namespace Server
{
    public class CompleteCaptureTrigger : IMonitorTrigger
    {
        public IMonitorContext Context { get; set; }
        public MonitorTriggerParameter Parameter { get; set; }

        public void Handle()
        {
            // 回答
            this.Answer();

            byte[] imgBuf = MonitorBase64.Decode(Parameter.Parameter);
            var remoteAddress = Parameter.ChannelHandlerContext.Channel.RemoteAddress;
            IPAddress ip = ((IPEndPoint)remoteAddress).Address;
            //Console.WriteLine("ip" + ip.Loopback);
            string filename = Util.CreateFilename() + ".jpg";
            string path = string.Format("{0}/{1}/{2}/", Context.Config.UploadDirectory, "camera", Util.CreateSubPath());
            Util.CreateDirectory(path);

            string fullname = path + filename;
            File.WriteAllBytes(fullname, imgBuf);
        }

        private void Answer()
        {
            MonitorEntity entity = new MonitorEntity(){
                CMD = (int)MonitorTriggerType.COMPLETECAPTURE,
                Body = string.Empty
            };
            Parameter.ChannelHandlerContext.WriteAndFlushAsync(MonitorSerializer.Serialize(entity));
        }
    }
}
