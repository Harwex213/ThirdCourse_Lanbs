using System;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Web;
using System.Web.WebSockets;

namespace Lab01
{
    public class HttpHandlerTask1 : IHttpHandler
    {
        public bool IsReusable => true;
        
        public void ProcessRequest(HttpContext context)
        {
            var res = context.Response;
            res.Write($"GET-Http-KOA:ParamA = {context.Request.QueryString["ParamA"]}, " +
                      $"ParamB = {context.Request.QueryString["ParamB"]}");
        }
    }

    public class HttpHandlerTask2 : IHttpHandler
    {
        public bool IsReusable => true;
        
        public void ProcessRequest(HttpContext context)
        {
            var res = context.Response;
            res.Write($"POST-Http-KOA:ParamA = {context.Request["ParamA"]}, " +
                      $"ParamB = {context.Request["ParamB"]}");
        }
    }
    
    public class HttpHandlerTask3 : IHttpHandler
    {
        public bool IsReusable => true;
        
        public void ProcessRequest(HttpContext context)
        {
            var res = context.Response;
            res.Write($"PUT-Http-KOA:ParamA = {context.Request["ParamA"]}, " +
                      $"ParamB = {context.Request["ParamB"]}");
        }
    }
    
    public class HttpHandlerTask4 : IHttpHandler
    {
        public bool IsReusable => true;
        
        public void ProcessRequest(HttpContext context)
        {
            var res = context.Response;
            res.Write(Convert.ToInt32(context.Request["X"]) +
                      Convert.ToInt32(context.Request["Y"]));
        }
    }

    public class HttpHandlerTask5 : IHttpHandler
    {
        public bool IsReusable => true;

        public void ProcessRequest(HttpContext context)
        {
            var res = context.Response;
            switch (context.Request.HttpMethod)
            {
                case "GET":
                    res.WriteFile("task5.html");
                    break;
                case "POST":
                    res.Write(Convert.ToInt32(context.Request["X"]) *
                              Convert.ToInt32(context.Request["Y"]));
                    break;
            }

        }
    }
    
    public class HttpHandlerTask6: IHttpHandler
    {
        public bool IsReusable => true;

        public void ProcessRequest(HttpContext context)
        {
            var res = context.Response;
            switch (context.Request.HttpMethod)
            {
                case "GET":
                    res.WriteFile("task6.html");
                    break;
                case "POST":
                    res.Write(Convert.ToInt32(context.Request["X"]) *
                              Convert.ToInt32(context.Request["Y"]));
                    break;
            }
        }
    }

    public class HttpHandlerWebsockets : IHttpHandler
    {
        public bool IsReusable => false;
        
        public void ProcessRequest(HttpContext context)
        {
            if (context.IsWebSocketRequest)
            {
                context.AcceptWebSocketRequest(ProcessWebSocket);
            }
            else
            {
                context.Response.WriteFile("taskWs.html");
            }
        }

        private static async Task ProcessWebSocket(AspNetWebSocketContext context)
        {
            var socket = context.WebSocket;
            while (socket.State == WebSocketState.Open)
            {
                Thread.Sleep(2000);
                var sendBuffer = new ArraySegment<byte>(Encoding.UTF8.GetBytes(DateTime.Now.ToLongTimeString()));
                await socket.SendAsync(sendBuffer, WebSocketMessageType.Text, true, CancellationToken.None);
            }
        }
    }
}