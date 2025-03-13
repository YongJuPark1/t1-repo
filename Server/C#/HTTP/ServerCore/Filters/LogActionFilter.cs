using ServerCore.Utils;
using log4net;
using Microsoft.AspNetCore.Mvc.Filters;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;
using System;

public class LogActionFilter : ActionFilterAttribute
{
    private static readonly ILog log = LogManager.GetLogger(typeof(LogActionFilter));

    public override void OnActionExecuting(ActionExecutingContext context)
    {
        if (!context.ActionArguments.TryGetValue("req", out object value))
        {
            base.OnActionExecuting(context);
            return;
        }

        var actionName = context.RouteData.Values["action"];
        var requestBody = JsonConvert.SerializeObject(value);
        var session_id = GetSessionId(requestBody);

        var requestId = Guid.NewGuid().ToString("N").Substring(0, 10);
        context.HttpContext.Items["request_id"] = requestId;
        context.HttpContext.Items["session_id"] = session_id;
        context.HttpContext.Items["time"] = DateUtil.GetDateTime().Ticks;

        log.Debug($"[REQ] [Request ID: {requestId}] {actionName} {requestBody}");

        base.OnActionExecuting(context);
    }

    public override void OnActionExecuted(ActionExecutedContext context)
    {
        var actionName = context.RouteData.Values["action"];
        var requestId = context.HttpContext.Items["request_id"] as string;  // 고유 식별자 가져오기
        var session_id = context.HttpContext.Items["session_id"] as string;
        var usn = TokenUtil.GetUsnFromToken(session_id);
        var name = TokenUtil.GetNameFromToken(session_id);

        long _now = DateUtil.GetDateTime().Ticks;
        long start_tick = (long)context.HttpContext.Items["time"];
        long elapsed_ticks = _now - start_tick;
        var elapsed_time = TimeSpan.FromTicks(elapsed_ticks);

        string responseDetails = null;

        if (context.Result is ObjectResult objectResult)
        {
            responseDetails = JsonConvert.SerializeObject(objectResult.Value);
        }
        else if (context.Result is JsonResult jsonResult)
        {
            responseDetails = JsonConvert.SerializeObject(jsonResult.Value);
        }
        else
        {
            responseDetails = "Unknown result type";
        }

        log.Debug($"[RES] [Request ID: {requestId}] ([{usn}][{name}]) {actionName} {responseDetails} Require Time: {elapsed_time}");

        if (elapsed_time.TotalSeconds > 2)
        {
            log.Warn($"[SLOW RES] [Request ID: {requestId}] ([{usn}][{name}]) {actionName} {responseDetails} Require Time: {elapsed_time}");
        }

        base.OnActionExecuted(context);
    }

    private static string GetSessionId(string requestBody)
    {
        if (string.IsNullOrEmpty(requestBody))
        {
            return string.Empty;
        }

        try
        {
            var jsonRequest = Jil.JSON.Deserialize<BaseRequest>(requestBody);
            return jsonRequest.session_id ?? string.Empty;
        }
        catch
        {
            return string.Empty;
        }
    }
}
