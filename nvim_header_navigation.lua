-- Add this to your Neovim config (~/.config/nvim/lua/header_navigation.lua)

local M = {}

-- Function to jump to implementation in header-only files
function M.goto_implementation()
    local current_word = vim.fn.expand('<cword>')
    local current_line = vim.fn.line('.')
    local buffer_lines = vim.api.nvim_buf_get_lines(0, 0, -1, false)
    
    -- Look for implementation patterns
    local impl_patterns = {
        "inline%s+" .. current_word .. "::" .. current_word,  -- Constructor
        "inline%s+.-::" .. current_word .. "%s*%(", -- Methods
        current_word .. "::" .. current_word .. "%s*%(", -- Non-inline constructor
        ".-::" .. current_word .. "%s*%(" -- Any method implementation
    }
    
    for i, line in ipairs(buffer_lines) do
        for _, pattern in ipairs(impl_patterns) do
            if line:match(pattern) and i ~= current_line then
                vim.api.nvim_win_set_cursor(0, {i, 0})
                print("Found implementation at line " .. i)
                return
            end
        end
    end
    
    -- Fallback to standard LSP
    vim.lsp.buf.definition()
end

-- Function to jump to declaration in header-only files  
function M.goto_declaration()
    local current_word = vim.fn.expand('<cword>')
    local current_line = vim.fn.line('.')
    local buffer_lines = vim.api.nvim_buf_get_lines(0, 0, -1, false)
    
    -- Look for declaration patterns (before implementation section)
    local in_implementation = false
    for i, line in ipairs(buffer_lines) do
        -- Check if we've reached implementation section
        if line:match("IMPLEMENTATION") or line:match("inline") then
            in_implementation = true
        end
        
        -- If we're not in implementation section, look for declarations
        if not in_implementation then
            local decl_patterns = {
                "%s+" .. current_word .. "%s*%(", -- Function declaration
                "class%s+" .. current_word .. "%s*{", -- Class declaration
                "%s+" .. current_word .. "%s*;", -- Variable/member declaration
            }
            
            for _, pattern in ipairs(decl_patterns) do
                if line:match(pattern) and i ~= current_line then
                    vim.api.nvim_win_set_cursor(0, {i, 0})
                    print("Found declaration at line " .. i)
                    return
                end
            end
        end
    end
    
    -- Fallback to standard LSP
    vim.lsp.buf.declaration()
end

-- Smart navigation that picks the best target
function M.smart_goto()
    local current_line = vim.fn.line('.')
    local buffer_lines = vim.api.nvim_buf_get_lines(0, 0, -1, false)
    local current_line_text = buffer_lines[current_line]
    
    -- If we're on a line with "inline" or in implementation section, go to declaration
    if current_line_text:match("inline") or current_line > 100 then -- Assuming impl starts around line 100
        M.goto_declaration()
    else
        -- Otherwise go to implementation
        M.goto_implementation()
    end
end

return M

-- Usage in your Neovim config:
-- local header_nav = require('header_navigation')
-- vim.keymap.set('n', 'gri', header_nav.goto_implementation, { desc = 'Go to implementation' })
-- vim.keymap.set('n', 'grd', header_nav.goto_declaration, { desc = 'Go to declaration' })
-- vim.keymap.set('n', 'grg', header_nav.smart_goto, { desc = 'Smart goto' })
