local node = require "seal2d.game_object.node"
local panel = require "seal2d.game_object.panel"
local action = require "seal2d_action"

local scroll_view = class("scroll_view", function()
	return node.new()
end)

local HORIZONTAL = 1
local VERTICAL = 2
local BOTH = 4

scroll_view.HORIZONTAL = HORIZONTAL
scroll_view.VERTICAL = VERTICAL
scroll_view.BOTH = BOTH

function scroll_view:ctor(opt)
	assert(opt and type(opt) == 'table')
	assert(type(opt.width) == 'number' and type(opt.height) == 'number')

    self.direction = opt.direction or BOTH

    self.container = node.new()
    self.container:set_anchor(0, 0)

    self.panel = panel.new(0, 0, opt.width, opt.height)
    self.panel:add_child(self.container)
    self:add_child(self.panel)

    local last_x, last_y
    local function on_touch(id, phase, x, y)
        local cx, cy = self.container:get_pos()
        if phase == TOUCH_BEGIN then

            last_x, last_y = x, y

        elseif phase == TOUCH_MOVED then

            local dx, dy = x - last_x, y - last_y

            if self.direction == HORIZONTAL then
                dy = 0
            elseif self.direction == VERTICAL then
                dx = 0
            elseif self.direction == BOTH then

            else
                error(string.format("invalid direction: %d", self.direction))
            end

            self.container:set_pos(cx + dx, cy + dy)
            last_x, last_y = x, y

        elseif phase == TOUCH_ENDED then
            local _, _, w, h = self.container:get_bounding_box()
            local min_x = cx
            local min_y = cy
            local max_x = cx + w
            local max_y = cy + h

            local to_x, to_y = self.container:get_pos()
            if min_x > opt.width - w then
                to_x = math.abs(w - opt.width)
            end

            if max_x < w then
                to_x = 0
            end

            if min_y > opt.height - h then
                to_y = math.abs(h - opt.height)
            end

            if max_y < h then
                to_y = 0
            end

            -- TODO: add ease animation
            self.container:run_action(action.move_to(0.1, to_x, to_y))
        end
    end

    self:on_event(function(event_type, ...)
        if event_type == "seal2d_event_touch" then
            on_touch(...)
        end
    end)

    self:set_size(opt.width, opt.height)
end

function scroll_view:add_content(child)
    local _, _, w, h = child:get_bounding_box()
    -- self.container:set_size(w, h)
	self.container:add_child(child)
end

function scroll_view:set_direction(direction)
    self.direction = direction
end

return scroll_view