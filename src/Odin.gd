extends KinematicBody2D

enum states{
	walk,
	fall,
	wait_for_walk,
	wait_for_second_speer,
	wait_for_hit_on_ground,
	hit_on_ground,
	wait_for_walk_hit_on_ground,
	knockback,
	awake_from_knockback,
	knockback_explosion,
	destroy_ground,
	teleport_wait,
	teleport_attack,
	intro,
	fall_game_over,
	ground_game_over,
	wait_for_walk_game_over,
	wait_for_grab_new_speer,
	wait_for_lightning_throw,
	fall_from_ground,
	none
}

export var GROUND_VELOCITY = 50
export var GROUND_WALK_POINT = 30
export var WAIT_FOR_WALK_TIME = 2.0
export var KNOCKBACK_TIME = 2.0
export var KNOCKBACK_EXPLOSION_WAIT_TIME = 0.25
export var KNOCKBACK_EXPLOSION_TIME = 0.5
export var KNOCKBACK_EXPLOSION_RADIUS = 100.0
export var HIT_ON_GROUND_RADIUS = 80.0
export var WAIT_FOR_HIT_ON_GROUND_TIME = 1.0
export var HIT_ON_GROUND_TIME = 0.5
export var AXE_DAMAGE = 2.0
export var SPEER_THROW_FREQUENCY = 1.5
export var SPEER_WAIT_TIME = 0.5
export var SPEER_THROW_TIME = 0.25
export var SPEER_THROW_AMOUNT = 2
export var FALL_POS_Y : float = 100.0

onready var sprite = get_node("Sprite")
onready var anim = sprite.get_node("AnimationPlayer")
onready var health_bar = sprite.get_node("Lock/HealthBar")
onready var axe_hitbox = sprite.get_node("AxeHitbox")
onready var speer_start = sprite.get_node("SpeerStart")
onready var speer_scene = preload("res://objects/Speer.tscn")

var width = 50
var height = 75

var velocity = Vector2(0,0)
var acceleration = Globals.GRAVITY
var state = states.walk
var health = 0.0
var player = null
var speer_thrown = 0

func _ready():
	health = health_bar.MAX_VALUE
	axe_hitbox.connect("body_entered",self,"on_axe_entered")
	
func _input(e: InputEvent):
	if e is InputEventKey:
		if e.is_pressed() and e.scancode == KEY_E:
			state = states.destroy_ground
			velocity = Vector2.ZERO
			anim.play("DestroyGround")
			
func _physics_process(delta):
	if state != states.destroy_ground:
		ground_movement(delta)

func _process(delta):
	handle_states(delta)

func ground_movement(delta):
	velocity += acceleration * delta
	velocity = move_and_slide(velocity,Vector2(0,-1))

var time_val = 0.0

