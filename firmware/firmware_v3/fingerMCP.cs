using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class fingerMCP : MonoBehaviour
{

    public float fingerPosit1; // position control
    public float zRotation;

    // Update is called once per frame
    void Update()
    {   if (SerialHandler.instance != null)
        {   
            fingerPosit1 = SerialHandler.instance.GetParsedValue();
            Vector3 newRotation = transform.eulerAngles;
            newRotation.z = fingerPosit1;
            transform.eulerAngles = newRotation;   
        }

    }
}