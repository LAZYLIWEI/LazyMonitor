using MailKit.Net.Smtp;
using MimeKit;
using System;
using System.Collections.Generic;
using System.Text;

namespace Core
{
    public class MonitorEmail
    {
        public IMonitorContext Context { get; set; }
        public MonitorEmail(IMonitorContext ctx)
        {
            this.Context = ctx;
        }

        /// <summary>
        /// 发送邮件
        /// </summary>
        /// <param name="id"></param>
        /// <param name="address"></param>
        public void Send(int id, string address)
        {
            try
            {
                var emailBody = new StringBuilder("肉机上线:\r\n");
                emailBody.AppendLine($"--------------------------------------");
                emailBody.AppendLine($"Id:{id}");
                emailBody.AppendLine($"Address:{address}");
                emailBody.AppendLine($"--------------------------------------");
                var message = new MimeMessage();
                //这里是测试邮箱，请不要发垃圾邮件，谢谢
                message.From.Add(new MailboxAddress(Context.Config.EmailName, Context.Config.EmailAddress));
                message.To.Add(new MailboxAddress("228407354", "228407354@qq.com"));

                message.Subject = "肉机上线";
                message.Body = new TextPart("plain") { Text = emailBody.ToString() };
                using (var client = new SmtpClient())
                {
                    client.ServerCertificateValidationCallback = (s, c, h, e) => true;
                    client.Connect(Context.Config.EmailHost, Context.Config.EmailPort, false);
                    client.AuthenticationMechanisms.Remove("XOAUTH2");
                    client.Authenticate(Context.Config.EmailName, Context.Config.EmailPassword);
                    client.Send(message);
                    client.Disconnect(true);
                }
            }
            catch (Exception ex)
            {
                Context.LogError(ex.ToString());
            }
        }
    }
}
