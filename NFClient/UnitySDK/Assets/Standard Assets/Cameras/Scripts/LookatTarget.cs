using System;
using UnityEngine;

namespace UnityStandardAssets.Cameras
{
    public class LookatTarget : AbstractTargetFollower
    {
        // A simple script to make one object look at another,
        // but with optional constraints which operate relative to
        // this gameobject's initial rotation.

        // Only rotates around local X and Y.

        // Works in local coordinates, so if this object is parented
        // to another moving gameobject, its local constraints will
        // operate correctly
        // (Think: looking out the side window of a car, or a gun turret
        // on a moving spaceship with a limited angular range)

        // to have no constraints on an axis, set the rotationRange greater than 360.

        [SerializeField] private Vector2 m_RotationRange;
        [SerializeField] private float m_FollowSpeed = 1;

        private Vector3 m_FollowAngles;
        private Quaternion m_OriginalRotation;

        protected Vector3 m_FollowVelocity;


        // Use this for initialization
        protected override void Start()
        {
            base.Start();
            m_OriginalRotation = transform.localRotation;
        }


        protected override void FollowTarget(float deltaTime)
        {
            // we make initial calculations from the original local rotation
            transform.localRotation = m_OriginalRotation;

            // tackle rotation around Y first
            Vector3 localTarget = transform.InverseTransformPoint(m_Target.position);
            float yAngle = Mathf.Atan2(localTarget.x, localTarget.z)*Mathf.Rad2Deg;

            yAngle = Mathf.Clamp(yAngle, -m_RotationRange.y*0.5f, m_RotationRange.y*0.5f);
            transform.localRotation = m_OriginalRotation*Quaternion.Euler(0, yAngle, 0);

            // then recalculate new local target position for rotation around X
            localTarget = transform.InverseTransformPoint(m_Target.position);
            float xAngle = Mathf.Atan2(localTarget.y, localTarget.z)*Mathf.Rad2Deg;
            xAngle = Mathf.Clamp(xAngle, -m_RotationRange.x*0.5f, m_RotationRange.x*0.5f);
            var targetAngles = new Vector3(m_FollowAngles.x + Mathf.DeltaAngle(m_FollowAngles.x, xAngle),
                                           m_FollowAngles.y + Mathf.DeltaAngle(m_FollowAngles.y, yAngle));

            // smoothly interpolate the current angles to the target angles
            m_FollowAngles = Vector3.SmoothDamp(m_FollowAngles, targetAngles, ref m_FollowVelocity, m_FollowSpeed);


            // and update the gameobject itself
            transform.localRotation = m_OriginalRotation*Quaternion.Euler(-m_FollowAngles.x, m_FollowAngles.y, 0);
        }
    }
}
