using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System;

namespace UnityAR
{
    public class pulgins
    {
        private const string LIBRARY_NAME = "My_DLL_13";
        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Imagerows();

        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool update_camera_frame(IntPtr colors32);

        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool get_camera_size(out int width, out int height);

        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool open_camera();

        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool close_camera();

        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool get_texture_filepath([MarshalAs(UnmanagedType.LPStr)] string filepath);


        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void cameraframe_detekeypoints();


        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool best_Match();

        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Homography();

    }
}
