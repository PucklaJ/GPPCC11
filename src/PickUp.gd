extends Node2D

export var PICK_UP_GROUND_VELOCITY: float = 100
export var PICK_UP_FALL_VELOCITY: float = 100
export var PICK_UP_TIME: float = 5.0
export var PICK_UP_BLINK_TIME: float = 2.5
export var PICK_UP_BLINK_INTERVAL: float = 0.5
export var PICK_UP_GROUND_MIN_X: float = 0.0
export var PICK_UP_GROUND_MAX_X: float = 320.0
export var PICK_UP_FALL_MIN_X: float = 0.0
export var PICK_UP_FALL_MAX_X: float = 280.0

onready var arrow: Sprite = get_node("Arrow")
onready var health: Sprite = get_node("Health")
onready var hitbox: Area2D = get_node("Hitbox")

const GROUND_HEIGHT: int = 20

var time_val: float = 0.0
var time_val1: float = 0.0
var on_ground: bool = false
var sprite: Sprite = null

func _ready():
	if randf() < 0.5:
		health.visible = false
		sprite = arrow
	else:
		arrow.visible = false
		sprite = health
	hitbox.connect("body_entered", self, "on_hitbox_entered")

	position.y = -sprite.texture.get_height() / 2.0
	var min_pos_x = PICK_UP_GROUND_MIN_X if Globals.state == Globals.states.boss_ground else PICK_UP_FALL_MIN_X
	var max_pos_x = PICK_UP_GROUND_MAX_X if Globals.state == Globals.states.boss_ground else PICK_UP_FALL_MAX_X
	position.x = clamp(rand_range(min_pos_x, max_pos_x), sprite.texture.get_width() / 2.0, Globals.GAME_WIDTH - sprite.texture.get_width() / 2.0)

func _process(delta: float):
	if Globals.state == Globals.states.boss_ground:
		position.y += PICK_UP_GROUND_VELOCITY * delta
		if position.y + sprite.texture.get_height() / 2.0 > Globals.GAME_HEIGHT - GROUND_HEIGHT:
			position.y = Globals.GAME_HEIGHT - GROUND_HEIGHT - sprite.texture.get_height() / 2.0
			on_ground = true

		if on_ground:
			time_val += delta
			if time_val > PICK_UP_BLINK_TIME:
				time_val1 += delta
				if time_val1 > PICK_UP_BLINK_INTERVAL:
					sprite.visible = not sprite.visible
					time_val1 = 0.0
			if time_val > PICK_UP_TIME:
				queue_free()
	elif Globals.state == Globals.states.boss_fall:
		position.y += PICK_UP_FALL_VELOCITY * delta

func on_hitbox_entered(body):
	if sprite == arrow:
		body.collect_arrow(2.0)
	else:
		body.damage(-2.0)
	queue_free()

func on_screen_exited():
	if Globals.state == Globals.states.boss_fall:
		queue_free()
