extends Node2D

export var SCROLL_SPEED = 10

var scrolls = []
var sprites = []
var width
var height

var scroll_active = true

func _ready():
    var scroll_container = get_child(1)
    var sprite_container = get_child(0)

    for child in sprite_container.get_children():
        sprites.append(child)
    for i in range(scroll_container.get_child_count()):
        scrolls.append(scroll_container.get_child(scroll_container.get_child_count()-i-1))
    width = scrolls[0].texture.get_width()
    height = scrolls[0].texture.get_height()

    for s in scrolls:
        randomize_texture(s)

func _process(delta):
    if scroll_active:
        for s in scrolls:
            s.position.y -= SCROLL_SPEED*delta

    if scrolls[-1].position.y + height < 0:
        scrolls[-1].position.y = scrolls[0].position.y + height
        randomize_texture(scrolls[-1])
        scrolls.resize(len(scrolls)+1)
        for i in range(1,len(scrolls)):
            scrolls[-i] = scrolls[-i-1]
        scrolls[0] = scrolls[-1]
        scrolls.pop_back()

func randomize_texture(scroll):
    scroll.texture =  sprites[randi() % len(sprites)].texture

func set_scroll_active(b):
    scroll_active = b

func set_scroll_speed(val):
    SCROLL_SPEED = val


