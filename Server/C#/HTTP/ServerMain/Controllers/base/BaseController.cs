using ServerCore.Enums;
using ServerCore.Filters;
using log4net;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Caching.Memory;
using Newtonsoft.Json;
using System.Globalization;
using System.Net.Mime;
using System.Reflection;
using System.Threading.Tasks;
using System.Threading;
using System;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.AspNetCore.Routing;
using ServerCore.Data.Cache;

[LogActionFilter]
[AuthActionFilter]
[Route("[action]")]
[ApiController]
[Produces(MediaTypeNames.Application.Json)]
public class BaseController : ControllerBase
{
    private static readonly ILog log = LogManager.GetLogger(typeof(BaseController));
  

    protected async Task<CommonResponse> InvokeService(BaseRequest req, CancellationToken cancellationToken)
    {
        try
        {
            var actionName = HttpContext.GetRouteData().Values["action"]?.ToString();
            if (string.IsNullOrEmpty(actionName))
            {
                throw new ArgumentException("Action name cannot be null or empty");
            }

            BaseService service = CreateServiceInstance(actionName)
                                  ?? throw new Exception($"Cannot find service for action: {actionName}");

            var session_id = HttpContext.Items["session_id"]?.ToString();
            IMemoryCache memCache = null;
            UserCache userCache = null;

            if (session_id != null)
            {
                memCache = HttpContext.RequestServices.GetService(typeof(IMemoryCache)) as IMemoryCache;
                userCache = memCache.Get<UserCache>(session_id);
            }
            return await service.ProcessAsync(userCache, req, cancellationToken).ConfigureAwait(false);
        }
        catch (Exception ex)
        {
            log.Warn($"[Exception][REQ] {JsonConvert.SerializeObject(req)}\r\n{ex}");
            return new CommonResponse(ResultEnum.FAIL);
        }
    }

    private BaseService CreateServiceInstance(string actionName)
    {
        var assembly = Assembly.GetExecutingAssembly();
        var fullServiceName = $"ServerMain.Services.{actionName}";

        return assembly.CreateInstance(
            fullServiceName,
            false,
            BindingFlags.Public | BindingFlags.Instance,
            null,
            null,
            CultureInfo.CurrentCulture,
            null) as BaseService;
    }
}
