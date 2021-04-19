using System;
using System.IO;
using System.Net.Mail;

namespace Exercise01
{
    class SmtpFacade
    {
        public void Send(string From, string To, string Subject, 
            string Body, Stream Attachment, string AttachmentMimeType)
        {
            MailMessage message = new MailMessage(From, To, Subject, Body);

            if (Attachment is not null)
                message.Attachments.Add(new Attachment(Attachment, AttachmentMimeType));

            SmtpClient client = new SmtpClient(/* some SMTP data*/);

            try
            {
                client.Send(message);
            } 
            catch (Exception exception)
            {
                Console.WriteLine("Exception caught: " + exception);
            }
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
        }
    }
}