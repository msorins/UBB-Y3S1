using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shoot : MonoBehaviour {
    public Rigidbody projectile;
    public Transform shotPos;
    public float shootForce = 1000f;
    public float moveSpeed = 10f;

	void Update () {
        float h = Input.GetAxis("Horizontal") * Time.deltaTime * moveSpeed;
        float v = Input.GetAxis("Vertical") * Time.deltaTime * moveSpeed;

        transform.Translate(new Vector3(h, v, 0));
        transform.position = new Vector3(transform.position.x, Mathf.Max(transform.position.y, 0.2f), transform.position.z);

        if(Input.GetButtonUp("Fire1")) {
            Rigidbody shot = Instantiate(projectile, transform.position, shotPos.rotation) as Rigidbody;
            shot.AddForce(shotPos.forward * shootForce);
        }
	}
}
