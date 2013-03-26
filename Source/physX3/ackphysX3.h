/////////////////////////////////////////////////////////
// Acknex PhysX3 plugin
// (c) finalherizo
/////////////////////////////////////////////////////////

#ifndef __ACKPHYSX3_H__
#define __ACKPHYSX3_H__

/////////////////////////////////////////////////////////
// PhysX3 Consts

#define PH_RIGID        1        // Constant for setting up physics body
#define PH_STATIC       3        // Constant for setting up static body
#define PH_CHAR         4        // Creates a character

#define PH_BOX          0
#define PH_SPHERE       1
#define PH_CAPSULE      2
#define PH_POLY         3
#define PH_CONVEX       4
#define PH_TERRAIN      5
#define PH_PLANE        6
#define PH_MODIFIED     8

#define PH_BALL         1
#define PH_HINGE        2
#define PH_SLIDER       3
#define PH_WHEEL        PH_CONVEX
#define PH_ROPE         5
#define PH_6DJOINT      6
#define PH_PRISMA       7
#define PH_FIXED        8

// Force Modes:
#define PX_FORCE                    0    // parameter has unit of mass * distance/ time^2, i.e. a force.
#define PX_IMPULSE                  1    // parameter has unit of mass * distance /time (DEFAULT).
#define PX_VELOCITY_CHANGE          2    // parameter has unit of distance / time, i.e. the effect is mass independent: a velocity change.
#define PX_ACCELERATION             3    // parameter has unit of distance/ time^2, i.e. an acceleration. It gets treated just like a force except the mass is not divided out before integration.

// Body flags
#define PX_BF_KINEMATIC             1     // Kinematic body flag
#define PX_BF_DISABLE_GRAVITY       2     // Disable Scene gravity for the body

// Trigger flag
#define PX_TF_TRIGGER_ON_ENTER     (1 << 1)
#define PX_TF_TRIGGER_ON_LEAVE     (1 << 2)
#define PX_TF_TRIGGER_ON_STAY      (1 << 3)
#define PX_TF_TRIGGER_ENABLE       (PX_TF_TRIGGER_ON_ENTER | PX_TF_TRIGGER_ON_LEAVE | PX_TF_TRIGGER_ON_STAY)

// Entity pair flag
#define PX_PF_NOTIFY_TOUCH_FOUND   (1 << 1)   // Notifies touch found for bodies
#define PX_PF_NOTIFY_TOUCH_LOST    (1 << 2)   // Notifies touch lost for bodies
#define PX_PF_IGNORE_PAIR          (1 << 3)   // Ignores collision between two bodies
#define PX_PF_NOTIFY_TOUCH         (PX_PF_NOTIFY_TOUCH_FOUND | PX_PF_NOTIFY_TOUCH_LOST)   // Notifies touch found+lost for bodies

// Collision callback event stage
#define PX_EVENT_TOUCH_FOUND       (1 << 2)   // Event is triggerd by a touch found
#define PX_EVENT_TOUCH_LOST        (1 << 4)   // Event is triggered by a touch lost

// Character controller collision flags
#define PX_CCT_COLLISION_SIDES     (1<<0)     // CCT collides to the side
#define PX_CCT_COLLISION_UP        (1<<1)     // CCT collides with something above it
#define PX_CCT_COLLISION_DOWN      (1<<2)     // CCT collides with something bellow it

#define PX_CCT_CAN_RIDE_ON_OBJECT  (1<<0)     // CCT will follow the motion of the object
#define PX_CCT_SLIDE               (1<<1)     // CCT will slide when on this object (an alternative to slop limit)

// Timestep methods
#define PX_TIMESTEP_FIXED     0     // The simulation automatically subdivides the passed elapsed time into maxTimeStep-sized substeps.
#define PX_TIMESTEP_VARIABLE  1

// Combine flags
#define PX_MC_AVERAGE 0
#define PX_MC_MIN 1
#define PX_MC_MULTIPLY 2
#define PX_MC_MAX 3
#define PX_MC_VALUES 4
#define PX_MC_PAD_32 0x7fffffff

// Material flags
#define PX_MF_DISABLE_FRICTION (1 << 0)
#define PX_MF_DISABLE_STRONG_FRICTION (1 << 1)

// SceneQuery filter flags
#define PX_QF_STATIC (1 << 0)          // Traverse static shapes
#define PX_QF_DYNAMIC (1 << 1)         // Traverse dynamic shapes
#define PX_QF_PREFILTER (1 << 2)       // Run the pre-intersection-test (unsupported)
#define PX_QF_POSTFILTER (1 << 3)      // Run the post-intersection-test (unsupported)	
#define PX_QF_MESH_MULTIPLE (1 << 4)   // Generate all hits for meshes rather than just the first (unsuported)
#define PX_QF_BACKFACE (1 << 5)        // Generate hits for exit points and back faces of tris - NOT CURRENTLY SUPPORTED (PhysX 3.2.0). 
#define PX_QF_ALL_SHAPES  (PX_QF_STATIC|PX_QF_DYNAMIC)

// Scene query flag
#define PX_SQ_IMPACT (1<<0)               // "impact" member is valid
#define PX_SQ_NORMAL (1<<1)               // "normal member is valid"
#define PX_SQ_DISTANCE (1<<2)             // "distance" member is valid
#define PX_SQ_UV (1<<3)                   // "u" and "v" barycentric coordinates are valid (Not applicable for sweep queries)
#define PX_SQ_INITIAL_OVERLAP (1<<4)      // Enable/disable initial overlap tests in sweeps.
#define PX_SQ_INITIAL_OVERLAP_KEEP (1<<5) // Only velid when PX_SQ_INITIAL_OVERLAP is used. Keep or discard shapes initially overlapping
#define PX_SQ_TOUCHING_HIT (1<<6)         // specified the hit object as a touching hit.
#define PX_SQ_BLOCKING_HIT (1<<7)         // specified the hit object as a blocking hit.
#define PX_SQ_DEFAULT (PX_SQ_IMPACT|PX_SQ_NORMAL|PX_SQ_DISTANCE|PX_SQ_UV)  // Default flag for sceneQuery

// Joints
#define PX_JF_BROKEN (1<<0)               // Read Only. Set if joint is broken
#define PX_JF_PROJECTION (1<<1)           // Enable projection
#define PX_JF_COLLISION_ENABLED (1<<2)    // Internal collision between entities
#define PX_JF_REPORTING (1<<3)            // Enable contact reporting for joints
#define PX_JF_VISUALIZATION (1<<4)        // Debug visualizations (Unsupported)

// Revolute joints - hinge
#define PX_HJ_LIMIT_ENABLED (1 << 0)      // Enable limits for hinge
#define PX_HJ_DRIVE_ENABLED (1 << 1)      // Enable Motor for hinge
#define PX_HJ_FREESPIN (1 << 2)           // Enable freespin for hinge

// Spherical joints - ball
#define PX_BJ_LIMIT_ENABLED (1 << 1)      // Enable limits for balll joints

// Distance joint flags - rope
#define PX_RJ_MAX_DISTANCE_ENABLED (1<<1) // Enable max distance for rope joint   
#define PX_RJ_MIN_DISTANCE_ENABLED (1<<2) // Enable min distance for rope joint
#define PX_RJ_SPRING_ENABLED       (1<<3) // Enable spring for rope joint

// Prismatic joints - slider
#define PX_SJ_LIMIT_ENABLED (1 << 1)      // Enable limits for slider (prismatic) joint

// D6 Joints
// Drive
#define PX_6D_DRIVE_X 0                  // Drive along the X-axis 
#define PX_6D_DRIVE_Y 2                  // Drive along the Y-axis 
#define PX_6D_DRIVE_Z 1                  // Drive along the Z-axis
#define PX_6D_DRIVE_SWING 3              // Drive of displacement from the X-axis
#define PX_6D_DRIVE_TWIST 4              // Drive of the displacement around the X-axis 
#define PX_6D_DRIVE_SLERP 5              // Drive of all three angular degrees along a SLERP-path
#define PX_6D_DRIVE_COUNT 6

// Drive flags
#define PX_6D_DF_ACCELERATION 1          // Drive spring is for the acceleration at the joint (rather than the force)   

// Axis
#define PX_6D_AXIS_X 0                   // Motion along the X axis  
#define PX_6D_AXIS_Y 2                   // Motion along the Y axis 
#define PX_6D_AXIS_Z 1                   // Motion along the Z axis
#define PX_6D_AXIS_TWIST 3               // Motion around the X axis 
#define PX_6D_AXIS_SWING1 5              // Motion around the Y axis
#define PX_6D_AXIS_SWING2 4              // Motion around the Z axis
#define PX_6D_AXIS_COUNT 6

// Motion
#define PX_6D_MOTION_LOCKED 0            // Motion is locked    
#define PX_6D_MOTION_LIMITED 1           // Motion is limited
#define PX_6D_MOTION_FREE 2              // Motion is free

//////////////////////////////////////////////////////////
// Vehicles
#define PX_VEHICLE_TYPE_DRIVE4W   0      // 4W Vehicle
#define PX_VEHICLE_TYPE_DRIVETANK 1      // NW Vehicle (TANK)

// Differential
#define PX_VEHICLE_DIFF_TYPE_LS_4WD         0
#define PX_VEHICLE_DIFF_TYPE_LS_FRONTWD     1
#define PX_VEHICLE_DIFF_TYPE_LS_REARWD      2
#define PX_VEHICLE_DIFF_TYPE_OPEN_4WD       3 
#define PX_VEHICLE_DIFF_TYPE_OPEN_FRONTWD   4 
#define PX_VEHICLE_DIFF_TYPE_OPEN_REARWD    5

// Gears
#define PX_VEHICLE_GEAR_REVERSE             0
#define PX_VEHICLE_GEAR_NEUTRAL             1 
#define PX_VEHICLE_GEAR_FIRST               2
#define PX_VEHICLE_GEAR_SECOND              3
#define PX_VEHICLE_GEAR_THIRD               4
#define PX_VEHICLE_GEAR_FOURTH              5 
#define PX_VEHICLE_GEAR_FIFTH               6 
#define PX_VEHICLE_GEAR_SIXTH               7 
#define PX_VEHICLE_GEAR_SEVENTH             8 
#define PX_VEHICLE_GEAR_EIGHTH              9 
#define PX_VEHICLE_GEAR_NINTH               10 
#define PX_VEHICLE_GEAR_TENTH               11 
#define PX_VEHICLE_GEAR_ELEVENTH            12 
#define PX_VEHICLE_GEAR_TWELFTH             13 
#define PX_VEHICLE_GEAR_THIRTEENTH          14
#define PX_VEHICLE_GEAR_FOURTEENTH          15 
#define PX_VEHICLE_GEAR_FIFTEENTH           16
#define PX_VEHICLE_GEAR_SIXTEENTH           17
#define PX_VEHICLE_GEAR_SEVENTEENTH         18 
#define PX_VEHICLE_GEAR_EIGHTEENTH          19 
#define PX_VEHICLE_GEAR_NINETEENTH          20 
#define PX_VEHICLE_GEAR_TWENTIETH           21 
#define PX_VEHICLE_GEAR_TWENTYFIRST         22 
#define PX_VEHICLE_GEAR_TWENTYSECOND        23 
#define PX_VEHICLE_GEAR_TWENTYTHIRD         24 
#define PX_VEHICLE_GEAR_TWENTYFOURTH        25 
#define PX_VEHICLE_GEAR_TWENTYFIFTH         26 
#define PX_VEHICLE_GEAR_TWENTYSIXTH         27 
#define PX_VEHICLE_GEAR_TWENTYSEVENTH       28 
#define PX_VEHICLE_GEAR_TWENTYEIGHTH        29 
#define PX_VEHICLE_GEAR_TWENTYNINTH         30 
#define PX_VEHICLE_GEAR_THIRTIETH           31 

// Analog Inupt
#define PX_VEHICLE_ANALOG_INPUT_ACCEL         0
#define PX_VEHICLE_ANALOG_INPUT_BRAKE         1
#define PX_VEHICLE_ANALOG_INPUT_HANDBRAKE     2
#define PX_VEHICLE_ANALOG_INPUT_STEER         3

// Vehicle Drive type
#define PX_VEHICLE_DRIVE_DIGITAL          0
#define PX_VEHICLE_DRIVE_ANALOG           1

// 4W Drive input
#define PX_VEHICLE_4W_DRIVE_CLEAR        -1
#define PX_VEHICLE_4W_DRIVE_ACCEL         0
#define PX_VEHICLE_4W_DRIVE_BRAKE         1
#define PX_VEHICLE_4W_DRIVE_HANDBRAKE     2
#define PX_VEHICLE_4W_DRIVE_STEER         3
#define PX_VEHICLE_4W_DRIVE_GEAR_UP       4
#define PX_VEHICLE_4W_DRIVE_GEAR_DOWN     5

