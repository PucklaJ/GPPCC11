extends KinematicBody2D

export var VELOCITY = 75
export var ACCELERATION = 75*6
export var JUMP_POWER = 162.5
export var DAMPING_DIVIDER = 0.2
export var MIN_VELOCITY = 10
export var AIR_DAMPING_MULTIPLIER = 2
export var AIR_ACCELERATION_MULTIPLIER = 0.8
export var KNOCKBACK_POWER = 100
export var KNOCKBACK_TIME = 1.0
export var ODIN_TOUCH_DAMAGE = 1.0
export var SWORD_DAMAGE = 2.0
export var SWORD_ATTACK_TIME = 0.30

export var VELOCITY_ANIMATION_THRESHOLD = 0.1

enum states {
    move,
    sword_attack,
    bow_attack,
    knockback,
    bow_prepare,
    intro,
}

onready var sprite = get_node("Sprite")
onready var anim = get_node("Sprite/AnimationPlayer")
onready var hitbox = get_node("Hitbox")
onready var health_bar = get_node("Sprite/Lock/HealthBar")
onready var arrow_bar = get_node("Sprite/Lock/ArrowBar")
onready var sword_hitbox = get_node("Sprite/SwordHitbox")

var velocity = Vector2(0,0)
var state = states.move
var enable_movement = true
var health = 0.0

func _ready():
    hitbox.connect("body_entered",self,"on_hitbox_entered")
    sword_hitbox.connect("body_entered",self,"on_sword_hitbox_entered")
    health = health_bar.MAX_VALUE

func ground_movement(delta):
    var acceleration = ACCELERATION*delta
    if not is_on_floor():
        acceleration *= AIR_ACCELERATION_MULTIPLIER
    if enable_movement and Input.is_action_pressed("move_right"):
        velocity.x = min(velocity.x + acceleration,VELOCITY)
    elif enable_movement and Input.is_action_pressed("move_left"):
        velocity.x = max(velocity.x - acceleration,-VELOCITY)
    else:
        if state != states.knockback or is_on_floor():
            var damping = -sign(velocity.x) * VELOCITY/(DAMPING_DIVIDER)*delta
            if not is_on_floor():
                damping /= AIR_DAMPING_MULTIPLIER
            velocity.x += damping
            if abs(velocity.x) < MIN_VELOCITY:
                velocity.x = 0

    if is_on_floor():
        if enable_movement and Input.is_action_just_pressed("jump"):
            velocity.y -= JUMP_POWER
    velocity += Globals.GRAVITY * delta

var time_val = 0.0

func handle_states(delta):
    sprite.position = Vector2(0,0)
    sword_hitbox.position = Vector2(0,0)
    match state:
        states.move:
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
            if Input.is_action_just_pressed("sword"):
                state = states.sword_attack
                enable_movement = false
                time_val = 0.0
                if Globals.state == Globals.states.boss_ground:
                    anim.play("SwordGround")
                else:
                    anim.play("SwordFall")
        states.knockback:
            time_val += delta
            if time_val > KNOCKBACK_TIME:
                enable_movement = true
                state = states.move
        states.sword_attack:
            time_val += delta
            if sprite.flip_h:
                sprite.position = Vector2(-18,0)
                sword_hitbox.position = Vector2(-16,0)
            if time_val > SWORD_ATTACK_TIME:
                if Globals.state == Globals.states.boss_ground:
                    anim.play("Stand")
                else:
                    anim.play("FallDown")
                state = states.move
                enable_movement = true

func _physics_process(delta):
    ground_movement(delta)
    velocity = move_and_slide(velocity,Vector2(0,-1))

func _process(delta):
    handle_states(delta)
    
func damage(amount):
    health = max(health - amount,0.0)
    health_bar.set_value(health)
    
func knockback(pos,multiplier=1.0):
    if state != states.knockback:
        if Globals.state == Globals.states.boss_ground:
            anim.play("KnockbackGround")
        else:
            anim.play("KnockbackFall")
        velocity = (position - pos).normalized() * KNOCKBACK_POWER * multiplier
        enable_movement = false
        sword_hitbox.monitoring = false
        time_val = 0.0
    state = states.knockback

func on_hitbox_entered(body):
    if body.can_inflict_damage() and can_be_damaged():
        knockback(body.position + Vector2(0,60))
        damage(ODIN_TOUCH_DAMAGE)

func on_sword_hitbox_entered(body):
    if body.can_be_damaged():
        body.knockback()
        body.damage(SWORD_DAMAGE)

func can_be_damaged():
    match state:
        states.knockback: return false
    return true
