extends CanvasLayer

export var gameover_text: String = "Game Over"

onready var again_text: String = $EndMessage/Container/Again.text

func on_blink():
	var again = $EndMessage/Container/Again
	if again.text.empty():
		again.text = again_text
	else:
		again.text = ""

func set_game_over():
	$EndMessage/Container/Message.text = gameover_text
