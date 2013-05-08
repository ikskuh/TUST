#ifndef _SYSTEM_C_
#define _SYSTEM_C_

 void sys_keys_init() {
 	// Reset default settings
	on_f2  = NULL;
	on_f3  = NULL;
	on_f4  = NULL;
	on_f5  = NULL;
	on_esc = NULL;
	
	on_close = sys_close;

	// Dynamic key settings
	nUpKey 					= key_for_str_ext("w");
	nDownKey 				= key_for_str_ext("s");
	nLeftKey 				= key_for_str_ext("a");
	nRightKey 				= key_for_str_ext("d");
	nJumpKey					= key_for_str_ext("space");
	nRunKey 					= key_for_str_ext("shiftl");
	nCrouchKey				= key_for_str_ext("ctrl");
	nInteractKey 			= key_for_str_ext("e");
	nChangeWeaponUpKey 	= key_for_str_ext("pgup");
	nChangeWeaponDownKey = key_for_str_ext("pgdn");
	nCharacterMenuKey		= key_for_str_ext("c");
	nESCKey					= key_for_str_ext("esc");
	nQuestsKey				= key_for_str_ext("q");
	nMapKey					= key_for_str_ext("m");
	nFire1Key				= key_for_str_ext("mouse_left");
	nFire2Key				= key_for_str_ext("mouse_right");

	n1Key						= key_for_str_ext("1");
	n2Key						= key_for_str_ext("2");
	n3Key						= key_for_str_ext("3");
	n4Key						= key_for_str_ext("4");
}

void sys_close() {
	sys_exit(NULL);
}

void sys_keys_check() {
	nUp					= key_pressed(nUpKey);
	nDown					= key_pressed(nDownKey);
	nLeft					= key_pressed(nLeftKey);
	nRight				= key_pressed(nRightKey);
	nJump					= key_pressed(nJumpKey);
	nRun					= key_pressed(nRunKey);
	nCrouch				= key_pressed(nCrouchKey);
	nInteract			= key_pressed(nInteractKey);
	nChangeWeaponUp 	= key_pressed(nChangeWeaponUpKey) || (mickey.z > 0);
	nChangeWeaponDown = key_pressed(nChangeWeaponDownKey)  || (mickey.z < 0);
	nCharacterMenu		= key_pressed(nCharacterMenuKey);
	nESC					= key_pressed(nESCKey);
	nQuests				= key_pressed(nQuestsKey);
	nMap					= key_pressed(nMapKey);
	nFire1				= key_pressed(nFire1Key);
	nFire2				= key_pressed(nFire2Key);

	n1						= key_pressed(n1Key);
	n2						= key_pressed(n2Key);
	n3						= key_pressed(n3Key);
	n4						= key_pressed(n4Key);
}

void sys_change_resolution(var _res, var _fullscreen) {
	video_switch(_res, 0, _fullscreen);
	menu_center();
}

STRING* str_for_key_ext(STRING* _target, var _key) {
	
	STRING* strNewKey = str_create("");
	switch(_key) {
		case 280: str_cpy(strNewKey, "mouse_left"); break;
		case 281: str_cpy(strNewKey, "mouse_right"); break;
		case 282: str_cpy(strNewKey, "mouse_middle"); break;
		default: str_for_key(strNewKey, _key); break;
	}
	
	if (_target == NULL) {
		return strNewKey;
	} else {
		str_cpy(_target, strNewKey);
		return NULL;
	}
}

var key_for_str_ext(STRING* _code) {
	if (_code == NULL) return -1;
	if (str_cmp(_code, "mouse_left")) return 280;
	if (str_cmp(_code, "mouse_right")) return 281;
	if (str_cmp(_code, "mouse_middle")) return 282;
	return key_for_str(_code);
}

var sys_key_used(var _key) {
	
	if ((nUpKey == _key) || (nDownKey == _key) || (nLeftKey == _key) || (nRightKey == _key) || (nRunKey == _key) || (nCrouchKey == _key) ||
		(nInteractKey == _key) || (nChangeWeaponUpKey == _key) || (nChangeWeaponDownKey == _key) || (nCharacterMenuKey == _key) ||
		(nQuestsKey == _key) || (nMapKey == _key) || (nFire1Key == _key) || (nFire2Key == _key) || (nJumpKey == _key)) {
			return 1;
		} else {
			return 0;
		}
}

#endif