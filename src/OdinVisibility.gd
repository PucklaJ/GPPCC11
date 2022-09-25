extends VisibilityNotifier2D

onready var sprite : Sprite = $"../Sprite"

func _process(_dt: float):
	rect = sprite.get_rect()
