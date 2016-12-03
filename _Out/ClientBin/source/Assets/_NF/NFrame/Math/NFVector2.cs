using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFrame
{
    public class NFVector2
    {
        private float x;
        private float y;

        public NFVector2()
        {
            this.x = 0.0f;
            this.y = 0.0f;
        }
        public NFVector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }

        public float X()
        {
            return this.x;
        }
        public float Y()
        {
            return this.y;

        }
        public void SetX(float x)
        {
            this.x = x;
        }
        public void SetY(float y)
        {
            this.y = y;
        }

        public bool IsZero()
        {
            return x < 0.001f && y < 0.001f;
        }

        public float Length()
        {
            return (float)Math.Sqrt(x * x + y * y);
        }

        public float SquaredLength()
        {
            return x * x + y * y;
        }

        public NFVector2 Direction()
        {
            if (this.IsZero())
            {
                return new NFVector2();
            }
            float length =  this.Length();
            return new NFVector2(x /length, y /length);
        }

        public bool FromString(string value)
        {
            string[] values = value.Split(',');
            if (values.Length != 2)
            {
                return false;
            }
            float.TryParse(values[0], out x);
            float.TryParse(values[1], out y);
            return true;
        }

        public string ToStr()
        {
            return x + "," + y;
        }

        public static NFVector2 add(NFVector2 va, NFVector2 vb)
        {
            NFVector2 vector = new NFVector2();
            vector.SetX(va.X() + vb.X());
            vector.SetY(va.Y() + vb.Y());
            return vector;
        }

        public static NFVector2 sub(NFVector2 va, NFVector2 vb)
        {
            NFVector2 vector = new NFVector2();
            vector.SetX(va.X() - vb.X());
            vector.SetY(va.Y() - vb.Y());
            return vector;
        }

        public static float Distance(NFVector2 va, NFVector2 vb)
        {
            NFVector2 vector = NFVector2.sub(va, vb);
            return vector.Length();
        }


    }
}
