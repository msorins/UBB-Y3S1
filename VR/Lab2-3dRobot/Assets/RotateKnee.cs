using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotateKnee : MonoBehaviour {
    GameObject thigh;
    RotateThigh leftThighRotate;

    void Start()
    {
        thigh = this.transform.parent.gameObject.transform.parent.gameObject;
        leftThighRotate = thigh.GetComponent<RotateThigh>();
        transform.rotation.Set(0.2f, 0.0f, 0.0f, 0.0f);
    }

    // Update is called once per frame
    void Update()
    {
        if (leftThighRotate != null && leftThighRotate.sleepFirst == false) 
        {
            print(leftThighRotate.moveForward);

            if (leftThighRotate.moveForward == true)
            {
                transform.Rotate(Vector3.left, Time.deltaTime * 100);
            }
            else
            {
                transform.Rotate(Vector3.right, Time.deltaTime * 100);
                //transform.SetPositionAndRotation(this.transform.position, thigh.transform.rotation);
            }
        }
        else
        {
            print("null");
        }
    }
}
