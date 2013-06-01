#ifndef _CCT_H_
	#define _CCT_H_
	
	/**
	 * \file cct.h
	 * Physics based player movement
	 */
	
	// Global skills: // Todo Put those to ONE player definition file
	#define health							skill20
	#define allowMove						skill21
	#define allowRotate					skill22
	
	// cct skills:
	#define standingSize					skill40
	#define crouchingSize				skill41
	#define halfStandingSize			skill42
	#define halfCrouchingSize			skill43
	#define cctRadius						skill44
	// cct size (stand):
	#define	standSize_x					skill45
	#define	standSize_y					skill46
	#define	standSize_z					skill47
	// cct size (crawl):
	#define	crawlSize_x					skill48
	#define	crawlSize_y					skill49
	#define	crawlSize_z					skill50
	
	// collusion groups:
	#define PLAYER_GROUP					3
	// movement speed:
	#define CRAWL_SPEED					0.3
	#define WALK_SPEED					0.6
	#define RUN_SPEED						1
	
	/**
	 * Action for the player.
	 */
	void actHero();
	
	// include cct template:
	#include "cct.c"
	
#endif