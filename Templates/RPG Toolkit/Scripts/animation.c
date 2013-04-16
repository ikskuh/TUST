// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 140610
//
// title: Animation Blending code
// class: Movement
// type: Animation
// help: Handles the animation blending of any model
//
// ENDHEADER
// ---------------------------------------------------------------------

void RunmyAnimation(ANIMATION *animation, ENTITY *Animator, ENTITY *targetAnimator) {
	var setFirstAnim = random(100); // first animation to be set random, because dont want every animation to look generic
	while(Animator) { // as long as the animator entity exists.
		// dont animate while current animation contains nothing.
		while(animation->ANIM_current == NULL) { wait(1); }
		
		// if new frame, start blending
		if (!str_cmpi(animation->ANIM_animblend, "blend") && !str_cmpi(animation->ANIM_blendframe, "nullframe")) { 
			animation->ANIM_animate2 = 0; 
			str_cpy(animation->ANIM_animblend, "blend");
		}
		
		// while blending;
		if (str_cmpi(animation->ANIM_animblend, "blend")) {
			// when the animation needs to be blended
			if(animation->ANIM_current->ANIM_currentDoBlend) {
				// animate the blend's first half
				ent_animate(Animator, animation->ANIM_previousframe, animation->ANIM_animate, ANM_CYCLE);
			}
			
			// blend rest of animation here
			ent_blendframe(Animator, targetAnimator, animation->ANIM_current->ANIM_animName, 0, animation->ANIM_animate2);
			
			// increase the blending
			animation->ANIM_animate2 += animation->ANIM_current->ANIM_blendSpeed * time_step;
			
			// when blending should continue into animation
			if (animation->ANIM_animate2 >= 100) {
				// reset blending
				animation->ANIM_animate = setFirstAnim;
				setFirstAnim = 0; // reset forever from now on.
				str_cpy(animation->ANIM_animblend, animation->ANIM_blendframe);
				str_cpy(animation->ANIM_blendframe, "nullframe");
			}
		} else {
			// do regular animation
			ent_animate(Animator, animation->ANIM_current->ANIM_animName, animation->ANIM_animate, ANM_CYCLE);
			animation->ANIM_animate += animation->ANIM_current->ANIM_animSpeed * time_step;
			animation->ANIM_animate %= 100;
		}
		
		wait(1);
	}
}