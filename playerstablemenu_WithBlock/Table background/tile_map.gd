extends TileMap

var gridsize = 5
var dic = {}
var last_tile = Vector2.ZERO  # Store last modified tile

func _ready():
	for x in gridsize * 2:
		for y in gridsize:
			dic[Vector2i(x, y)] = { "Position": Vector2i(x, y) }  # Store Vector2i keys
			set_cell(0, Vector2(x, y), 0, Vector2i(0,0), 0)

func _process(_delta):
	var tile = local_to_map(get_global_mouse_position())
	tile = Vector2i(tile.x, tile.y)  # Convert tile to Vector2i

	print("Checking tile:", tile, "Exists:", dic.has(tile))  # Debugging

	erase_cell(1, last_tile)  # Only erase last modified tile

	if dic.has(tile):
		set_cell(1, tile, 1, Vector2i(0,0), 0)
		last_tile = tile
