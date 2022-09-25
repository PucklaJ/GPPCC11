extends Node2D

onready var odin = get_node("Odin")
onready var player = get_node("Player")
onready var pickup_timer = get_node("PickUpTimer")
onready var pickup_scene = preload("res://objects/PickUp.tscn")

func _ready():
	odin.player = player
	odin.position = Vector2(220, 85)
	pickup_timer.connect("timeout",self,"on_pickup_timer_timeout")

func on_pickup_timer_timeout():
	add_child(pickup_scene.instance())
