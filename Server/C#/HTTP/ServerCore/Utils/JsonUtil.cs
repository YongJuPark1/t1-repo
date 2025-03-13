using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore.Utils
{
    public static class JsonUtil
    {
        public static string ToJson<T>(T data)
        {
            //return JsonSerializer.Serialize(data);
            return Jil.JSON.Serialize<T>(data);
        }
        public static T FromJson<T>(string jsonData)
        {
            // JSON 문자열을 객체로 역직렬화
            return Jil.JSON.Deserialize<T>(jsonData);
        }
    }
}
