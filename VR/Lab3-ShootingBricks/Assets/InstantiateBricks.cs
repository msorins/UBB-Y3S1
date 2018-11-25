using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InstantiateBricks : MonoBehaviour {
    public Transform brick;

	// Use this for initialization
	void Start () {
        Vector3 pos = new Vector3(0.0f, 0.5f, 12.0f);
        for (int i = 10; i >= 1; i--) {
            for (int j = 1;  j<= i; j++)
            {
                Instantiate(brick, pos, Quaternion.identity);
                pos.x += 1.0f;
            }
            pos.y += 1.0f;
            pos.x = 0.0f;
        }

        pos = new Vector3(-1.0f, 0.5f, 12.0f);
        for (int i = 10; i >= 1; i--)
        {
            for (int j = 1; j <= i; j++)
            {
                Instantiate(brick, pos, Quaternion.identity);
                pos.x -= 1.0f;
            }
            pos.y += 1.0f;
            pos.x = -1.0f;
        }


    }
	
	// Update is called once per frame
	void Update () {
		
	}
}
