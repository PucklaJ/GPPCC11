extends Sprite

const Backgrounds : Array = [
	preload("res://textures/Background1.png"),
	preload("res://textures/Background2.png"),
	preload("res://textures/Background3.png")
]

func _ready():
	randomize()
	texture = Backgrounds[randi() % Backgrounds.size()]
