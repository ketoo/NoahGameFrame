using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using NFSDK;
using UnityStandardAssets.Characters.ThirdPerson;

public class OtherPlayer : MonoBehaviour {

	private float speed;
	private Vector3 targetPos;
	private Vector3 moveDirection;
    private CharacterController mController;
	private Animator mAnimation;

    public float gravity = 20.0f;
    private bool grounded = false;

	void Start ()
    {
        // get the components on the object we need ( should not be null due to require component so no need to check )
		mAnimation = GetComponent<Animator>();
		mController = GetComponent<CharacterController>();

		mController.detectCollisions = false;
		targetPos = this.transform.position;


	}

	public void JumpTo(Vector3 pos)
    {
        mAnimation.Play("jump");
    }

    public void MoveTo(float speed, Vector3 pos)
    {
		this.speed = 0.4f;
		this.targetPos = pos;

		transform.LookAt(pos);

        this.moveDirection = targetPos - this.transform.position;

		mAnimation.Play("run");
    }

    private void Update()
    {
		float fDis = Mathf.Abs(Vector3.Distance(this.transform.position, targetPos));
		if (fDis < 0.1f)
		{
			moveDirection = Vector3.zero;
			mAnimation.Play("idle");
		}
		else
		{

            //mController.SimpleMove(this.moveDirection.normalized * speed);
            if (grounded)
            {
                moveDirection.y = 0;
            }
            else
            {
                moveDirection.y -= gravity * Time.deltaTime;
            }

			Vector3 vector = this.moveDirection.normalized * speed * Time.deltaTime;
			if (vector.sqrMagnitude >= fDis)
			{
				transform.position = targetPos;
			}
			else
			{
				CollisionFlags flags = mController.Move(vector);
                grounded = (flags & CollisionFlags.CollidedBelow) != 0; 	
			}
		}
    }
}
