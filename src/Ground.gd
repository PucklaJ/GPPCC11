extends StaticBody2D

const GroundExplodeSprite : PackedScene = preload("res://objects/GroundExplodeSprite.tscn")

export var explode_count_x : int = 10
export var explode_count_y : int  = 2

func explode():
	var texture_width = $Ground.texture.get_width()
	var texture_height = $Ground.texture.get_height()
	var explode_sprite_width = float(texture_width) / float(explode_count_x)
	var explode_sprite_height = float(texture_height) / float(explode_count_y)
	
	for x in range(explode_count_x):
		for y in range(explode_count_y):
			var pos_x = global_position.x + float(x) * explode_sprite_width
			var pos_y = global_position.y + float(y) * explode_sprite_height
			var explode_sprite : Sprite = GroundExplodeSprite.instance()
			explode_sprite.texture = $Ground.texture
			explode_sprite.region_rect.position.x = float(x) * explode_sprite_width
			explode_sprite.region_rect.position.y = float(y) * explode_sprite_height
			explode_sprite.region_rect.size.x = explode_sprite_width
			explode_sprite.region_rect.size.y = explode_sprite_height
			explode_sprite.region_enabled = true
			explode_sprite.position.x = pos_x + explode_sprite_width / 2.0
			explode_sprite.position.y = pos_y + explode_sprite_height / 2.0
			get_parent().add_child(explode_sprite)
	
	queue_free()
