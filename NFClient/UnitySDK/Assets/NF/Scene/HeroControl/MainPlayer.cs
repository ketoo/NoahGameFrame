using UnityEngine;
using NFSDK;


public class MainPlayer : MonoBehaviour {

	NFPlayerModule mPlayerModule;

	public float speed = 4.0f;
    public float jumpSpeed = 10.0f;
    public float gravity = 20.0f;
    public bool MouseChange = true;
    private Vector3 moveDirection = Vector3.zero;
    private bool grounded = false;

	private CharacterController mController;

	void Start()
    {
        mPlayerModule = NFCPluginManager.Instance().FindModule<NFPlayerModule>();

        mController = GetComponent<CharacterController>();

        // get the transform of the main camera
        if (Camera.main != null)
        {
			Camera.main.GetComponent<CameraFollow>().target = this.transform;
        }
    }

    void FixedUpdate()
    {
        if (grounded)
        {
            moveDirection.y = 0;
            float h = Input.GetAxis("Horizontal");
            float v = Input.GetAxis("Vertical");
            /*
            if (Joystick.h != 0 || Joystick.v != 0)
            {
                h = Joystick.h; 
                v = Joystick.v;
            }
*/
            ///可前后左右平移
            moveDirection = new Vector3(h, 0, v);
            transform.LookAt(moveDirection + transform.position);

            moveDirection *= speed;


            if (Input.GetButton("Jump")) //跳跃
            {
                moveDirection.y = jumpSpeed;


                //mPlayerModule.RequireMove(transform.position);
            }

        }
        //重力
        moveDirection.y -= gravity * Time.deltaTime;

		CollisionFlags flags = mController.Move(moveDirection * Time.deltaTime);
        grounded = (flags & CollisionFlags.CollidedBelow) != 0; //当controller处在空中间，grounded为false，即跳动和行走都无效
    }


    /*
	// Use this for initialization
    private float mSyncTime = 1.0f;
    private float mSyncTimeTick = 0.0f;
    private FreeLookCam mCamera;
    private Vector3 m_LastMove;

    private ThirdPersonCharacter m_Character; // A reference to the ThirdPersonCharacter on the object
    private Transform m_Cam;                  // A reference to the main camera in the scenes transform
    private Vector3 m_Move;
    private bool m_Jump;                      // the world-relative desired move direction, calculated from the camForward and user input.



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
		GetKeyGetKeyGetKeyGetKey
        // pass all parameters to the character control script
        m_Character.Move(m_Move, crouch, m_Jump);

        m_Jump = false;

        // 同步坐标给服务器
        bool bMove = false;
        if((h != 0.0f || v != 0.0f))
            bMove = true;

        if (mSyncTimeTick > mSyncTime || (mSyncTimeTick > 0 && !bMove) || m_LastMove != m_Mov)
        {
			mPlayerModule.RequireMove(transform.position);
            mSyncTimeTick = 0.0f;
        }

        if (bMove)
        {
            mSyncTimeTick += Time.deltaTime;
        }
        m_LastMove = m_Move;
    }
    */
}
