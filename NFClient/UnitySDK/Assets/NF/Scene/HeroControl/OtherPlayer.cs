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
    private bool grounded = true;
    private bool newPosition = false;

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
		this.speed = 4.0f;
		this.targetPos = pos;

        newPosition = true;
    }

    private void Update()
    {
        if (newPosition == false)
            return;
        
        float fDis = Mathf.Abs(Vector3.Distance(this.transform.position, targetPos));
		if (fDis < speed*0.1)
		{
			moveDirection = Vector3.zero;
			mAnimation.Play("idle");
            newPosition = false;
        }
		else
        {
            transform.LookAt(targetPos);
            this.moveDirection = targetPos - this.transform.position;
            mAnimation.Play("run");

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
				//transform.position = targetPos;
                mAnimation.Play("idle");
                newPosition = false;
            }
			else
			{
				CollisionFlags flags = mController.Move(vector);
                //grounded = (flags & CollisionFlags.CollidedBelow) != 0; 	
			}
		}
    }
}
