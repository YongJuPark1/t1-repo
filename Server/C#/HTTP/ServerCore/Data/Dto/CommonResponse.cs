using ServerCore.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


public class CommonResponse
{

    public int Result_code { get; set; } = 0;
    public string Result_message { get; set; } = string.Empty;
    public object Data { get; set; }

    public CommonResponse() { }
    public CommonResponse(ResultEnum resultCode)
    {
        this.Result_code = (int)resultCode;
        this.Result_message = resultCode.ToString();
        this.Data = null;
    }
    public CommonResponse(ResultEnum resultCode, string message)
    {
        this.Result_code = (int)resultCode;
        this.Result_message = message;
        this.Data = null;
    }
    public CommonResponse(ResultEnum resultCode, BaseResponse data)
    {
        this.Result_code = (int)resultCode;
        this.Result_message = resultCode.ToString();
        this.Data = data;
    }
}




