using DotNetty.Transport.Channels;
using System;
using System.Collections.Generic;
using System.Text;

namespace Core
{
    /// <summary>
    /// session组
    /// </summary>
    public class MonitorSessionGroup
    {
        public int GlobalId { get; set; }
        public List<MonitorSession> SessionGroup { get; }
        public MonitorSessionGroup()
        {
            this.SessionGroup = new List<MonitorSession>();
            this.GlobalId = 100000;
        }

        public void Add(IChannelHandlerContext ctx)
        {
            this.Add(new MonitorSession(ctx, string.Empty));
        }

        public MonitorSession Add(MonitorSession session)
        {
            session.Id = this.GlobalId++;
            this.SessionGroup.Add(session);
            return session;
        }

        public MonitorSession Find(int id)
        {
            MonitorSession findSession = null;
            foreach (var item in SessionGroup)
            {
                if (item.Id == id)
                {
                    findSession = item;
                    break;
                }
            }

            return findSession;
        }

        public MonitorSession Find(IChannelHandlerContext ctx)
        {
            MonitorSession findSession = null;
            foreach (var item in SessionGroup)
            {
                if (item.Context == ctx)
                {
                    findSession = item;
                    break;
                }
            }

            return findSession;
        }

        public List<MonitorSession> Find(string alias)
        {
            List<MonitorSession> findSessionGroup = new List<MonitorSession>();
            foreach (var item in SessionGroup)
            {
                if (item.Alias == alias)
                {
                    findSessionGroup.Add(item);
                }
            }

            return findSessionGroup;
        }

        public void Remove(IChannelHandlerContext ctx)
        {
            MonitorSession findSession = Find(ctx);
            if (findSession != null)
            {
                this.SessionGroup.Remove(findSession);
            }
        }

        public void Remove(string alias)
        {
            List<MonitorSession> findSessionGroup = Find(alias);
            foreach (var item in findSessionGroup)
            {
                this.SessionGroup.Remove(item);
            }
        }

        public void Remove(int id)
        {
            MonitorSession findSession = Find(id);
            if (findSession != null)
            {
                this.SessionGroup.Remove(findSession);
            }
        }

    }
}
