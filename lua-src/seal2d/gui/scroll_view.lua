local node = require "seal2d.game_object.node"
local panel = require "seal2d.game_object.panel"

local scroll_view = class("scroll_view", function()
	return node.new()
end)

local HORIZONTAL = 0
local VERTICAL = 1
local BOTH = 2

function scroll_view:ctor(opt)
	assert(opt and type(opt) == 'table')
	assert(type(opt.width) == 'number' and type(opt.height) == 'number')

	self.panel = panel.new(0, 0, opt.width, opt.height)
end

function scroll_view:add_content(child)
	self.panel:add_child(child)
end

return scroll_view