// Tank Drive input
#define PX_VEHICLE_4W_DRIVE_CLEAR           -1
#define PX_VEHICLE_TANK_DRIVE_ACCEL          0
#define PX_VEHICLE_TANK_DRIVE_BRAKE_LEFT     1
#define PX_VEHICLE_TANK_DRIVE_BRAKE_RIGHT    2
#define PX_VEHICLE_TANK_DRIVE_THRUST_LEFT    3
#define PX_VEHICLE_TANK_DRIVE_THRUST_RIGHT   4
#define PX_VEHICLE_TANK_DRIVE_GEAR_UP        5
#define PX_VEHICLE_TANK_DRIVE_GEAR_DOWN      6

// 4W Wheel ordering
#define PX_VEHICLE_FRONT_LEFT_WHEEL         0
#define PX_VEHICLE_FRONT_RIGHT_WHEEL        1
#define PX_VEHICLE_REAR_LEFT_WHEEL          2
#define PX_VEHICLE_REAR_RIGHT_WHEEL         3

// Tank Wheel ordering
#define PX_VEHICLE_TANK_WHEEL_FRONT_LEFT              0
#define PX_VEHICLE_TANK_WHEEL_FRONT_RIGHT             1
#define PX_VEHICLE_TANK_WHEEL_1ST_FROM_FRONT_LEFT     2
#define PX_VEHICLE_TANK_WHEEL_1ST_FROM_FRONT_RIGHT    3
#define PX_VEHICLE_TANK_WHEEL_2ND_FROM_FRONT_LEFT     4
#define PX_VEHICLE_TANK_WHEEL_2ND_FROM_FRONT_RIGHT    5
#define PX_VEHICLE_TANK_WHEEL_3RD_FROM_FRONT_LEFT     6
#define PX_VEHICLE_TANK_WHEEL_3RD_FROM_FRONT_RIGHT    7
#define PX_VEHICLE_TANK_WHEEL_4TH_FROM_FRONT_LEFT     8
#define PX_VEHICLE_TANK_WHEEL_4TH_FROM_FRONT_RIGHT    9
#define PX_VEHICLE_TANK_WHEEL_5TH_FROM_FRONT_LEFT     10
#define PX_VEHICLE_TANK_WHEEL_5TH_FROM_FRONT_RIGHT    11
#define PX_VEHICLE_TANK_WHEEL_6TH_FROM_FRONT_LEFT     12
#define PX_VEHICLE_TANK_WHEEL_6TH_FROM_FRONT_RIGHT    13
#define PX_VEHICLE_TANK_WHEEL_7TH_FROM_FRONT_LEFT     14
#define PX_VEHICLE_TANK_WHEEL_7TH_FROM_FRONT_RIGHT    15
#define PX_VEHICLE_TANK_WHEEL_8TH_FROM_FRONT_LEFT     16
#define PX_VEHICLE_TANK_WHEEL_8TH_FROM_FRONT_RIGHT    17
#define PX_VEHICLE_TANK_WHEEL_9TH_FROM_FRONT_LEFT     18
#define PX_VEHICLE_TANK_WHEEL_9TH_FROM_FRONT_RIGHT    19


/////////////////////////////////////////////////////////

/// \brief Run physx simulation
///
/// @param deltatime
/// @return PxScene pointer
void* physX3_run(var deltatime);

/// \brief Open physX engine
///
/// @return PxPhysics pointer
void* physX3_load();

/// \brief Destroy PhysX
void physX3_destroy();

/// \brief register/unregister entity from physX simulation
///
/// @param entity
/// @param body
/// @param hull
var pX3ent_settype(ENTITY *entity, var body, var hull);


/// \brief Get physX stats
///
/// @param num 1 = return the number of actors in the level
/// @return If 1, Then return the number of actors in the level
var pX3_stats(var num);

/// \brief Set physX step rate
///
/// @param step The new step rate
/// @param maxIter The maximul iteration. The default is 8
/// @param method The Stepping method PX_TIMESTEP_FIXED or PX_TIMESTEP_VARIABLE
/// @return 1 if successful 0 otherwise
var pX3_setsteprate(var step, var maxIter, var method);

/// \brief Continus collision detection.
/// 
/// Px3ent_setCCDSkeleton(entity) to see the effect
/// @param active The activation value
/// @return Nonzeto if successful, 0 otherwise
var pX3_setccd(var active);

/// \brief Set the world scale
///
/// Modify the scale used to convert Acknex objects. A good value is aroung 0.05. Default value is 1/40
/// @param size The new scale to apply
/// @return 1 if success and 0 if not
var pX3_setunit(var size);

/// \brief Pause/Unpause the physX simulation
///
/// @param pause The pause state
void pX3_pause(var pause);

/// \brief Set collision filtering between two groups
///
/// Set the two groups and the collision flag
/// @param group1 The first group
/// @param group2 The second group
/// @return Nonzero if successfull and 0 if not
var pX3_setgroupcollision(var group1, var group2, var flag);

/// \brief Set force mode 
///
/// Affects: pXent_addcentralforce, pXent_addforceglobal, pXent_addforcelocal,...
/// @param force The force mode
void pX3_setforcemode(var force);

/// \brief Set or disable a trigger flag
///
/// @param entity The entity to be used as a trigger
/// @param type The type of trigger to manipulate not used
/// @param flag The activation of the flag (on or off)
/// @param return 1 if successful or 0
var pX3ent_settriggerflag(ENTITY *entity, var type, var flag);

/// \brief Activates or deactivates flag on an physX actor
///
/// @param entity The entity to be modified
/// @param bodyFlag The flag to manipulate
/// @param flag The value of the flag
/// @return 1 if successful 
var pX3ent_setbodyflag(ENTITY *entity, var bodyFlag, var flag);

/// \brief Set CCD Flag
///
/// Set ccd flag on all entity's shapes
/// @param entity The entity to be manipulated
/// @param flag The value of the ccd
var pX3ent_setccdflag(ENTITY *entity, var flag);

/// \brief Set actor's iteration count
///
/// Range 1 - 255
/// default 4 position, 1 velocity
/// @param entity The physX entity
/// @param count The new iteration count
/// @return 1 if successful 0 otherwise
var pX3ent_setiterationcount(ENTITY *entity, var position, var velocity);
#define pX3ent_setiterations(e, c) pX3ent_setiterationcount(e, c, 1)

/// \brief Set actor's shapes contact offset
///
/// Default 0.02f
/// @param entity The physX actor
/// @param skinwidth The new skinwidth
/// @return 1 if successful 0 otherwise
var pX3ent_setcontactoffset(ENTITY *entity, var offset);
#define pX3ent_setskinwidth pX3ent_setcontactoffset

/// \brief Set actor to sleep if 1 and wake him up with 0
///
/// @param entity The entity to manipulate
/// @param sleep The sleeping flag
/// @return 1 if successful or 0 if not
var pX3ent_setsleep(ENTITY *entity, var sleep);

/// \brief Set collision flag between two entities
///
/// @param entity1 The first entity
/// @param entity2 The second entity
/// @param flag The collision flag value
/// @return 1 if successfull or 0
var pX3ent_setcollisionflag(ENTITY *entity1, ENTITY *entity2, var flag);

/// \brief Use an entity's dimentions to add new shape to the entity
///
/// @param entity The actor to be modified
/// @param shapeEnt The shape entity
/// @param hull hull type
/// @return The shape pointer if successful or NULL
void *pX3ent_addshape(ENTITY *entity, ENTITY *shapeEnt, var hull);

/// \brief Removes a shape from an actor
///
/// @param entity The physics entity
/// @param shapenum The shape number beggining with 0
/// @param return 1 if successful 0 otherwise
var pX3ent_removeshape(ENTITY *entity, var shapenum);

/// \brief Place actor directly in the scene
///
/// @param entity The actor to be placed
/// @param pos The new position
var pX3ent_setposition(ENTITY *entity, VECTOR *pos);

/// \brief Set linear velocity of an actor
///
/// @param entity The physX actor
/// @param speed The velocity to be applied
/// @return 1 if fuccessful 0 otherwise
var pX3ent_setvelocity(ENTITY *entity, VECTOR *speed);

/// \brief Get Linear velocity of an actor
///
/// If localPoint is NULL or equals to vector(0, 0, 0), return the velocity of the object. Otherwise the local point velocity.
/// @param entity A registered physX entity
/// @param outVelocity The wictor that will receive the velocity
/// @param localPoint If localPoint is NULL or equals to vector(0, 0, 0), return the velocity of the object. Otherwise the local point velocity.
/// @return Nonzero if fuccessful 0 otherwise
var pX3ent_getvelocity(ENTITY *entity, VECTOR *outVelocity, VECTOR *localPoint);

/// \brief Set angular velocity of an actor
///
/// @param entity The physX actor
/// @param speed The velocity to be applied
/// @return 1 if fuccessful 0 otherwise
var pX3ent_setangvelocity(ENTITY *entity, VECTOR *speed);

/// \brief Retrieve angular velocity of a physX entity
///
/// @param entity A registered physX actor
/// @param outVelocity Actual angular velocity
/// @return Nonzero if successful 0 otherwise
var pX3ent_getangvelocity(ENTITY *entity, VECTOR *outVelocity);

/// \brief Move a character or a kinematic entity
///
/// RigidBodies different that are not kinematic or character are displaced directly
/// @param entity The physX entity
/// @param vecRelPos Relative position vector or NULL
/// @param vecAbsPos Absolute position vector
/// @return Non zero on PH_CHAR collisions 0 otherwise
var pX3ent_move(ENTITY *entity, VECTOR *vecRelPos, VECTOR *vecAbsPos);

/// \brief Rotate a character around relative or absolute angle
///
/// @param entity The physX actor
/// @param vecRelAngle Relative angle
/// @param vecAbsAngle Absolute angle
/// @return Non zero on PH_CHAR collisions 0 otherwise
var pX3ent_rotate(ENTITY *entity, ANGLE *vecRelAngle, ANGLE *vecAbsAngle);

/// \brief Set entity's group
///
/// @param entity A registered physX actor
/// @param group The new group for the entity
/// @return 1 if successful 0 otherwise
var pX3ent_setgroup(ENTITY *entity, var group);

/// \brief Get the actor's current group
///
/// @param entity A registered physX entity
/// @return entities'group or -1
var pX3ent_getgroup(ENTITY *entity);

/// \brief Create a complex material
///
/// @param standard Material parameters. range : 0-100
/// @param combine Friction combine vector
/// @return material Id if successful, 0 otherwise
var pX3_creatematerial(VECTOR *standard, VECTOR *combineFlags);

/// \brief Delete a user created material. Material id must be a valid material id and above 0.
///
/// The material will be removed by the sdk when no shape is using it.
/// @param materialId The id of the material to remove.
/// @return nonzero if successful, 0 otherwise
var pX3_deletematerial(var materialId);

/// \brief Set a complex material on an actor
///
/// @param entity A registered physX entity
/// @param standard Material parameters. range : 0-100
/// @param combine Friction combine vector
/// @return nonzero if successful 0 otherwise
var pX3ent_setmaterial(ENTITY *entity, VECTOR *standard, VECTOR *combineFlags);

/// \brief Set a material defined by its Id on an entity
///
/// The id must be a valid Id
/// @param entity A registered physX actor
/// @param materialId The Id of the material
/// @return the material id if successful, 0 otherwise
var pX3ent_setmaterialid(ENTITY *entity, var materialId);

/// \brief Creates a radial explosion
///
/// @param entity A registered physX actor
/// @param vPos explosion position
/// @param force The explosion strength
/// @param radius The radius of the explosion
/// @return Nonzero if successfull, zero otherwise
var pX3ent_addexplosion(ENTITY *entity, VECTOR *vPos, var force, var radius);

/// \brief Enable/Disable entity from scene queries
///
// Set Flag on all entitie's shape
/// @param entity A registered physX actor
/// @param enable The enable flag. 1 to enable raycast for the entity 0 to disable
/// @return 1 if successfull 0 otherwise
var pX3ent_raycastenable(ENTITY *entity, var enable);

/// \brief Perform a raycast against all shapes. Modifies the you pointer
///
/// A pointer to the found entity is saved to the last parameter if set
/// If a shape intersects the ray, the physX entity is saved in the you pointer
/// @param vPos The starting point of the ray
/// @param vDir The direction vector
/// @param distance The distance to be tested, max distance is used if 0 is provided
/// @param flags The flag to be used for filtering (DYNAMIC|STATIC Shapes) 0 to use all shapes
/// @param groups The groups bitmask to be used for filtering, 0 to use all groups. groupMask = (1 << (group - 1))
/// @param hitEntity[out] A pointer to store the found entity or NULL. The entity can be foud on the you pointer
/// @return 1 if a shape is hit 0 otherwise
var pX3_raycastany(VECTOR *vPos, VECTOR *vDir, var distance, var flags, var groups, ENTITY **hitEntity);

