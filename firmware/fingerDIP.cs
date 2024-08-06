using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class fingerDIP : MonoBehaviour
{
    private float horizontalInput;

    // Update is called once per frame
    void Update()
    {
        if (SerialHandler.instance != null)
        {
            horizontalInput = SerialHandler.instance.GetHorizontalInput();
            transform.Rotate(0, 0, 0.005f * horizontalInput);
        }
    }
}