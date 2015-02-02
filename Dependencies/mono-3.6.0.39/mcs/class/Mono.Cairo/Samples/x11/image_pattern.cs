//
//
//	Mono.Cairo drawing samples using X11 as drawing surface
//	Autor: Hisham Mardam Bey <hisham@hisham.cc>
//

//
// Copyright (C) 2005 Novell, Inc (http://www.novell.com)
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

using System;
using System.Reflection;
using System.Runtime.InteropServices;
using Cairo;

public class X11Test
{
        static readonly double  M_PI = 3.14159265358979323846;
	
        static void draw (Cairo.Context gr, int width, int height)
	{
		int w, h;
		ImageSurface image;
		Matrix matrix;
		SurfacePattern pattern;
		
		gr.Scale (width, height);
		gr.LineWidth = 0.04;
		
		image = new ImageSurface ("data/e.png");
		w = image.Width;
		h = image.Height;
		
		pattern = new SurfacePattern (image);
		pattern.Extend = Cairo.Extend.Repeat;
		
		gr.Translate (0.5, 0.5);
		gr.Rotate (M_PI / 4);
		gr.Scale (1 / Math.Sqrt (2), 1 / Math.Sqrt (2));
		gr.Translate (- 0.5, - 0.5);
		
		matrix = new Matrix ();
		matrix.InitScale (w * 5.0, h * 5.0);
		
		pattern.Matrix = matrix;
		
		gr.Pattern = pattern;
		
		gr.Rectangle ( new PointD (0, 0),
					       1.0, 1.0);
		gr.Fill ();
		
		pattern.Destroy ();
		image.Destroy();
	}
	
	
	
	static void Main (string [] args)
	{
		Window win = new Window (500, 500);
		
		win.Show ();
		
		Cairo.XlibSurface s = new Cairo.XlibSurface (win.Display,
			       win.XWindow,
			       X11.XDefaultVisual (win.Display, win.Screen),
			       (int)win.Width, (int)win.Height);

		
		Cairo.Context g = new Cairo.Context (s);
		
		draw (g, 500, 500);
		
		IntPtr xev = new IntPtr ();
		
		while (true) {			
			X11.XNextEvent (win.Display, xev);
		}		
	}
}
