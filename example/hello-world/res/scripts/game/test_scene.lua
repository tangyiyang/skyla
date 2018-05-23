local node = require "skyla.game_object.node"
local sprite = require "skyla.game_object.sprite"
local button = require "skyla.gui.button"
local list_view = require "skyla.gui.list_view"
local scale9sprite = require "skyla.game_object.scale9sprite"

local test_scene = class("test_scene", function()
    return node.new()
end)

local function wrap_before_test(self, func, test_layer)
    return function()
        test_layer:remove_all_children()
        return func(self, test_layer)
    end
end

local function sprite_test(self, parent)
    local sprite_test_layer = require "test.sprite_test_layer"
    parent:add_child(sprite_test_layer.new())
end

local function scale9sprite_test(self, parent)
    local visible_rect = require("skyla.context"):get_visible_rect()

    local s = scale9sprite.new("#color_pad.png")
    s:set_pos(visible_rect.width/2, visible_rect.height/2)
    s:set_border(25, 25, 25, 25)
    s:set_size(400, 400)
    parent:add_child(s)
end

local function bmfont_test(self, parent)
	local bmfont = require "skyla.game_object.bmfont"
	local case = "The quick brown fox jumps over the lazy dog"

	local fonts = {
		"fonts/animated_32_ffffff.fnt",
		"fonts/arial_bold_32_light_blue.fnt"
	}

	local visible_rect = require("skyla.context"):get_visible_rect()
	for i = 1, #fonts do
		local f = fonts[i]
		local t = bmfont.new(case, f)
		t:set_pos(visible_rect.width/2, visible_rect.height - (i-1)*64)
		t:set_anchor(0.5, 1)
		parent:add_child(t)
	end
end

local function spine_test(self, parent)

	local visible_rect = require("skyla.context"):get_visible_rect()
	local spine = require "skyla.game_object.spine"
	local s = spine.new("spines/unit_skull.json", "spines/unit_skull.atlas")
    local names = s:get_all_anim_names()

     -- left side list view
    local list = list_view.new {
                                mode = "col",
                                width = 104, height = visible_rect.height,
                                n_cell = #names,
                                cell_create_func = function(index)
                                    print("name = ", names[index])
                                    local name = names[index]
                                    local b = button.new {
                                                        normal = "#ui_button_tiny.png",
                                                        text = name,
                                                        font = "fonts/animated_32_ffffff.fnt",
                                                        callback = function()
                                                            print("click name = ", name)
                                                            s:set_anim(name, 0, 1)
                                                        end
                                                    }
                                    return b
                                end
                            }
    list:set_anchor(0, 0.5)
    list:set_pos(100, visible_rect.height/2)
    parent:add_child(list)


	s:set_anim("attack_1", 0, 1)
	s:set_pos(visible_rect.width/2, visible_rect.height/2)

	parent:add_child(s)
end


local function scene_loader_test(self, parent)
    local scene_loader_test = require "test.scene_loader_test_layer"

    parent:add_child(scene_loader_test.new())
end

local function primitive_basic_test(self, parent)
    local primitive = require("skyla.game_object.primitive")

    local p = primitive.new()
    p:add_line(0, 0, 100, 100, 0xff0000ff);
    parent:add_child(p)
end

local function load_tests(self, test_layer)
    local visible_rect = require("skyla.context"):get_visible_rect()

    local test_cases = {
    	{ name = "sprite", func = sprite_test },
        { name = "scale9", func = scale9sprite_test },
    	{ name = "bmfont", func = bmfont_test },
    	{ name = "spine", func = spine_test },
        { name = "scene_loader", func = scene_loader_test },
	}

    local function cell_create_func(index)
    	local case = test_cases[index]
        local b = button.new {
                                normal = "#ui_button_tiny.png",
                                text = case.name,
                                font = "fonts/animated_32_ffffff.fnt",
                                callback = function()
                                    return wrap_before_test(self, test_cases[index].func, test_layer)()
                                end
                            }
        return b
    end


    printf("we have %d test cases", #test_cases)

    -- left side list view
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
    sprite.load_frames("images/ui_atlas.json", "images/ui_atlas.png")


    local scene_loader = require "skyla.scene_loader"

    -- all the test nodes is added to this layer.
    local test_layer = node.new()
    self:add_child(test_layer)

	load_tests(self, test_layer)
end


return test_scene