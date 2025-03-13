using ServerCore.Data.Cache;
using ServerCore.Enums;
using log4net;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Filters;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Caching.Memory;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata.Ecma335;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore.Filters
{
    public class AuthActionFilter : ActionFilterAttribute
    {
        private static readonly ILog log = LogManager.GetLogger(typeof(AuthActionFilter));

        public override void OnActionExecuting(ActionExecutingContext context)
        {
            var actionName = context.RouteData.Values["action"]?.ToString();

            if (!string.IsNullOrEmpty(actionName))
            {
                if (actionName.Equals("TestAPI"))
                {
                    base.OnActionExecuting(context);
                    return;
                }
            }

            string session_id = context.HttpContext.Items["session_id"] as string;

            UserCache userCache = GetUserCache(context, session_id);
            if(userCache == null)
            {
                context.Result = new JsonResult(new CommonResponse(ResultEnum.FAIL));
                base.OnActionExecuting(context); 
                return;
            }

            base.OnActionExecuting(context);
        }

        private static UserCache GetUserCache(ActionExecutingContext context, string session_id)
        {
            if (string.IsNullOrEmpty(session_id))
            {
                log.Debug("[AuthActionFilter::userCache]세션에러(session is null or empty");
                return null;
            }

            IMemoryCache memCache = context.HttpContext.RequestServices.GetService(typeof(IMemoryCache)) as IMemoryCache;
            UserCache userCache = memCache?.Get<UserCache>(session_id);
            if (userCache == null)
            {
                return null;
            }
            else
            {
                // 캐시 연장
                memCache.Set(session_id, userCache, new MemoryCacheEntryOptions().SetSlidingExpiration(TimeSpan.FromHours(1)));
            }

            return userCache;
        }
    }
}
