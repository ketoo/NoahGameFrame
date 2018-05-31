﻿using UnityEngine;
using System.Collections;
using NFSDK;
using UnityStandardAssets.Cameras;
using UnityStandardAssets.CrossPlatformInput;
using UnityStandardAssets.Characters.ThirdPerson;

public class MainPlayer : MonoBehaviour {

	// Use this for initialization
    private float mSyncTime = 1.0f;
    private float mSyncTimeTick = 0.0f;
    private FreeLookCam mCamera;
    private Vector3 m_LastMove;

    private ThirdPersonCharacter m_Character; // A reference to the ThirdPersonCharacter on the object
    private Transform m_Cam;                  // A reference to the main camera in the scenes transform
    private Vector3 m_CamForward;             // The current forward direction of the camera
    private Vector3 m_Move;
    private bool m_Jump;                      // the world-relative desired move direction, calculated from the camForward and user input.

	void Start () 
    {
        mCamera = GameObject.Find("FreeLookCameraRig").GetComponent<FreeLookCam>();
        mCamera.SetTarget(transform);

        // get the transform of the main camera
        if (Camera.main != null)
        {
            m_Cam = Camera.main.transform;
        }
        else
        {
            Debug.LogWarning(
                "Warning: no main camera found. Third person character needs a Camera tagged \"MainCamera\", for camera-relative controls.");
            // we use self-relative controls in this case, which probably isn't what the user wants, but hey, we warned them!
        }

        // get the third person character ( this should never be null due to require component )
        m_Character = GetComponent<ThirdPersonCharacter>();
	}

    private void Update()
    {
        if (!m_Jump)
        {
            m_Jump = CrossPlatformInputManager.GetButtonDown("Jump");
        }
    }
	// Update is called once per frame
    void FixedUpdate()
    {
        // read inputs
        float h = CrossPlatformInputManager.GetAxis("Horizontal");
        float v = CrossPlatformInputManager.GetAxis("Vertical");
        bool crouch = Input.GetKey(KeyCode.C);

        // calculate move direction to pass to character
        if (m_Cam != null)
        {
            // calculate camera relative direction to move:
            m_CamForward = Vector3.Scale(m_Cam.forward, new Vector3(1, 0, 1)).normalized;
            m_Move = v * m_CamForward + h * m_Cam.right;
        }
        else
        {
            // we use world-relative directions in the case of no main camera
            m_Move = v * Vector3.forward + h * Vector3.right;
        }
#if !MOBILE_INPUT
        // walk speed multiplier
        if (Input.GetKey(KeyCode.LeftShift)) m_Move *= 0.5f;
#endif

        // pass all parameters to the character control script
        m_Character.Move(m_Move, crouch, m_Jump);

        m_Jump = false;

        // 同步坐标给服务器
        bool bMove = false;
        if((h != 0.0f || v != 0.0f))
            bMove = true;

        if (mSyncTimeTick > mSyncTime || (mSyncTimeTick > 0 && !bMove) || m_LastMove != m_Move)
        {
            NFCPlayerLogic.Instance().RequireMove(transform.position);
            mSyncTimeTick = 0.0f;
        }

        if (bMove)
        {
            mSyncTimeTick += Time.deltaTime;
        }

        m_LastMove = m_Move;
    }
}
