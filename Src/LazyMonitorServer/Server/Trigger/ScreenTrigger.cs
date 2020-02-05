using Core;
using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;

namespace Server
{
    public class ScreenTrigger : IMonitorTrigger
    {
        public IMonitorContext Context { get; set; }
        public MonitorTriggerParameter Parameter { get; set; }

        public void Handle()
        {
            byte[] imgBuf = MonitorBase64.Decode(Parameter.Parameter);
            var remoteAddress = Parameter.ChannelHandlerContext.Channel.RemoteAddress;
            IPAddress ip = ((IPEndPoint)remoteAddress).Address;
            //Console.WriteLine("ip" + ip.Loopback);
            string filename = Util.CreateFilename() + ".jpg";
            string path = string.Format("{0}/{1}/{2}/", Context.Config.UploadDirectory, "screen", Util.CreateSubPath());
            Util.CreateDirectory(path);

            string fullname = path + filename;
            File.WriteAllBytes(fullname, imgBuf);
        }
    }
}
