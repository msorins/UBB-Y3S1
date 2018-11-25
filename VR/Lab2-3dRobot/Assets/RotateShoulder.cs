using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotateShoulder : MonoBehaviour {
    public bool moveForward = true;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
      

        if(moveForward == true) {
            transform.Rotate(Vector3.right, Time.deltaTime * 40);
        } else {
            transform.Rotate(Vector3.left, Time.deltaTime * 40);
        }

        if(transform.rotation.x >= 0.2 && moveForward == true) {
            moveForward = false;

            return;
        }
        if(transform.rotation.x <= -0.2 && moveForward == false) {
            moveForward = true;
            return;
        }

	}
}