/// \brief Perform a raycast against all shapes. Modifies the you pointer, the hit and target structure if an actor has been found
///
/// If a shape intersects the ray, the physX entity is saved in the you pointer
/// @param vPos The starting point of the ray
/// @param vDir The direction vector
/// @param distance The distance to be tested, max distance is used if 0 is provided
/// @param flags The flag to be used for filtering (DYNAMIC|STATIC Shapes) 0 to use all shapes
/// @param groups The groups bitmask to be used for filtering, 0 to use all groups. groupMask = (1 << (group - 1))
/// @param outFlags Specifies which properties would be written to the hit information
/// @return If DISTANCE is queried, returns the distance of the hit target (you pointer ot hit.entity can be used to test if an actor has been hit), otherwise returns the result of the raycast (Nonzero if an actor is hit, 0 otherwise)
var pX3_raycastsingle(VECTOR *vPos, VECTOR *vDir, var distance, var flags, var groups, var outFlags);

/// \brief Perform a raycast against all shapes.
///
/// The ray does not stop on the first actor found. Engine pointers (target, you, hit, normal) values are set to the last values of the last element in the hit structure.
/// @param vPos The starting point of the ray
/// @param vDir The direction vector
/// @param distance The distance to be tested, max distance is used if 0 is provided
/// @param flags The flag to be used for filtering (DYNAMIC|STATIC Shapes) 0 to use all shapes
/// @param groups The groups bitmask to be used for filtering, 0 to use all groups. groupMask = (1 << (group - 1))
/// @param outFlags Specifies which properties would be written to the hit information
/// @param contactBuffer RayCast hit information buffer
/// @param bufferSize The size of the hit buffer
/// @param blocking[out] Nonzero if a blocking hit was found. If found, it is the last in the buffer, preceded by any touching hits which are closer. Otherwise the touching hits are listed. 
/// @return Number of hits in the buffer or -1 if the buffer overflowed
var pX3_raycastmultiple(VECTOR *vPos, VECTOR *vDir, var distance, var flags, var groups, var outFlags, CONTACT *contactBuffer, var bufferSize, var *blocking);

/// \brief Perform a sphere sweep against all shapes in the scene. Returns any blocking hit, not necessarily the closest
///
/// A pointer to the found entity is saved to the last parameter if set. The you pointer is also modified if an actor intersects the sweep test.
/// @param vOrigin The origin of the sphere
/// @param radius The radius of the sphere
/// @param vDir The direction of the sweep
/// @param distance The distance to be tested, max distance is used if 0 is provided
/// @param flags The flag to be used for filtering (DYNAMIC|STATIC Shapes) 0 to use all shapes
/// @param groups The groups bitmask to be used for filtering, 0 to use all groups. groupMask = (1 << (group - 1))
/// @param outFlags Specifies which properties would be written to the hit information
/// @param hitEntity[out] A pointer to store the found entity or NULL. The entity can be foud on the you pointer
/// @return 1 if a shape is hit 0 otherwise
var pX3_spherecastany(VECTOR *vOrigin, var radius, VECTOR *vDir, var distance, var flags, var groups, var outFlags, ENTITY **hitEntity);

/// \brief Perform a capsule sweep against all shapes in the scene. Returns any blocking hit, not necessarily the closest
///
/// A pointer to the found entity is saved to the last parameter if set. The you pointer is also modified if an actor intersects the sweep test.
/// @param vOrigin The origin of the sphere
/// @param radius The radius of the sphere
/// @param vDir The direction of the sweep
/// @param distance The distance to be tested, max distance is used if 0 is provided
/// @param flags The flag to be used for filtering (DYNAMIC|STATIC Shapes) 0 to use all shapes
/// @param groups The groups bitmask to be used for filtering, 0 to use all groups. groupMask = (1 << (group - 1))
/// @param outFlags Specifies which properties would be written to the hit information
/// @param hitEntity[out] A pointer to store the found entity or NULL. The entity can be foud on the you pointer
/// @return 1 if a shape is hit 0 otherwise
var pX3_capsulecastany(VECTOR *vStart, VECTOR *vEnd, var radius, VECTOR *vDir, var distance, var flags, var groups, var outFlags, ENTITY **hitEntity);

/// \brief Perform a sphere sweep against all shapes in the scene. Returns the first blocking hit.
///
/// The you pointer is modified if an actor intersects the sweep test.
/// @param vOrigin The origin of the sphere
/// @param radius The radius of the sphere
/// @param vDir The direction of the sweep
/// @param distance The distance to be tested, max distance is used if 0 is provided
/// @param flags The flag to be used for filtering (DYNAMIC|STATIC Shapes) 0 to use all shapes
/// @param groups The groups bitmask to be used for filtering, 0 to use all groups. groupMask = (1 << (group - 1))
/// @param outFlags Specifies which properties would be written to the hit information
/// @return 1 if a shape is hit 0 otherwise
var pX3_spherecastsingle(VECTOR *vOrigin, var radius, VECTOR *vDir, var distance, var flags, var groups, var outFlags);

/// \brief Perform a capsule sweep against all shapes in the scene. Returns the first blocking hit.
///
/// The you pointer is modified if an actor intersects the sweep test.
/// @param vOrigin The origin of the sphere
/// @param radius The radius of the sphere
/// @param vDir The direction of the sweep
/// @param distance The distance to be tested, max distance is used if 0 is provided
/// @param flags The flag to be used for filtering (DYNAMIC|STATIC Shapes) 0 to use all shapes
/// @param groups The groups bitmask to be used for filtering, 0 to use all groups. groupMask = (1 << (group - 1))
/// @param outFlags Specifies which properties would be written to the hit information
/// @return 1 if a shape is hit 0 otherwise
var pX3_capsulecastsingle(VECTOR *vStart, VECTOR *vEnd, var radius, VECTOR *vDir, var distance, var flags, var groups, var outFlags);

/// \brief Perform a sweep test against all shapes.
///
/// The ray does not stop on the first actor. Engine pointers values are set to the last value of the last element in the hit structure.
/// The you pointer is modified if an actor intersects the sweep test.
/// @param vOrigin The origin of the sphere
/// @param radius The radius of the sphere
/// @param vDir The direction of the sweep
/// @param distance The distance to be tested, max distance is used if 0 is provided
/// @param flags The flag to be used for filtering (DYNAMIC|STATIC Shapes) 0 to use all shapes
/// @param groups The groups bitmask to be used for filtering, 0 to use all groups. groupMask = (1 << (group - 1))
/// @param outFlags Specifies which properties would be written to the hit information
/// @param contactBuffer RayCast hit information buffer
/// @param bufferSize The size of the hit buffer
/// @param blocking[out] Nonzero if a blocking hit was found. If found, it is the last in the buffer, preceded by any touching hits which are closer. Otherwise the touching hits are listed. 
/// @return 1 if a shape is hit 0 otherwise
var pX3_spherecastmultiple(VECTOR *vOrigin, var radius, VECTOR *vDir, var distance, var flags, var groups, var outFlags,  CONTACT *contactBuffer, var bufferSize, var *blocking);

/// \brief Perform a capsule sweep test against all shapes.
///
/// The ray does not stop on the first actor. Engine pointers values are set to the last value of the last element in the hit structure.
/// The you pointer is modified if an actor intersects the sweep test.
/// @param vStart The start of the capsule
/// @param vEnd The end point of the capsule
/// @param radius The radius of the capsule
/// @param vDir The direction of the sweep
/// @param distance The distance to be tested, max distance is used if 0 is provided
/// @param flags The flag to be used for filtering (DYNAMIC|STATIC Shapes) 0 to use all shapes
/// @param groups The groups bitmask to be used for filtering, 0 to use all groups. groupMask = (1 << (group - 1))
/// @param outFlags Specifies which properties would be written to the hit information
/// @param contactBuffer RayCast hit information buffer
/// @param bufferSize The size of the hit buffer
/// @param blocking[out] Nonzero if a blocking hit was found. If found, it is the last in the buffer, preceded by any touching hits which are closer. Otherwise the touching hits are listed. 
/// @return 1 if a shape is hit 0 otherwise
var pX3_capsulecastmultiple(VECTOR *vStart, VECTOR *vEnd, var radius, VECTOR *vDir, var distance, var flags, var groups, var outFlags,  CONTACT *contactBuffer, var bufferSize, var *blocking);

/// \brief Overlap test using a sphere.
///
/// Can be used to check if a space is free or occupied.
/// Modify you, target, hit.
/// @param vOrigin The origin of the sphere in world coordinate
/// @param radius The radisu of the sphere
/// @param flags The flag to be used for filtering (DYNAMIC|STATIC Shapes) 0 to use all shapes
/// @param groups The groups bitmask to be used for filtering, 0 to use all groups. groupMask = (1 << (group - 1))
/// @param overlapEntity[out] A pointer to store the found entity or NULL. The entity can be foud on the you pointer
/// @return Nonzero if there are shapes touching the sphere 0 otherwise
var pX3_spherecheckany(VECTOR *vOrigin, var radius, var flags, var groups, ENTITY **hitEntity);

/// \brief Overlap test using a capsule.
///
/// Can be used to check if a space is free or occupied.
/// Modify you, target, hit.
/// @param vStart The start point of the capsule
/// @param vEnd The end point of the capsule
/// @param radius The radisu of the sphere
/// @param flags The flag to be used for filtering (DYNAMIC|STATIC Shapes) 0 to use all shapes
/// @param groups The groups bitmask to be used for filtering, 0 to use all groups. groupMask = (1 << (group - 1))
/// @param overlapEntity[out] A pointer to store the found entity or NULL. The entity can be foud on the you pointer
/// @return Nonzero if there are shapes touching the capsule 0 otherwise
var pX3_capsulecheckany(VECTOR *vStart, VECTOR *vEnd, var radius, var flags, var groups, ENTITY **hitEntity);

/// \brief Overlap test using a sphere.
///
/// Can be used to check if a space is free or occupied.
/// Modify you, target, hit.
/// @param geometry The input geometry
/// @param transform The geometry transform
/// @param flags The flag to be used for filtering (DYNAMIC|STATIC Shapes) 0 to use all shapes
/// @param groups The groups bitmask to be used for filtering, 0 to use all groups. groupMask = (1 << (group - 1))
/// @param contactBuffer overlap hit information buffer
/// @param bufferSize The size of the hit buffer
/// @return Nonzero if there are shapes touching the geometry 0 otherwise
var pX3_spherecheckmultiple(VECTOR *vOrigin, var radius, var flags, var groups, CONTACT *contactBuffer, var bufferSize);

/// \brief Overlap test using a geometry.
///
/// Can be used to check if a space is free or occupied.
/// Modify you, target, hit.
/// @param vStart The start point of the capsule
/// @param vEnd The end point of the capsule
/// @param flags The flag to be used for filtering (DYNAMIC|STATIC Shapes) 0 to use all shapes
/// @param groups The groups bitmask to be used for filtering, 0 to use all groups. groupMask = (1 << (group - 1))
/// @param contactBuffer overlap hit information buffer
/// @param bufferSize The size of the hit buffer
/// @return Nonzero if there are shapes touching the geometry 0 otherwise
var pX3_capsulecheckmultiple(VECTOR *vStart, VECTOR *vEnd, var radius, var flags, var groups, CONTACT *contactBuffer, var bufferSize);

/// \brief Set simulation gravity
///
/// Gravity can be changed anytime
/// @param gravity The new gravity value. default vector(0.0, 0.0, -9.81);
/// return Nonzero if successful 0 otherwise
var pX3_setgravity(VECTOR *gravity);

/// \brief Sets the mass-normalized kinetic energy threshold below which an actor may go to sleep. 
///
/// Actors whose kinetic energy divided by their mass is above this threshold will not be put to sleep.
/// @param entity A registered PhysX entity
/// @param thresshold Energy below which an actor may go to sleep. Range: (0,inf] 
/// @return Nonzero if successful 0 otherwise
var pX3ent_setsleepthreshold(ENTITY *entity, var threshold);
#define pX3ent_setautodisable(e, l, v) pX3ent_setsleepthreshold(e, l)

/// \brief Gets the mass-normalized kinetic energy threshold below which an actor may go to sleep. 
///
/// Actors whose kinetic energy divided by their mass is above this threshold will not be put to sleep.
/// @param entity A registered PhysX entity
/// @return Nonzero if successful 0 otherwise
var pX3ent_getsleepthreshold(ENTITY *entity);

