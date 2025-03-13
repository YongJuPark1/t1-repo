using ServerCore.Service;
using ServerCore.Settings;
using log4net;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.ResponseCompression;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.OpenApi.Models;
using System;


namespace ServerMain
{
    public class Startup
    {
        public IConfiguration Configuration { get; set; }
        public static AppSettings Settings { get; private set; }

        public Startup(IWebHostEnvironment env)
        {
            var buidler = new ConfigurationBuilder()
                .SetBasePath(env.ContentRootPath)
                .AddJsonFile($"appsettings.{env.EnvironmentName}.json", optional: true, reloadOnChange: true)
                .AddEnvironmentVariables();

            Configuration = buidler.Build();
            Settings = Configuration.Get<AppSettings>();
            ValidateSettings(Settings);
        }

        public void ConfigureServices(IServiceCollection services)
        {
            services.AddControllers();
            services.AddDataProtection();
            services.AddMemoryCache();
            services.AddDistributedMemoryCache();
            services.AddSwaggerGen(c =>
            {
                c.SwaggerDoc("v1", new OpenApiInfo { Title = "Server Main", Version = "v1", Description = "test sessionid : " });
            });

            services.AddSession(options =>
            {
                options.IdleTimeout = TimeSpan.FromHours(2);
                options.Cookie.SecurePolicy = CookieSecurePolicy.Always;
                options.Cookie.MaxAge = TimeSpan.FromHours(4); // 쿠키는 4시간 동안 유효
                options.Cookie.IsEssential = true;
            });

            // log4net ILog 서비스 등록
            services.AddSingleton(provider =>
            {
                // 특정 클래스의 로거를 가져오거나, 모든 로그를 위한 전역 로거를 사용할 수 있습니다.
                return LogManager.GetLogger(typeof(Startup));
            });

            services.AddHostedService<CommonBackgroundService>();

            services.AddResponseCompression(options =>
            {

                options.Providers.Add<GzipCompressionProvider>();
                options.EnableForHttps = true;
            });

        }

        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }

            app.UseSwagger();
            app.UseSwaggerUI(c => c.SwaggerEndpoint("/swagger/v1/swagger.json", "ServerMain v1"));

            app.UseHttpsRedirection();

            app.UseRouting();

            app.UseSession();

            app.UseResponseCompression();

            app.UseEndpoints(endpoints =>
            {
                endpoints.MapControllers();
            });
        }


        private static void ValidateSettings(AppSettings settings)
        {
            if (settings == null)
            {
                throw new InvalidOperationException("AppSettings configuration is missing.");
            }

            if (settings.ConnectionString == null || string.IsNullOrEmpty(settings.ConnectionString.Game))
            {
                throw new InvalidOperationException("ConnectionString is not configured.");
            }

            if (settings.AESKey == null || string.IsNullOrEmpty(settings.AESKey.Key))
            {
                throw new InvalidOperationException("AESKey is not configured.");
            }
        }
    }
}
