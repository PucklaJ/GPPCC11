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

onready var sprite = get_node("Sprite")
onready var anim = sprite.get_node("AnimationPlayer")

var width = 50
var height = 75

var velocity = Vector2(0,0)
var state = states.walk

func ground_movement(delta):
    velocity += Globals.GRAVITY * delta
    velocity = move_and_slide(velocity,Vector2(0,-1))

func handle_states():
    match state:
        states.walk:
            anim.play("Run")
            if not sprite.flip_h:
                velocity.x = -GROUND_VELOCITY
                if position.x + width / 2.0 < GROUND_WALK_POINT:
                    sprite.flip_h = true
            else:
                velocity.x = GROUND_VELOCITY
                if position.x + width / 2.0 > Globals.GAME_WIDTH - GROUND_WALK_POINT:
                    sprite.flip_h = false

func _physics_process(delta):
    ground_movement(delta)

func _process(_delta):
    handle_states()
