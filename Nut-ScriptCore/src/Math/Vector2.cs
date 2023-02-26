namespace Nut.Math
{
    public struct Vector2
    {
        public float X, Y;

        public Vector2(float x, float y) { X = x; Y = y; }

        public static Vector2 Zero => new Vector2(0, 0);

        public void Add(float value) { X += value; Y += value; }
        public void Substract(float value) { X -= value; Y -= value; }
        public void Multiply(float value) { X *= value; Y *= value; }
        public void Divde(float value) { X /= value; Y /= value; }


        public static Vector2 operator+(Vector2 left, Vector2 right) { return new Vector2(left.X + right.X, left.Y + right.Y); }
        public static Vector2 operator-(Vector2 left, Vector2 right) { return new Vector2(left.X - right.X, left.Y - right.Y); }
        public static Vector2 operator*(Vector2 left, Vector2 right) { return new Vector2(left.X * right.X, left.Y * right.Y); }
        public static Vector2 operator/(Vector2 left, Vector2 right) { return new Vector2(left.X / right.X, left.Y / right.Y); }

        public static Vector2 operator +(Vector2 left, float right) { return new Vector2(left.X + right, left.Y + right); }
        public static Vector2 operator -(Vector2 left, float right) { return new Vector2(left.X - right, left.Y - right); }
        public static Vector2 operator *(Vector2 left, float right) { return new Vector2(left.X * right, left.Y * right); }
        public static Vector2 operator /(Vector2 left, float right) { return new Vector2(left.X / right, left.Y / right); }
    }
}
