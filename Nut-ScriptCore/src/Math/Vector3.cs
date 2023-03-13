namespace Nut.Math
{
    public struct Vector3
    {
        public float X, Y, Z;

        public Vector3(float x, float y, float z) { X = x; Y = y; Z = z; }
        public Vector3(Vector2 xy, float z) { X = xy.X; Y = xy.Y; Z = z; }

        public static Vector3 Zero => new Vector3(0, 0, 0);
        public Vector2 XY => new Vector2(X, Y);

        public void Add(float value) { X += value; Y += value; Z += value; }
        public void Substract(float value) { X -= value; Y -= value; Z -= value; }
        public void Multiply(float value) { X *= value; Y *= value; Z *= value; }
        public void Divde(float value) { X /= value; Y /= value; Z /= value; }


        public static Vector3 operator +(Vector3 left, Vector3 right) { return new Vector3(left.X + right.X, left.Y + right.Y, left.Z + right.Z); }
        public static Vector3 operator -(Vector3 left, Vector3 right) { return new Vector3(left.X - right.X, left.Y - right.Y, left.Z - right.Z); }
        public static Vector3 operator *(Vector3 left, Vector3 right) { return new Vector3(left.X * right.X, left.Y * right.Y, left.Z * right.Z); }
        public static Vector3 operator /(Vector3 left, Vector3 right) { return new Vector3(left.X / right.X, left.Y / right.Y, left.Z / right.Z); }

        public static Vector3 operator +(Vector3 left, float right) { return new Vector3(left.X + right, left.Y + right, left.Z + right); }
        public static Vector3 operator -(Vector3 left, float right) { return new Vector3(left.X - right, left.Y - right, left.Z - right); }
        public static Vector3 operator *(Vector3 left, float right) { return new Vector3(left.X * right, left.Y * right, left.Z * right); }
        public static Vector3 operator /(Vector3 left, float right) { return new Vector3(left.X / right, left.Y / right, left.Z / right); }
    }
}
