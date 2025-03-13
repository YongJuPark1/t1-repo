using ServerCore.Service;
using log4net;
using log4net.Config;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Server.Kestrel.Core;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using System;
using System.IO;
using System.Net;

namespace ServerMain
{
    public class Program
    {
        protected static readonly ILog log = LogManager.GetLogger(typeof(Program));

        public static void Main(string[] args)
        {
            var host = CreateHostBuilder(args).Build();

            XmlConfigurator.ConfigureAndWatch(new FileInfo("log4net.config"));

            host.Run();

            //test
        }

        public static IHostBuilder CreateHostBuilder(string[] args)
        {
            var environment = Environment.GetEnvironmentVariable("ASPNETCORE_ENVIRONMENT") ?? "null";


            var config = new ConfigurationBuilder()
                    .SetBasePath(Environment.CurrentDirectory)
                    .AddJsonFile($"appsettings.{environment}.json", optional: true, reloadOnChange: true)
                    .AddJsonFile("WebProtocolSettings.json", optional: true)
                    .AddCommandLine(args)
                    .Build();

            var certPath = config.GetSection("Kestrel:Endpoints:Https:Certificate:Path").Value;
            var certPassword = config.GetSection("Kestrel:Endpoints:Https:Certificate:Password").Value;


            return Host.CreateDefaultBuilder(args)
                .ConfigureWebHostDefaults(webBuilder =>
                {

                    webBuilder.ConfigureKestrel(serverOptions =>
                    {
                        serverOptions.Limits.MaxConcurrentConnections = 2500;
                        serverOptions.Limits.MaxConcurrentUpgradedConnections = 5000;
                        serverOptions.Limits.MaxRequestBodySize = 100 * 1024;
                        serverOptions.Limits.MinRequestBodyDataRate = new MinDataRate(bytesPerSecond: 100, gracePeriod: TimeSpan.FromSeconds(10));
                        serverOptions.Limits.MinResponseDataRate = new MinDataRate(bytesPerSecond: 100, gracePeriod: TimeSpan.FromSeconds(10));

                        serverOptions.ConfigureEndpointDefaults(listenOptions =>
                        {
                            if (certPath != null && certPassword != null)
                            {
                                listenOptions.UseHttps(certPath, certPassword);
                                listenOptions.Protocols = HttpProtocols.Http1AndHttp2;
                            }
                            else
                            {
                                throw new InvalidOperationException("AppSettings configuration is missing(certPath or certPassword).");
                            }
                        });

                    })
                    .UseStartup<Startup>();

                });
        }
    }
}
