// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 060610
//
// title: Animation Blending code
// class: Movement
// type: Animation
// help: Handles the animation blending of any model
//
// prefix: ANIM_
// idcode: 4
//
// ENDHEADER
// ---------------------------------------------------------------------

typedef struct LINKED_ANIM {
	STRING *ANIM_animName;
	
	int ANIM_currentDoBlend;
	var ANIM_animSpeed;
	var ANIM_blendSpeed;
	
	struct LINKED_ANIM *ANIM_next; // if NULL, list is ended
} LINKED_ANIM;

typedef struct {
	STRING *ANIM_idle;
	STRING *ANIM_move;
} DEFAULT_ANIM;

typedef struct {
	var ANIM_animate;
	var ANIM_animate2;
	
	STRING *ANIM_animblend;
	STRING *ANIM_blendframe;
	STRING *ANIM_previousframe;
	
	LINKED_ANIM *ANIM_first; // store linked animations list's first animation
	LINKED_ANIM *ANIM_current; // set animation that is used currently
	
	DEFAULT_ANIM *ANIM_default; // stores the 2 default animations for every character
} ANIMATION;

// prototypes
void get_animations(STRING *file, STRING *character, ANIMATION *anim);
STRING *get_defaultAnimation(STRING *file, STRING *character, STRING *search);
void setNewAnimationBlend(ANIMATION *animation, STRING *animFrame);

// includes
#include "animation.c"

DEFAULT_ANIM *init_defaultAnimations(ENTITY *animator) {
	DEFAULT_ANIM *myDefaultAnimations = sys_nxalloc(sizeof(DEFAULT_ANIM));
	
	myDefaultAnimations->ANIM_idle = get_defaultAnimation(animator.string1, animator.string2, "idle");
	myDefaultAnimations->ANIM_move = get_defaultAnimation(animator.string1, animator.string2, "move");
	
	return(myDefaultAnimations);
}

LINKED_ANIM *init_linkedAnimation(STRING *anim, int isBlend, var animspeed, var blendspeed) {
	LINKED_ANIM *myLinkedAnim = sys_nxalloc(sizeof(LINKED_ANIM));
	myLinkedAnim->ANIM_animName = str_create(anim);
	
	myLinkedAnim->ANIM_currentDoBlend = isBlend;
	myLinkedAnim->ANIM_animSpeed = animspeed;
	myLinkedAnim->ANIM_blendSpeed = blendspeed;
	
	myLinkedAnim->ANIM_next = NULL; // if NULL, list is ended
	
	return(myLinkedAnim);
}

ANIMATION *init_myAnimation(ENTITY *animator, ENTITY *targetAnimator) {
	ANIMATION *myAnimation = sys_nxalloc(sizeof(ANIMATION)); // nxalloc since it's required to run on a character inside the level
	
	myAnimation->ANIM_animate = 0;
	myAnimation->ANIM_animate2 = 0;
	
	myAnimation->ANIM_animblend = str_create("");
	myAnimation->ANIM_blendframe = str_create("");
	myAnimation->ANIM_previousframe = str_create("");
	
	// Print error on screen when the NPC has no/wrong file to load setup from attached
	if(!file_open_read(animator.string1)) {
		STRING *errorStr = str_create("");
		str_cpy(errorStr, "the NPC (");
		str_cat(errorStr, str_for_entname(NULL, animator));
		str_cat(errorStr,  ") has a wrong file to load attached (");
		str_cat(errorStr, animator.string1);
		str_cat(errorStr, ").");
		
		printf(_chr(errorStr), NULL);
	}
	
	myAnimation->ANIM_first = NULL; // empty the first linked animation
	get_animations(animator.string1, animator.string2, myAnimation); // generate linked list of animations
	
	// ANIM_default contains 2 animations that are used as defaults (but might contain different animation names for each instance)
	myAnimation->ANIM_default = init_defaultAnimations(animator);
	// set current animation to be idle default
	setNewAnimationBlend(myAnimation, myAnimation->ANIM_default->ANIM_idle);
	
	// run the animation loop that animates this actor
	RunmyAnimation(myAnimation, animator, targetAnimator); // run animation for this instance
	
	return(myAnimation);
}

