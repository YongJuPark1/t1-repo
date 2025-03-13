using ServerCore.Data.Cache;
using log4net;
using Microsoft.Extensions.Caching.Memory;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;


public abstract class BaseService
{
    protected static readonly ILog log = LogManager.GetLogger(typeof(BaseService));
    protected IMemoryCache memCache;
    
    protected string session_id = string.Empty;

    public void Init(IMemoryCache memCache, string session_id)
    {   
        this.memCache = memCache;
        this.session_id = session_id;
    }

    protected void InvalidateUserCache()
    {
        if (memCache != null)
        {
            if (session_id != null)
            {
                memCache.Remove(session_id);
            }
        }
    }

    public abstract Task<CommonResponse> ProcessAsync(UserCache userCache, BaseRequest baseReq, CancellationToken cancellationToken);
}

