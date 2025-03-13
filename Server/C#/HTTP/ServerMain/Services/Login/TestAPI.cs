using Azure;
using ServerCore.Data.Cache;
using ServerCore.Enums;
using ServerCore.Service;
using ServerCore.Utils;
using ServerMain;
using DocumentFormat.OpenXml.Spreadsheet;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata.Internal;
using Microsoft.Extensions.Caching.Memory;
using Microsoft.IdentityModel.Tokens;
using System;
using System.Collections.Generic;
using System.IdentityModel.Tokens.Jwt;
using System.Linq;
using System.Reflection;
using System.Security.Claims;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ServerMain.Services
{
    public class TestReq : BaseRequest
    {
        public string id { get; set; }
        public string pw { get; set; }
    }

    public class TestRes : BaseResponse
    {
        public long usn { get; set; }
    }

    public class TestAPI : BaseService
    {
        public override async Task<CommonResponse> ProcessAsync(UserCache userCache, BaseRequest baseReq, CancellationToken cancellationToken)
        {
            TestReq req = (TestReq)baseReq;
            TestRes res = new TestRes();
            res.usn = 9999;           
            return new CommonResponse(ResultEnum.SUCCESS, res);
        }
    }
}
