extends Node2D

const FallScene : PackedScene = preload("res://scenes/FallScene.tscn")

onready var odin = get_node("Odin")
onready var player = get_node("Player")
onready var pickup_timer = get_node("PickUpTimer")
onready var pickup_scene = preload("res://objects/PickUp.tscn")

var odin_exited = false
var player_exited = false

func _ready():
	odin.player = player
	odin.position = Vector2(220, 85)
	pickup_timer.connect("timeout",self,"on_pickup_timer_timeout")
	
func _process(_dt: float):
	if player_exited and odin_exited:
		# Transitation to FallScene
		var cam = $Cam
		remove_child(player)
		remove_child(odin)
		remove_child(cam)
		
		var fall_scene : Node2D = FallScene.instance()
		fall_scene.add_child(player)
		fall_scene.add_child(odin)
		fall_scene.add_child(cam)
		
		player.position.y = -100
		player.state = player.states.fall_intro
		player.get_node("Collision").set_deferred("disabled", true)
		odin.position.x = 250
		odin.position.y = -100
		odin.state = odin.states.fall
		Globals.state = Globals.states.boss_fall
		
		queue_free()
		get_parent().add_child(fall_scene)

func on_pickup_timer_timeout():
	add_child(pickup_scene.instance())
	
func on_odin_exited():
	odin_exited = true
	
func on_player_exited():
	player_exited = true
