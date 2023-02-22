namespace Nut.Input
{
    public static class Input
    {
        public static bool IsKeyDown(KeyCode keyCode) { return NativeCalls.Native_IsKeyDown(keyCode); }
    }
}
