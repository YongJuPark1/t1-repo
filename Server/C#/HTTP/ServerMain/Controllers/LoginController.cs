using ServerMain.Services;
using Microsoft.AspNetCore.Mvc;
using System.Threading;
using System.Threading.Tasks;

namespace ServerMain.Controllers
{
    public class LoginController : BaseController
    {
        [HttpPost("/Test")]
        public async Task<CommonResponse> TestAPI(TestReq req, CancellationToken cancellation) =>
            await InvokeService(req, cancellation).ConfigureAwait(false);
    }
}
