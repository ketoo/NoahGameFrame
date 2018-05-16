using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using NFSDK;
using UnityStandardAssets.Characters.ThirdPerson;

[RequireComponent(typeof(UnityEngine.AI.NavMeshAgent))]
[RequireComponent(typeof(ThirdPersonCharacter))]
public class OtherPlayer : MonoBehaviour {

	// Use this for initialization
    public UnityEngine.AI.NavMeshAgent agent { get; private set; }             // the navmesh agent required for the path finding
    public Transform target;                                    // target to aim for
    public Vector3 targetPos;                                    // target to aim for

    private CharacterController mController;

	void Start ()
    {
        // get the components on the object we need ( should not be null due to require component so no need to check )
        agent = GetComponentInChildren<UnityEngine.AI.NavMeshAgent>();

        agent.updateRotation = false;
        agent.updatePosition = true;
        // 其他玩家移动速度减慢
        agent.speed = 0.9f;
	}

    public void MoveTo(Vector3 pos)
    {
        if (target != null)
            target.position = pos;
        targetPos = pos;
    }

    private void Update()
    {
        if (target != null)
            agent.SetDestination(target.position);
        else
            agent.SetDestination(targetPos);

		//mController.Move(agent.desiredVelocity);
    }

    public void SetTarget(Transform target)
    {
        this.target = target;
    }
}
