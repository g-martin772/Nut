using Nut.Input;
using Nut.Math;
using Nut.Scene;
using static Nut.InternalCalls;

namespace Sandbox
{
    public class Player : Entity
    {
        float speed;
        TransformComponent tc;

        public override void OnCreate()
        {
            Native_Print($"OnPlayerCreate");

            speed = 10.0f;
            tc = GetComponent<TransformComponent>();
        }

        public override void OnUpdate(float ts)
        {
            //Native_Print($"OnPlayerUpdate: {ts}");

            Vector3 pos = tc.Translation;
            
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

            tc.Translation = pos;
        }
    }
}
