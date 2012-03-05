using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace feed_net_api
{
    //static class Program
    //{
    //    static void Main(string[] args)
    //    {
    //        WmdfAdapter.Initialize();


    //        OpenArgs open_args = new OpenArgs();
    //        open_args.connection_id = 0;
    //        open_args.user_name = "admin";
    //        open_args.password = "admin";
    //        open_args.server_addr = "127.0.0.1";//"10.100.6.124";
    //        open_args.server_port = 6950;
    //        open_args.marktet_open_time = 080000;
    //        open_args.markt_close_time = 180000;
    //        open_args.codec_type = CodecType.CT_FAST;
    //        WmdfAdapter adapter = new WmdfAdapter(open_args);

    //        //FilterArgs filter_args = new FilterArgs();
    //        //filter_args.connection_id = 0;
    //        //filter_args.filter_masks = "600000.SH,600036.SH";
    //        //adapter.SetFilterMask(ref filter_args);

    //        adapter.SetCallBack(new OnReceveMessage(OnReceveMessage));
    //        adapter.Open();
    //        while (true)
    //        {
    //            string command = Console.ReadLine();
    //            if (command.ToUpper() == "EXIT")
    //            {
    //                adapter.Close();
    //            }
    //        }

    //    }
    //    static void OnReceveMessage(MdfMessage mdf_msg)
    //    {
    //        //示例处理上交所L2的数据
    //        if (mdf_msg.product_id == WmdfConstant.PRODUCT_SSE_LEVEL2_ID)
    //        {
    //            if (mdf_msg.message_id == WmdfConstant.SSEL2_MARKETDATA_ID)
    //            {
    //                for (int index = 0; index < mdf_msg.record_count; index++)
    //                {
    //                    string wind_code = ((L2MarketSnapshot)mdf_msg.records[index]).wind_code;
    //                    if ((wind_code != "600000.SH") && (wind_code != "600036.SH"))
    //                    {
    //                        Console.WriteLine(wind_code);
    //                    }
    //                }

    //            }
    //            else if (mdf_msg.message_id == WmdfConstant.SSEL2_TRANSACTION_ID)
    //            {
    //                for (int index = 0; index < mdf_msg.record_count; index++)
    //                {
    //                    Console.WriteLine(((L2Transaction)mdf_msg.records[index]).wind_code);
    //                }
    //            }
    //            else if (mdf_msg.message_id == WmdfConstant.SSEL2_INDEXDATA_ID)
    //            {
    //                for (int index = 0; index < mdf_msg.record_count; index++)
    //                {
    //                    Console.WriteLine(((IndexSnapshot)mdf_msg.records[index]).wind_code);
    //                }
    //            }
    //            else if (mdf_msg.message_id == WmdfConstant.SSEL2_ORDERQUEUE_ID)
    //            {
    //                for (int index = 0; index < mdf_msg.record_count; index++)
    //                {
    //                    Console.WriteLine(((L2OrderQueue)mdf_msg.records[index]).wind_code);
    //                }
    //            }
    //        }
    //    }
    //}
}
