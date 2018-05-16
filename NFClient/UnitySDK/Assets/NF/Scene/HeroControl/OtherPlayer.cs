using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using NFSDK;
using UnityStandardAssets.Characters.ThirdPerson;

public class OtherPlayer : MonoBehaviour {

	// Use this for initialization
	private Transform target;                                    // target to aim for
    private Vector3 targetPos;                                    // target to aim for

    private CharacterController mController;
	private Animator mAnimation;
	private UnityEngine.AI.NavMeshAgent agent;            // the navmesh agent required for the path finding
    
	void Start ()
    {
        // get the components on the object we need ( should not be null due to require component so no need to check )
		mAnimation = GetComponent<Animator>();
		agent = GetComponentInChildren<UnityEngine.AI.NavMeshAgent>();
		if (agent == null)
		{
			agent = transform.gameObject.AddComponent<UnityEngine.AI.NavMeshAgent>();
		}

        agent.updateRotation = false;
        agent.updatePosition = true;
        // 其他玩家移动速度减慢
        agent.speed = 0.9f;
	}

	public void JumpTo(Vector3 pos)
    {
        mAnimation.Play("jump");
    }

    public void MoveTo(Vector3 pos)
    {
        if (target != null)
            target.position = pos;
        targetPos = pos;

		if (mAnimation)
		{
			mAnimation.Play("run");
		}
    }

    private void Update()
    {
        if (target != null)
            agent.SetDestination(target.position);
        else
            agent.SetDestination(targetPos);

		if (Vector3.Distance(this.transform.position, targetPos) < 0.05f)
		{
			mAnimation.Play("idle");
		}

		//mController.mo(agent.desiredVelocity);
    }

    public void SetTarget(Transform target)
    {
        this.target = target;
    }
}
