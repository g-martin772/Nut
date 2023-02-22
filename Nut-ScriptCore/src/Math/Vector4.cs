namespace Nut.Math
{
    public struct Vector4
    {
        public float X, Y, Z, W;

        public Vector4(float x, float y, float z, float w) { X = x; Y = y; Z = z; W = w;}

        public void Add(float value) { X += value; Y += value; Z += value; W += value; }
        public void Substract(float value) { X -= value; Y -= value; Z -= value; W -= value; }
        public void Multiply(float value) { X *= value; Y *= value; Z *= value; W *= value; }
        public void Divde(float value) { X /= value; Y /= value; Z /= value; W /= value; }


        public static Vector4 operator +(Vector4 left, Vector4 right) { return new Vector4(left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W); }
        public static Vector4 operator -(Vector4 left, Vector4 right) { return new Vector4(left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W); }
        public static Vector4 operator *(Vector4 left, Vector4 right) { return new Vector4(left.X * right.X, left.Y * right.Y, left.Z * right.Z, left.W * right.W); }
        public static Vector4 operator /(Vector4 left, Vector4 right) { return new Vector4(left.X / right.X, left.Y / right.Y, left.Z / right.Z, left.W / right.W); }
    }
}
