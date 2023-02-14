Scene: Untitled Scene
Entitys:
  - Entity: 8189606469567907201
    TagComponent:
      Tag: Empty Entity
    TransformComponent:
      Translation: [1.74488676, -2.33978677, 0]
      Rotation: [0, 0, 0.747760177]
      Scalation: [9.06881809, 0.999999881, 1]
    SpriteRendererComponent:
      Color: [0, 1, 0.154440165, 1]
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
  - Entity: 2521038104214275109
    TagComponent:
      Tag: Empty Entity
    TransformComponent:
      Translation: [-0.549448967, -0.270960778, 0]
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
      Friction: 0.129999995
      Density: 1
      Restitution: 0.670000017
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
  - Entity: 3341696091676390481
    TagComponent:
      Tag: Square
    TransformComponent:
      Translation: [-0.818814874, -1.66445923, 0]
      Rotation: [0, 0, -0.584609807]
      Scalation: [6.75711298, 0.523697376, 1]
    SpriteRendererComponent:
      Color: [0, 1, 0, 1]
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
  - Entity: 1018872447240047970
    TagComponent:
      Tag: Circle
    TransformComponent:
      Translation: [-0.677401483, 2.06231833, 0]
      Rotation: [0, 0, 0]
      Scalation: [1, 1, 1]
    CircleRendererComponent:
      Color: [1, 1, 1, 1]
      Fade: 0.00499999989
      Radius: 0.5
      Thickness: 1
    RigidBody2DComponent:
      Type: Dynamic
      FixedRotation: false
    CircleCollider2DComponent:
      Offset: [0, 0]
      Radius: 0.5
      Density: 1
      Friction: 0.389999986
      Restitution: 0.49000001
      RestitutionThreshold: 0.5