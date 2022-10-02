extends Node2D

export var OBSTACLE_DAMAGE: float = 1.0
export var OBSTACLE_SPEED: float = 10.0
export var OBSTACLE_MIN_X: float = 60.0
export var OBSTACLE_MAX_X: float = 300.0

func _ready():
	match randi() % 3:
		0:
			$Obstacle2.visible = false
			$Obstacle3.visible = false
		1:
			$Obstacle1.visible = false
			$Obstacle3.visible = false
		2:
			$Obstacle1.visible = false
			$Obstacle2.visible = false

	position.y = Globals.GAME_HEIGHT + $Obstacle1.texture.get_height() / 2.0
	position.x = rand_range(OBSTACLE_MIN_X, OBSTACLE_MAX_X)
	
func _process(delta: float):
	position.y -= OBSTACLE_SPEED * delta

func on_hitbox_entered(body: Node):
	if body.can_be_damaged():
		body.damage(OBSTACLE_DAMAGE)
		body.knockback(global_position)
