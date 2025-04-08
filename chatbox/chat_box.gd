extends Control

@onready var chat_log = get_node("VBoxContainer/RichTextLabel")
@onready var input_label = get_node("VBoxContainer/HBoxContainer/Label")
@onready var input_field = get_node("VBoxContainer/HBoxContainer/LineEdit")

var groups = [
	{'name': 'Team', 'color': '#00abc7'},
	{'name': 'Match', 'color': '#ffdd8b'},
	{'name': 'Global', 'color': '#ffffff'}
]
var group_index = 0
var user_name = 'Player'

func _ready():
	input_field.text_submitted.connect(text_entered)
	change_group(0)

func _input(event):
	if event is InputEventKey:
		if event.pressed:
			match event.keycode:
				KEY_ENTER:
					input_field.grab_focus()
				KEY_ESCAPE:
					input_field.release_focus()
				KEY_TAB:
					change_group(1)

func change_group(value):
	group_index += value
	if group_index >= groups.size():
		group_index = 0
	input_label.text = "[" + groups[group_index]['name'] + "]"
	input_label.set("theme_override_colors/font_color", Color(groups[group_index]['color']))

func add_message(username: String, text: String, group := 0, color := ""):
	chat_log.append_text("\n")
	
	var color_tag = color if color != "" else groups[group]['color']
	var message = "[color=" + color_tag + "]"
	if username != "":
		message += "[" + username + "]: "
	message += text + "[/color]"
	
	chat_log.append_text(message)
	chat_log.scroll_to_line(chat_log.get_line_count())  # Auto-scroll to bottom

func text_entered(text: String):
	if text.strip_edges() == "/h":
		add_message("", "There is no help message yet!", 0, "#ff5757")
	else:
		add_message(user_name, text.strip_edges(), group_index)
		print("Message sent:", text)
	
	input_field.text = ""  # Clear the input field
