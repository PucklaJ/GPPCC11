extends Sprite

export var ARROW_VELOCITY = 100
export var ARROW_DAMAGE = 1.0

onready var hitbox = get_node("Hitbox")

func _ready():
    hitbox.connect("body_entered",self,"on_hitbox_entered")

func _process(delta):
    if flip_h:
        position.x += ARROW_VELOCITY * delta
    else:
        position.x -= ARROW_VELOCITY * delta
    if position.x + texture.get_width() < 0.0 or position.x > Globals.GAME_WIDTH:
        queue_free()

func on_hitbox_entered(body):
    if body.can_be_damaged():
        body.knockback()
        body.damage(ARROW_DAMAGE)
        queue_free()