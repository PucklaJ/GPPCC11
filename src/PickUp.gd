extends Node2D

export var PICK_UP_VELOCITY = 100
export var PICK_UP_TIME = 5.0
export var PICK_UP_BLINK_TIME = 2.5
export var PICK_UP_BLINK_INTERVAL = 0.5

onready var arrow = get_node("Arrow")
onready var health = get_node("Health")
onready var hitbox = get_node("Hitbox")

const GROUND_HEIGHT = 20

var time_val = 0.0
var time_val1 = 0.0
var on_ground = false
var sprite = null

func _ready():
    if randf() < 0.5:
        health.visible = false
        sprite = arrow
    else:
        arrow.visible = false
        sprite = health
    hitbox.connect("body_entered",self,"on_hitbox_entered")

    position.y = -sprite.texture.get_height()
    position.x = rand_range(0,Globals.GAME_WIDTH-sprite.texture.get_width())


func _process(delta):
    position += Vector2(0,PICK_UP_VELOCITY) * delta
    if position.y + sprite.texture.get_height() > Globals.GAME_HEIGHT - GROUND_HEIGHT:
        position.y = Globals.GAME_HEIGHT - GROUND_HEIGHT - sprite.texture.get_height()
        on_ground = true

    if on_ground:
        time_val += delta
        if time_val > PICK_UP_BLINK_TIME:
            time_val1 += delta
            if time_val1 > PICK_UP_BLINK_INTERVAL:
                sprite.visible = not sprite.visible
                time_val1 = 0.0
        if time_val > PICK_UP_TIME:
            queue_free()


func on_hitbox_entered(body):
    if sprite == arrow:
        body.collect_arrow(2.0)
    else:
        body.damage(-2.0)
    queue_free()

