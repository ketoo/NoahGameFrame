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

		if (mAnimation)
		{
			mAnimation.Play("run");
		}
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
			this.moveDirection = targetPos - this.transform.position;
		}

        moveDirection.y -= gravity * Time.deltaTime;
		CollisionFlags flags = mController.Move(this.moveDirection.normalized * speed * Time.deltaTime);
        grounded = (flags & CollisionFlags.CollidedBelow) != 0;
    }
}