/// \brief Apply a linear force to the entity's center of mass.
///
/// @param entity A registered physX actor
/// @param force Vector determining the force to be exerted on the entity at its center of mass
/// @param Nonzero if successful 0 otherwise
var pX3ent_addforce(ENTITY *entity, VECTOR *force);
#define pX3ent_addforcecentral(e, f) pX3ent_addforce(e, f)

/// \brief Applies a force (or impulse) defined in the global coordinate frame, acting at a particular point in global coordinates, to the actor. 
///
/// Note that if the force does not act along the center of mass of the actor, this will also add the corresponding torque.
/// Because forces are reset at the end of every timestep, you can maintain a total external force on an object by calling this once every frame.
/// @param entity A registered physX actor
/// @param force Vector determining the force to be exerted on the entity at its center of mass
/// @param pos Vector specifying where the force will be applied
/// @param Nonzero if successful 0 otherwise
var pX3ent_addforceatpos(ENTITY *entity, VECTOR *force, VECTOR *pos);
#define pX3ent_addforceglobal(e, f, p) pX3ent_addforceatpos(e, f, p)

/// \brief Applies a force (or impulse) defined in the local coordinate frame, acting at a particular point in local coordinates, to the actor. 
///
/// Note that if the force does not act along the center of mass of the actor, this will also add the corresponding torque.
/// Because forces are reset at the end of every timestep, you can maintain a total external force on an object by calling this once every frame.
/// @param entity A registered physX actor
/// @param force Vector determining the force to be exerted on the entity at its center of mass
/// @param pos Vector specifying where the force will be applied
/// @param Nonzero if successful 0 otherwise
var pX3ent_addforceatlocalpos(ENTITY *entity, VECTOR *force, VECTOR *pos);
#define pX3ent_addforcelocal(e, f, p) pX3ent_addforceatlocalpos(e, f, p)

/// \brief Applies a force (or impulse) defined in the local coordinate frame, acting at a particular point in global coordinates, to the actor. 
///
/// Note that if the force does not act along the center of mass of the actor, this will also add the corresponding torque.
/// Because forces are reset at the end of every timestep, you can maintain a total external force on an object by calling this once every frame.
/// @param entity A registered physX actor
/// @param force Vector determining the force defined in local frame to be exerted on the entity
/// @param pos Vector specifying where the force defined in global frame will be applied
/// @param Nonzero if successful 0 otherwise
var pX3ent_addlocalforceatpos(ENTITY *entity, VECTOR *force, VECTOR *pos);
#define pX3ent_addlocalforceglobal(e, f, p) pX3ent_addlocalforceatpos(e, f, p)

/// \brief Applies a force (or impulse) defined in the local coordinate frame, acting at a particular point in local coordinates, to the actor. 
///
/// Note that if the force does not act along the center of mass of the actor, this will also add the corresponding torque.
/// Because forces are reset at the end of every timestep, you can maintain a total external force on an object by calling this once every frame.
/// @param entity A registered physX actor
/// @param force Vector determining the force defined in local frame to be exerted on the entity
/// @param pos Vector specifying where the force defined in local frame will be applied
/// @param Nonzero if successful 0 otherwise
var pX3ent_addlocalforceatlocalpos(ENTITY *entity, VECTOR *force, VECTOR *pos);
#define pX3ent_addlocalforcelocal(e, f, p) pX3ent_addlocalforceatlocalpos(e, f, p)

/// \brief Add alinear velocity to the entity's center of mass
///
/// @param entity A registered physX entity
/// @param vSpeed A vector determining the speed to be applied
/// @param Nonzero if successful 0 otherwise
var pX3ent_addvelcentral(ENTITY *entity, VECTOR *vSpeed);

/// \brief Applies an impulsive torque defined in the global coordinate frame to the actor. 
///
/// @param entity A registered physX actor
/// @param vTorque Torque to apply defined in the global frame. Range: torque vector 
/// @return Nonzero if successful 0 otherwise
var pX3ent_addtorque(ENTITY *entity, VECTOR *vTorque);
#define pX3ent_addtorqueglobal(e, t) pX3ent_addtorque(e, t)

/// \brief Applies an impulsive torque defined in the local coordinate frame to the actor. 
///
/// @param entity A registered physX actor
/// @param vTorque Torque to apply defined in the local frame. Range: torque vector 
/// @return Nonzero if successful 0 otherwise
var pX3ent_addtorquelocal(ENTITY *entity, VECTOR *vTorque);

/// \brief set the maximum angular velocity permitted for this actor. 
///
/// @param entity A registered physX actor
/// @param maxAngVel Max allowable angular velocity for actor. Range: (0,inf). Default : 7
/// @return Nonzero if successful 0 otherwise
var pX3ent_setmaxangularvelocity(ENTITY *entity, var maxAngVel);
#define pX3ent_setmaxspeed(e, s) pX3ent_setmaxangularvelocity(e, s)

/// \brief Sets the mass of a dynamic actor. 
///
/// The mass must be positive.
/// @param entity A registered physX actor
/// @param mass The new mass of the actor
/// @return Entity mass if successful, 0 otherwise
var pX3ent_setmass(ENTITY *entity, var mass);

/// \brief Gets the mass of a dynamic actor. 
///
/// @param entity A registered physX actor
/// @return Entity mass if successful, 0 otherwise
var pX3ent_getmass(ENTITY *entity);

/// \brief Sets the density of the dynamic actor. The mass will be calculated automatically. 
///
/// The density must be positive.
/// @param entity A registered physX actor
/// @param density The new density of the actor. If 0 is supplied, the density will be set automatically to 1
/// @return Entity mass if successful, 0 otherwise
var pX3ent_setdensity(ENTITY *entity, var density);

/// \brief Sets the local position of the center of mass, and the relative inertia tensor.
///
/// @param entity A registered physX actor
/// @param offset The position of the mass in local frame.
/// @param inertia A vector containing relative inertia tensor.
/// @return Nonzero if successful, 0 otherwise
var pX3ent_setmassoffset(ENTITY *entity, VECTOR *offset, VECTOR *inertia);

/// \brief Gets the local position of the center of mass.
///
/// @param entity A registered physX actor
/// @param offset[out] The position of the mass in local frame.
/// @param inertia[out] A vector containing relative inertia tensor.
/// @return Nonzero if successful, 0 otherwise
var pX3ent_getmassoffset(ENTITY *entity, VECTOR *offset, VECTOR *inertia);

/// \brief Temporarily disable an entity from physX. Can be used to displace entity directly.
///
/// @param entity A registered physX actor.
/// @param enable A value of 1 recreates the actor, a value of 0 removes the actor from the simulation
/// @return Nonzero if successful, 0 otherwise
var pX3ent_enable(ENTITY *entity, var enable);

/// \brief Return the activation state of the entity
/// @param entity A registered physX actor.
/// @return Nonzero if entity is enabled, 0 otherwise
var pX3ent_getenable(ENTITY *entity);

/// \brief Set entity damping. Set linear and angular damping  value.
///
/// @param entity A registered physX entity
/// @param linear Linear damping coefficient. Range: [0,inf)
/// @param angular Angular damping coefficient. Range: [0,inf)
/// @return Nonzero if entity is enabled, 0 otherwise
var pX3ent_setdamping(ENTITY *entity, var linear, var angular);

/// \brief Get linear damping value
///
/// @param entity A reigstered physX entity
/// @return The linear damping value
var pX3ent_getlineardamping(ENTITY *entity);

/// \brief Get angular damping value
///
/// @param entity A reigstered physX entity
/// @return The angular damping value
var pX3ent_getangulardamping(ENTITY *entity);

/// \brief Set the static friction of the material attached to the entity.
///
/// If the material is attached to many actors, they will all be modified.
/// @param entity A registered physX actor
/// @param sfriction The new static friction value
/// @return Nonzero if succesful, 0 otherwise
var pX3ent_setstaticfriction(ENTITY *entity, var sfriction);

/// \brief Set the dynamic friction of the material attached to the entity.
///
/// If the material is attached to many actors, they will all be modified.
/// @param entity A registered physX actor
/// @param dfriction The new static friction value
/// @return Nonzero if succesful, 0 otherwise
var pX3ent_setdynamicfriction(ENTITY *entity, var dfriction);

/// \brief Set the static and dynamic frictions of the material attached to the entity.
///
/// If the material is attached to many actors, they will all be modified. Static and dynamic friction will have the same value.
/// @param entity A registered physX actor
/// @param friction The new friction value
/// @return Nonzero if succesful, 0 otherwise
var pX3ent_setfriction(ENTITY *entity, var friction) { var s = pX3ent_setstaticfriction(entity, friction); var d = pX3ent_setdynamicfriction(entity, friction); if ((s+d) > 0) return 1; return 0; }

/// \brief Set the restitution of the material attached to the entity.
///
/// If the material is attached to many actors, they will all be modified.
/// @param entity A registered physX actor
/// @param restitution The new restitution value
/// @return Nonzero if entity is enabled, 0 otherwise
var pX3ent_setrestitution(ENTITY *entity, var restitution);
#define pX3ent_setelasticity(e, r) pX3ent_setrestitution(e, r)

/// \brief Create a breakable joint on entity1 and entity2.
///
/// @param constraintType The type of constraint PH_HINGE, PH_BALL, PH_SLIDER, PH_ROPE, PH_6DJOINT
/// @param entity1 The first entity of constraint
/// @param entity2 Second entity or NULL for constraining to the world. Must be diffferent from entity1
/// @param internalCollision 1 let entity1 and entity2 collide, 0 for no internal collision.
/// @return The constraint index, or 0 if failed.
var pX3con_add(var constraintType, ENTITY *entity1, ENTITY *entity2, var internalCollision);

/// \brief Remove an existing constraint
///
/// @param index Joint index
/// @return Nonzero if successful, 0 otherwise
var pX3con_remove(int index);

/// \brief Get the entities attached to the joint
///
/// @param jointId The id of the joint
/// @param entity1 A pointer to an entity pointer to be set to the first entity
/// @param entity1 A pointer to an entity pointer to be set to the second
var pX3con_getactors(var jointId, ENTITY **entity1, ENTITY **entity2);

/// \brief Get the break force for the joint
///
/// @param jointId The id of the joint
/// @param force The maximum force the joint can apply before breaking. Set to null to ignore
/// @param torque The maximum torque the joint can apply before breaking. Set to null to ignore
/// @return Nonzero if successful, 0 otherwise
var pX3con_getbreakforce(var jointId, var *force, var *torque);

/// \brief Get constraint flags relative to the joint
///
/// @param jointId The id of the joint
/// @return The value of the constraint flag
var pX3con_getflags(var jointId);

/// \brief Retrieve the constraint force most recently applied to maintain this constraint. 
///
/// @param jointId The id of the joint
/// @param linear The constraint force
/// @param angular The constraint torque
/// @return Nonzero if successful, 0 otherwise
var pX3con_getforce(var jointId, VECTOR *linear, VECTOR *angular);

/// \brief Get the global position of the joint
///
/// @param jointId The id of the joint
/// @param position The global position of the joint
/// @param rotation The rotation of the joint normal
/// @return Nonzero if successful, 0 otherwise
var pX3con_getposition(var jointId, VECTOR *position, VECTOR *rotation);

/// \brief Get the entities attached to the joint
///
/// @param jointId The id of the joint
/// @param entity1 A pointer to an entity pointer to be set to the first entity. Set to NULL to a
/// @param entity2 A pointer to an entity pointer to be set to the second entity. Set to NULL to ignore
/// @return Nonzero if successful, 0 otherwise
var pX3con_setactors(var jointId, ENTITY *entity1, ENTITY *entity2);

/// \brief Set the break force for the joint
///
/// @param jointId The id of the joint
/// @param force The maximum force the joint can apply before breaking.
/// @param torque The maximum torque the joint can apply before breaking.
/// @return Nonzero if successful, 0 otherwise
var pX3con_setbreakforce(var jointId, var force, var torque);

/// \brief Set a particular joint flag
///
/// @param jointId The id of the joint
/// @param flag The flag to modify
/// @param value The activation state of the flag
/// @return 1 if successful, 0 otherwise.
var pX3con_setflag(var jointId, var flag, var value);

/// \brief Set all the joint flags at once
///
/// @param jointId The id of the joint
/// @param flag The new value of the flags
/// @return 1 if successful, 0 otherwise.
var pX3con_setflags(var jointId, var flags);

/// \brief Set the global position of the joint
///
/// @param jointId The id of the joint
/// @param position The global position of the joint
/// @param rotation The rotation of the joint normal
/// @return Nonzero if successful, 0 otherwise
var pX3con_setposition(var jointId, VECTOR *position, VECTOR *rotation);

/// \brief Get joint type
///
/// @param jointId The id of the joint
/// @return The type of the joint
var pX3con_gettype(var jointId);

/// \brief gets the maximum torque the drive can exert. 
///
/// This function will only work for revolute joints
/// @param jointId The id of the joint
/// @return Nonzero if successful, 0 otherwise.
var pX3con_hinge_getdriveforcelimit(var jointId);

