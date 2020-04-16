extends Node

enum states {
    menu,
    boss_ground,
    boss_fall,
    boss_destroy_ground,
    ground_fall_transition,
    ground_intro,
    game_over,
    intro,
    ground_win,
    fall_win_transition,
    ground_win_intro,
    win,
}

var GAME_WIDTH = 320
var GAME_HEIGHT = 180
var GRAVITY = Vector2(0,125)

var state = states.boss_ground

func _input(event):
    if event is InputEventKey:
        if event.pressed and event.scancode == KEY_H:
            if Engine.time_scale == 1.0:
                Engine.time_scale = 0.05
            else:
                Engine.time_scale = 1.0