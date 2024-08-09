using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class fingerPIP : MonoBehaviour
{
    public float fingerPosit2;
    public float zRotation1;
    public float zRotation;
    public GameObject MCPObject;

    // Update is called once per frame
    void Update()
    {
        zRotation = MCPObject.transform.eulerAngles.z;
        zRotation1 = transform.eulerAngles.z-zRotation;
         if (SerialHandler.instance != null)
        {   
            fingerPosit2 = SerialHandler.instance.GetParsedValue();
            Vector3 newRotation = transform.eulerAngles;
            newRotation.z = fingerPosit2+zRotation;
            transform.eulerAngles = newRotation;   
        }

    }
}