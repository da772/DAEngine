#pragma once

enum class Material : unsigned long long
{
	/*prop_gladiator_arena_Mat : C:\Dev\DAEngine\game\assets\source\models\props\structure\prop_gladiator_arena.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\structure\prop_gladiator_arena_Mat.mat*/
	prop_gladiator_arena_Mat = 13498856280089420462,

	/*Anim_DK2_Combo_A3_IP_01_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\ue_mannequin\Anim_DK2_Combo_A3_IP_01.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\ue_mannequin\Anim_DK2_Combo_A3_IP_01_Mat.mat*/
	Anim_DK2_Combo_A3_IP_01_Mat = 6484763570596795630,

	/*navmesh_Mat : C:\Dev\DAEngine\game\assets\source\models\navmesh.fbx : C:\Dev\DAEngine\game\assets\materials\models\navmesh_Mat.mat*/
	navmesh_Mat = 12495307499327277934,

	/*plane100_Mat : C:\Dev\DAEngine\game\assets\source\models\plane100.fbx : C:\Dev\DAEngine\game\assets\materials\models\plane100_Mat.mat*/
	plane100_Mat = 9415002426168040242,

	/*ramp_Mat : C:\Dev\DAEngine\game\assets\source\models\ramp.fbx : C:\Dev\DAEngine\game\assets\materials\models\ramp_Mat.mat*/
	ramp_Mat = 1645884462572916557,

	/*cone_Mat : C:\Dev\DAEngine\game\assets\source\models\cone.fbx : C:\Dev\DAEngine\game\assets\materials\models\cone_Mat.mat*/
	cone_Mat = 16282026761436331405,

	/*cube_Mat : C:\Dev\DAEngine\game\assets\source\models\cube.fbx : C:\Dev\DAEngine\game\assets\materials\models\cube_Mat.mat*/
	cube_Mat = 14781322892457219859,

	/*SwordSlash_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\mannequin\SwordSlash.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\mannequin\SwordSlash_Mat.mat*/
	SwordSlash_Mat = 7510888223653092176,

	/*target_dummy_collision_Mat : C:\Dev\DAEngine\game\assets\source\models\props\misc\target_dummy_collision.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\misc\target_dummy_collision_Mat.mat*/
	target_dummy_collision_Mat = 3740346684485599312,

	/*prop_collision_gladiator_arena_Mat : C:\Dev\DAEngine\game\assets\source\models\props\structure\prop_collision_gladiator_arena.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\structure\prop_collision_gladiator_arena_Mat.mat*/
	prop_collision_gladiator_arena_Mat = 10908815277523823656,

	/*plane_Mat : C:\Dev\DAEngine\game\assets\source\models\plane.fbx : C:\Dev\DAEngine\game\assets\materials\models\plane_Mat.mat*/
	plane_Mat = 12657596669329658240,

	/*dungeon_test_Mat : C:\Dev\DAEngine\game\assets\source\models\props\level\dungeon_test.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\level\dungeon_test_Mat.mat*/
	dungeon_test_Mat = 6338350325235310644,

	/*jog_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\archer\jog.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\archer\jog_Mat.mat*/
	jog_Mat = 15158584354014232820,

	/*capsule_Mat : C:\Dev\DAEngine\game\assets\source\models\capsule.fbx : C:\Dev\DAEngine\game\assets\materials\models\capsule_Mat.mat*/
	capsule_Mat = 10670505574562822452,

	/*swing2_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\archer\swing2.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\archer\swing2_Mat.mat*/
	swing2_Mat = 10531789416567720886,

	/*sprint_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\archer\sprint.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\archer\sprint_Mat.mat*/
	sprint_Mat = 5577018276204427702,

	/*sphere_Mat : C:\Dev\DAEngine\game\assets\source\models\sphere.fbx : C:\Dev\DAEngine\game\assets\materials\models\sphere_Mat.mat*/
	sphere_Mat = 12420006414563143606,

	/*man_idle_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\mannequin\man_idle.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\mannequin\man_idle_Mat.mat*/
	man_idle_Mat = 14278532425744868510,

	/*transform_Mat : C:\Dev\DAEngine\game\assets\source\models\transform.fbx : C:\Dev\DAEngine\game\assets\materials\models\transform_Mat.mat*/
	transform_Mat = 4069030017790548318,

	/*jogLeft_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\archer\jogLeft.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\archer\jogLeft_Mat.mat*/
	jogLeft_Mat = 12123917042591267598,

	/*prop_med_anvil_Mat : C:\Dev\DAEngine\game\assets\source\models\props\medieval\prop_med_anvil.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\medieval\prop_med_anvil_Mat.mat*/
	prop_med_anvil_Mat = 5831649434215333646,

	/*prop_veh_sedan_wheel_01a_Mat : C:\Dev\DAEngine\game\assets\source\models\props\veh\prop_veh_sedan\prop_veh_sedan_wheel_01a.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\veh\prop_veh_sedan\prop_veh_sedan_wheel_01a_Mat.mat*/
	prop_veh_sedan_wheel_01a_Mat = 10009363735087325262,

	/*testbed_collision_Mat : C:\Dev\DAEngine\game\assets\source\models\props\level\testbed_collision.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\level\testbed_collision_Mat.mat*/
	testbed_collision_Mat = 13924706938907831482,

	/*testbed_Mat : C:\Dev\DAEngine\game\assets\source\models\props\level\testbed.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\level\testbed_Mat.mat*/
	testbed_Mat = 11190092474523006565,

	/*prop_road_tile_01a_Mat : C:\Dev\DAEngine\game\assets\source\models\props\road\prop_road_tile_01a.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\road\prop_road_tile_01a_Mat.mat*/
	prop_road_tile_01a_Mat = 3120076700562839335,

