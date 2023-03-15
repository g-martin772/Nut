Scene: Untitled Scene
Entitys:
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
  - Entity: 1234567890
    TagComponent:
      Tag: Square
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scalation: [1, 1, 1]
    SpriteRendererComponent:
      Color: [0, 1, 0, 1]
      Texture: assets\textures\ChernoLogo.png
      TilingFactor: 1