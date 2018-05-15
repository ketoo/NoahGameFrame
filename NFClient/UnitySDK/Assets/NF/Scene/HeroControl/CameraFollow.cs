using UnityEngine;
using System.Collections;

public class CameraFollow : MonoBehaviour
{

	public Transform target;
    private Vector3 offsetPosition;
    private bool isRotating = false;

    public float distance;
    public float scrollSpeed = 3;
    public float rotareSpeed = 2;



    void Start()
    {
		DontDestroyOnLoad(this.gameObject);

		if (target == null)
		{
			return;
		}

		transform.LookAt(target.position);
		offsetPosition = transform.position - target.position;//得到偏移量

    }


    void Update()
    {
		if (target == null)
        {
            return;
        }

		transform.position = offsetPosition + target.position;
        RotateView();
        ScrollView();

    }

    //处理视野的拉近和拉远效果
    void ScrollView()
    {
        // print(Input.GetAxis("Mouse ScrollWheel"));//鼠标向后滑动返回负数（拉近视野），向前正数（拉远视野）
        distance = offsetPosition.magnitude;
        distance += Input.GetAxis("Mouse ScrollWheel") * scrollSpeed;
        distance = Mathf.Clamp(distance, 2, 18);
        offsetPosition = offsetPosition.normalized * distance;//改变位置便移
    }


    //控制视野左右上下旋转

    void RotateView()
    {
        //Input.GetAxis("Mouse X");//得到鼠标水平方向的滑动
        //Input.GetAxis("Mouse Y");//得到鼠标在垂直方向的滑动
        if (Input.GetMouseButtonDown(1))
        {
            isRotating = true;

        }

        if (Input.GetMouseButtonUp(1))
        {
            isRotating = false;

        }

        if (isRotating)
        {

			transform.RotateAround(target.position, target.up, rotareSpeed * Input.GetAxis("Mouse X"));//围捞角色滑动 左右

            Vector3 originalPos = transform.position;
            Quaternion originalRotation = transform.rotation;

			transform.RotateAround(target.position, transform.right, -rotareSpeed * Input.GetAxis("Mouse Y"));//上下 (会影响到的属性一个是Position,一个是rotation)
                                                                                                              //限制上下滑动的度数大小
            float x = transform.eulerAngles.x;
            if (x < 10 || x > 80)//当超出范围之后，我们将属性归位，让旋转无效
            {
                transform.position = originalPos;
                transform.rotation = originalRotation;

            }

        }

		offsetPosition = transform.position - target.position;

    }
}