	/*dungeon_test_col_Mat : C:\Dev\DAEngine\game\assets\source\models\props\level\dungeon_test_col.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\level\dungeon_test_col_Mat.mat*/
	dungeon_test_col_Mat = 14122193480356504063,

	/*jogBack_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\archer\jogBack.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\archer\jogBack_Mat.mat*/
	jogBack_Mat = 8479353460525147781,

	/*prop_weap_sword_Mat : C:\Dev\DAEngine\game\assets\source\models\props\weapon\prop_weap_sword.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\weapon\prop_weap_sword_Mat.mat*/
	prop_weap_sword_Mat = 2159062025713362053,

	/*Anim_DK2_Combo_A1_IP_01_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\ue_mannequin\Anim_DK2_Combo_A1_IP_01.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\ue_mannequin\Anim_DK2_Combo_A1_IP_01_Mat.mat*/
	Anim_DK2_Combo_A1_IP_01_Mat = 1873295947901505218,

	/*Anim_DK2_Idle_RM_01_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\ue_mannequin\Anim_DK2_Idle_RM_01.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\ue_mannequin\Anim_DK2_Idle_RM_01_Mat.mat*/
	Anim_DK2_Idle_RM_01_Mat = 4658287449225032543,

	/*Anim_DK2_Run_F_IP_01_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\ue_mannequin\Anim_DK2_Run_F_IP_01.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\ue_mannequin\Anim_DK2_Run_F_IP_01_Mat.mat*/
	Anim_DK2_Run_F_IP_01_Mat = 11892740312115666697,

	/*Anim_DK2_Combo_A2_IP_01_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\ue_mannequin\Anim_DK2_Combo_A2_IP_01.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\ue_mannequin\Anim_DK2_Combo_A2_IP_01_Mat.mat*/
	Anim_DK2_Combo_A2_IP_01_Mat = 11560831039861606728,

	/*Anim_DK2_Combo_A3_RM_01_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\ue_mannequin\Anim_DK2_Combo_A3_RM_01.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\ue_mannequin\Anim_DK2_Combo_A3_RM_01_Mat.mat*/
	Anim_DK2_Combo_A3_RM_01_Mat = 15415048074555418124,

	/*YBot_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\mannequin\YBot.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\mannequin\YBot_Mat.mat*/
	YBot_Mat = 9732349511555370609,

	/*SwordRun_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\mannequin\SwordRun.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\mannequin\SwordRun_Mat.mat*/
	SwordRun_Mat = 17455518904461592665,

	/*prop_veh_sports_wheel_01a_Mat : C:\Dev\DAEngine\game\assets\source\models\props\veh\prop_veh_sports_wheel_01a.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\veh\prop_veh_sports_wheel_01a_Mat.mat*/
	prop_veh_sports_wheel_01a_Mat = 11591486839601900313,

	/*zombie_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\mannequin\zombie.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\mannequin\zombie_Mat.mat*/
	zombie_Mat = 13340105459182453502,

	/*Dancing1_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\mannequin\Dancing1.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\mannequin\Dancing1_Mat.mat*/
	Dancing1_Mat = 15469123389698454709,

	/*SwordWalk_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\mannequin\SwordWalk.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\mannequin\SwordWalk_Mat.mat*/
	SwordWalk_Mat = 1113570505941161717,

	/*prop_collision_veh_sports_01a_Mat : C:\Dev\DAEngine\game\assets\source\models\props\veh\prop_collision_veh_sports_01a.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\veh\prop_collision_veh_sports_01a_Mat.mat*/
	prop_collision_veh_sports_01a_Mat = 12091021081924747433,

	/*archer_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\archer\archer.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\archer\archer_Mat.mat*/
	archer_Mat = 1042181048974828566,

	/*prop_veh_sports_01a_Mat : C:\Dev\DAEngine\game\assets\source\models\props\veh\prop_veh_sports_01a.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\veh\prop_veh_sports_01a_Mat.mat*/
	prop_veh_sports_01a_Mat = 14506259139934677718,

	/*target_dummy_Mat : C:\Dev\DAEngine\game\assets\source\models\props\misc\target_dummy.fbx : C:\Dev\DAEngine\game\assets\materials\models\props\misc\target_dummy_Mat.mat*/
	target_dummy_Mat = 11174268676722991835,

	/*Walking_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\mannequin\Walking.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\mannequin\Walking_Mat.mat*/
	Walking_Mat = 3167290580927814087,

	/*jogRight_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\archer\jogRight.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\archer\jogRight_Mat.mat*/
	jogRight_Mat = 15095906000471871687,

	/*idle_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\archer\idle.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\archer\idle_Mat.mat*/
	idle_Mat = 12817728439670340378,

	/*swing_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\archer\swing.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\archer\swing_Mat.mat*/
	swing_Mat = 10559246967134096218,

	/*button_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\archer\button.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\archer\button_Mat.mat*/
	button_Mat = 2333145356347585839,

	/*Dancing2_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\mannequin\Dancing2.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\mannequin\Dancing2_Mat.mat*/
	Dancing2_Mat = 13141984314835940087,

	/*pesant_f_02_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\pesant_f\pesant_f_02.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\pesant_f\pesant_f_02_Mat.mat*/
	pesant_f_02_Mat = 10706792666821530809,

	/*camilla_02_Mat : C:\Dev\DAEngine\game\assets\source\skeletons\camilla\camilla_02.fbx : C:\Dev\DAEngine\game\assets\materials\skeletons\camilla\camilla_02_Mat.mat*/
	camilla_02_Mat = 6480982823654524653,

};
