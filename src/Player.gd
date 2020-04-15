extends KinematicBody2D

export var VELOCITY = 75
export var ACCELERATION = 75*6
export var JUMP_POWER = 162.5
export var DAMPING_DIVIDER = 0.2
export var MIN_VELOCITY = 10
export var AIR_DAMPING_MULTIPLIER = 2
export var AIR_ACCELERATION_MULTIPLIER = 0.8

export var VELOCITY_ANIMATION_THRESHOLD = 0.1

onready var sprite = get_node("Sprite")
onready var anim = get_node("Sprite/AnimationPlayer")
onready var hitbox = get_node("Hitbox")

var velocity = Vector2(0,0)

func _ready():
    hitbox.connect("body_entered",self,"on_hitbox_entered")

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

func _process(_delta):
    if abs(velocity.x) > VELOCITY_ANIMATION_THRESHOLD:
        if velocity.x < 0:
            sprite.flip_h = true
        else:
            sprite.flip_h = false
    if is_on_floor():
        if abs(velocity.x) > VELOCITY_ANIMATION_THRESHOLD:
            anim.play("Run")
        else:
            anim.play("Stand")
    else:
        if velocity.y < 0:
            anim.play("JumpUp")
        else:
            anim.play("JumpDown")

func on_hitbox_entered(body):
    print(body)