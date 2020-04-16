extends Node2D

onready var odin = get_node("Odin")
onready var player = get_node("Player")

func _ready():
    odin.player = player