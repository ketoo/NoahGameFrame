using UnityEngine;
using System.Collections;
using NFSDK;

public class OtherPlayer : MonoBehaviour {

	// Use this for initialization
    private float m_speed = 5.0f;
    public Vector3 targetPos;
    public bool mNewPos;

	void Start () 
    {
	}

    public void MoveTo(Vector3 pos)
    {
        targetPos = pos;
        mNewPos = true;
    }
	
	// Update is called once per frame
    void Update()
    {
        if (!mNewPos)
            return;

        //获取当前物体到目标物体的距离
        float distance = Vector3.Distance(targetPos, transform.position);
 
         //判断是否超出距离
        if (distance > 0.1f)
        {
            Vector3 dir = targetPos - transform.position;
            dir.Normalize();
            transform.Translate(dir * m_speed * Time.deltaTime);
        }
        else
        {
            mNewPos = false;
        }
	}
}
