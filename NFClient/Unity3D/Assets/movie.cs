using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class movie : MonoBehaviour {

    public class player
    {
        public int[] wh = null;
    }
    //public player player1;
    AndroidJavaObject videoPlaying;
    int[] wh = null;

    [SerializeField] private UnityEngine.UI.RawImage liveRawImage;
    // Use this for initialization
    void Start()
    {
        if (Application.platform != RuntimePlatform.Android)
            return;
            //    player1 = new player;
            liveRawImage = GetComponent<RawImage>();
        Debug.Log("Start");

        //videoPlaying = new AndroidJavaObject("com.fuho.fromfleetcloud.video.FleetLive", "WW00001ASED5", 1, "fleet.vacron.com", "admin", "admin", "L"); videoPlaying = new AndroidJavaObject("com.fuho.fromfleetcloud.video.FleetLive", "WW00001ASED5", 1, "fleet.vacron.com", "admin", "admin", "L");
        videoPlaying = new AndroidJavaObject("com.fuho.fromfleetcloud.video.FleetLive", "VG500122", 1, "54.199.103.26", "admin", "admin", "L");
        //videoPlaying = new AndroidJavaObject("com.fuho.fromfleetcloud.video.FleetLive", "TT001802", 1, "125.227.91.156", "guest", "123456", "L");


        IntPtr methodID = AndroidJNI.GetMethodID(videoPlaying.GetRawClass(), "start", "()[I");
        AndroidJavaObject nullObject = null;
        UnityEngine.jvalue[] args = AndroidJNIHelper.CreateJNIArgArray(new[] { nullObject });
        IntPtr intArray = AndroidJNI.CallObjectMethod(videoPlaying.GetRawObject(), methodID, args);

        if (intArray != IntPtr.Zero)
            wh = AndroidJNI.FromIntArray(intArray);

        Debug.LogFormat("============================= width : {0}, height : {0} =============================", wh[0], wh[1]);
    }

    // Update is called once per frame
    void Update()
    {
        if (Application.platform != RuntimePlatform.Android)
            return;

        Debug.Log("Update");

        try
        {

            IntPtr methodID2 = AndroidJNI.GetMethodID(videoPlaying.GetRawClass(), "getVideoImage", "()[I");
            AndroidJavaObject nullObject2 = null;


            UnityEngine.jvalue[] args2 = AndroidJNIHelper.CreateJNIArgArray(new[] { nullObject2 });
            IntPtr intArray2 = AndroidJNI.CallObjectMethod(videoPlaying.GetRawObject(), methodID2, args2);

            if (intArray2 == IntPtr.Zero)
                return;

            int[] videoData = AndroidJNI.FromIntArray(intArray2);

            int length = videoData.Length;
            if (length <= 0 || wh == null || wh[0] <= 0 || wh[1] <= 0)
                return;

            byte[] bytes = new byte[length * sizeof(int)];
            Buffer.BlockCopy(videoData, 0, bytes, 0, bytes.Length);

            Texture2D bmp = new Texture2D(wh[0], wh[1], TextureFormat.ARGB32, false);

            bmp.LoadRawTextureData(bytes);
            bmp.Apply();

            //			Sprite sprite = Sprite.Create(bmp, new Rect(0, 0, bmp.width, bmp.height), new Vector2 (0.5f, 0.5f), 100F, 0, SpriteMeshType.FullRect);

            //			Texture2D nativeTexture = Texture2D.CreateExternalTexture(wh[0], wh[1], TextureFormat.ARGB32, false, false, (IntPtr)intArray2);
            //			bmp.UpdateExternalTexture(nativeTexture.GetNativeTexturePtr());
            //			bmp.Apply();

            liveRawImage.texture = bmp;

            Resources.UnloadUnusedAssets();

            Debug.LogFormat("============================= Update : videoData int len : {0} =============================", length);
        }
        catch (Exception e)
        {
            Debug.LogFormat("============================= Update : Exception {0} ====================================", e.Message);
        }

    }

    void OnGUI()
    {
        if (Application.platform != RuntimePlatform.Android)
            return;

        GUI.Button(new Rect(200, 200, 350, 50), "2Loginss");
        Debug.Log("OnGUI");

    }

    void Destroy()
    {
        if (Application.platform != RuntimePlatform.Android)
            return;
        Debug.Log("Destroy");

        if (videoPlaying != null)
        {
            videoPlaying.Call("stop");
            videoPlaying = null;
        }
    }
}
