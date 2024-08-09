using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class fingerDIP : MonoBehaviour
{
    public float fingerPosit3;
    public GameObject MCPObject;
    public GameObject PIPObject;
    public float zRotation1;
    public float zRotation2;
    public float zRotation;

    // Update is called once per frame
    void Update()
    {
        zRotation = MCPObject.transform.eulerAngles.z;
        zRotation1 = PIPObject.transform.eulerAngles.z-zRotation;
        zRotation2 = transform.eulerAngles.z-zRotation1-zRotation;

        if (SerialHandler.instance != null)
        {   
            fingerPosit3 = SerialHandler.instance.GetParsedValue();
            Vector3 newRotation = transform.eulerAngles;
            newRotation.z = fingerPosit3+zRotation+zRotation1;
            transform.eulerAngles = newRotation;   
        }

    }
}