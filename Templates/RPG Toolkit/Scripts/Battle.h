// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 070910
//
// title: Code that handles the Full Battle System
// class: Battle System
// type: Battle System
// help: contains code handling all the Battle System's data
// 
// prefix: BAT_
// idcode: 13
// 
// ENDHEADER
// ---------------------------------------------------------------------

// linked list for all attacks
typedef struct BAT_ATTACK {
	STRING *ATTACK_Name; // attack itself
	STRING* ATTACK_function; // attack function (custom per attack)
	
//	BAT_STAT *ATTACK_status; // stores all this attack's possible stats
//	BAT_ELEMENT *ATTACK_elements; // stores all this attack's possible elements
	
	struct BAT_ATTACK *BAT_ATTACK_Next;
} BAT_ATTACK;

// linked list for all spells
typedef struct BAT_SPELL {
	STRING *SPELL_Name; // attack itself
	STRING* SPELL_function; // attack function (custom per attack)
	
//	BAT_STAT *SPELL_status; // stores all this spell's possible stats
//	BAT_ELEMENT *SPELL_elements; // stores all this spell's possible elements
	
	struct BAT_SPELL *BAT_SPELL_Next;
} BAT_SPELL;

// list storing all stats (STR, DEF, MAG, etc. etc.)
typedef struct BAT_STAT {
	STRING *STAT_Name;
	var STAT_amount; // stores amount of this stat
	
	struct BAT_STAT *BAT_STAT_Next;
} BAT_STAT;

// list storing all elements (fire, water, ice, etc. etc.)
typedef struct BAT_ELEMENT {
	STRING *ELEMENT_Name;
	var ELEMENT_modifier; // stores weakness or strength (<=0 - IMMUNE, >0 = stat modifier)
	
	struct BAT_ELEMENT *BAT_STAT_Next;
} BAT_ELEMENT;

// Weapon setup per character
typedef struct {
	STRING *WEAPON_Name;
	
	BAT_STAT *WEAPON_status; // stores all this weapon's possible stats
	BAT_ELEMENT *WEAPON_elements; // stores all this weapon's possible elements
	
} BAT_weapon;

// Shield (armor & shield) setup per character
typedef struct {
	STRING *SHIELD_Name;
	
	BAT_STAT *SHIELD_status; // stores all this weapon's possible stats
	BAT_ELEMENT *SHIELD_elements; // stores all this weapon's possible elements
	
} BAT_shield;

// struct that contains all stats etc per character
typedef struct {
	STRING *CHAR_Name;
	
	ENTITY *CHAR_Ent;
	
	var CHAR_HP; // stores health
	var CHAR_MP; // stores mana
	
	BAT_weapon *CHAR_weapon;
	BAT_shield *CHAR_shield;
	
	BAT_STAT *BAT_status; // stores all this characters stats (first, and all next can be grabbed through this)
	BAT_ELEMENT *BAT_elements; // stores all this characters elements (first, and all next can be grabbed through this)
	
	BAT_ATTACK *BAT_attacks; // stores all this characters possible attacks (first, and all next can be grabbed through this)
	BAT_SPELL *BAT_spells; // stores all this characters possible spells (first, and all next can be grabbed through this)
	
	// status effects
	
	STRING* BAT_CustomAIEvent; // stores the function name for custom AI
	
	// pickup item (and rare?)
	// money gain
	// experience gain
} BAT_Character;

// variables
// includes

// have functions here that initialise a new character w/ all it's stats etc
// -- read from external files?
BAT_Character *init_BattleCharacter(STRING *filename) {
	BAT_Character *myBatChar = sys_malloc(sizeof(BAT_Character));
	
	myBatChar->CHAR_Name = str_create(""); // read from file
	myBatChar->CHAR_Ent = NULL; // ent_create later on
	
	// all these should be read from files later
	myBatChar->CHAR_HP = 0;
	myBatChar->CHAR_MP = 0;
	
	myBatChar->CHAR_weapon = NULL;
	myBatChar->CHAR_shield = NULL;
	
	myBatChar->BAT_status = NULL;
	myBatChar->BAT_elements = NULL;
	
	myBatChar->BAT_attacks = NULL;
	myBatChar->BAT_spells = NULL;
	
	myBatChar->BAT_CustomAIEvent = str_create(""); // should contain standard AI, can be replaced with custom then.
	
	return(myBatChar);
}

// have gamecode/logic here
// -- battle level creation, on NULL, in current level
// -- Round check
// -- inventory return?
// -- basic menu (selection), done from .c file