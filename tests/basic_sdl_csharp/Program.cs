using System.Runtime.InteropServices;
using System.Collections;
using System.Collections.Generic;
using System;

new CameraRender();

public class CameraRender
{
    [DllImport("..\\..\\..\\..\\..\\build\\costume.dll")]
    private static extern int matrix_init(int width, int height, int debug);

    [DllImport("..\\..\\..\\..\\..\\build\\costume.dll")]
    private static extern void matrix_tick();

    [DllImport("..\\..\\..\\..\\..\\build\\costume.dll")]
    private static extern void matrix_put(Pixel[] pixels);

    [DllImport("..\\..\\..\\..\\..\\build\\costume.dll")]
    private static extern void matrix_flip();

    [DllImport("..\\..\\..\\..\\..\\build\\costume.dll")]
    private static extern void matrix_release();

    struct Pixel {
        public byte a, b, g, r;   
    };

    const int WIDTH = 800, HEIGHT = 600;
    const double MAX_DIST = 600;

    public CameraRender()
    {
        Console.WriteLine("starting in main");

        matrix_init(WIDTH, HEIGHT, 1);

        Pixel[] pixels = new Pixel[WIDTH * HEIGHT];

        int t = 0;
        int dir = 1;
        bool quit = false;

        while (!quit)
        {
            // fill with random data
            for (int x = 0; x < WIDTH; x++)
            {
                for (int y = 0; y < HEIGHT; y++)
                {
                    int i = (y * WIDTH + x);
                    pixels[i].a = get_value(x, y, 400, 600 - t / 2);
                    pixels[i].g = get_value(x, y, 0 + t / 4, 0 + t / 2);
                    pixels[i].b = get_value(x, y, 800 - t, 0);
                }
            }

            matrix_put(pixels);
            matrix_flip();
            matrix_tick();

            t += dir;
            if (t >= 800) dir = -1;
            if (t <= 0) dir = 1;

            while (Console.KeyAvailable)
            {
                ConsoleKeyInfo k = Console.ReadKey();
                if (k.Key == ConsoleKey.Escape || k.Key == ConsoleKey.Q)
                {
                    quit = true;
                    break;
                }
            }
        }

        matrix_release();
    }

    double distance(double x1, double y1, double x2, double y2)
    {
        double dx = Math.Abs(x1 - x2);
        double dy = Math.Abs(y1 - y2);
        return Math.Sqrt(dx * dx + dy * dy);
    }

    double map_range(double x, double x_min, double x_max, double y_min, double y_max)
    {
        double t = (x - x_min) * (y_max - y_min) / (x_max - x_min) + y_min;
        if (t > y_max) return y_max;
        if (t < y_min) return y_min;
        return t;
    }

    byte get_value(int x, int y, int tx, int ty)
    {
        return (byte)(255 - Math.Round(map_range(distance(x, y, tx, ty), 0, MAX_DIST, 0, 255)));
    }

    void set_pixel(int x, int y, byte r, byte g, byte b, byte[] arr)
    {
        int i = (y * WIDTH + x) * 3;
        arr[i] = r;
        arr[i + 1] = g;
        arr[i + 2] = b;
    }
}
