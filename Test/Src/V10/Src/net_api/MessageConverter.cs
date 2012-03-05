using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace feed_net_api
{
    public class MessageConverter
    {
        public static TimeSpan ConvertTime(int time)
        {
            return new TimeSpan(time == Int32.MinValue ? Int32.MinValue : time * 10000L);
        }

        public static MdfMessage ConvertMessage(ref MdfRecord record)
        {
            if (record.record_count <= 0) return null;
            if ((record.product_id == WmdfConstant.PRODEUCT_SYSTEM_ID) 
                || (record.product_id == WmdfConstant.PRODEUCT_MONITOR_ID))
            {
                return null;
            }
            MdfMessage mdf_msg = new MdfMessage();
            mdf_msg.local_time = record.local_time;
            mdf_msg.packet_num = record.packet_num;
            mdf_msg.product_id = record.product_id;
            mdf_msg.message_id = record.message_id;
            mdf_msg.major_version = record.major_version;
            mdf_msg.minor_version = record.minor_version;
            mdf_msg.record_count = record.record_count;
            if (record.product_id == WmdfConstant.PRODUCT_SSE_LEVEL1_ID)
            {
                if (record.message_id == WmdfConstant.SSEL1_MARKETDATA_ID)
                {
                    mdf_msg.records = ProcessSHSZL1MarketData(record);
                }
            }
            //深交所level1
            else if (record.product_id == WmdfConstant.PRODUCT_SZSE_LEVEL1_ID)
            {
                if (record.message_id == WmdfConstant.SZSEL1_MARKETDATA_ID)
                {
                    mdf_msg.records = ProcessSHSZL1MarketData(record);
                }
            }
            //上交所level2
            else if (record.product_id == WmdfConstant.PRODUCT_SSE_LEVEL2_ID)
            {
                if (record.message_id == WmdfConstant.SSEL2_MARKETDATA_ID)
                {
                    mdf_msg.records = ProcessL2MarketData(record);
                }
                else if (record.message_id == WmdfConstant.SSEL2_TRANSACTION_ID)
                {
                    mdf_msg.records = ProcessL2Transaction(record);
                }
                else if (record.message_id == WmdfConstant.SSEL2_INDEXDATA_ID)
                {
                    mdf_msg.records = ProcessIndexSnapshot(record);
                }
                else if (record.message_id == WmdfConstant.SSEL2_ORDERQUEUE_ID)
                {
                    mdf_msg.records = ProcessL2OrderQueues(record);
                }
            }
            //深交所L2
            else if (record.product_id == WmdfConstant.PRODUCT_SZSE_LEVEL2_ID)
            {
                if (record.message_id == WmdfConstant.SZSEL2_MARKETDATA_ID)
                {
                    mdf_msg.records = ProcessL2MarketData(record);
                }
                else if (record.message_id == WmdfConstant.SZSEL2_TRANSACTION_ID)
                {
                    mdf_msg.records = ProcessL2Transaction(record);
                }
                else if (record.message_id == WmdfConstant.SZSEL2_INDEXDATA_ID)
                {
                    mdf_msg.records = ProcessIndexSnapshot(record);
                }
                else if (record.message_id == WmdfConstant.SZSEL2_ORDERQUEUE_ID)
                {
                    mdf_msg.records = ProcessL2OrderQueues(record);
                }
                else if (record.message_id == WmdfConstant.SZSEL2_SINGLEORDER_ID)
                {
                    mdf_msg.records = ProcessSZSEL2Order(record);
                }
            }
            //新华富时L1
            else if (record.product_id == WmdfConstant.PRODUCT_FTSE_LEVEL1_ID)
            {
                mdf_msg.records = ProcessIndexSnapshot(record);
            }
            //中金所L1
            else if (record.product_id == WmdfConstant.PRODUCT_CFFE_LEVEL1_ID)
            {
                mdf_msg.records = ProcessFutureL1MarketData(record);
            }
            //中金所L2
            else if (record.product_id == WmdfConstant.PRODUCT_CFFE_LEVEL2_ID)
            {
                mdf_msg.records = ProcessCFFEL2MarketData(record);
            }
            //大商所L1
            else if (record.product_id == WmdfConstant.PRODUCT_DCE_LEVEL1_ID)
            {
                mdf_msg.records = ProcessFutureL1MarketData(record);
            }
            //上期所L1
            else if (record.product_id == WmdfConstant.PRODUCT_SHFE_LEVEL1_ID)
            {
                mdf_msg.records = ProcessFutureL1MarketData(record);
            }
            //郑商所L1
            else if (record.product_id == WmdfConstant.PRODUCT_CZCE_LEVEL1_ID)
            {
                mdf_msg.records = ProcessFutureL1MarketData(record);
            }

            return mdf_msg;            
        }

        private static L1MarketSnapshot[] ProcessSHSZL1MarketData(MdfRecord record)
        {
            sL1MarketSnapshot[] datas = new sL1MarketSnapshot[record.record_count];
            L1MarketSnapshot[] dataitems = new L1MarketSnapshot[record.record_count];
            int addr = record.body.ToInt32();
            int size = Marshal.SizeOf(typeof(sL1MarketSnapshot));
            for (int index = 0; index < record.record_count; index++)
            {
                datas[index] = (sL1MarketSnapshot)Marshal.PtrToStructure(new IntPtr(addr + index * size), typeof(sL1MarketSnapshot));
                dataitems[index] = new L1MarketSnapshot();
            }
            
            for (int index = 0; index < record.record_count; index++)
            {
                dataitems[index].wind_code = Encoding.ASCII.GetString(datas[index].wind_code).TrimEnd('\0');
                dataitems[index].data_time_stamp = MessageConverter.ConvertTime(datas[index].data_time_stamp);
                dataitems[index].pre_close = datas[index].pre_close;
                dataitems[index].open_price = datas[index].open_price;
                dataitems[index].high_price = datas[index].high_price;
                dataitems[index].low_price = datas[index].low_price;
                dataitems[index].new_price = datas[index].new_price;
                dataitems[index].total_volume = datas[index].total_volume;
                dataitems[index].total_amount = datas[index].total_amount;
                dataitems[index].bid_price1 = datas[index].bid_price1;
                dataitems[index].bid_price2 = datas[index].bid_price2;
                dataitems[index].bid_price3 = datas[index].bid_price3;
                dataitems[index].bid_price4 = datas[index].bid_price4;
                dataitems[index].bid_price5 = datas[index].bid_price5;
                dataitems[index].ask_price1 = datas[index].ask_price1;
                dataitems[index].ask_price2 = datas[index].ask_price2;
                dataitems[index].ask_price3 = datas[index].ask_price3;
                dataitems[index].ask_price4 = datas[index].ask_price4;
                dataitems[index].ask_price5 = datas[index].ask_price5;
                dataitems[index].ask_size1 = datas[index].ask_size1;
                dataitems[index].ask_size2 = datas[index].ask_size2;
                dataitems[index].ask_size3 = datas[index].ask_size3;
                dataitems[index].ask_size4 = datas[index].ask_size4;
                dataitems[index].ask_size5 = datas[index].ask_size5;
                dataitems[index].bid_size1 = datas[index].bid_size1;
                dataitems[index].bid_size2 = datas[index].bid_size2;
                dataitems[index].bid_size3 = datas[index].bid_size3;
                dataitems[index].bid_size4 = datas[index].bid_size4;
                dataitems[index].bid_size5 = datas[index].bid_size5;
            }
            return dataitems;
        }
        private static L2MarketSnapshot[] ProcessL2MarketData(MdfRecord record)
        {
            sL2MarketSnapshot[] datas = new sL2MarketSnapshot[record.record_count];
            L2MarketSnapshot[] dataitems = new L2MarketSnapshot[record.record_count];
            int addr = record.body.ToInt32();
            int size = Marshal.SizeOf(typeof(sL2MarketSnapshot));
            for (int index = 0; index != record.record_count; ++index)
            {
                datas[index] = (sL2MarketSnapshot)Marshal.PtrToStructure(new IntPtr(addr + index * size), typeof(sL2MarketSnapshot));
                dataitems[index] = new L2MarketSnapshot();
            }
            
            for (int index = 0; index < record.record_count; ++index)
            {
                byte[] bytes = datas[index].wind_code;
                dataitems[index].wind_code = Encoding.ASCII.GetString(bytes).TrimEnd('\0');
                dataitems[index].data_time_stamp = MessageConverter.ConvertTime(datas[index].data_time_stamp);
                dataitems[index].prev_close = datas[index].prev_close;
                dataitems[index].open_price = datas[index].open_price;
                dataitems[index].high_price = datas[index].high_price;
                dataitems[index].low_price = datas[index].low_price;
                dataitems[index].new_price = datas[index].new_price;
                dataitems[index].total_volume = datas[index].total_volume;
                dataitems[index].total_amount = datas[index].total_amount;
                dataitems[index].bid_price1 = datas[index].bid_price1;
                dataitems[index].bid_price2 = datas[index].bid_price2;
                dataitems[index].bid_price3 = datas[index].bid_price3;
                dataitems[index].bid_price4 = datas[index].bid_price4;
                dataitems[index].bid_price5 = datas[index].bid_price5;
                dataitems[index].bid_price6 = datas[index].bid_price6;
                dataitems[index].bid_price7 = datas[index].bid_price7;
                dataitems[index].bid_price8 = datas[index].bid_price8;
                dataitems[index].bid_price9 = datas[index].bid_price9;
                dataitems[index].bid_price10 = datas[index].bid_price10;
                dataitems[index].ask_price1 = datas[index].ask_price1;
                dataitems[index].ask_price2 = datas[index].ask_price2;
                dataitems[index].ask_price3 = datas[index].ask_price3;
                dataitems[index].ask_price4 = datas[index].ask_price4;
                dataitems[index].ask_price5 = datas[index].ask_price5;
                dataitems[index].ask_price6 = datas[index].ask_price6;
                dataitems[index].ask_price7 = datas[index].ask_price7;
                dataitems[index].ask_price8 = datas[index].ask_price8;
                dataitems[index].ask_price9 = datas[index].ask_price9;
                dataitems[index].ask_price10 = datas[index].ask_price10;
                dataitems[index].ask_size1 = datas[index].ask_size1;
                dataitems[index].ask_size2 = datas[index].ask_size2;
                dataitems[index].ask_size3 = datas[index].ask_size3;
                dataitems[index].ask_size4 = datas[index].ask_size4;
                dataitems[index].ask_size5 = datas[index].ask_size5;
                dataitems[index].ask_size6 = datas[index].ask_size6;
                dataitems[index].ask_size7 = datas[index].ask_size7;
                dataitems[index].ask_size8 = datas[index].ask_size8;
                dataitems[index].ask_size9 = datas[index].ask_size9;
                dataitems[index].ask_size10 = datas[index].ask_size10;
                dataitems[index].bid_size1 = datas[index].bid_size1;
                dataitems[index].bid_size2 = datas[index].bid_size2;
                dataitems[index].bid_size3 = datas[index].bid_size3;
                dataitems[index].bid_size4 = datas[index].bid_size4;
                dataitems[index].bid_size5 = datas[index].bid_size5;
                dataitems[index].bid_size6 = datas[index].bid_size6;
                dataitems[index].bid_size7 = datas[index].bid_size7;
                dataitems[index].bid_size8 = datas[index].bid_size8;
                dataitems[index].bid_size9 = datas[index].bid_size9;
                dataitems[index].bid_size10 = datas[index].bid_size10;
                dataitems[index].transactions_count = datas[index].transactions_count;
                dataitems[index].weighted_average_bid_price = datas[index].weighted_average_bid_price;
                dataitems[index].weighted_average_ask_price = datas[index].weighted_average_ask_price;
                dataitems[index].l2_total_bid_size = datas[index].l2_total_bid_size;
                dataitems[index].l2_total_ask_size = datas[index].l2_total_ask_size;
                dataitems[index].peratio1 = datas[index].peratio1;
                dataitems[index].peratio2 = datas[index].peratio2;
                dataitems[index].trade_status = datas[index].trade_status;
            }
            return dataitems;
        }
        private static L2Transaction[] ProcessL2Transaction(MdfRecord record)
        {
            L2Transaction[] dataitems = new L2Transaction[record.record_count];
            sL2Transaction[] datas = new sL2Transaction[record.record_count];
            int addr = record.body.ToInt32();
            int size = Marshal.SizeOf(typeof(sL2Transaction));
            for (int index = 0; index != record.record_count; ++index)
            {
                datas[index] = (sL2Transaction)Marshal.PtrToStructure(new IntPtr(addr + index * size), typeof(sL2Transaction));
                dataitems[index] = new L2Transaction();
            }
            
            for (int index = 0; index != record.record_count; ++index)
            {
                dataitems[index].wind_code = Encoding.ASCII.GetString(datas[index].wind_code).TrimEnd('\0');
                dataitems[index].trade_time = MessageConverter.ConvertTime(datas[index].trade_time);
                dataitems[index].volume = datas[index].volume;
                dataitems[index].price = datas[index].price;
                dataitems[index].amount = datas[index].amount;
                dataitems[index].set_no = datas[index].set_no;
                dataitems[index].rec_no = datas[index].rec_no;
                dataitems[index].buyOrderRecNo = datas[index].buyOrderRecNo;
                dataitems[index].sellOrderRecNo = datas[index].sellOrderRecNo;
                dataitems[index].order_kind = datas[index].order_kind;
                dataitems[index].function_code = datas[index].function_code;
            }
            return dataitems;
        }
        private static IndexSnapshot[] ProcessIndexSnapshot(MdfRecord record)
        {
            sIndexSnapshot[] datas = new sIndexSnapshot[record.record_count];
            IndexSnapshot[] dataitems = new IndexSnapshot[record.record_count];
            int addr = record.body.ToInt32();
            int size = Marshal.SizeOf(typeof(sIndexSnapshot));
            for (int index = 0; index != record.record_count; ++index)
            {
                datas[index] = (sIndexSnapshot)Marshal.PtrToStructure(new IntPtr(addr + index * size), typeof(sIndexSnapshot));
                dataitems[index] = new IndexSnapshot();
            }
            
            for (int index = 0; index != record.record_count; ++index)
            {
                dataitems[index].wind_code = Encoding.ASCII.GetString(datas[index].wind_code).TrimEnd('\0');
                dataitems[index].data_time_stamp = MessageConverter.ConvertTime(datas[index].data_time_stamp);
                dataitems[index].pre_close = datas[index].pre_close;
                dataitems[index].open_price = datas[index].open_price;
                dataitems[index].high_price = datas[index].high_price;
                dataitems[index].low_price = datas[index].low_price;
                dataitems[index].new_price = datas[index].new_price;
                dataitems[index].total_volume = datas[index].total_volume;
                dataitems[index].total_amount = datas[index].total_amount;
            }
            return dataitems;
        }
        private static FuturesMarketSnapshot[] ProcessFutureL1MarketData(MdfRecord record)
        {
            sFuturesMarketSnapshot[] datas = new sFuturesMarketSnapshot[record.record_count];
            FuturesMarketSnapshot[] dataitems = new FuturesMarketSnapshot[record.record_count];
            int addr = record.body.ToInt32();
            int size = Marshal.SizeOf(typeof(sFuturesMarketSnapshot));
            for (int index = 0; index != record.record_count; ++index)
            {
                datas[index] = (sFuturesMarketSnapshot)Marshal.PtrToStructure(new IntPtr(addr + index * size), typeof(sFuturesMarketSnapshot));
                dataitems[index] = new FuturesMarketSnapshot();
            }
            
            for (int index = 0; index != record.record_count; ++index)
            {
                dataitems[index].wind_code = Encoding.ASCII.GetString(datas[index].wind_code).TrimEnd('\0');
                dataitems[index].trade_date = datas[index].trade_date;
                dataitems[index].data_time_stamp = MessageConverter.ConvertTime(datas[index].data_time_stamp);
                dataitems[index].pre_close = datas[index].pre_close;
                dataitems[index].open_price = datas[index].open_price;
                dataitems[index].high_price = datas[index].high_price;
                dataitems[index].low_price = datas[index].low_price;
                dataitems[index].new_price = datas[index].new_price;
                dataitems[index].total_volume = datas[index].total_volume;
                dataitems[index].total_amount = datas[index].total_amount;
                dataitems[index].bid_price1 = datas[index].bid_price1;
                dataitems[index].ask_price1 = datas[index].ask_price1;
                dataitems[index].ask_size1 = datas[index].ask_size1;
                dataitems[index].bid_size1 = datas[index].bid_size1;
                dataitems[index].pre_settle = datas[index].pre_settle;
                dataitems[index].pre_position = datas[index].pre_position;
                dataitems[index].settle = datas[index].settle;
                dataitems[index].position = datas[index].position ;
                dataitems[index].average_price = datas[index].average_price;
                dataitems[index].high_limit = datas[index].high_limit;
                dataitems[index].low_limit = datas[index].low_limit;
            }
            return dataitems;
        }
        private static CFEL2MarketSnapshot[] ProcessCFFEL2MarketData(MdfRecord record)
        {
            sCFEL2MarketSnapshot[] datas = new sCFEL2MarketSnapshot[record.record_count];
            CFEL2MarketSnapshot[] dataitems = new CFEL2MarketSnapshot[record.record_count];
            int addr = record.body.ToInt32();
            int size = Marshal.SizeOf(typeof(sCFEL2MarketSnapshot));
            for (int index = 0; index != record.record_count; ++index)
            {
                datas[index] = (sCFEL2MarketSnapshot)Marshal.PtrToStructure(new IntPtr(addr + index * size), typeof(sCFEL2MarketSnapshot));
                dataitems[index] = new CFEL2MarketSnapshot();
            }
            
            for (int index = 0; index != record.record_count; ++index)
            {
                dataitems[index].wind_code = Encoding.ASCII.GetString(datas[index].wind_code).TrimEnd('\0');
                dataitems[index].trade_date = new Date(datas[index].trade_date);
                dataitems[index].data_time_stamp = MessageConverter.ConvertTime(datas[index].data_time_stamp);
                dataitems[index].prev_close = datas[index].prev_close;
                dataitems[index].open_price = datas[index].open_price;
                dataitems[index].high_price = datas[index].high_price;
                dataitems[index].low_price = datas[index].low_price;
                dataitems[index].new_price = datas[index].new_price;
                dataitems[index].total_volume = datas[index].total_volume;
                dataitems[index].total_amount = datas[index].total_amount;
                dataitems[index].bid_price1 = datas[index].bid_price1;
                dataitems[index].bid_price2 = datas[index].bid_price2;
                dataitems[index].bid_price3 = datas[index].bid_price3;
                dataitems[index].bid_price4 = datas[index].bid_price4;
                dataitems[index].bid_price5 = datas[index].bid_price5;
                dataitems[index].ask_price1 = datas[index].ask_price1;
                dataitems[index].ask_price2 = datas[index].ask_price2;
                dataitems[index].ask_price3 = datas[index].ask_price3;
                dataitems[index].ask_price4 = datas[index].ask_price4;
                dataitems[index].ask_price5 = datas[index].ask_price5;
                dataitems[index].ask_size1 = datas[index].ask_size1;
                dataitems[index].ask_size2 = datas[index].ask_size2;
                dataitems[index].ask_size3 = datas[index].ask_size3;
                dataitems[index].ask_size4 = datas[index].ask_size4;
                dataitems[index].ask_size5 = datas[index].ask_size5;
                dataitems[index].bid_size1 = datas[index].bid_size1;
                dataitems[index].bid_size2 = datas[index].bid_size2;
                dataitems[index].bid_size3 = datas[index].bid_size3;
                dataitems[index].bid_size4 = datas[index].bid_size4;
                dataitems[index].bid_size5 = datas[index].bid_size5;
                dataitems[index].pre_settle = datas[index].pre_settle;
                dataitems[index].pre_position = datas[index].pre_position;
                dataitems[index].position = datas[index].position;
                dataitems[index].high_limit = datas[index].high_limit;
                dataitems[index].low_limit = datas[index].low_limit;
            }
            return dataitems;
        }
        private static L2OrderQueue[] ProcessL2OrderQueues(MdfRecord record)
        {
            sL2OrderQueue[] datas = new sL2OrderQueue[record.record_count];
            L2OrderQueue[] dataitems = new L2OrderQueue[record.record_count];
            int addr = record.body.ToInt32();
            int size = Marshal.SizeOf(typeof(sL2OrderQueue));
            for (int index = 0; index != record.record_count; ++index)
            {
                datas[index] = (sL2OrderQueue)Marshal.PtrToStructure(new IntPtr(addr + index * size), typeof(sL2OrderQueue));
                dataitems[index] = new L2OrderQueue();
            }
            for (int index = 0; index < record.record_count; ++index)
            {
                dataitems[index].wind_code = Encoding.ASCII.GetString(datas[index].wind_code).TrimEnd('\0');
                dataitems[index].date_time_stamp = MessageConverter.ConvertTime(datas[index].date_time_stamp);
                dataitems[index].order_side = datas[index].order_side;
                dataitems[index].image_status = datas[index].image_status;
                dataitems[index].price_level_count = datas[index].price_level_count;

                sL2OrderQueuePriceLevel[] price_levels = new sL2OrderQueuePriceLevel[datas[index].price_level_count];
                dataitems[index].price_levels = new L2OrderQueuePriceLevel[datas[index].price_level_count];
                int addr_of_pricelevel = datas[index].price_levels.ToInt32();
                int size_of_pricelevel = Marshal.SizeOf(typeof(sL2OrderQueuePriceLevel));
                for (int j = 0; j != datas[index].price_level_count; ++j)
                {
                    price_levels[j] = (sL2OrderQueuePriceLevel)Marshal.PtrToStructure(new IntPtr(addr_of_pricelevel + j * size_of_pricelevel), typeof(sL2OrderQueuePriceLevel));
                    dataitems[index].price_levels[j] = new L2OrderQueuePriceLevel(); 
                }
                
                for(int j = 0; j < datas[index].price_level_count;++j)
                {
                    dataitems[index].price_levels[j].operator_type = price_levels[j].operator_type;
                    dataitems[index].price_levels[j].order_price = price_levels[j].order_price;
                    dataitems[index].price_levels[j].total_number = price_levels[j].total_number;
                    dataitems[index].price_levels[j].orders_number = price_levels[j].orders_number;

                    sL2OrderQueueItem[] orderqueueitems = new sL2OrderQueueItem[price_levels[j].orders_number];
                    dataitems[index].price_levels[j].order_items = new L2OrderQueueItem[price_levels[j].orders_number];
                    int addr_of_item = price_levels[j].order_items.ToInt32();
                    int size_of_item = Marshal.SizeOf(typeof(sL2OrderQueueItem));
                    for(int k = 0;k!=price_levels[j].orders_number;++k)
                    {
                        orderqueueitems[k] = (sL2OrderQueueItem)Marshal.PtrToStructure(new IntPtr(addr_of_item + k * size_of_item), typeof(sL2OrderQueueItem));
                        dataitems[index].price_levels[j].order_items[k] = new L2OrderQueueItem();
                    }
                    for(int k = 0;k!=price_levels[j].orders_number;++k)
                    {

                        dataitems[index].price_levels[j].order_items[k].operator_type = orderqueueitems[k].operator_type;
                        dataitems[index].price_levels[j].order_items[k].entry_id = orderqueueitems[k].entry_id;
                        dataitems[index].price_levels[j].order_items[k].size = orderqueueitems[k].size;
                    }
                }

            }
            return dataitems;
        }
        private static SZSEL2Order[] ProcessSZSEL2Order(MdfRecord record)
        {
            sSZSEL2Order[] datas = new sSZSEL2Order[record.record_count];
            SZSEL2Order[] dataitems = new SZSEL2Order[record.record_count];
            int addr = record.body.ToInt32();
            int size = Marshal.SizeOf(typeof(sSZSEL2Order));
            for (int index = 0; index != record.record_count; ++index)
            {
                datas[index] = (sSZSEL2Order)Marshal.PtrToStructure(new IntPtr(addr + index * size), typeof(sSZSEL2Order));
                dataitems[index] = new SZSEL2Order();
            }
            
            for (int index = 0; index != record.record_count; ++index)
            {
                dataitems[index].wind_code = Encoding.ASCII.GetString(datas[index].wind_code).TrimEnd('\0');
                dataitems[index].order_time = MessageConverter.ConvertTime(datas[index].order_time);
                dataitems[index].set_no = datas[index].set_no;
                dataitems[index].rec_no = datas[index].rec_no;
                dataitems[index].size = datas[index].size;
                dataitems[index].price = datas[index].price;
                dataitems[index].order_kind = datas[index].order_kind;
                dataitems[index].function_code = datas[index].function_code;
            }
            return dataitems;
        }
    }
}
