local imgui_helper = {}

function imgui_helper.decimal_input(propery_name, value, update_func)
    local ok, text = imgui.InputText(propery_name,
                               value,
                               imgui.ImGuiInputTextFlags_CharsDecimal
                             | imgui.ImGuiInputTextFlags_EnterReturnsTrue)
    if ok then
        local n = tonumber(text)
        if n then
            if update_func then
                update_func(n)
            end
        end
    end
end

return imgui_helper