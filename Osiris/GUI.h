#pragma once

#include <string>

class GUI final {
public:
    void render() noexcept;
    bool isOpen{ false };
private:
    static void checkboxedColorPicker(const std::string&, bool*, float*) noexcept;
    static void hotkey(int&) noexcept;
    void renderMenuBar() noexcept;
	void MainMenu()  noexcept;
	

    struct {
		bool MainMenu{ false };

    } window;
};

extern GUI gui;
