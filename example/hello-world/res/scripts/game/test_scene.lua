local node = require "seal2d.game_object.node"

local test_scene = class("test_scene", function()
    return node.new()
end)

local function sprite_test(self)
    local sprite = require "seal2d.game_object.sprite"
    local a = sprite.new("images/ui_button_middle.png")
    a:set_pos(100, 100)
    a:set_anchor(0, 0)
    self:add_child(a)

    a:set_debug_aabb_visible(true)
end

local function bmfont_test(self)
	local bmfont = require "seal2d.game_object.bmfont"
	local case = "The quick brown fox jumps over the lazy dog"

	local fonts = {
		"fonts/animated_32_ffffff.fnt",
		"fonts/arial_bold_32_light_blue.fnt"
	}

	local visible_rect = require("seal2d.context"):get_visible_rect()
	for i = 1, #fonts do
		local f = fonts[i]
		local t = bmfont.new(case, f)
		t:set_pos(visible_rect.width/2, visible_rect.height - (i-1)*64)
		t:set_anchor(0.5, 1)
		self:add_child(t)
	end
end

local function spine_test(self)
	local visible_rect = require("seal2d.context"):get_visible_rect()
	local spine = require "seal2d.game_object.spine"
	local s = spine.new("spines/unit_skull.json", "spines/unit_skull.atlas")
	s:set_anim("attack_1", 0, 1)
	s:set_pos(visible_rect.width/2, visible_rect.height/2)
	self:add_child(s)
end

local function primitive_basic_test(self)
    local primitive = require("seal2d.game_object.primitive")

    local p = primitive.new()
    p:add_line(0, 0, 100, 100, 0xff0000ff);
    self:add_child(p)
end

local function load_tests(self)
	local list_view = require("seal2d.gui.list_view")
    local button = require "seal2d.gui.button"
    local visible_rect = require("seal2d.context"):get_visible_rect()

    local test_cases = {
    	{ name = "sprite", func = sprite_test },
    	{ name = "bmfont", func = bmfont_test },
    	{ name = "spine", func = spine_test }
	}

    local function cell_create_func(index)
    	print("index = ", index)
    	local case = test_cases[index]
        local b = button.new {
                                normal = "images/ui_button_tiny.png",
                                text = case.name,
                                font = "fonts/animated_32_ffffff.fnt",
                                callback = function()
									return case.func(self)
								end
                            }
        return b
    end

    print("#test_cases = ", #test_cases)
    local list = list_view.new {
                                mode = "col",
                                width = 104, height = visible_rect.height,
                                n_cell = #test_cases,
                                cell_create_func = cell_create_func
                            }
    list:set_anchor(0, 0.5)
    list:set_pos(0, visible_rect.height/2)
    self:add_child(list)
end

function test_scene:ctor()
	load_tests(self)
end


return test_scene