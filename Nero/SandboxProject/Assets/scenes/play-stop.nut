Scene: Untitled Scene
Entitys:
  - Entity: 3341696091676390481
    TagComponent:
      Tag: Square
    TransformComponent:
      Translation: [0.391386747, -1.75927746, 0]
      Rotation: [0, 0, 0]
      Scalation: [6.75711203, 0.523697615, 1]
    SpriteRendererComponent:
      Color: [0, 1, 0, 1]
      Texture: none
      TilingFactor: 1
    RigidBody2DComponent:
      Type: Static
      FixedRotation: false
    BoxCollider2DComponent:
      Size: [1, 1]
      Offset: [0, 0]
      Friction: 0
      Density: 1
      Restitution: 0
      RestitutionThreshold: 0.5
  - Entity: 6258611999961939162
    TagComponent:
      Tag: Camera
    TransformComponent:
      Translation: [0, -0.300973892, 8.16994667]
      Rotation: [0, 0, 0]
      Scalation: [1, 1, 1]
    CameraComponent:
      Camera:
        ProjectionType: 0
        PerspectiveFOV: 0.785398185
        PerspectiveNear: 0.00100000005
        PerspectiveFar: 1000
        OrthographicSize: 10
        OrthographicNear: -1
        OrthographicFar: 1
      Primary: true
      FixedAspectRatio: false
  - Entity: 2521038104214275109
    TagComponent:
      Tag: Empty Entity
    TransformComponent:
      Translation: [0.511814654, 1.15911329, 0]
      Rotation: [0, 0, -0.864951253]
      Scalation: [0.999999881, 0.999999881, 1]
    SpriteRendererComponent:
      Color: [1, 1, 1, 1]
      Texture: none
      TilingFactor: 1
    RigidBody2DComponent:
      Type: Dynamic
      FixedRotation: false
    BoxCollider2DComponent:
      Size: [0.5, 0.5]
      Offset: [0, 0]
      Friction: 0
      Density: 1
      Restitution: 0
      RestitutionThreshold: 0.5