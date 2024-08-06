using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class SerialHandler : MonoBehaviour
{
    private SerialPort data_stream = new SerialPort("COM7", 19200);
    public string receivedString;
    public string[] datas;
    public float horizontalInput;

    public static SerialHandler instance;

    void Awake()
    {
        // 싱글톤 인스턴스 설정
        if (instance == null)
        {
            instance = this;
            DontDestroyOnLoad(gameObject); // 씬 전환 시 객체 유지
        }
        else
        {
            Destroy(gameObject);
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        data_stream.Open();
    }

    // Update is called once per frame
    void Update()
    {
        if (data_stream.IsOpen)
        {
            try
            {
                receivedString = data_stream.ReadLine();
                datas = receivedString.Split(',');
                horizontalInput = float.Parse(datas[1]);


            }
            catch (System.Exception)
            {
                // 데이터 읽기 오류 처리
            }
        }
    }

    public float GetHorizontalInput()
    {
        return  horizontalInput;
    }
}