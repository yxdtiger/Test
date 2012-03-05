using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Runtime.InteropServices;

namespace feed_net_api
{
    public delegate void OnReceveMessage(MdfMessage mdf_message);

    public class WmdfAdapter
    {
        public WmdfAdapter()
        {
            _call_back = new RecordCallback(OnRecordCallback);
        }
       
        public static eReturnValue Initialize()
        {
            return WmdfInvoke.wmdf_initialize();
        }

        public static eReturnValue Finalize()
        {
            return WmdfInvoke.wmdf_finalize();
        }

        public bool Open(OpenArgs open_args)
        {
            _Connection_Id = open_args.connection_id;
            return WmdfInvoke.wmdf_open_connection(ref open_args) == eReturnValue.SUCCESS;
        }

        public bool Open(OpenExArgs openex_args)
        {
            _Connection_Id = openex_args.connection_id;
            return WmdfInvoke.wmdf_open_connection_ex(ref openex_args) == eReturnValue.SUCCESS;
        }

        public bool Close()
        {
            CloseArgs args = new CloseArgs();
            args.connection_id = _Connection_Id;
            
            return WmdfInvoke.wmdf_close_connection(ref args) == eReturnValue.SUCCESS;
        }

        public eReturnValue SetFilterMask(ref FilterArgs args)
        {
            return WmdfInvoke.wmdf_set_filter_mask(ref args);
        }

        public eReturnValue GetAPIVersion(ref APIVersionArgs args)
        {
            return WmdfInvoke.wmdf_get_api_version(ref args);
        }

        public eReturnValue GetServerInfo(ref ServerInfoArgs args)
        {
            return WmdfInvoke.wmdf_get_server_info(ref args);
        }

        public eReturnValue SetCallBack(OnReceveMessage receive_message_event)
        {
            _ReceiveMessageEvent += receive_message_event;
            CallbackArgs args = new CallbackArgs();
            args.connection_id = _Connection_Id;
            args.record_callback = _call_back;
            args.event_callback = null;
            return WmdfInvoke.wmdf_set_callback(ref args);
        }
        public void ReceiveRecord()
        {
            //while (true)
            //{
            //    RecordArgs args = new RecordArgs();
            //    args.connection_id = _Open_Args.connection_id;
            //    IntPtr args_ptr = Marshal.AllocHGlobal(Marshal.SizeOf(args));
            //    Marshal.StructureToPtr(args, args_ptr, false);
            //    WmdfInvoke.wmdf_get_record(ref args);
            //    //args = (RecordArgs)Marshal.PtrToStructure(args_ptr, typeof(RecordArgs));
            //    if (args.record != null)
            //    {
            //        if (args.record != null)
            //        {
            //            MdfRecord record;
            //            IntPtr record_ptr = new IntPtr(args.record.ToInt32());
            //            record = (MdfRecord)Marshal.PtrToStructure(record_ptr, typeof(MdfRecord));
            //            //to do convert to message;
            //            MdfMessage msg = MessageConverter.ConvertMessage(ref record);
            //            if ((_ReceiveMessageEvent != null) && (msg != null))
            //            {
            //                _ReceiveMessageEvent(msg);
            //            }
            //        }
            //        WmdfInvoke.wmdf_release_record(ref args);
            //    }
            //    Marshal.FreeHGlobal(args_ptr);
            //    Thread.Sleep(1);
            //}
        }

        private void OnRecordCallback(ref RecordArgs args)
        {
            if (args.record != null)
            {
                MdfRecord record = new MdfRecord();
                record = (MdfRecord)Marshal.PtrToStructure(args.record, typeof(MdfRecord));
                //to do convert to message;
                MdfMessage msg = MessageConverter.ConvertMessage(ref record);
                if ((_ReceiveMessageEvent != null) && (msg != null))
                {
                    _ReceiveMessageEvent(msg);
                }
            }
            WmdfInvoke.wmdf_release_record(ref args);
        }
        private byte _Connection_Id;
        private event OnReceveMessage _ReceiveMessageEvent;
        private RecordCallback _call_back;
        
    }
}
