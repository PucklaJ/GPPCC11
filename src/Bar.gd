extends Node2D

var BLUE_OFFSET = 13/2-1

export var MAX_VALUE = 20.0
export var SPRITE_NAME = "RedHealth"
export var ALIGNMENT = 1

onready var red_health = get_node("Sprites/RedHealth")
onready var blue_health = get_node("Sprites/BlueHealth")
onready var red_half_health = get_node("Sprites/RedHalfHealth")
onready var blue_half_health = get_node("Sprites/BlueHalfHealth")
onready var arrow = get_node("Sprites/Arrow")

var value = MAX_VALUE

func update_sprite():
    for i in range(1,11):
        var sprite_index = i
        if ALIGNMENT == 2:
            sprite_index = 10 - i + 1
        var sprite = get_node("Bar/Bar"+str(sprite_index))
        sprite.visible = value / 2.0 > i - 1
        var is_half = sprite.visible and value / 2.0 < i
        sprite.visible = is_half or sprite.visible
        if SPRITE_NAME == "RedHealth":
            if is_half:
                sprite.texture = red_half_health.texture
            else:
                sprite.texture = red_health.texture
        elif SPRITE_NAME == "BlueHealth":
            if is_half:
                sprite.texture = blue_half_health.texture
                sprite.offset = Vector2(BLUE_OFFSET,0.0)
            else:
                sprite.texture = blue_health.texture
        elif SPRITE_NAME == "Arrow":
            sprite.texture = arrow.texture

func _ready():
    value = MAX_VALUE
    update_sprite()

func set_value(val):
    value = val
    update_sprite()