extends Sprite

export var rotation_speed : float = 20.0
export var move_speed : float = 100.0

onready var move_direction: Vector2 = Vector2(rand_range(-1.0, 1.0), rand_range(-1.0, 1.0)).normalized()

func _ready():
	$visibility.rect.size = get_rect().size
	
func _process(dt: float):
	rotate(deg2rad(rotation_speed * dt))
	translate(move_direction * move_speed * dt)

func on_screen_exited():
	queue_free()
