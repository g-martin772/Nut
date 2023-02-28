using Nut.Core;
using Nut.Input;
using Nut.Math;
using Nut.Scene;

namespace Sandbox
{
    public class Player1 : Entity
    {
        public float speed;
        public RigidBody2DComponent rb;

        public override void OnCreate()
        {
            speed = 10;
            rb = GetComponent<RigidBody2DComponent>();
        }

        public override void OnUpdate(float ts)
        {
            Vector2 velocity = Vector2.Zero;

            if (Input.IsKeyDown(Key.W))
                velocity.Y += speed;
            else if (Input.IsKeyDown(Key.S))
                velocity.Y -= speed;

            if (Input.IsKeyDown(Key.A))
                velocity.X -= speed;
            else if (Input.IsKeyDown(Key.D))
                velocity.X += speed;

            velocity *= ts;
            rb.ApplyLinearImpulse(velocity);
        }
    }

    public class Player2 : Entity
    {
        public float speed;
        public RigidBody2DComponent rb;

        public override void OnCreate()
        {
            speed = 10;
            rb = GetComponent<RigidBody2DComponent>();
        }

        public override void OnUpdate(float ts)
        {
            Vector2 velocity = Vector2.Zero;

            if (Input.IsKeyDown(Key.I))
                velocity.Y += speed;
            else if (Input.IsKeyDown(Key.K))
                velocity.Y -= speed;

            if (Input.IsKeyDown(Key.J))
                velocity.X -= speed;
            else if (Input.IsKeyDown(Key.L))
                velocity.X += speed;

            velocity *= ts;
            rb.ApplyLinearImpulse(velocity);
        }
    }
}
