namespace Nut.Input
{
    public static class Input
    {
        public static bool IsKeyDown(Key keyCode) { return InternalCalls.Native_IsKeyDown(keyCode); }
    }
}
