#!/bin/bash

echo "=== Debugging clangd LSP for GnomeEngine ==="
echo

echo "1. Checking clangd version:"
clangd --version
echo

echo "2. Checking compile_commands.json exists:"
ls -la compile_commands.json
echo

echo "3. Checking .clangd config:"
cat .clangd
echo

echo "4. Testing clangd on main.cpp:"
echo "   (Look for any error messages below)"
clangd --check=src/main.cpp --log=verbose 2>&1 | grep -i -E "(error|warning|failed|missing|not found)"
echo

echo "5. Checking if standard headers exist:"
echo "   C++ iostream header:"
ls -la /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/iostream 2>/dev/null || echo "   NOT FOUND"
echo "   GLFW header:"
ls -la /opt/homebrew/include/GLFW/glfw3.h 2>/dev/null || echo "   NOT FOUND"
echo

echo "6. Current working directory:"
pwd
echo

echo "=== To run with Neovim debugging: ==="
echo "1. Open Neovim: nvim src/main.cpp"
echo "2. In Neovim, run: :LspInfo"
echo "3. In Neovim, run: :lua vim.lsp.set_log_level('debug')"
echo "4. Check logs at: ~/.local/state/nvim/lsp.log"
echo
