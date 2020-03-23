extends KinematicBody2D

export var VELOCITY = 75
export var ACCELERATION = 75*6
export var JUMP_POWER = 162.5
export var DAMPING_DIVIDER = 0.2
export var MIN_VELOCITY = 10
export var AIR_DAMPING_MULTIPLIER = 2
export var AIR_ACCELERATION_MULTIPLIER = 0.8

onready var sprite = get_node("Sprite")

var velocity = Vector2(0,0)

func ground_movement(delta):
    var acceleration = ACCELERATION*delta
    if not is_on_floor():
        acceleration *= AIR_ACCELERATION_MULTIPLIER
    if Input.is_action_pressed("move_right"):
        velocity.x = min(velocity.x + acceleration,VELOCITY)
    elif Input.is_action_pressed("move_left"):
        velocity.x = max(velocity.x - acceleration,-VELOCITY)
    else:
        var damping = -sign(velocity.x) * VELOCITY/(DAMPING_DIVIDER)*delta
        if not is_on_floor():
            damping /= AIR_DAMPING_MULTIPLIER
        velocity.x += damping
        if abs(velocity.x) < MIN_VELOCITY:
            velocity.x = 0

    if is_on_floor():
        if Input.is_action_just_pressed("jump"):
            velocity.y -= JUMP_POWER
    velocity += Globals.GRAVITY * delta

func _physics_process(delta):
    ground_movement(delta)
    velocity = move_and_slide(velocity,Vector2(0,-1))
    