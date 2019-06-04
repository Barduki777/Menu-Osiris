#pragma once

namespace button_flags {
	enum method {
		button_in_attack = (1 << 0),
		button_in_jump = (1 << 1),
		button_in_duck = (1 << 2),
		button_in_forward = (1 << 3),
		button_in_back = (1 << 4),
		button_in_use = (1 << 5),
		button_in_cancel = (1 << 6),
		button_in_left = (1 << 7),
		button_in_right = (1 << 8),
		button_in_move_left = (1 << 9),
		button_in_move_right = (1 << 10),
		button_in_attack2 = (1 << 11),
		button_in_run = (1 << 12),
		button_in_reload = (1 << 13),
		button_in_alt1 = (1 << 14),
		button_in_alt2 = (1 << 15),
		button_in_score = (1 << 16),
		button_in_speed = (1 << 17),
		button_in_walk = (1 << 18),
		button_in_zoom = (1 << 19),
		button_in_weapon1 = (1 << 20),
		button_in_weapon2 = (1 << 21),
		button_in_bull_rush = (1 << 22),
		button_in_grenade1 = (1 << 23),
		button_in_grenade2 = (1 << 24),
		button_in_attack3 = (1 << 25)
	};
}