extends Node2D

const FallScene : PackedScene = preload("res://scenes/FallScene.tscn")
const EndMessage: PackedScene = preload("res://objects/EndMessage.tscn")

onready var odin = get_node("Odin")
onready var player = get_node("Player")
onready var pickup_timer = get_node("PickUpTimer")
onready var pickup_scene = preload("res://objects/PickUp.tscn")

var odin_exited = false
var player_exited = false
var game_is_over = false

func _ready():
	odin.player = player
	odin.position = Vector2(220, 85)
	pickup_timer.connect("timeout",self,"on_pickup_timer_timeout")
	
func _process(_dt: float):
	if player_exited and odin_exited:
		# Transitation to FallScene
		remove_child(player)
		remove_child(odin)
		
		var fall_scene : Node2D = FallScene.instance()
		fall_scene.add_child(player)
		fall_scene.add_child(odin)
		fall_scene.get_node("Tree").scroll_active = true
		
		player.position.y = -100
		player.state = player.states.fall_intro
		player.get_node("Collision").set_deferred("disabled", true)
		odin.position.x = 250
		odin.position.y = -100
		odin.state = odin.states.fall_intro
		Globals.state = Globals.states.boss_fall
		
		queue_free()
		get_parent().add_child(fall_scene)
	if odin.state == odin.states.dead and not game_is_over:
		game_is_over = true
		var end_message = EndMessage.instance()
		add_child(end_message)
	elif player.state == player.states.dead and not game_is_over:
		game_is_over = true
		var end_message = EndMessage.instance()
		end_message.set_game_over()
		add_child(end_message)

func on_pickup_timer_timeout():
	add_child(pickup_scene.instance())
	
func on_odin_exited():
	odin_exited = true
	
func on_player_exited():
	player_exited = true
