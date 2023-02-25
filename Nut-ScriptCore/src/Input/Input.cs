namespace Nut.Input
{
    public static class Input
    {
        public static bool IsKeyDown(KeyCode keyCode) { return InternalCalls.Native_IsKeyDown(keyCode); }
    }
}
