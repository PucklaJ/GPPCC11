extends Sprite

export var SPEER_VELOCITY = 100
export var SPEER_DAMAGE = 1.0

onready var hitbox = get_node("Hitbox")

var direction = Vector2(1,0)

func _ready():
	hitbox.connect("body_entered",self,"on_hitbox_entered")

func _enter_tree():
	rotation = direction.angle()

func _process(delta):
	position += direction * SPEER_VELOCITY * delta

	if position.x < -100 or position.y < -100 or position.x > Globals.GAME_WIDTH + 100 or position.y > Globals.GAME_HEIGHT + 100:
		queue_free()

func on_hitbox_entered(body):
	if body.can_be_damaged():
		queue_free()
		body.knockback(position)
		body.damage(SPEER_DAMAGE)


