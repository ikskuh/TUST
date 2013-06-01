#ifndef _CCT_H_
	#define _CCT_H_
	
	/**
	 * \file cct.h
	 * Physics based player movement
	 */
	
	// Global skills: // Todo Put those to ONE player definition file
	#define HEALT							skill20
	#define ALLOW_MOVE					skill21
	#define ALLOW_ROTATE					skill22
	
	// cct skills:
	#define STANDING_SIZE				skill40
	#define CROUCING_SIZE				skill41
	#define HALF_STANDING_SIZE			skill42
	#define HALF_CROUCHING_SIZE		skill43
	#define CCT_RADIUS					skill44
	// cct size (stand):
	#define	STAND_SIZE_X				skill45
	//#define	STAND_SIZE_Y			skill46
	#define	STAND_SIZE_Z				skill47
	// cct size (crawl):
	#define	CRAWL_SIZE_X				skill48
	//#define	CRAWL_SIZE_Y			skill49
	#define	CRAWL_SIZE_Z				skill50
	
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