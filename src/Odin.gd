extends KinematicBody2D

enum states{
    walk,
    wait_for_walk,
    wait_for_second_speer,
    wait_for_hit_on_ground,
    hit_on_ground,
    wait_for_walk_hit_on_ground,
    knockback,
    awake_from_knockback,
    knockback_explosion,
    destroy_ground_jump_up,
    destroy_ground_jump_down,
    destroy_ground,
    teleport_wait,
    teleport_attack,
    intro,
    fall_game_over,
    ground_game_over,
    wait_for_walk_game_over,
    wait_for_grab_new_speer,
    wait_for_lightning_throw
}

export var GROUND_VELOCITY = 50
export var GROUND_WALK_POINT = 30
export var WAIT_FOR_WALK_TIME = 2.0
export var KNOCKBACK_TIME = 2.0
export var KNOCKBACK_EXPLOSION_WAIT_TIME = 0.25
export var KNOCKBACK_EXPLOSION_TIME = 0.5
export var KNOCKBACK_EXPLOSION_RADIUS = 100.0
export var HIT_ON_GROUND_RADIUS = 100.0
export var WAIT_FOR_HIT_ON_GROUND_TIME = 1.0
export var HIT_ON_GROUND_TIME = 0.5
export var AXE_DAMAGE = 2.0

onready var sprite = get_node("Sprite")
onready var anim = sprite.get_node("AnimationPlayer")
onready var health_bar = sprite.get_node("Lock/HealthBar")
onready var axe_hitbox = sprite.get_node("AxeHitbox")

var width = 50
var height = 75

var velocity = Vector2(0,0)
var state = states.walk
var health = 0.0
var player = null

func _ready():
    health = health_bar.MAX_VALUE
    axe_hitbox.connect("body_entered",self,"on_axe_entered")

func ground_movement(delta):
    velocity += Globals.GRAVITY * delta
    velocity = move_and_slide(velocity,Vector2(0,-1))

var time_val = 0.0

func handle_states(delta):
    sprite.position = Vector2(0,0)
    axe_hitbox.position = Vector2(0,0)
    match state:
        states.walk:
            anim.play("Run")
            if not sprite.flip_h:
                velocity.x = -GROUND_VELOCITY
                if position.x + width / 2.0 < GROUND_WALK_POINT:
                    position.x = GROUND_WALK_POINT - width / 2.0
                    state = states.wait_for_walk
                    time_val = 0.0
            else:
                velocity.x = GROUND_VELOCITY
                if position.x + width / 2.0 > Globals.GAME_WIDTH - GROUND_WALK_POINT:
                    position.x = Globals.GAME_WIDTH - GROUND_WALK_POINT - width / 2.0
                    state = states.wait_for_walk
                    time_val = 0.0
            if player.position.distance_squared_to(position) < HIT_ON_GROUND_RADIUS * HIT_ON_GROUND_RADIUS:
                state = states.wait_for_hit_on_ground
                time_val = 0.0
                anim.play("HitWait1")
                velocity.x = 0.0
                if player.position.x > position.x:
                    sprite.flip_h = true
                else:
                    sprite.flip_h = false
        states.wait_for_walk:
            anim.play("Stand")
            velocity.x = 0.0
            if time_val > WAIT_FOR_WALK_TIME:
                sprite.flip_h = not sprite.flip_h
                state = states.walk
            time_val += delta
        states.knockback:
            time_val += delta
            if time_val > KNOCKBACK_TIME:
                if Globals.state == Globals.states.boss_ground:
                    anim.play("Stand")
                else:
                    anim.play("Fall")
                state = states.awake_from_knockback
                time_val = 0.0
        states.awake_from_knockback:
            time_val += delta
            if time_val > KNOCKBACK_EXPLOSION_WAIT_TIME:
                anim.play("KnockbackExplosion")
                state = states.knockback_explosion
                time_val = 0.0
                knockback_explosion()
        states.knockback_explosion:
            time_val += delta
            if time_val > KNOCKBACK_EXPLOSION_TIME:
                if Globals.state == Globals.states.boss_ground:
                    anim.play("Stand")
                else:
                    anim.play("Fall")
                state = states.walk
                time_val = 0.0
        states.wait_for_hit_on_ground:
            if not sprite.flip_h:
                sprite.position = Vector2(-50,0)
            else:
                axe_hitbox.position = Vector2(39,0)
            time_val += delta
            if time_val > WAIT_FOR_HIT_ON_GROUND_TIME:
                anim.play("Hit")
                time_val = 0.0
                state = states.hit_on_ground
        states.hit_on_ground:
            if not sprite.flip_h:
                sprite.position = Vector2(-50,0)
            else:
                axe_hitbox.position = Vector2(39,0)
            time_val += delta
            if time_val > HIT_ON_GROUND_TIME:
                time_val = 0.0
                state = states.walk
                if Globals.state == Globals.states.boss_ground:
                    anim.play("Stand")
                else:
                    anim.play("Fall")

func _physics_process(delta):
    ground_movement(delta)

func _process(delta):
    handle_states(delta)

func knockback():
    state = states.knockback
    velocity.x = 0.0
    anim.play("Knockback")
    time_val = 0.0
    axe_hitbox.monitoring = false

func damage(amount):
    health = max(health-amount,0.0)
    health_bar.set_value(health)

func knockback_explosion():
    if player.position.distance_squared_to(position) < KNOCKBACK_EXPLOSION_RADIUS*KNOCKBACK_EXPLOSION_RADIUS:
        player.knockback(position+Vector2(0,60),2.0)

func can_be_damaged():
    match state:
        states.knockback,states.awake_from_knockback,states.knockback_explosion: return false
    return true

func can_inflict_damage():
    return can_be_damaged()

func on_axe_entered(body):
    if body.can_be_damaged():
        body.knockback(axe_hitbox.get_child(0).global_position)
        body.damage(AXE_DAMAGE)
