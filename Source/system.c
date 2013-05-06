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
	nUpKey 					= key_for_str("w");
	nDownKey 				= key_for_str("s");
	nLeftKey 				= key_for_str("a");
	nRightKey 				= key_for_str("d");
	nJumpKey					= key_for_str("space");
	nRunKey 					= key_for_str("shiftl");
	nCrouchKey				= key_for_str("ctrl");
	nInteractKey 			= key_for_str("e");
	nChangeWeaponUpKey 	= key_for_str("pgup");
	nChangeWeaponDownKey = key_for_str("pgdn");
	nCharacterMenuKey		= key_for_str("c");
	nESCKey					= key_for_str("esc");
	nQuestsKey				= key_for_str("q");
	nMapKey					= key_for_str("m");
	nFire1Key				= key_for_str("o");
	nFire2Key				= key_for_str("p");

	n1Key						= key_for_str("1");
	n2Key						= key_for_str("2");
	n3Key						= key_for_str("3");
	n4Key						= key_for_str("4");
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
	nFire1				= key_pressed(nFire1Key) || mouse_left;
	nFire2				= key_pressed(nFire2Key) || mouse_right;

	n1						= key_pressed(n1Key);
	n2						= key_pressed(n2Key);
	n3						= key_pressed(n3Key);
	n4						= key_pressed(n4Key);
}

#endif