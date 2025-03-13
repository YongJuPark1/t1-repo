using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Net.Http.Json;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore.Utils
{
    public class HttpUtil
    {
        public readonly HttpClient httpClient;

        public HttpUtil()
        {
            HttpClientHandler clientHandler = new HttpClientHandler
            {
                ServerCertificateCustomValidationCallback = (sender, cert, chain, sslPolicyErrors) => { return true; }
            };
            httpClient = new HttpClient(clientHandler);
            httpClient.DefaultRequestHeaders.Add("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36");
            httpClient.DefaultRequestHeaders.Accept.Clear();
            httpClient.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
            httpClient.DefaultRequestHeaders.TryAddWithoutValidation("Content-Type", "application/json");

        }       

        public async Task<T> PostRequest<T>(string apiName, object parameters, string uri, string key, string unixTimeStamp, string accesskey)
        {
            HttpClient client = httpClient;

            try
            {
                //client = new HttpClient();
                client.BaseAddress = new Uri(uri);
                client.DefaultRequestHeaders.TryAddWithoutValidation("x-ncp-iam-access-key", accesskey);
                client.DefaultRequestHeaders.TryAddWithoutValidation("x-ncp-apigw-timestamp", unixTimeStamp);
                client.DefaultRequestHeaders.TryAddWithoutValidation("x-ncp-apigw-signature-v2", key);

                HttpResponseMessage result = null;

                string Serialized = JsonConvert.SerializeObject(parameters);
                HttpContent content = new StringContent(Serialized, Encoding.Unicode, "application/json");

                if (parameters != null)
                {
                    result = await client.PostAsync(apiName, content);

                }
                else
                {
                    result = await client.PostAsync(apiName, null);
                }

                if (result.StatusCode == HttpStatusCode.Accepted && result.StatusCode == HttpStatusCode.OK)
                {
                    return result.Content.ReadFromJsonAsync<T>().Result;
                }
                else
                {
                    return (T)Activator.CreateInstance(typeof(T), new object[] { result.StatusCode, result.ReasonPhrase });
                }
            }
            catch
            {
                return default(T);
            }
            finally
            {
                if (client != null)
                {
                    client.Dispose();
                    client = null;
                }
            }
        }

    }
}
