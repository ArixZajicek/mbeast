using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;
using System.Runtime.InteropServices;
using System;
using Unity.Collections;
using UnityEditor.Experimental.GraphView;
using UnityEngine.U2D;

public class CameraRender : MonoBehaviour
{
    public bool UseNativeLib;

    const int WIDTH = 256, HEIGHT = 64;

    [DllImport("costume.dll")]
    private static extern int matrix_init(int width, int height, int debug);

    [DllImport("costume.dll")]
    private static extern void matrix_tick();

    [DllImport("costume.dll")]
    private static extern void matrix_put(byte[] pixels);

    [DllImport("costume.dll")]
    private static extern void matrix_flip();

    [DllImport("costume.dll")]
    private static extern void matrix_release();

    // Start is called before the first frame update
    void Start()
    {
        if (UseNativeLib)
        {
            matrix_init(WIDTH, HEIGHT, 1);           
        }
    }

    // Update is called once per frame
    void Update()
    {
        matrix_tick();
    }
    
    private void OnPostRender()
    {
        OnPostRenderCallback(GetComponent<Camera>());
    } 

    private void OnPostRenderCallback(Camera camera)
    {
        if (camera != null)
        {
            Texture2D pixels = new Texture2D(WIDTH, HEIGHT, TextureFormat.RGBA32, false);
            pixels.ReadPixels(new Rect(0, 0, WIDTH, HEIGHT), 0, 0, false);
            matrix_put(pixels.GetRawTextureData());
            matrix_flip();
        }
    }
    
    private void OnApplicationQuit()
    {
        matrix_release();
    }
}
