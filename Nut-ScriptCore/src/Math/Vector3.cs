namespace Nut.Math
{
    public struct Vector3
    {
        public float X, Y, Z;

        public Vector3(float x, float y, float z) { X = x; Y = y; Z = z; }

        public void Add(float value) { X += value; Y += value; Z += value; }
        public void Substract(float value) { X -= value; Y -= value; Z -= value; }
        public void Multiply(float value) { X *= value; Y *= value; Z *= value; }
        public void Divde(float value) { X /= value; Y /= value; Z /= value; }


        public static Vector3 operator +(Vector3 left, Vector3 right) { return new Vector3(left.X + right.X, left.Y + right.Y, left.Z + right.Z); }
        public static Vector3 operator -(Vector3 left, Vector3 right) { return new Vector3(left.X - right.X, left.Y - right.Y, left.Z - right.Z); }
        public static Vector3 operator *(Vector3 left, Vector3 right) { return new Vector3(left.X * right.X, left.Y * right.Y, left.Z * right.Z); }
        public static Vector3 operator /(Vector3 left, Vector3 right) { return new Vector3(left.X / right.X, left.Y / right.Y, left.Z / right.Z); }
    }
}
