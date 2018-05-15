using UnityEngine;
using System.Collections;

public class CameraFollow : MonoBehaviour
{
	[Header("Camera")]
    [Tooltip("Reference to the target GameObject.")]
    public Transform target;
    [Tooltip("Multiplier for camera sensitivity.")]
    [Range(0f, 300)]
    public float sensitivity = 120f;
    [Tooltip("Current relative offset to the target.")]
    public Vector3 offset;
    [Tooltip("Minimum relative offset to the target GameObject.")]
    public Vector3 minOffset;
    [Tooltip("Maximum relative offset to the target GameObject.")]
    public Vector3 maxOffset;
    [Tooltip("Rotation limits for the X-axis in degrees. X represents the lowest and Y the highest value.")]
    public Vector2 rotationLimitsX;
    [Tooltip("Rotation limits for the Y-axis in degrees. X represents the lowest and Y the highest value.")]
    public Vector2 rotationLimitsY;
    [Tooltip("Whether the rotation on the X-axis should be limited.")]
    public bool limitXRotation = false;
    [Tooltip("Whether the rotation on the Y-axis should be limited.")]
    public bool limitYRotation = false;

    private Transform cameraTransform;
    private Vector2 cameraRotation;

    void Awake()
    {
        DontDestroyOnLoad(this.gameObject);
    }
       
    
    // Use this for initialization
    private void Start()
    {
        if (target == null)
        {
            Debug.LogWarning(gameObject.name + ": No target found!");
        }

        cameraTransform = transform;
    }

    // Update is called once per frame
    private void Update()
    {

    }

    // LateUpdate is called every frame, if the Behaviour is enabled
    private void LateUpdate()
    {
        cameraRotation.x = 0;
        cameraRotation.y = 45;

        if (target && Input.GetMouseButton(1))
        {
            /*
            cameraRotation.x += Input.GetAxis("Mouse X") * sensitivity * Time.deltaTime;
            cameraRotation.y -= Input.GetAxis("Mouse Y") * sensitivity * Time.deltaTime;

            if(limitXRotation)
            {
                cameraRotation.x = Mathf.Clamp(cameraRotation.x, rotationLimitsX.x, rotationLimitsX.y);
            }
            if(limitYRotation)
            {
                cameraRotation.y = Mathf.Clamp(cameraRotation.y, rotationLimitsY.x, rotationLimitsY.y);
            }
            */
        }
        if (target)
        {
            offset.z -= Input.GetAxis("Mouse ScrollWheel") * sensitivity * Time.deltaTime;
            offset.z = Mathf.Clamp(offset.z, minOffset.z, maxOffset.z);

            Quaternion rotation = Quaternion.Euler(cameraRotation.y, cameraRotation.x, 0);
            Vector3 position = rotation * new Vector3(offset.x, offset.y, offset.z) + target.position;

            cameraTransform.rotation = rotation;
            cameraTransform.position = position;
        }

    }
 
}