/// \brief Get the gear ratio
///
/// This function will only work for revolute joints
/// @param jointId The id of the joint
/// @return The value of the gear ratio if successful, 0 otherwise.
var pX3con_hinge_getdrivegearratio(var jointId);

/// \brief Get the drive velocity
///
/// This function will only work for revolute joints
/// @param jointId The id of the joint
/// @return The value of the drive velocity if successful, 0 otherwise.
var pX3con_hinge_getdrivevelocity(var jointId);

/// \brief Get the joint limit parameters
///
/// This function will only work for revolute and prismatic joints
/// @param jointId The id of the joint
/// @param params1 Vector to receive the first set of parameters (lower, upper and distance parameters)
/// @param params2 Vector to receive the second set of parameters (restitution, spring and damping parameters)
/// @return Nonzero if successful, 0 otherwise.
var pX3con_hinge_getlimit(var jointId, VECTOR *params1, VECTOR *params2);

/// \brief Get the angular tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @return The value of the angular tolerance threshold
var pX3con_hinge_getprojangtolerance(var jointId);

/// \brief Get the linear tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @return The value of the linear tolerance threshold
var pX3con_hinge_getprojlineartolerance(var jointId);

/// \brief Get the flags relative to the joint
///
/// @param jointId The id of the joint
/// @return The value of flags
var pX3con_hinge_getflags(var jointId);

/// \brief Set the drive force limit value
///
/// @param jointId The id of the joint
/// @param limit The new limit value
/// @return Nonzero if successful, 0 otherwise
var pX3con_hinge_setdriveforcelimit(var jointId, var limit);

/// \brief Set the drive gear ratio value
///
/// @param jointId The id of the joint
/// @param limit The new drive gear ratio value
/// @return Nonzero if successful, 0 otherwise
var pX3con_hinge_setdrivegearratio(var jointId, var ratio);

/// \brief Set the drive velocity value.
///
/// Drive flag must be enabled for this to work
/// @param jointId The id of the joint
/// @param velocity The new velocity value
/// @return Nonzero if successful, 0 otherwise
var pX3con_hinge_setdrivevelocity(var jointId, var velocity);

/// \brief Set the joint limit parameters
///
/// This function will only work for revolute joints. THe limit flag must be enabled for this to work.
/// @param jointId The id of the joint
/// @param params1 Vector containing the first set of parameters (lower, upper and distance parameters). Set to NULL to ignore
/// @param params2 Vector containing the second set of parameters (restitution, spring and damping parameters). Set to NULL to ignore
/// @return Nonzero if successful, 0 otherwise.
var pX3con_hinge_setlimit(var jointId, VECTOR *params1, VECTOR *params2);

/// \brief Set the angular tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @param tolerance The value of the angular tolerance threshold in degree
/// @return 1 if successful, 0 otherwise.
var pX3con_hinge_setprojangtolerance(var jointId, var tolerance);

/// \brief Set the linear tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @param tolerance The value of the linear tolerance threshold in quants
/// @return 1 if successful, 0 otherwise.
var pX3con_hinge_setprojlineartolerance(var jointId, var tolerance);

/// \brief Set a particular hinge flag
///
/// @param jointId The id of the joint
/// @param flag The flag to modify
/// @param value The activation state of the flag
/// @return 1 if successful, 0 otherwise.
var pX3con_hinge_setflag(var jointId, var flag, var value);

/// \brief Set all the hinge flags at once
///
/// @param jointId The id of the joint
/// @param flag The new value of the flags
/// @return 1 if successful, 0 otherwise.
var pX3con_hinge_setflags(var jointId, var flags);

/// \brief Get the limit cone of the ball joint
///
/// This function will only work for sphere joints
/// @param jointId The id of the joint
/// @param params1 Vector to receive the first set of parameters (yangle, zangle and limitContactDistance parameters)
/// @param params2 Vector to receive the second set of parameters (restitution, spring and damping parameters)
/// @return Nonzero if successful, 0 otherwise.
var pX3con_ball_getlimitcone(var jointId, VECTOR *params1, VECTOR *params2);

/// \brief Get the linear tolerance threshold for the projection
///
/// The value is scaled to 3DGS scale
/// @param jointId The id of the joint
/// @return The value of the linear tolerance threshold
var pX3con_ball_getprojlineartolerance(var jointId);

/// \brief Get the flags relative to the joint
///
/// @param jointId The id of the joint
/// @return The value of flags
var pX3con_ball_getflags(var jointId);

/// \brief Set the joint limit cone parameters
///
/// This function will only work for spherical joints. THe limit flag must be enabled for this to work.
/// @param jointId The id of the joint
/// @param params1 Vector to receive the first set of parameters (yangle, zangle and limitContactDistance parameters). Set to NULL to ignore
/// @param params2 Vector containing the second set of parameters (restitution, spring and damping parameters). Set to NULL to ignore
/// @return Nonzero if successful, 0 otherwise.
var pX3con_ball_setlimitcone(var jointId, VECTOR *params1, VECTOR *params2);

/// \brief Set the linear tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @param tolerance The value of the linear tolerance threshold in quants
/// @return 1 if successful, 0 otherwise.
var pX3con_ball_setprojlineartolerance(var jointId, var tolerance);

/// \brief Set a particular ball joint flag
///
/// @param jointId The id of the joint
/// @param flag The flag to modify
/// @param value The activation state of the flag
/// @return 1 if successful, 0 otherwise.
var pX3con_ball_setflag(var jointId, var flag, var value);

/// \brief Set all the ball joint flags at once
///
/// @param jointId The id of the joint
/// @param flag The new value of the flags
/// @return 1 if successful, 0 otherwise.
var pX3con_ball_setflags(var jointId, var flags);

/// \brief Get the damping of the joint spring.  
///
/// This function will only work for distance joints
/// @param jointId The id of the joint
/// @return The value of the damping if successful, 0 otherwise.
var pX3con_rope_getdamping(var jointId);

/// \brief Get the flags relative to the joint
///
/// @param jointId The id of the joint
/// @return The value of flags
var pX3con_rope_getflags(var jointId);

/// \brief Get the allowed maximum distance for the joint.  
///
/// This function will only work for distance joints
/// @param jointId The id of the joint
/// @return The value of the maximum distance if successful, 0 otherwise.
var pX3con_rope_getmaxdistance(var jointId);

/// \brief Get the allowed minimum distance for the joint.  
///
/// This function will only work for distance joints
/// @param jointId The id of the joint
/// @return The value of the minimum distance if successful, 0 otherwise.
var pX3con_rope_getmindistance(var jointId);

/// \brief Get the strength of the joint spring. 
///
/// This function will only work for distance joints
/// @param jointId The id of the joint
/// @return The value of the spring strngth if successful, 0 otherwise.
var pX3con_rope_getspring(var jointId);

/// \brief Get the error tolerance of the joint. 
///
/// the distance beyond the joint's [min, max] range before the joint becomes active.
/// This function will only work for distance joints
/// @param jointId The id of the joint
/// @return The value of the spring strngth if successful, 0 otherwise.
var pX3con_rope_gettolerance(var jointId);

/// \brief Set the damping of the joint spring.  
///
/// This function will only work for distance joints
/// @param jointId The id of the joint
/// @return Nonzero if successful, 0 otherwise.
var pX3con_rope_setdamping(var jointId, var damping);

/// \brief Set a particular rope joint flag
///
/// @param jointId The id of the joint
/// @param flag The flag to modify
/// @param value The activation state of the flag
/// @return 1 if successful, 0 otherwise.
var pX3con_rope_setflag(var jointId, var flag, var value);

/// \brief Set all the rope joint flags at once
///
/// @param jointId The id of the joint
/// @param flags The new value of the flags
/// @return 1 if successful, 0 otherwise.
var pX3con_rope_setflags(var jointId, var flags);

/// \brief Set the allowed maximum distance for the joint.  
///
/// This function will only work for distance joints
/// @param jointId The id of the joint
/// @param maxDistance The value of the joint max distance
/// @return Nonzero if successful, 0 otherwise.
var pX3con_rope_setmaxdistance(var jointId, var maxDistance);

/// \brief Set the allowed minimum distance for the joint.  
///
/// This function will only work for distance joints
/// @param jointId The id of the joint
/// @param minDistance The value of the joint min distance
/// @return Nonzero if successful, 0 otherwise.
var pX3con_rope_setmindistance(var jointId, var minDistance);

/// \brief Set the force of the joint spring.  
///
/// This function will only work for distance joints
/// @param jointId The id of the joint
/// @param spring The value of the joint spring
/// @return Nonzero if successful, 0 otherwise.
var pX3con_rope_setspring(var jointId, var spring);

/// \brief Set the error tolerance of the joint.  
///
/// This function will only work for distance joints
/// @param jointId The id of the joint
/// @param tolerance The value of the joint tolerance
/// @return Nonzero if successful, 0 otherwise.
var pX3con_rope_settolerance(var jointId, var tolerance);

/// \brief Get the angular tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @return The value of the angular tolerance threshold in degree
var pX3con_fixed_getprojangtolerance(var jointId);

/// \brief Get the linear tolerance threshold for the projection
///
/// The value is scaled to 3DGS scale
/// @param jointId The id of the joint
/// @return The value of the linear tolerance threshold
var pX3con_fixed_getprojlineartolerance(var jointId);

/// \brief Set the angular tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @param tolerance The value of the angular tolerance threshold in degree
/// @return 1 if successful, 0 otherwise.
var pX3con_fixed_setprojangtolerance(var jointId, var tolerance);

/// \brief Set the linear tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @param tolerance The value of the linear tolerance threshold in quants
/// @return 1 if successful, 0 otherwise.
var pX3con_fixed_setprojlineartolerance(var jointId, var tolerance);

/// \brief Get the flags relative to the joint
///
/// @param jointId The id of the joint
/// @return The value of flags
var pX3con_slider_getflags(var jointId);

/// \brief Get the joint limit parameters
///
/// This function will only work for slider/prismatic joints
/// @param jointId The id of the joint
/// @param params1 Vector to receive the first set of parameters (lower, upper and distance parameters)
/// @param params2 Vector to receive the second set of parameters (restitution, spring and damping parameters)
/// @return Nonzero if successful, 0 otherwise.
var pX3con_slider_getlimit(var jointId, VECTOR *params1, VECTOR *params2);

/// \brief Get the angular tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @return The value of the angular tolerance threshold in degree
var pX3con_slider_getprojangtolerance(var jointId);

/// \brief Get the linear tolerance threshold for the projection
///
/// The value is scaled to 3DGS scale
/// @param jointId The id of the joint
/// @return The value of the linear tolerance threshold
var pX3con_slider_getprojlineartolerance(var jointId);

/// \brief Set the joint limit parameters
///
/// This function will only work for slider/prismatic joints. THe limit flag must be enabled for this to work.
/// @param jointId The id of the joint
/// @param params1 Vector containing the first set of parameters (lower, upper and distance parameters). Set to NULL to ignore
/// @param params2 Vector containing the second set of parameters (restitution, spring and damping parameters). Set to NULL to ignore
/// @return Nonzero if successful, 0 otherwise.
var pX3con_slider_setlimit(var jointId, VECTOR *params1, VECTOR *params2);

/// \brief Set the angular tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @param tolerance The value of the angular tolerance threshold in degree
/// @return 1 if successful, 0 otherwise.
var pX3con_slider_setprojangtolerance(var jointId, var tolerance);

/// \brief Set the linear tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @param tolerance The value of the linear tolerance threshold in quants
/// @return 1 if successful, 0 otherwise.
var pX3con_slider_setprojlineartolerance(var jointId, var tolerance);

/// \brief Set a particular hinge flag
///
/// @param jointId The id of the joint
/// @param flag The flag to modify
/// @param value The activation state of the flag
/// @return 1 if successful, 0 otherwise.
var pX3con_slider_setflag(var jointId, var flag, var value);

/// \brief Set all the hinge flags at once
///
/// @param jointId The id of the joint
/// @param flag The new value of the flags
/// @return 1 if successful, 0 otherwise.
var pX3con_slider_setflags(var jointId, var flags);

/// \brief Get the drive parameters for the specified drive type. 
///
/// This function only work with 6d joints
/// @param jointId The id of the joint
/// @param index The specified drive type
/// @param params The drive params. vector(drive.forceLimit, drive.spring, drive.damping)
/// @param flags The drive flags
/// @return Nonzero if successful, 0 otherwise
var pX3con_6d_getdrive(var jointId, var index, VECTOR *params, var *flags);

/// \brief Get the drive goal pose. 
///
/// This function only works with 6d joints
/// @param jointId The id of the joint
/// @param position The position of the drive in quants
/// @param rotation The rotation of the drive in degree
var pX3con_6d_getdriveposition(var jointId, VECTOR *position, VECTOR *rotation);

