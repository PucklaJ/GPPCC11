extends Node2D

onready var pickup_scene: PackedScene = preload("res://objects/PickUp.tscn")

func _ready():
	pass


func on_pickup_timer():
	add_child(pickup_scene.instance())