func handle_states(delta):
	if state != states.destroy_ground:
		sprite.position.x = 0
	axe_hitbox.position = Vector2(0,0)
	speer_start.position = Vector2(15,34)
	match state:
		states.walk:
			anim.play("Run")
			if not sprite.flip_h:
				velocity.x = -GROUND_VELOCITY
				if position.x + width / 2.0 < GROUND_WALK_POINT:
					position.x = GROUND_WALK_POINT - width / 2.0
					state = states.wait_for_walk
					time_val = 0.0
			else:
				velocity.x = GROUND_VELOCITY
				if position.x + width / 2.0 > Globals.GAME_WIDTH - GROUND_WALK_POINT:
					position.x = Globals.GAME_WIDTH - GROUND_WALK_POINT - width / 2.0
					state = states.wait_for_walk
					time_val = 0.0
			if player.get_position().distance_squared_to(get_position()) < HIT_ON_GROUND_RADIUS * HIT_ON_GROUND_RADIUS:
				state = states.wait_for_hit_on_ground
				time_val = 0.0
				anim.play("HitWait1")
				velocity.x = 0.0
				if player.position.x > position.x:
					sprite.flip_h = true
				else:
					sprite.flip_h = false
			elif time_val > SPEER_THROW_FREQUENCY:
				time_val = 0.0
				if Globals.state == Globals.states.boss_ground:
					anim.play("SpeerWaitGround")
				else:
					anim.play("SpeerWaitFall")
				velocity.x = 0.0
				if player.get_position().x > get_position().x:
					sprite.flip_h = true
				else:
					sprite.flip_h = false
				state = states.wait_for_second_speer
			time_val += delta
		states.fall:
			anim.play("Fall")
			if position.y >= FALL_POS_Y:
				acceleration = Vector2.ZERO
				position.y = FALL_POS_Y
		states.wait_for_walk:
			anim.play("Stand")
			velocity.x = 0.0
			if time_val > WAIT_FOR_WALK_TIME:
				sprite.flip_h = not sprite.flip_h
				state = states.walk
			time_val += delta
			if player.get_position().distance_squared_to(get_position()) < HIT_ON_GROUND_RADIUS * HIT_ON_GROUND_RADIUS:
				state = states.wait_for_hit_on_ground
				time_val = 0.0
				anim.play("HitWait1")
				velocity.x = 0.0
				if player.position.x > position.x:
					sprite.flip_h = true
				else:
					sprite.flip_h = false
		states.knockback:
			time_val += delta
			if time_val > KNOCKBACK_TIME:
				if Globals.state == Globals.states.boss_ground:
					anim.play("Stand")
				else:
					anim.play("Fall")
				state = states.awake_from_knockback
				time_val = 0.0
		states.awake_from_knockback:
			time_val += delta
			if time_val > KNOCKBACK_EXPLOSION_WAIT_TIME:
				anim.play("KnockbackExplosion")
				state = states.knockback_explosion
				time_val = 0.0
				knockback_explosion()
		states.knockback_explosion:
			time_val += delta
			if time_val > KNOCKBACK_EXPLOSION_TIME:
				if Globals.state == Globals.states.boss_ground:
					anim.play("Stand")
				else:
					anim.play("Fall")
				state = states.walk
				time_val = 0.0
		states.wait_for_hit_on_ground:
			if not sprite.flip_h:
				sprite.position.x = -50
			else:
				axe_hitbox.position = Vector2(39,0)
			time_val += delta
			if time_val > WAIT_FOR_HIT_ON_GROUND_TIME:
				anim.play("Hit")
				time_val = 0.0
				state = states.hit_on_ground
		states.hit_on_ground:
			if not sprite.flip_h:
				sprite.position.x = -50
			else:
				axe_hitbox.position = Vector2(39,0)
			time_val += delta
			if time_val > HIT_ON_GROUND_TIME:
				time_val = 0.0
				state = states.walk
				if Globals.state == Globals.states.boss_ground:
					anim.play("Stand")
				else:
					anim.play("Fall")
		states.wait_for_second_speer:
			time_val += delta
			if sprite.flip_h:
				sprite.position.x = -20
				speer_start.position = Vector2(55,34)
			if time_val > SPEER_WAIT_TIME:
				if speer_thrown == SPEER_THROW_AMOUNT:
					state = states.walk
					if Globals.state == Globals.states.boss_ground:
						anim.play("Stand")
					else:
						anim.play("Fall")
					speer_thrown = 0
					time_val = 0.0
				else:
					if Globals.state == Globals.states.boss_ground:
						anim.play("SpeerThrowGround")
					else:
						anim.play("SpeerThrowFall")
					time_val = 0.0
					throw_speer()
					state = states.wait_for_grab_new_speer
		states.wait_for_grab_new_speer:
			time_val += delta
			if sprite.flip_h:
				sprite.position.x = -20
				speer_start.position = Vector2(55,34)
			if time_val > SPEER_THROW_TIME:
				time_val = 0.0
				state = states.wait_for_second_speer

func knockback():
	state = states.knockback
	velocity.x = 0.0
	anim.play("Knockback")
	time_val = 0.0
	speer_thrown = 0.0
	axe_hitbox.monitoring = false

func damage(amount):
	health = max(health-amount,0.0)
	health_bar.set_value(health)

func knockback_explosion():
	if player.get_position().distance_squared_to(get_position()) < KNOCKBACK_EXPLOSION_RADIUS*KNOCKBACK_EXPLOSION_RADIUS:
		player.knockback(position+Vector2(0,60),2.0)

func can_be_damaged():
	match state:
		states.knockback,states.awake_from_knockback,states.knockback_explosion: return false
	return true

func can_inflict_damage():
	return can_be_damaged()

func on_axe_entered(body):
	if body.can_be_damaged():
		body.knockback(axe_hitbox.get_child(0).global_position)
		body.damage(AXE_DAMAGE)

func get_position():
	return position + Vector2(width,height)/2.0

func throw_speer():
	speer_thrown += 1
	var speer = speer_scene.instance()
	speer.position = speer_start.global_position
	speer.direction = (player.get_position() - speer_start.global_position).normalized()
	get_tree().get_root().add_child(speer)
	
func init_destroy_ground():
	$Sprite/AxeHitbox/CollisionShape2D.set_deferred("disabled", true)
	$Head.set_deferred("disabled", true)
	$Body.set_deferred("disabled", true)
	acceleration = Vector2.ZERO
	
func end_destroy_ground():
	$Head.set_deferred("disabled", false)
	$Body.set_deferred("disabled", false)
	$Sprite/AxeHitbox/CollisionShape2D.set_deferred("disabled", false)
	$"../Ground".explode()
	acceleration = Globals.GRAVITY
	player.state = player.states.fall_from_ground
	player.enable_movement = false
	player.anim.play("FallDown")
	state = states.fall_from_ground
	anim.play("Fall")


func on_screen_exited():
	if state == states.fall_from_ground:
		get_parent().on_odin_exited()
