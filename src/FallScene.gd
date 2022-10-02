extends Node2D

const pickup_scene: PackedScene = preload("res://objects/PickUp.tscn")
const obstacle_scene: PackedScene = preload("res://objects/Obstacle.tscn")

func _ready():
	pass

func on_pickup_timer():
	add_child(pickup_scene.instance())

func on_obstacle_timer():
	add_child(obstacle_scene.instance())