/// \brief Get the target goal velocity for joint drive. 
///
/// This function only works with 6d joints
/// @param jointId The id of the joint
/// @param linear The goal velocity for linear drive 
/// @param angular The goal velocity for angular drive
var pX3con_6d_getdrivevelocity(var jointId, VECTOR *linear, VECTOR *angular);

/// \brief Get the linear limit for the joint. 
///
/// This function will only work for 6d joint
/// @param jointId The id of the joint
/// @param params1 Vector to receive the first set of parameters (value, 0 and distance parameters)
/// @param params2 Vector to receive the second set of parameters (restitution, spring and damping parameters)
/// @return Nonzero if successful, 0 otherwise.
var pX3con_6d_getlinearlimit(var jointId, VECTOR *params1, VECTOR *params2);

/// \brief Get the motion type around the specified axis.
///
/// This function only works with 6d joints
/// @param jointId The id of the joint
/// @param index The specified drive type
/// @return The motion value
var pX3con_6d_getmotion(var jointId, var index);

/// \brief Get the angular tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @return The value of the angular tolerance threshold in degree
var pX3con_6d_getprojangtolerance(var jointId);

/// \brief Get the linear tolerance threshold for the projection
///
/// The value is scaled to 3DGS scale
/// @param jointId The id of the joint
/// @return The value of the linear tolerance threshold
var pX3con_6d_getprojlineartolerance(var jointId);

/// \brief Get the cone limit for the joint.
///
/// This function will only work for 6d joints
/// @param jointId The id of the joint
/// @param params1 Vector to receive the first set of parameters (yangle, zangle and limitContactDistance parameters)
/// @param params2 Vector to receive the second set of parameters (restitution, spring and damping parameters)
/// @return Nonzero if successful, 0 otherwise.
var pX3con_6d_getswinglimit(var jointId, VECTOR *params1, VECTOR *params2);

/// \brief Get the twist limit for the joint. 
///
/// This function will only work for 6d joints
/// @param jointId The id of the joint
/// @param params1 Vector to receive the first set of parameters (lower, upper and distance parameters)
/// @param params2 Vector to receive the second set of parameters (restitution, spring and damping parameters)
/// @return Nonzero if successful, 0 otherwise.
var pX3con_6d_gettwistlimit(var jointId, VECTOR *params1, VECTOR *params2);

/// \brief Set the drive parameters for the specified drive type. 
///
/// This function only works with 6d joints
/// @param jointId The id of the joint
/// @param index The specified drive type
/// @param params The drive params. vector(drive.forceLimit, drive.spring, drive.damping)
/// @param flags The drive flags. 
/// @return Nonzero if successful, 0 otherwise
var pX3con_6d_setdrive(var jointId, var index, VECTOR *params, var flags);

/// \brief Set the drive goal pose. 
///
/// This function only works with 6d joints
/// @param jointId The id of the joint
/// @param position The position of the drive in quants
/// @param rotation The rotation of the drive in degree
var pX3con_6d_setdriveposition(var jointId, VECTOR *position, VECTOR *rotation);

/// \brief Set the target goal velocity for joint drive. 
///
/// This function only works with 6d joints
/// @param jointId The id of the joint
/// @param linear The goal velocity for linear drive 
/// @param angular The goal velocity for angular drive
var pX3con_6d_setdrivevelocity(var jointId, VECTOR *linear, VECTOR *angular);

/// \brief Set the linear limit for the joint. 
///
/// This function will only work for 6d joint
/// @param jointId The id of the joint
/// @param params1 Vector to receive the first set of parameters (value, 0 and distance parameters)
/// @param params2 Vector to receive the second set of parameters (restitution, spring and damping parameters)
/// @return Nonzero if successful, 0 otherwise.
var pX3con_6d_setlinearlimit(var jointId, VECTOR *params1, VECTOR *params2);

/// \brief Set the motion type around the specified axis.
///
/// This function only works with 6d joints
/// @param jointId The id of the joint
/// @param index The specified drive type
/// @return The motion value
var pX3con_6d_setmotion(var jointId, var index, var motion);

/// \brief Set the angular tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @param tolerance The value of the angular tolerance threshold in degree
/// @return 1 if successful, 0 otherwise.
var pX3con_6d_setprojangtolerance(var jointId, var tolerance);

/// \brief Set the linear tolerance threshold for the projection
///
/// @param jointId The id of the joint
/// @param tolerance The value of the linear tolerance threshold in quants
/// @return 1 if successful, 0 otherwise.
var pX3con_6d_setprojlineartolerance(var jointId, var tolerance);

/// \brief Set the swing limit cone for the joint.
///
/// This function will only work for 6d joints
/// @param jointId The id of the joint
/// @param params1 Vector to receive the first set of parameters (yangle, zangle and limitContactDistance parameters)
/// @param params2 Vector to receive the second set of parameters (restitution, spring and damping parameters)
/// @return Nonzero if successful, 0 otherwise.
var pX3con_6d_setswinglimit(var jointId, VECTOR *params1, VECTOR *params2);

/// \brief Set the twist limit for the joint. 
///
/// This function will only work for 6d joints
/// @param jointId The id of the joint
/// @param params1 Vector to receive the first set of parameters (lower, upper and distance parameters)
/// @param params2 Vector to receive the second set of parameters (restitution, spring and damping parameters)
/// @return Nonzero if successful, 0 otherwise.
var pX3con_6d_settwistlimit(var jointId, VECTOR *params1, VECTOR *params2);

/////////////////////////////////////////////////////////
// Utilities

/// \brief Transform a global vector to a local one, taking into account entity's position and rotation
///
/// @param entity A registered physX entity
/// @param inGlobal The position to convert in word coordinate
/// @param outLocal The new position vector in local coordiante
/// @return Nonzero if successful, 0 otherwise
var pX3ent_makelocal(ENTITY *entity, VECTOR *inGlobal, VECTOR *outLocal);

/// \brief Transform a local vector to a global one, taking into account entity's position and rotation
///
/// @param entity A registered physX entity
/// @param inLocal The position to convert in actor local frame coordinate
/// @param outGlobal The new position vector in global frame coordiante
/// @return Nonzero if successful, 0 otherwise
var pX3ent_makeglobal(ENTITY *entity, VECTOR *inLocal, VECTOR *outGlobal);

/// \brief Get entity's shape position in global coordinate
/// 
/// @param entity A registered physX actor
/// @param shapeNum The index of shape beginning with 0.
/// @param position[out] The shape position in global coordinate
/// @param rotation[out] The shape rotation in global coordinate
/// @return Nonzero if successful, 0 otherwise
var pX3ent_getshapepose(ENTITY *entity, var shapeNum, VECTOR *position, VECTOR *rotation);

/// \brief Get the number of shapes of an actor
///
/// @param entity A registered physX actor
/// @return The number of shapes composing the actor
var pX3ent_getnumshapes(ENTITY *entity);

/// \brief Set the offset of the entity from the position of the body
///
/// @param entity A registered physX actor
/// @param offset The new offset. NULL to use a zero vector
/// @return The new offset
VECTOR *pX3ent_setcenteroffset(ENTITY *entity, VECTOR *offset);

/// \brief Get the offset of the entity from the position of the body
///
/// @param entity A registered physX actor
/// @param offset[out] The new offset. NULL to return a temporary vector
/// @return The offset vector
VECTOR *pX3ent_getcenteroffset(ENTITY *entity, VECTOR *offset);

//////////////////////////////////////////////////////////
// Vehicles implementation

/// \brief Create a vehicle actor
///
/// Initialize underlaying physX objects
/// @param chassis The chassis of the vehicle. If entity is a registered physX actor, the shape of the entity will be used as the chassis of the vehicle.
///			If entity is not a registered physX actor, a dynamic physX actor will be created using a box hull.
/// @param type The Vehicle Drive Type PX_VEHICLE_TYPE_DRIVE4W, or PX_VEHICLE_TYPE_DRIVETANK
/// @param chassis The vehicle chassis. The entity's hull will be used if entity is a registered physX actor, otherwise a box shape.
/// @param numWheels Number of wheels of the vehicle.
///			If vehicle is of type PX_VEHICLE_TYPE_DRIVE4W, it is the number of non driven wheels and can be set to 0 to have a car with 4 wheels.
///			If vehicle is of type PX_VEHICLE_TYPE_DRIVETANK, it is the number of extra wheels (totalWeels will be 4 + numWheels).
/// @return Nonzero if successful, 0 otherwise.
var pX3ent_vehicle(ENTITY *entity, var type, var numWheels);

/// \brief Setup a vehicle using the SDK
///
/// @param entity A registered vehicle entity
/// @return NonZero is successful, 0 otherwise.
var pX3ent_vehicle_setup(ENTITY *entity);

/// \brief Set a vehicle to its rest state. 
///
/// @param entity A registered vehicle actor
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_settoreststate(ENTITY *entity);

/// \brief Set the group for individual shapes of an actor
///
/// This function is an advanced version of pX3ent_setgroup and is only usefull for vehicles
/// @param entity A registered physX actor
/// @param shapeNum The shape index
/// @param group The group off the shape
/// @return Nonzero if successful, 0 otherwise
var pX3ent_setshapegroup(ENTITY *entity, var shapeNum, var group);

/// \brief Get the group for individual shapes of an actor
///
/// This function is an advanced version of pX3ent_getgroup and is only usefull for vehicles
/// @param entity A registered physX actor
/// @param shapeNum The shape index
/// @return The group of the shape
var pX3ent_getshapegroup(ENTITY *entity, var shapeNum);

/// \brief Modify drivable state of an entity. Entity is drivable if flag is set, nondrivable if not
///
/// @param entityA registered physX actor
/// @param drivable The drivable state of the entity
/// @return Nonzero if successful, 0 otherwise
var pX3ent_setdrivable(ENTITY *entity, var drivable);

/// \brief Set a complex material on an actor's shape
///
/// A new material is created. Only needed for advanced material assignation.
/// @param entity A registered physX entity
/// @param standard Material parameters. range : 0-100
/// @param combine Friction combine vector
/// @return material Id if successful, 0 otherwise
var pX3ent_setshapematerial(ENTITY *entity, var shapeNum, VECTOR *standard, VECTOR *combineFlags);

/// \brief Set a material defined by its Id on an actor's shape
///
/// The id must be a valid Id. This is needed for advanced material assignation
/// @param entity A registered physX actor
/// @param materialId The Id of the material
/// @return the material id if successful, 0 otherwise
var pX3ent_setshapematerialid(ENTITY *entity, var shapeNum, var materialId);

/// \brief Create surface to tire friction pairs
///
/// @param nbTireTypes The number of tire types
/// @param nbSurfaces The number of surface types
/// @param materialIds An array of material Ids. The size must be equal or greater than the number of surfaces
/// @param surfaceTypes An array of surface types. The size must be equal or greater than the number of surfaces
/// @return Nonzero if successful, 0 otherwise.
var pX3_vehicle_createsurfacetirefrictionpairs(var nbTireTypes, var nbSurfaces, var *materialIds, var *surfaceTypes);

/// \brief Set the friction for a specified pair of tire/drivable surface type pair
///
/// @param surface The surface type
/// @param tire The tire type
/// @param friction The friction value
/// @return Nonzero if successful, 0 otherwise
var pX3_vehicle_settypepairfriction(var surface, var tire, var friction);

/// \brief Get the friction for a specified pair of tire/drivable surface type pair
///
/// @param surface The surface type
/// @param tire The tire type
/// @param friction The friction value
/// @return The value of friction for the pair
var pX3_vehicle_gettypepairfriction(var surface, var tire);

/// \brief Set parameters for vehicle wheels
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param params1 First set of parameter for vehicle wheels. vector(radius, width, mass)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setwheelparams1(ENTITY *entity, var wheelId, VECTOR *params1);

/// \brief Get parameters for vehicle wheels
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param params1[out] First set of parameter for vehicle wheels. vector(radius, width, mass)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getwheelparams1(ENTITY *entity, var wheelId, VECTOR *params1);

/// \brief Set parameters for vehicle wheels
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param params2 Second set of parameter for vehicle wheels. vector(moi, dampingRate, toeangle)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setwheelparams2(ENTITY *entity, var wheelId, VECTOR *params2);

/// \brief Get parameters for vehicle wheels
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param params1[out] First set of parameter for vehicle wheels. vector(moi, dampingRate, toeangle)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getwheelparams2(ENTITY *entity, var wheelId, VECTOR *params2);

/// \brief Set parameters for vehicle wheels
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param params3 Third set of parameter for vehicle wheels. vector(maxbraketorque, maxhandbraketorque, maxsteer)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setwheelparams3(ENTITY *entity, var wheelId, VECTOR *params3);

