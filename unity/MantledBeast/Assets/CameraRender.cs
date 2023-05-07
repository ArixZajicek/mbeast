using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class CameraRender : MonoBehaviour
{
    [DllImport("libcostume.so")]
    private static extern int matrix_init(int width, int height, int debug);

    [DllImport("libcostume.so")]
    private static extern int matrix_put(RenderBuffer val);

    [DllImport("libcostume.so")]
    private static extern int matrix_flip();

    [DllImport("libcostume.so")]
    private static extern int matrix_release();

    private Texture2D destinationTexture;

    // Start is called before the first frame update
    void Start()
    {
        matrix_release();
        matrix_init(800, 600, 1);
        destinationTexture = new Texture2D(800, 600, TextureFormat.RGB24, false);
    }

    // Update is called once per frame
    void Update()
    {
        Texture2D tex = new Texture2D(800, 600);
        tex.ReadPixels(new Rect(0, 0, 800, 600), 0, 0, false);
        matrix_put(tex);
        matrix_flip();
    }
}
