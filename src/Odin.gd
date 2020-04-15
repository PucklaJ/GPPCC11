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

onready var sprite = get_node("Sprite")
onready var anim = sprite.get_node("AnimationPlayer")

var width = 50
var height = 75

var velocity = Vector2(0,0)
var state = states.walk

func ground_movement(delta):
    velocity += Globals.GRAVITY * delta
    velocity = move_and_slide(velocity,Vector2(0,-1))

var time_val = 0.0

func handle_states(delta):
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
        states.wait_for_walk:
            anim.play("Stand")
            velocity.x = 0.0
            if time_val > WAIT_FOR_WALK_TIME:
                sprite.flip_h = not sprite.flip_h
                state = states.walk
            time_val += delta

func _physics_process(delta):
    ground_movement(delta)

func _process(delta):
    handle_states(delta)
