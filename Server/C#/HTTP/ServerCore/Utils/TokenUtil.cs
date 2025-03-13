using Microsoft.IdentityModel.Tokens;
using System;
using System.Collections.Generic;
using System.IdentityModel.Tokens.Jwt;
using System.Linq;
using System.Security.Claims;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore.Utils
{
    public class TokenUtil
    {
        private const string SECURITY_KEY = "br5!onfHa12azvbthSagrett%!@ze1233732891";

        public static string GenerateJWTToken(long usn, string nickname)
        {
            var credentials = new SigningCredentials(new SymmetricSecurityKey(Encoding.UTF8.GetBytes(SECURITY_KEY)), SecurityAlgorithms.HmacSha256);

            var claims = new[]
            {
                new Claim("usn", Convert.ToString(usn)),
                new Claim("name", nickname)
            };
            var token = new JwtSecurityToken(
                claims: claims,
                expires: DateTime.Now.AddMonths(1),
                signingCredentials: credentials
            );

            return new JwtSecurityTokenHandler().WriteToken(token);
        }

        public static long GetUsnFromToken(string token)
        {
            if (string.IsNullOrEmpty(token)) return 0L;

            try
            {
                new JwtSecurityTokenHandler().ValidateToken(token, new TokenValidationParameters
                {
                    ValidateIssuerSigningKey = true,
                    IssuerSigningKey = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(SECURITY_KEY)),
                    ValidateIssuer = false,
                    ValidateAudience = false,
                    ClockSkew = TimeSpan.Zero
                }, out SecurityToken validatedToken);

                return Convert.ToInt64(((JwtSecurityToken)validatedToken).Claims.First(claim => claim.Type == "usn").Value);
            }
            catch
            {
                return 0L;
            }
        }

        public static string GetNameFromToken(string token)
        {
            if (string.IsNullOrEmpty(token)) return string.Empty;

            try
            {
                new JwtSecurityTokenHandler().ValidateToken(token, new TokenValidationParameters
                {
                    ValidateIssuerSigningKey = true,
                    IssuerSigningKey = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(SECURITY_KEY)),
                    ValidateIssuer = false,
                    ValidateAudience = false,
                    ClockSkew = TimeSpan.Zero
                }, out SecurityToken validatedToken);

                return ((JwtSecurityToken)validatedToken).Claims.First(claim => claim.Type == "name").Value.ToString();
            }
            catch
            {
                return string.Empty;
            }
        }
    }
}
