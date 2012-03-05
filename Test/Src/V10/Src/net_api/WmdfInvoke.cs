using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace feed_net_api
{
    public class WmdfInvoke
    {
        private const string WMDF_API = "feedapi.dll";
        [DllImport(WMDF_API)]
        public static extern eReturnValue wmdf_initialize();
        [DllImport(WMDF_API)]
        public static extern eReturnValue wmdf_finalize();
        [DllImport(WMDF_API)]
        public static extern eReturnValue wmdf_open_connection(ref OpenArgs args);
        [DllImport(WMDF_API)]
        public static extern eReturnValue wmdf_open_connection_ex(ref OpenExArgs args);
        [DllImport(WMDF_API)]
        public static extern eReturnValue wmdf_close_connection(ref CloseArgs args);
        [DllImport(WMDF_API)]
        public static extern eReturnValue wmdf_set_filter_mask(ref FilterArgs args);
        [DllImport(WMDF_API)]
        public static extern eReturnValue wmdf_get_api_version(ref APIVersionArgs args);
        [DllImport(WMDF_API)]
        public static extern eReturnValue wmdf_set_callback(ref CallbackArgs args);
        [DllImport(WMDF_API)]
        public static extern eReturnValue wmdf_get_server_info(ref ServerInfoArgs args);
        [DllImport(WMDF_API)]
        public static extern eReturnValue wmdf_get_record(ref RecordArgs args);
        [DllImport(WMDF_API)]
        public static extern eReturnValue wmdf_release_record(ref RecordArgs args);

    }
}