/// \brief Get parameters for vehicle wheels
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param params3[out] Third set of parameter for vehicle wheels. vector(maxbraketorque, maxhandbraketorque, maxsteer)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getwheelparams3(ENTITY *entity, var wheelId, VECTOR *params3);

/// \brief Set the type of the tire. Wets, snow, winter, summer, all-terrain, mud etc
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param type The Tire type. Wets, snow, winter, summer, all-terrain, mud etc
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_settiretype(ENTITY *entity, var wheelId, var type);

/// \brief Set the type of the tire. Wets, snow, winter, summer, all-terrain, mud etc
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @return The Tire type. Wets, snow, winter, summer, all-terrain, mud etc
var pX3ent_vehicle_gettiretype(ENTITY *entity, var wheelId);

/// \brief Set parameters for vehicle suspension data
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param params1 First set of parameter for suspension data. vector(springStrength, springdamperRate, sprungMass)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setsuspensionparams1(ENTITY *entity, var wheelId, VECTOR *params1);

/// \brief Get parameters for vehicle suspension data
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param params1[out] First set of parameter for suspension data. vector(springStrength, springdamperRate, sprungMass)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getsuspensionparams1(ENTITY *entity, var wheelId, VECTOR *params1);

/// \brief Set parameters for vehicle suspension data
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param params2 Second set of parameter for suspension data. vector(maxCompression, maxDroop, unused)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setsuspensionparams2(ENTITY *entity, var wheelId, VECTOR *params2);

/// \brief Get parameters for vehicle suspension data
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param params2[out] Second set of parameter for suspension data. vector(maxCompression, maxDroop, unused)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getsuspensionparams2(ENTITY *entity, var wheelId, VECTOR *params2);

/// \brief Set the direction of travel of the suspension of the idth wheel. 
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param direction 
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setsusptraveldirection(ENTITY *entity, var wheelId, VECTOR *direction);

/// \brief Get the direction of travel of the suspension of the idth wheel. 
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param direction
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getsusptraveldirection(ENTITY *entity, var wheelId, VECTOR *direction);

/// \brief Set the offset from the rigid body centre of mass to the centre of the idth wheel. 
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param offset 
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setwheelcentreoffset(ENTITY *entity, var wheelId, VECTOR *offset);

/// \brief Get the offset from the rigid body centre of mass to the centre of the idth wheel. 
///
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param direction
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getwheelcentreoffset(ENTITY *entity, var wheelId, VECTOR *offset);

/// \brief Set the application point of the suspension force of the suspension of the idth wheel. 
///
/// Specified relative to the centre of mass of the rigid body 
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param offset 
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setsuspforceapppointoffset(ENTITY *entity, var wheelId, VECTOR *offset);

/// \brief Get the application point of the suspension force of the suspension of the idth wheel. 
///
/// Specified relative to the centre of mass of the rigid body 
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param direction
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getsuspforceapppointoffset(ENTITY *entity, var wheelId, VECTOR *offset);

/// \brief Set the application point of the tire force of the tire of the idth wheel. 
///
/// Specified relative to the centre of mass of the rigid body 
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param offset 
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_settireforceapppointoffset(ENTITY *entity, var wheelId, VECTOR *offset);

/// \brief Set the application point of the tire force of the tire of the idth wheel. 
///
/// Specified relative to the centre of mass of the rigid body 
/// @param entity A registered vehicle actor
/// @param wheelId The will to be modified
/// @param direction
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_gettireforceapppointoffset(ENTITY *entity, var wheelId, VECTOR *offset);

/// \brief Set parameters for vehicle 4W differential data
///
/// This function only works for 4W vehicles
/// @param entity A registered 4W vehicle actor
/// @param params1 First set of parameter for differential data. vector(frontRearSplit, fontLeftRightSplit, rearLeftRightSplit)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setdifferentialparams1(ENTITY *entity, VECTOR *params1);

/// \brief Get parameters for vehicle 4W differential data
///
/// This function only works for 4W vehicles
/// @param entity A registered 4W vehicle actor
/// @param params1[out] First set of parameter for differential data. vector(frontRearSplit, fontLeftRightSplit, rearLeftRightSplit)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getdifferentialparams1(ENTITY *entity, VECTOR *params1);

/// \brief Set parameters for vehicle 4W differential data
///
/// This function only works for 4W vehicles
/// @param entity A registered 4W vehicle actor
/// @param params2 Second set of parameters for differential data. vector(centreBias, frontBias, rearBias)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setdifferentialparams2(ENTITY *entity, VECTOR *params2);

/// \brief Get parameters for vehicle 4W differential data
///
/// This function only works for 4W vehicles
/// @param entity A registered 4W vehicle actor
/// @param params2[out] Second set of parameters for differential data. vector(centreBias, frontBias, rearBias)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getdifferentialparams2(ENTITY *entity, VECTOR *params2);

/// \brief Set the type of differential used for the vehicle
///
/// This function only works for 4W vehicles
/// @param entity A registered 4W vehicle actor
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setdifferentialtype(ENTITY *entity, var type);

/// \brief Get the type of differential used for the vehicle
///
/// This function only works for 4W vehicles
/// @param entity A registered 4W vehicle actor
/// @return The differential type
var pX3ent_vehicle_getdifferentialtype(ENTITY *entity);

/// \brief Set parameters for vehicle engine
///
/// @param entity A registered vehicle actor
/// @param params1 First set of parameters for vehicle engine. vector(peakTorque, maxOmega, 0)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setengineparams1(ENTITY *entity, VECTOR *params1);

/// \brief Set parameters for vehicle engine
///
/// @param entity A registered vehicle actor
/// @param params1 First set of parameters for vehicle engine. vector(peakTorque, maxOmega, 0)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getengineparams1(ENTITY *entity, VECTOR *params1);

/// \brief Set parameters for vehicle engine
///
/// @param entity A registered vehicle actor
/// @param params2 Second set of parameters for vehicle engine. vector(daming rate full throttle, damping rate zero throttle engaged, damping rate zero throttle disengaged)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setengineparams2(ENTITY *entity, VECTOR *params2);

/// \brief Get parameters for vehicle engine
///
/// @param entity A registered vehicle actor
/// @param params2 Second set of parameters for vehicle engine. vector(daming rate full throttle, damping rate zero throttle engaged, damping rate zero throttle disengaged)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getengineparams2(ENTITY *entity, VECTOR *params2);

/// \brief Set parameters for vehicle gears data
///
/// @param entity A registered vehicle actor
/// @param params Parameters for vehicle gear. vector(finalRatio, numRatio, switchTime)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setgearsparams(ENTITY *entity, VECTOR *params);

/// \brief Get parameters for vehicle gears data
///
/// @param entity A registered vehicle actor
/// @param params Parameters for vehicle gear. vector(finalRatio, numRatio, switchTime)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getgearsparams(ENTITY *entity, VECTOR *params);

/// \brief Set parameters for vehicle gears data
///
/// @param entity A registered vehicle actor
/// @param ratios An array of gear ratios. Length numRatios
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setgearsratios(ENTITY *entity, var *ratios);

/// \brief Get parameters for vehicle gears data
///
/// @param entity A registered vehicle actor
/// @param ratios[out] An array to be populated of gear ratios. Length numRatios
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getgearsratios(ENTITY *entity, var *ratios);

/// \brief Set parameters for vehicle clutch data
///
/// @param entity A registered vehicle actor
/// @param strength. Torque strength
///		Torque generated by clutch is proportional to the clutch strength and the velocity difference between 
///		the engine speed and the speed of the driven wheels after accounting for the gear ratio. 
///		Range: (0,MAX_NUM_GEAR_RATIOS)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setclutchstrength(ENTITY *entity, var strength);

/// \brief Set parameters for vehicle clutch data
///
/// @param entity A registered vehicle actor
/// @return The clutch strength
var pX3ent_vehicle_getclutchstrength(ENTITY *entity);

/// \brief Set parameters for vehicle Ackermann Geometry Data
///
/// The function is only valid for 4W Vehicles
/// @param entity A registered 4W vehicle actor
/// @param params Parameters for vehicle ackermann  geometry data. vector(front width, rear with, axle separation)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setackermannparams(ENTITY *entity, VECTOR *params);

/// \brief Get parameters for vehicle Ackermann Geometry Data
///
/// The function is only valid for 4W Vehicles
/// @param entity A registered 4W vehicle actor
/// @param params[out] Parameters for vehicle ackermann  geometry data. vector(front width, rear with, axle separation)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_getackermannparams(ENTITY *entity, VECTOR *params);

/// \brief Set Accuracy of Ackermann steer calculation
///
/// @param entity A registered vehicle actor
/// @param accuracy Accuracy of Ackermann steer calculation. 
///		Accuracy with value 0.0f results in no Ackermann steer-correction Accuracy with value 1.0 results in perfect Ackermann steer-correction. Range: (0,100)
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setackermannaccuracy(ENTITY *entity, var accuracy);

/// \brief Get Accuracy of Ackermann steer calculation
///
/// @param entity A registered vehicle actor
/// @return Accuracy of Ackermann steer calculation
var pX3ent_vehicle_getackermannaccuracy(ENTITY *entity);

/// \brief Set mapping between wheel id and position of corresponding wheel shape in the list of actor shapes.
///
/// @param entity A registered vehicle actor
/// @param wheelId The id of the wheel
/// @param shapeId The id of the corresponding shape
/// @return nonzero if successful, 0 otherwise
var pX3ent_vehicle_setwheelshapemapping(ENTITY *entity, var wheelId, var shapeId);

/// \brief Set an analog control value to drive the vehicle. 
///
/// @param entity A registered vehicle actor
/// @param val The value of the input
/// @param type The type of the input (Accel, Brake, ...)
/// @return Nonzero if successfull, zero otherwise
var pX3ent_vehicle_setanaloginput(ENTITY *entity, var val, var type);

/// \brief Get the analog control value that has been applied to the vehicle. 
///
/// @param entity A registered vehicle actor
/// @param type The type of the input (Accel, Brake, ...)
/// @return Nonzero if successfull, zero otherwise
var pX3ent_vehicle_getanaloginput(ENTITY *entity, var type);

/// \brief Set that the gearup button has been pressed. 
///
/// @param entity A registered vehicle actor
/// @param active The activation value of the gear
/// @return Nonzero if successfull, zero otherwise
var pX3ent_vehicle_setgearup(ENTITY *entity, var active);

/// \brief Set that the geardown button has been pressed. 
///
/// @param entity A registered vehicle actor
/// @param active The activation value of the gear
/// @return Nonzero if successfull, zero otherwise
var pX3ent_vehicle_setgeardown(ENTITY *entity, var active);

/// \brief Check if the gearup button has been pressed. 
///
/// @param entity A registered vehicle actor
/// @return Nonzero if successfull, zero otherwise
var pX3ent_vehicle_getgearup(ENTITY *entity);

/// \brief Check if the geardown button has been pressed. 
///
/// @param entity A registered vehicle actor
/// @return Nonzero if successfull, zero otherwise
var pX3ent_vehicle_getgeardown(ENTITY *entity);

/// \brief Set the flag that will be used to select auto-gears. 
///
/// @param entity A registered vehicle actor
/// @param active The value of the flag
/// @return Nonzero if successfull, zero otherwise
var pX3ent_vehicle_setuseautogears(ENTITY *entity, var active);

/// \brief Get the flag status that is used to select auto-gears. 
///
/// @param entity A registered vehicle actor
/// @return The value of the flag
var pX3ent_vehicle_getuseautogears(ENTITY *entity);

/// \brief Toggle the autogears flag. 
///
/// @param entity A registered vehicle actor
/// @return Nonzero if successfull, zero otherwise
var pX3ent_vehicle_toggleautogears(ENTITY *entity);

/// \brief Get the current gear. 
///
/// @param entity A registered vehicle actor
/// @return The current gear value
var pX3ent_vehicle_getcurrentgear(ENTITY *entity);

/// \brief Get the target gear. 
///
/// @param entity A registered vehicle actor
/// @return The target gear value
var pX3ent_vehicle_gettargetgear(ENTITY *entity);

/// \brief Start a gear change to a target gear. 
///
/// @param entity A registered vehicle actor
/// @param The target gear value
/// @return Nonzero if successfull, zero otherwise
var pX3ent_vehicle_startgearchange(ENTITY *entity, var target);

/// \brief Force an immediate gear change to a target gear. 
///
/// @param entity A registered vehicle actor
/// @param The target gear value
/// @return Nonzero if successfull, zero otherwise
var pX3ent_vehicle_forcegearchange(ENTITY *entity, var target);

/// \brief Set the rotation speed of the engine (radians per second). 
///
/// @param entity A registered vehicle actor
/// @return Nonzero if successful, 0 otherwise.
var pX3ent_vehicle_setenginerotationspeed(ENTITY *entity, var speed);

