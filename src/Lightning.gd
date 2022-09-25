extends Sprite

export var velocity : float = 100.0
export var end_position: float = 20.0
export var small_lightning_damage : float = 2.0
export var big_lightning_damage : float = 1.0

func _ready():
	$AnimationPlayer.play("Small")

func _process(dt: float):
	position.x -= velocity * dt
	if position.x < end_position:
		position.x = 160.0 / 2.0
		$AnimationPlayer.play("Big")
		$Hitbox/CollisionShape2D.shape.extents = Vector2(80.0, 45.0)
		$EndTimer.start()
		set_process(false)


func on_hitbox_entered(body: Node):
	if body.can_be_damaged():
		queue_free()
		body.knockback(position)
		if $AnimationPlayer.current_animation == "Small":
			body.damage(small_lightning_damage)
		else:
			body.damage(big_lightning_damage)
