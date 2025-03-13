using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore.Settings
{
    public class AppSettings
    {
        public ConnectionItem ConnectionString { get; set; }
        public AESKey AESKey { get; set; }
    }

    public class ConnectionItem
    {
        public string Game { get; set; }
    }

    public class AESKey
    {
        public string Key { get; set; }
    }


}
