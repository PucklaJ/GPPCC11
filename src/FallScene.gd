extends Node2D

const pickup_scene: PackedScene = preload("res://objects/PickUp.tscn")
const obstacle_scene: PackedScene = preload("res://objects/Obstacle.tscn")
const end_message_scene: PackedScene = preload("res://objects/EndMessage.tscn")

onready var player = get_node("Player")
onready var odin = get_node("Odin")

var game_is_over: bool = false

func _ready():
	pass
	
func _process(_dt: float):
	if player.state == player.states.dead and not game_is_over:
		game_is_over = true
		var end_message = end_message_scene.instance()
		end_message.set_game_over()
		add_child(end_message)

func on_pickup_timer():
	add_child(pickup_scene.instance())

func on_obstacle_timer():
	add_child(obstacle_scene.instance())
