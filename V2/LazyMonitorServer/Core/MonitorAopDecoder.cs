using DotNetty.Buffers;
using DotNetty.Codecs;
using DotNetty.Transport.Channels;
using System;
using System.Collections.Generic;
using System.Text;

namespace Core
{
    public class MonitorAopDecoder : ByteToMessageDecoder
    {
        protected override void Decode(IChannelHandlerContext context, IByteBuffer input, List<object> output)
        {
            //int len = input.ReadIntLE();
            //string msg = input.ReadString(len, Encoding.UTF8);
            //output.Add(msg);
            
            

        }
    }
}
