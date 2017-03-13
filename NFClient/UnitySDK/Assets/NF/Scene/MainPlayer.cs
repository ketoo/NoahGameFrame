using UnityEngine;
using System.Collections;
using NFSDK;
using UnityStandardAssets.Cameras;

public class MainPlayer : MonoBehaviour {

	// Use this for initialization
    private float mSyncTime = 1.0f;
    private float mSyncTimeTick = 0.0f;
    private float m_speed = 5.0f;
    private FreeLookCam mCamera;

	void Start () 
    {
        mCamera = GameObject.Find("FreeLookCameraRig").GetComponent<FreeLookCam>();
        mCamera.SetTarget(transform);
	}
	
	// Update is called once per frame
    void FixedUpdate()
    {
        bool bMove = false;
        if (Input.GetKey(KeyCode.W)|Input.GetKey(KeyCode.UpArrow)) //前
        {
            this.transform.Translate(mCamera.transform.forward * m_speed * Time.deltaTime);
            bMove = true;
        }
        if (Input.GetKey(KeyCode.S) | Input.GetKey(KeyCode.DownArrow)) //后
        {
            this.transform.Translate(mCamera.transform.forward * -m_speed * Time.deltaTime);
            bMove = true;
        }
        if (Input.GetKey(KeyCode.A) | Input.GetKey(KeyCode.LeftArrow)) //左
        {
            this.transform.Translate(mCamera.transform.right * -m_speed * Time.deltaTime);
            bMove = true;
        }
        if (Input.GetKey(KeyCode.D) | Input.GetKey(KeyCode.RightArrow)) //右
        {
            this.transform.Translate(mCamera.transform.right * m_speed * Time.deltaTime);
            bMove = true;
        }

        if (bMove)
        {
            mSyncTimeTick += Time.deltaTime;
        }

        if (mSyncTimeTick > mSyncTime || (mSyncTimeTick > 0 && !bMove))
        {
            NFCPlayerLogic.Instance().RequireMove(transform.position);
            mSyncTimeTick = 0.0f;
        }

        //float horizontal = Input.GetAxis("Horizontal"); //A D 左右
        //float vertical = Input.GetAxis("Vertical"); //W S 上 下

        //mSyncTimeTick

        //transform.Translate(Vector3.forward * vertical * m_speed * Time.deltaTime);//W S 上 下
        //transform.Translate(Vector3.right * horizontal * m_speed * Time.deltaTime);//A D 左右
        //NFCPlayerLogic.Instance().RequireMove(transform.position);
    }
}
