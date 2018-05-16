using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using NFSDK;
using UnityStandardAssets.Characters.ThirdPerson;

[RequireComponent(typeof(UnityEngine.AI.NavMeshAgent))]
public class OtherPlayer : MonoBehaviour {

	// Use this for initialization
    public UnityEngine.AI.NavMeshAgent agent { get; private set; }             // the navmesh agent required for the path finding
    public Transform target;                                    // target to aim for
    public Vector3 targetPos;                                    // target to aim for

    private CharacterController mController;
	private Animator mAnimation;

	void Start ()
    {
        // get the components on the object we need ( should not be null due to require component so no need to check )
        agent = GetComponentInChildren<UnityEngine.AI.NavMeshAgent>();
		mAnimation = GetComponent<Animator>();
			
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


        mAnimation.Play("run");
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

		//mController.Move(agent.desiredVelocity);
    }

    public void SetTarget(Transform target)
    {
        this.target = target;
    }
}
