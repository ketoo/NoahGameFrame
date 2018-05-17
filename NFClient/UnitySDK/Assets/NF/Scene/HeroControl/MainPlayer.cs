using UnityEngine;
using NFSDK;


public class MainPlayer : MonoBehaviour {

	NFPlayerModule mPlayerModule;

    private float mSyncTime = 0.3f;
    private float mSyncTimeTick = 0.0f;
    
	public float speed = 0.6f;
    public float jumpSpeed = 10.0f;
    public float gravity = 20.0f;
    public bool MouseChange = true;
    private Vector3 moveDirection = Vector3.zero;
    private bool grounded = false;

	private CharacterController mController;
	private Animator mAnimation;
	private bool mbMoved = false;
	void Start()
    {
        mPlayerModule = NFCPluginManager.Instance().FindModule<NFPlayerModule>();

        mController = GetComponent<CharacterController>();
		mAnimation = GetComponent<Animator>();

		mController.detectCollisions = false;

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

			if (moveDirection.sqrMagnitude > 0.01f)
			{
				mbMoved = true;
				mAnimation.Play("run");
			}
			else
			{
                mAnimation.Play("idle");
			}

            if (Input.GetButton("Jump")) //跳跃
            {
				moveDirection.y = jumpSpeed;
                mAnimation.Play("jump");
                
				mPlayerModule.RequireJump(transform.position);
            }

        }

        //重力
        moveDirection.y -= gravity * Time.deltaTime;

		CollisionFlags flags = mController.Move(moveDirection * Time.deltaTime);
        grounded = (flags & CollisionFlags.CollidedBelow) != 0; //当controller处在空中间，grounded为false，即跳动和行走都无效


		Sync();
    }

    void Sync()
	{
		mSyncTimeTick += Time.deltaTime;

		if (mbMoved && grounded && mSyncTimeTick > mSyncTime)
        {
			mPlayerModule.RequireMove(transform.position);
            mSyncTimeTick = 0.0f;
			mbMoved = false;
        }
	}
}
