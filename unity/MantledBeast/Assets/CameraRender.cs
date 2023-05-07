using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;
using System.Runtime.InteropServices;
using System;
using Unity.Collections;

public class CameraRender : MonoBehaviour
{
    [DllImport("costume")]
    private static extern int matrix_init(int width, int height, int debug);

    [DllImport("costume")]
    private static extern int matrix_put(NativeArray<Color32> bytes);

    [DllImport("costume")]
    private static extern int matrix_flip();

    [DllImport("costume")]
    private static extern int matrix_release();

    public Renderer screenGrabRenderer;

    private Texture2D pixels;
    private bool readyForGrab;

    // Start is called before the first frame update
    void Start()
    {
        // Set up pixels tex for grabbing into
        pixels = new Texture2D(Screen.width, Screen.height, TextureFormat.RGB24, false);
        screenGrabRenderer.material.mainTexture = pixels;
        Camera.onPostRender += OnPostRenderCallback;

        matrix_release();
        matrix_init(Screen.width, Screen.height, 1);
        readyForGrab = true;
    }

    // Update is called once per frame
    void Update()
    {
    }

    private void OnPostRenderCallback(Camera camera)
    {
        if (readyForGrab && camera == Camera.main)
        {
            readyForGrab = false;
            pixels.ReadPixels(new Rect(0, 0, Screen.width, Screen.height), 0, 0, false);

            matrix_put(pixels.GetRawTextureData<Color32>());
            matrix_flip();

            readyForGrab = true;
        }
    }
}
