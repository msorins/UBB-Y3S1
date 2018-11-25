using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotateElbow : MonoBehaviour {
    GameObject shoulder;
    RotateShoulder shoulderRotate;

    void Start()
    {
        shoulder = this.transform.parent.gameObject.transform.parent.gameObject;
        shoulderRotate = shoulder.GetComponent<RotateShoulder>();
        transform.rotation.Set(0.2f, 0.0f, 0.0f, 0.0f);
    }

    // Update is called once per frame
    void Update()
    {
        if( shoulderRotate != null) {
            if (shoulderRotate.moveForward == true)
            {
                transform.Rotate(Vector3.right, Time.deltaTime * 60);
            }
            else
            {
                transform.Rotate(Vector3.left, Time.deltaTime * 60);
            }
        } else {
            print("null");
        }
    }
}