// for reading all animations from a file into the characters animation struct
void get_animations(STRING *file, STRING *character, ANIMATION *anim) {
	var filehandle_n = file_open_read(file);
	
	// Print error on screen if the character that has to be loaded does not exist in the file
	if(file_find(filehandle_n, character) < 0) { // get character
		STRING *errorStr = str_create("");
		str_cpy(errorStr, "the NPC the you are looking for (");
		str_cat(errorStr, character);
		str_cat(errorStr,  ") does not exist in the file (");
		str_cat(errorStr, file);
		str_cat(errorStr, ").");
		
		printf(_chr(errorStr), NULL);
	}
	
	file_find(filehandle_n, "animations: "); // get animation handle
	
	STRING* animStr = str_create("");
	file_str_read(filehandle_n, animStr); // animation should be divided with spaces... (ex: idle run walk jump swim)
	
	file_close(filehandle_n);
	
	var animStrLength; // stores the next animation name
	var curAnimStrLength;
	
	STRING* currentAnim = str_create("");
	STRING* findAnim = str_create("");
	STRING* getAnim = str_create("");
	
	var valueLength;
	var lengthClipValue;
	var animValueArray[3];
	var i;
	
	STRING* valueStr = str_create("");
	
	LINKED_ANIM *previous = init_linkedAnimation("empty", 0, 0, 0);
	LINKED_ANIM *current;
	
	while(1) {
		// find new animations string
		animStrLength = str_stri(animStr, " "); // check for the next space seperating the animations
		if(animStrLength == 0) { break; } // if no more animations are found, break this loop.
		
		// process animation into new game animation
		str_cpy(currentAnim, animStr); // store animations string
		str_clip(animStr, animStrLength); // remove animation from animations string
		curAnimStrLength = str_len(animStr);
		str_trunc(currentAnim, curAnimStrLength+1); // remove all but first animation name (and the extra space)
		
		filehandle_n = file_open_read(file);
		
		// add new struct for linked animation list here
		str_cpy(findAnim, currentAnim);
		str_cat(findAnim, " animation: "); // make animation name
		
		file_find(filehandle_n, character); // get character
		file_find(filehandle_n, findAnim); // find animation
		
		file_str_read(filehandle_n, getAnim); // get parameters
		
		file_close(filehandle_n);
		
		for(i=0; i<3; i++){
			// get the animation handle
			switch(i) {
				case 0:
					valueLength = str_stri(getAnim, "blend=");
					lengthClipValue = str_len("blend");
					
					break;
					
				case 1:
					valueLength = str_stri(getAnim, "animspeed=");
					lengthClipValue = str_len("animspeed");
					break;
					
				case 2:
					valueLength = str_stri(getAnim, "blendspeed=");
					lengthClipValue = str_len("blendspeed");
					break;
			}
			
			str_clip(getAnim, valueLength+lengthClipValue);
			
			// get value now
			valueLength = str_stri(getAnim, " ");
			str_cpy(valueStr, getAnim); // store animations string
			
			valueLength = str_len(valueStr);
			str_trunc(valueStr, valueLength+1);
			
			animValueArray[i] = str_to_num(valueStr);
		}
		
		current = init_linkedAnimation(currentAnim, animValueArray[0], animValueArray[1], animValueArray[2]); // get first animation
		if(!str_cmpi(previous->ANIM_animName, "empty")) { previous->ANIM_next = current; } else { anim->ANIM_first = current; } // add first linked animation into the player
		previous = current; // link current to previous to inherit for the next animation set.
	}
	
	sys_free(previous);
	sys_free(current);
}

STRING *get_defaultAnimation(STRING *file, STRING *character, STRING *search) {
	STRING* animStr = str_create(""); // get data string
	STRING* searchStr = str_create("");
	
	var filehandle_n = file_open_read(file);
	file_find(filehandle_n, character); // get character
	str_cpy(searchStr,"default ");
	str_cat(searchStr, search);
	str_cat(searchStr, " animation: "); // get animation handle
	file_find(filehandle_n, searchStr);
	
	file_str_read(filehandle_n, animStr); // animation should be divided with spaces... (ex: idle run walk jump swim)
	
	file_close(filehandle_n);
	
	return(animStr);
}

LINKED_ANIM *getMyAnimation(ANIMATION *animation, STRING *anim) {
	LINKED_ANIM *myLinkedAnim = animation->ANIM_first; // Make sure to return NULL, if nothing was found
	
	while(myLinkedAnim != NULL) {
		if(str_cmpi(myLinkedAnim->ANIM_animName, anim)) { return(myLinkedAnim); } // if the current animation is the one needed
		myLinkedAnim = myLinkedAnim->ANIM_next;
	}
	
	return(NULL);
}

void setNewAnimationBlend(ANIMATION *animation, STRING *animFrame) {
	// get old animation name for blending
	if(animation->ANIM_current != NULL) {
		str_cpy(animation->ANIM_previousframe, animation->ANIM_current->ANIM_animName);
	}
	
	// get new animation and set to use that for blending
	animation->ANIM_current = getMyAnimation(animation, animFrame);
	str_cpy(animation->ANIM_blendframe, animation->ANIM_current->ANIM_animName);
}