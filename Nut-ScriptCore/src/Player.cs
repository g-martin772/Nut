using Nut.Input;
using Nut.Math;
using Nut.Scene;
using static Nut.NativeCalls;

namespace Sandbox
{
    public class Player : Entity
    {
        float speed;

        public override void OnCreate()
        {
            speed = 10.0f;
            Native_Print($"OnPlayerCreate");
        }

        public override void OnUpdate(float ts)
        {
            //Native_Print($"OnPlayerUpdate: {ts}");

            Vector3 pos = Translation;
            
            if(Input.IsKeyDown(KeyCode.KEY_W))
            {
                pos.Y += speed * ts;
            }
            else if (Input.IsKeyDown(KeyCode.KEY_S))
            {
                pos.Y -= speed * ts;
            }

            if (Input.IsKeyDown(KeyCode.KEY_A))
            {
                pos.X -= speed * ts;
            }
            else if (Input.IsKeyDown(KeyCode.KEY_D))
            {
                pos.X += speed * ts;
            }

            Translation = pos;
        }
    }
}