/// \brief Return the rotation speed of the engine. 
///
/// @param entity A registered vehicle actor
/// @return The engine rotation speed
var pX3ent_vehicle_getenginerotationspeed(ENTITY *entity);

/// \brief Set vehicle inputs using smoothing from physX SDK
///
/// @param entity A registered vehicle actor
/// @param val The value of the input
/// @param input The input type (Accel, Brake, ...)
/// @return Nonzero if successfull, zero otherwise
var pX3ent_vehicle_setdrive(ENTITY *entity, var value, var input);

/// \brief Get vehicle inputs
///
/// @param entity A registered vehicle actor
/// @param input The input type (Accel, Brake, ...)
/// @return The input value
var pX3ent_vehicle_getdrive(ENTITY *entity, var input);

/// \brief Set the wheel rotation speed (radians per second). 
///
/// @param entity A registered vehicle actor
/// @param wheelId The id of the wheel
/// @param speed The rotation speed of the wheel in radians per second
/// @return Nonzero if successful, 0 otherwise
var pX3ent_vehicle_setwheelrotationspeed(ENTITY *entity, var wheelId, var speed);

/// \brief Return the rotation speed of a specified wheel. 
///
/// @param entity A registered vehicle actor
/// @param wheelId The id of the wheel
/// @return The rotation speed of a specified wheel.
var pX3ent_vehicle_getwheelrotationspeed(ENTITY *entity, var wheelId);

/// \brief Return the steer angle of a specified wheel. 
///
/// @param entity A registered vehicle actor
/// @param wheelId The id of the wheel
/// @return The steer angle of the wheel
var pX3ent_vehicle_getsteer(ENTITY *entity, var wheelId);

/// \brief Set the wheel rotation angle (radian). 
///
/// @param entity A registered physX entity
/// @param wheelId The Id of the wheel
/// @param angle The rotation angle of the specified wheel about the axis of rolling rotation in radian.
/// @return Nonzero if successful, Zero Otherwise
var pX3ent_vehicle_setwheelrotationangle(ENTITY *entity, var wheelId, var angle);

/// \brief Return the rotation angle of the specified wheel about the axis of rolling rotation.  
///
/// @param entity A registered vehicle actor
/// @param wheelId The id of the wheel
/// @return The rotation angle of the specified wheel about the axis of rolling rotation.  
var pX3ent_vehicle_getwheelrotationangle(ENTITY *entity, var wheelId);

/// \brief Return the susp jounce of the specified suspension. 
///
/// @param entity A registered vehicle actor
/// @param wheelId The id of the suspension
/// @return The jounce value of the suspension
var pX3ent_vehicle_getsuspjounce(ENTITY *entity, var suspId);

/// \brief Return the longitudinal slip of the specified tire. 
///
/// @param entity A registered vehicle actor
/// @param tireId The id of the tire
/// @return The longitudinal slip of the specified tire
var pX3ent_vehicle_gettirelongslip(ENTITY *entity, var tireId);

/// \brief Return the lateral slip of the specified tire. 
///
/// @param entity A registered vehicle actor
/// @param tireId The id of the tire
/// @return The lateral slip of the specified tire.
var pX3ent_vehicle_gettirelatslip(ENTITY *entity, var tireId);

/// \brief Return the friction applied to the specified tire.  
///
/// @param entity A registered vehicle actor
/// @param tireId The id of the tire
/// @return The friction applied to the specified tire.
var pX3ent_vehicle_gettirefriction(ENTITY *entity, var tireId);

/// \brief Return the drivable surface type underneath the specified tire.  
///
/// @param entity A registered vehicle actor
/// @param tireId The id of the tire
/// @return The drivable surface type underneath the specified tire.
var pX3ent_vehicle_gettiredrivablesurfacetype(ENTITY *entity, var tireId);

/// \brief Return the raycast start, direction, and length of the specified suspension line.  
///
/// @param entity A registered vehicle actor
/// @param suspId The id of the suspension
/// @param start The start of the suspension line. Set to NULL to ignore
/// @param end The end of the suspension line. Set to NULL to ignore
/// @return The length of the specified suspension line.
var pX3ent_vehicle_getsuspraycast(ENTITY *entity, var suspId, VECTOR *start, VECTOR *end);

/// \brief Return the suspension line start ad suspension line dir   
///
/// @param entity A registered vehicle actor
/// @param suspId The id of the suspension
/// @param start The start of the suspension line. Set to NULL to ignore
/// @param dir The direction of the suspension line
/// @return The length of the specified suspension line.
var pX3ent_vehicle_getsuspline(ENTITY *entity, var suspId, VECTOR *start, VECTOR *dir);

/// \brief Compute the rigid body velocity component along the forward vector. 
///
/// @param entity A registered vehicle actor
/// @return The rigid body velocity component along the forward vector. 
var pX3ent_vehicle_computeforwardspeed(ENTITY *entity);

/// \brief Compute the rigid body velocity component along the right vector. 
///
/// @param entity A registered vehicle actor
/// @return The rigid body velocity component along the right vector. 
var pX3ent_vehicle_computesidewaysspeed(ENTITY *entity);

/// \brief Test if a specific wheel is off the ground. 
///
/// @param entity A registered vehicle actor
/// @param wheelId The wheel id to test
/// @return Nonzero if wheel is in air. 0 otherwise 
var pX3ent_vehicle_wheelinair(ENTITY *entity, var wheelId);

/// \brief Test if all wheels are off the ground.  
///
/// @param entity A registered vehicle actor
/// @return Nonzero if in air. 0 otherwise 
var pX3ent_vehicle_inair(ENTITY *entity);

/// \brief Set vehicle smooth rise and fall rates
///
/// @param entity A registered vehicle actor
/// @param rise The rise rate used for drive smoothing. var[5] in the order : 
///		- accel, brake, handbrake, steer_left, steer_right for 4W
///		- accel, brake_left, brake_right, thrust_left, thrust_right for Tanks
///		Set to NULL to keep actual configuration
/// @param fall The fall rate used for drive smoothing. var[5] ()
/// @return Nonzero if successfull, 0 otherwise
var pX3ent_vehicle_setsmoothing(ENTITY *entity, var *rise, var *fall);

/// \brief Set steer vs forward speed table for a 4w vehicle
///
/// @param entity A registered physX actor
/// @param dataPair The steer vs forward speed table. var [2*8] max.
/// @param numPair The number of pairs defined in the table. Table length should be 2 * numPair. Max 8
/// @return Nonzero if successfull, zero otherwise.
var pX3ent_vehicle_setsteervsforwardspeed(ENTITY *entity, var *dataPair, var numPair);

/// \brief Set driving model. 
///
/// eDRIVE_MODEL_STANDARD: turning achieved by braking on one side, accelerating on the other side. eDRIVE_MODEL_SPECIAL: turning achieved by accelerating forwards on one side, accelerating backwards on the other side. default value is eDRIVE_MODEL_STANDARD
/// @param entity A registered physX actor (tank)
/// @param model The drive model eDRIVE_MODEL_STANDARD or eDRIVE_MODEL_SPECIAL
/// @return nonzero if successful, Zero otherwise.
var pX3ent_vehicle_setdrivemodel(ENTITY *entity, var model);

/// \brief Get driving model.
///
/// @param entity A registered physX actor
/// @return -1 if failed or driving model
var pX3ent_vehicle_getdrivemodel(ENTITY *entity);

/// \brief Update character volume
///
/// @param entity A registered Character controller
/// @param params New volume bounding box parameters
/// @return Nonzero if successfull, 0 otherwise
var pX3ent_setcctextents(ENTITY *entity, VECTOR *params);

/// \brief Get character volume
///
/// @param entity A registered Character controller
/// @param params[out] The character bounding volume
/// @return Nonzero if successfull, 0 otherwise
var pX3ent_getcctextents(ENTITY *entity, VECTOR *params);

/// \brief Resize character height while keeping foot position. Used for crouching/stading
///
/// @param entity A registered Character entity
/// @param var height The new height of the character
/// @return Nonzero if successfull, 0 otherwise
var pX3ent_resizechar(ENTITY *entity, var height);

/// \brief Set foot position of a character
///
/// @param entity A registered Character controller
/// @param pos The new foot position of the character
/// @return Nonzero if successfull, 0 otherwise
var pX3ent_setfootposition(ENTITY *entity, VECTOR *pos);

/// \brief Get foot position of a character
///
/// @param entity A registered Character controller
/// @param pos[out] The foot position of the character. NULL to return a temporary vector
/// @return pos
VECTOR *pX3ent_getfootposition(ENTITY *entity, VECTOR *pos);

/// \brief Set the behaviour of a character controller after touching the actor.
///
/// The supported behaviours are : 
///		<ul>
///			<li>PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT defines if the character can effectively travel with the object it is standing on. For example a character standing on a dynamic bridge should follow the motion of the PxShape it is standing on.
///				But it should not be the case if the character stands on, say a PxShape bottle rolling on the ground.</li> (PhysX doc)
///			<li>PxControllerBehaviorFlag::eCCT_SLIDE defines if the character should slide or not when standing on the object. This can be used as an alternative to the previously discussed slope limit feature, to define non walk-able objects rather than non-walkable parts. 
///				It can also be used to make a capsule character fall off a platform's edge automatically, when the center of the capsule crosses the platform's edge.</li> (PhysX doc)
/// @param entity A registered physX actor
/// @param behaviour The behaviour value (Can be a combination of both).
/// @return Nonzero if successfull. 0 otherwise
var pX3ent_setcharbehaviour(ENTITY *entity, var behaviour);
#define pX3ent_setcharbehavior(e, b) pX3ent_setcharbehaviour(e, b)

/// \brief Get char collision flags. Is used to test character collisions.
///
/// @param entity A registered physX character
/// @return The character collision flags
var pX3ent_getcharcollisionflags(ENTITY *entity);

/// \brief Get the collision of acharacter controller against the floor
///
/// @param entity A registered physX character
/// return Nonzero if character is grounded, 0 otherwise
var pX3ent_ischargrounded(ENTITY *entity);

/////////////////////////////////////////////////////////
// PhysX3 Utility Implementation
void* PxPhysics = NULL;
void* PxScene = NULL;

function on_exit_px3() { return; }
function on_level_load_px3() { return; }
function on_ent_remove_px3(ENTITY* ent) { return; }

/// \brief Level load handler
function physX3_level_load()
{
	if (!PxPhysics) return;
	
	if (level_ent)
		pX3ent_settype(level_ent,PH_STATIC,PH_POLY);
	for (you = ent_next(NULL); you; you = ent_next(you))
	{ 
		if (you.flags & PASSABLE) continue;	
		var type = ent_type(you);
		if(type == 5 && (you.emask & DYNAMIC)) continue;	// only register static models
		if(type < 2 && type > 5)  continue; // blocks, models, or terrain only
		if(type == 4)
			pX3ent_settype(you,PH_STATIC,PH_TERRAIN);
		else	
			pX3ent_settype(you,PH_STATIC,PH_POLY);
 	}
	on_level_load_px3();
}

/// \brief entity remove handler
///
/// @param ent The entity to be removed
function physX3_ent_remove(ENTITY* ent)
{
	if(!PxPhysics) return;
	if(ent.body)
		pX3ent_settype(ent,0,0);
	on_ent_remove_px3(ent);
}

/// \brief Close PhysX3 simulation
///
/// Destroy all PhysX3 pointers
function physX3_close()
{
	physX3_destroy();
	PxScene = NULL;
	PxPhysics = NULL;
	on_exit_px3();
}

/// \brief Start physX simulation loop
///
/// Set handlers to register/unregister physX objects
function physX3_open()
{
	if(PxPhysics) return; // already initialized
	
	PxPhysics = physX3_load();
	PxScene = physX3_run(0);
	pX3_setsteprate(60,8,0);
	pX3_setunit(1/40);

	on_exit_px3 = on_exit;	// store previous on_exit function
	on_exit = physX3_close;
	on_level_load_px3 = on_level_load;
	on_level_load = physX3_level_load;
	on_ent_remove_px3 = on_ent_remove;
	on_ent_remove = physX3_ent_remove;

	while(PxPhysics) {
#ifdef FIXED_TIME
//This for-loop fixes the physX framerate to 60 when the frame rate is below 60 fps
		int loc_n = integer(time_step * 60 / 16) + 1;
		int loc_i = 0;
		if (freeze_mode <= 0)		
			for (; loc_i < loc_n; loc_i++)
			   physX3_run(0);
#else
		if (freeze_mode <= 0)		
			physX3_run(time_frame/16);
#endif
		proc_mode = PROC_EARLY;	// call physX3_run before entity actions.
		wait(1);
	}	
}

#endif //__ACKPHYSX3_H__
