using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotateThigh : MonoBehaviour {
    public bool sleepFirst = true;
    public bool moveForward = true;

    private float startTime;
    // Use this for initialization
    void Start()
    {
        startTime = Time.time;
    }

    // Update is called once per frame
    void Update()
    {
        if(sleepFirst) {
            StartCoroutine(DoWait());
        } else {
            if (moveForward == true)
            {
                transform.Rotate(Vector3.right, Time.deltaTime * 70);
            }
            else
            {
                transform.Rotate(Vector3.left, Time.deltaTime * 70);
            }

            if (transform.rotation.x >= 0.4 && moveForward == true)
            {
                moveForward = false;

                return;
            }
            if (transform.rotation.x <= -0.05 && moveForward == false)
            {
                moveForward = true;
                return;
            }
        }
      

    }

    IEnumerator DoWait()
    {
        yield return new WaitForSeconds(0.65f);
        sleepFirst = false;
    }
}

