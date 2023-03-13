using Nut.Input;
using Nut.Math;
using Nut.Scene;
using Nut.Core;

namespace Sandbox
{
    public class Camera : Entity
    {
        public float DistanceFromPlayer = 5.0f;

        private TransformComponent m_Transform;
        private Entity m_Player;

        public override void OnCreate()
        {
            m_Transform = GetComponent<TransformComponent>();
        }

        public override void OnUpdate(float ts)
        {
            if (m_Player is null) 
                m_Player = FindEntityByName("Player1");

            m_Transform.Translation = new Vector3(m_Player.GetComponent<TransformComponent>().Translation.XY, DistanceFromPlayer);

            //float speed = 1.0f;
            //Vector3 velocity = Vector3.Zero;

            //if (Input.IsKeyDown(Key.W))
            //    velocity.Y = 1.0f;
            //else if (Input.IsKeyDown(Key.S))
            //    velocity.Y = -1.0f;
            //if (Input.IsKeyDown(Key.A))
            //    velocity.X = -1.0f;
            //else if (Input.IsKeyDown(Key.D))
            //    velocity.X = 1.0f;
            //velocity *= speed;
            //Vector3 translation = m_Transform.Translation;
            //translation += velocity * ts;
            //m_Transform.Translation = translation;
        }
    }

}
