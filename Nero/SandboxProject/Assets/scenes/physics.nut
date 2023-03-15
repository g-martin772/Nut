Scene: Untitled Scene
Entitys:
  - Entity: 1234567890
    TagComponent:
      Tag: Empty Entity
    TransformComponent:
      Translation: [-0.0752663612, -2.25801229, 0]
      Rotation: [0, 0, 0]
      Scalation: [7.86070061, 0.342105359, 1]
    SpriteRendererComponent:
      Color: [1, 1, 1, 1]
      Texture: none
      TilingFactor: 1
    RigidBody2DComponent:
      Type: Static
      FixedRotation: false
    BoxCollider2DComponent:
      Size: [0.5, 0.5]
      Offset: [0, 0]
      Friction: 0
      Density: 1
      Restitution: 0
      RestitutionThreshold: 0.5
  - Entity: 1234567890
    TagComponent:
      Tag: Square
    TransformComponent:
      Translation: [-0.139892161, 1.25640047, 0]
      Rotation: [0, 0, 1.09110808]
      Scalation: [0.999989152, 0.999989152, 1]
    SpriteRendererComponent:
      Color: [0, 1, 0, 1]
      Texture: none
      TilingFactor: 1
    RigidBody2DComponent:
      Type: Dynamic
      FixedRotation: false
    BoxCollider2DComponent:
      Size: [0.5, 0.5]
      Offset: [0, 0]
      Friction: 0.25
      Density: 1
      Restitution: 1
      RestitutionThreshold: 0.75
  - Entity: 1234567890
    TagComponent:
      Tag: Camera
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scalation: [1, 1, 1]
    CameraComponent:
      Camera:
        ProjectionType: 1
        PerspectiveFOV: 0.785398185
        PerspectiveNear: 0.00100000005
        PerspectiveFar: 1000
        OrthographicSize: 10
        OrthographicNear: -1
        OrthographicFar: 1
      Primary: true
      FixedAspectRatio: false