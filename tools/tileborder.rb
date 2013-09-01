# Program to add borders to tiles in tilesets. Borders are composed of pixels on the edges of a tile.
#
# Author: Marcin Kacprzak <martin.kacprzak@gmail.com>

#require 'rubygems'
require 'RMagick'

if ARGV.count < 1 then
  puts "usage: ruby #{$0} filename [tilewidth] [tileheight]"
  exit
end

FILE_NAME = ARGV[0]

unless File.exist?(FILE_NAME) then
  puts "Error: #{FILE_NAME}: no such file!"
  exit
end

FILE_EXTNAME = File.extname(FILE_NAME)
FILE_BASENAME = File.basename(FILE_NAME, FILE_EXTNAME)

TILE_WIDTH  = ARGV[1] || 32
TILE_HEIGHT = ARGV[2] || 32

img = Magick::Image.read(FILE_NAME)[0]

TILE_ROWS = img.rows / TILE_WIDTH
TILE_COLS = img.columns / TILE_HEIGHT

new_img_rows = img.rows + TILE_ROWS * 2
new_img_cols = img.columns + TILE_COLS * 2

# Create a new empty image to composite the tiles upon:
new_img = Magick::Image.new(new_img_cols, new_img_rows) do
  self.background_color = "none"
end

# Tiles will be an array of Image objects
tiles = (TILE_COLS * TILE_ROWS).times.inject([]) do |arr, idx|
  arr << Magick::Image.constitute(TILE_WIDTH, TILE_HEIGHT, 'RGBA',
                                  img.dispatch(idx % TILE_COLS * TILE_WIDTH,
                                               idx / TILE_COLS * TILE_HEIGHT,
                                               TILE_WIDTH, TILE_HEIGHT, 'RGBA'))
end

tiles.each_with_index do |tile, idx|
  x = idx % TILE_COLS
  y = idx / TILE_COLS

  col = x * TILE_WIDTH + x * 2 + 1
  row = y * TILE_HEIGHT + y * 2 + 1

  # Corners
  topleft = tile.get_pixels(0, 0, 1, 1)
  new_img.store_pixels(col - 1, row - 1, 1, 1, topleft)

  bottom_left = tile.get_pixels(0, TILE_HEIGHT - 1, 1, 1)
  new_img.store_pixels(col - 1, row + TILE_HEIGHT, 1, 1, bottom_left)

  top_right = tile.get_pixels(TILE_WIDTH - 1, 0, 1, 1)
  new_img.store_pixels(col + TILE_WIDTH, row - 1, 1, 1, top_right)

  bottom_right = tile.get_pixels(TILE_WIDTH - 1, TILE_HEIGHT - 1, 1, 1)
  new_img.store_pixels(col + TILE_WIDTH, row + TILE_HEIGHT, 1, 1, bottom_right)

  # Edges
  left_edge = tile.get_pixels(0, 0, 1, TILE_HEIGHT)
  new_img.store_pixels(col - 1, row, 1, TILE_HEIGHT, left_edge)

  right_edge = tile.get_pixels(TILE_WIDTH - 1, 0, 1, TILE_HEIGHT)
  new_img.store_pixels(col + TILE_WIDTH, row, 1, TILE_HEIGHT, right_edge)

  top_edge = tile.get_pixels(0, 0, TILE_WIDTH, 1)
  new_img.store_pixels(col, row - 1, TILE_WIDTH, 1, top_edge)

  bottom_edge = tile.get_pixels(0, TILE_HEIGHT - 1, TILE_WIDTH, 1)
  new_img.store_pixels(col, row + TILE_HEIGHT, TILE_WIDTH, 1, bottom_edge)

  # Meat
  new_img.composite!(tile, col, row, Magick::OverCompositeOp)
end

new_img.write("#{FILE_BASENAME}-border#{FILE_EXTNAME}")

