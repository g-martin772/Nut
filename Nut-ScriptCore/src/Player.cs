using Nut.Input;
using Nut.Math;
using Nut.Scene;
using Nut.Core;
using System;

namespace Sandbox
{
    public class Player : Entity
    {
        float speed;
        TransformComponent tc;
        SpriteRendererComponent src;
        RigidBody2DComponent rb;

        public override void OnCreate()
        {
            Logger.Trace($"OnPlayerCreate");

            speed = 0.35f;
            tc = GetComponent<TransformComponent>();
            src = GetComponent<SpriteRendererComponent>();
            rb = GetComponent<RigidBody2DComponent>();
        }

        public override void OnUpdate(float ts)
        {
            //Native_Print($"OnPlayerUpdate: {ts}");

            Vector3 pos = tc.Translation;
            Vector3 rot = tc.Rotation;
            Vector3 size = tc.Scale;

            Vector2 velocity = Vector2.Zero;

            if (Input.IsKeyDown(KeyCode.KEY_W))
            {
                velocity.Y += speed;
            }
            else if (Input.IsKeyDown(KeyCode.KEY_S))
            {
                velocity.Y -= speed;
            }

            if (Input.IsKeyDown(KeyCode.KEY_A))
            {
                velocity.X -= speed;
            }
            else if (Input.IsKeyDown(KeyCode.KEY_D))
            {
                velocity.X += speed;
            }

            if (Input.IsKeyDown(KeyCode.KEY_Q))
            {
                rot.Z += speed * ts;
            }
            else if (Input.IsKeyDown(KeyCode.KEY_E))
            {
                rot.Z -= speed * ts;
            }

            if (Input.IsKeyDown(KeyCode.KEY_R))
            {
                size.X += speed * ts;
            }
            else if (Input.IsKeyDown(KeyCode.KEY_F))
            {
                size.X -= speed * ts;
            }

            tc.Scale = size;
            tc.Translation = pos;
            tc.Rotation = rot;

            velocity *= speed;

            rb.ApplyLinearImpulse(velocity);
        }
    }
}
