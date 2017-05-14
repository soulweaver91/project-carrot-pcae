#include <memory>
#include <QMap>
#include <QString>
#include "Jazz2AnimLib.h"
#include "Palette.h"
#include "IDMapping.h"

std::shared_ptr<AnimIDMap> IDMapper::getAnimMap(Jazz2AnimVersion version) {
    std::shared_ptr<AnimIDMap> map = std::make_shared<AnimIDMap>(BASE_GAME_ANIM_MAP);
    
    // Go backwards, adding and removing assets that are different between versions.
    // This order makes it easier to keep track of the exact positions.
    if (version == Jazz2AnimVersion::TSF || version == Jazz2AnimVersion::CC) {
        map->append({
            {
                { "doggy", "xmas_attack" },
                { "doggy", "xmas_walk" }
            }, {
                { "sparks", "ghost_idle" }
            }
        });
    }
    if (version == Jazz2AnimVersion::HH || version == Jazz2AnimVersion::TSF || version == Jazz2AnimVersion::CC) {
        map->insert(109, {
            { "bilsy", "xmas_throw_fireball" },
            { "bilsy", "xmas_appear" },
            { "bilsy", "xmas_vanish" },
            { "bilsy", "xmas_bullet_fireball" },
            { "bilsy", "xmas_idle" }
        });
        map->insert(110, {
            { "lizard", "xmas_copter_attack" },
            { "lizard", "xmas_bomb" },
            { "lizard", "xmas_copter_idle" },
            { "lizard", "xmas_copter" },
            { "lizard", "xmas_walk" }
        });
        map->insert(111, {
            { "turtle", "xmas_attack" },
            { "turtle", "xmas_idle_flavor" },
            { "turtle", "xmas_turn_start" },
            { "turtle", "xmas_turn_end" },
            { "turtle", "xmas_shell_reverse" },
            { "turtle", "xmas_shell" },
            { "turtle", "xmas_idle" },
            { "turtle", "xmas_walk" }
        });
    }
    if (version == Jazz2AnimVersion::TSF || version == Jazz2AnimVersion::CC) {
        auto spazItems = &(*(map.get()))[85];
        spazItems->remove(93, 11);
        spazItems->remove(69);
        spazItems->remove(64, 3);
        spazItems->remove(62);
        spazItems->remove(48, 7);
        spazItems->remove(20);
        spazItems->remove(10);
        spazItems->insert(8, { "spaz", "crouch" });
        spazItems->remove(7);
        spazItems->remove(0);
    }
    if (version == Jazz2AnimVersion::CC) {
        auto uiItems = &(*(map.get()))[61];
        uiItems->append({
            { "ui", "cc_title", MENU_PALETTE },
            { "ui", "cc_title_small", MENU_PALETTE }
        });
        uiItems->insert(3, { "ui", "menu_snow", MENU_PALETTE });
    }
    if (version == Jazz2AnimVersion::TSF || version == Jazz2AnimVersion::CC) {
        auto section = &(*(map.get()))[61];
        section->insert(3, { "ui", "tsf_title", MENU_PALETTE });

        section = &(*(map.get()))[60];
        section->insert(10, { "ui", "character_art_lori", MENU_PALETTE });
        section->insert(8, { "ui", "character_name_lori", MENU_PALETTE });
        section->insert(3, { "ui", "character_art_difficulty_lori", MENU_PALETTE });

        map->insert(60, {
            { "lori", "airboard" },
            { "lori", "airboard_turn" },
            { "lori", "buttstomp_end" },
            { "lori", "corpse" },
            { "lori", "die" },
            { "lori", "crouch_start" },
            { "lori", "crouch" },
            { "lori", "crouch_shoot" },
            { "lori", "crouch_end" },
            { "lori", "vine_walk" },
            { "lori", "eol" },
            { "lori", "fall" },
            { "lori", "buttstomp" },
            { "lori", "fall_end" },
            { "lori", "shoot" },
            { "lori", "shoot_ver" },
            { "lori", "shoot_end" },
            { "lori", "transform_frog_end" },
            { "lori", "vine_shoot_start" },
            { "lori", "vine_shoot_up_end" },
            { "lori", "vine_shoot_up" },
            { "lori", "vine_idle" },
            { "lori", "vine_idle_flavor" },
            { "lori", "vine_shoot_end" },
            { "lori", "vine_shoot" },
            { "lori", "copter" },
            { "lori", "copter_shoot_start" },
            { "lori", "copter_shoot" },
            { "lori", "pole_h" },
            { "lori", "hurt" },
            { "lori", "idle_flavor_1" },
            { "lori", "idle_flavor_2" },
            { "lori", "idle_flavor_3" },
            { "lori", "idle_flavor_4" },
            { "lori", "idle_flavor_5" },
            { "lori", "vine_shoot_up_start" },
            { "lori", "fall_shoot" },
            { "lori", "jump_unknown_1" },
            { "lori", "jump_unknown_2" },
            { "lori", "jump" },
            { "lori", "ledge" },
            { "lori", "lift" },
            { "lori", "lift_jump_light" },
            { "lori", "lift_jump_heavy" },
            { "lori", "lookup_start" },
            { "lori", "dizzy_walk" },
            { "lori", "push" },
            { "lori", "shoot_start" },
            { "lori", "revup_start" },
            { "lori", "revup" },
            { "lori", "revup_end" },
            { "lori", "fall_diag" },
            { "lori", "jump_diag" },
            { "lori", "ball" },
            { "lori", "run" },
            { "lori", "dash_start" },
            { "lori", "dash" },
            { "lori", "dash_stop" },
            { "lori", "walk_stop" },
            { "lori", "run_stop" },
            { "lori", "spring" },
            { "lori", "idle" },
            { "lori", "uppercut_placeholder_1" },
            { "lori", "uppercut_placeholder_2" },
            { "lori", "sidekick" },
            { "lori", "dizzy" },
            { "lori", "swim_diag_downright" },
            { "lori", "swim_right" },
            { "lori", "swim_diag_right_to_downright" },
            { "lori", "swim_diag_right_to_upright" },
            { "lori", "swim_diag_upright" },
            { "lori", "swing" },
            { "lori", "warp_in" },
            { "lori", "warp_out_freefall" },
            { "lori", "freefall" },
            { "lori", "warp_in_freefall" },
            { "lori", "warp_out" },
            { "lori", "pole_v" }
        });
        map->insert(61, {
            { "lori", "idle_2" },
            { "lori", "gun" }
        });
        map->insert(62, {});

        section = &(*(map.get()))[55];
        section->remove(0, 7);

        section = &(*(map.get()))[54];
        section->remove(93, 11);
        section->remove(69);
        section->remove(64, 3);
        section->remove(62);
        section->remove(48, 7);
        section->remove(20);
        section->remove(10);
        section->remove(0);

        section = &(*(map.get()))[46];
        section->insert(7, { "lori", "transform_frog" });
    }
    if (version == Jazz2AnimVersion::CC) {
        auto uiItems = &(*(map.get()))[45];
        uiItems->append({ "ui", "cc_logo" });
    }
    if (version == Jazz2AnimVersion::TSF || version == Jazz2AnimVersion::CC) {
        auto section = &(*(map.get()))[38];
        section->insert(4, { "ui", "icon_lori" });

        map->insert(35, {});
    }
    
    return map;
}

std::shared_ptr<SampleIDMap> IDMapper::getSampleMap(Jazz2AnimVersion version) {
    std::shared_ptr<SampleIDMap> map = std::make_shared<SampleIDMap>(BASE_GAME_SAMPLE_MAP);

    // Go backwards, adding and removing assets that are different between versions.
    // This order makes it easier to keep track of the exact positions.
    if (version == Jazz2AnimVersion::TSF || version == Jazz2AnimVersion::CC) {
        map->append({
            {
                { "doggy", "xmas_attack" },
                { "doggy", "xmas_noise" },
                { "doggy", "xmas_woof_1" },
                { "doggy", "xmas_woof_2" },
                { "doggy", "xmas_woof_3" }
            },{
                { }
            }
        });
    }
    if (version == Jazz2AnimVersion::HH || version == Jazz2AnimVersion::TSF || version == Jazz2AnimVersion::CC) {
        map->insert(109, {
            { "bilsy", "xmas_appear_2" },
            { "bilsy", "xmas_snap" },
            { "bilsy", "xmas_throw_fireball" },
            { "bilsy", "xmas_fire_start" },
            { "bilsy", "xmas_scary" },
            { "bilsy", "xmas_thunder" },
            { "bilsy", "xmas_appear_1" }
        });
        map->insert(110, {
            { "lizard", "xmas_noise_1" },
            { "lizard", "xmas_noise_2" },
            { "lizard", "xmas_noise_3" },
            { "lizard", "xmas_noise_4" },
        });
        map->insert(111, {
            { "turtle", "xmas_attack_bite" },
            { "turtle", "xmas_turn_start" },
            { "turtle", "xmas_shell_collide" },
            { "turtle", "xmas_idle_1" },
            { "turtle", "xmas_idle_2" },
            { "turtle", "xmas_attack_neck" },
            { "turtle", "xmas_noise_1" },
            { "turtle", "xmas_noise_2" },
            { "turtle", "xmas_noise_3" },
            { "turtle", "xmas_noise_4" },
            { "turtle", "xmas_turn_end" }
        });
    }
    if (version == Jazz2AnimVersion::TSF || version == Jazz2AnimVersion::CC) {
        auto section = &(*(map.get()))[77];
        section->remove(0);

        map->insert(60, {});
        map->insert(61, {});
        map->insert(62, {
            { "lori", "die" },
            { "lori", "hurt_1" },
            { "lori", "hurt_2" },
            { "lori", "hurt_3" },
            { "lori", "hurt_4" },
            { "lori", "hurt_5" },
            { "lori", "hurt_6" },
            { "lori", "hurt_7" },
            { "lori", "hurt_8" },
            { "lori", "unknown_mic1" },
            { "lori", "unknown_mic2" },
            { "lori", "sidekick" },
            { "lori", "fall" },
            { "lori", "jump_1" },
            { "lori", "jump_2" },
            { "lori", "jump_3" },
            { "lori", "jump_4" },
            { "lori", "unused_touch" },
            { "lori", "yahoo" }
        });

        map->insert(35, {
            { "lori", "level_complete" }
        });
    }

    return map;
}

const AnimMapping IDMapper::EMPTY_ANIM_MAPPING = { "", "" };
const SampleMapping IDMapper::EMPTY_SAMPLE_MAPPING = { "", "" };

const AnimIDMap IDMapper::BASE_GAME_ANIM_MAP = {
    { // 0
        { "unknown", "flame_blue" },
        { "common", "bomb" },
        { "common", "smoke_poof" },
        { "common", "explosion_rf" },
        { "common", "explosion_small" },
        { "common", "explosion_large" },
        { "common", "smoke_circling_gray" },
        { "common", "smoke_circling_brown" },
        { "unknown", "bubble" },
        { "unknown", "brown_thing" },
        { "common", "explosion_pepper" },
        { "unknown", "bullet_maybe_electro" },
        { "unknown", "bullet_maybe_electro_trail" },
        { "unknown", "flame_red" },
        { "weapon", "bullet_shield_fireball" },
        { "unknown", "flare_diag_downleft" },
        { "unknown", "flare_hor" },
        { "weapon", "bullet_blaster_upgraded_hor" },
        { "ui", "blaster_upgraded_jazz" },
        { "ui", "blaster_upgraded_spaz" },
        { "weapon", "bullet_blaster_hor" },
        { "weapon", "bullet_blaster_upgraded_ver" },
        { "weapon", "bullet_blaster_ver" },
        { "weapon", "bullet_bouncer" },
        { "pickup", "ammo_bouncer_upgraded" },
        { "pickup", "ammo_bouncer" },
        { "weapon", "bullet_bouncer_upgraded" },
        { "weapon", "bullet_freezer_hor" },
        { "pickup", "ammo_freezer_upgraded" },
        { "pickup", "ammo_freezer" },
        { "weapon", "bullet_freezer_upgraded_hor" },
        { "weapon", "bullet_freezer_ver" },
        { "weapon", "bullet_freezer_upgraded_ver" },
        { "pickup", "ammo_seeker_upgraded" },
        { "pickup", "ammo_seeker" },
        { "weapon", "bullet_seeker_ver_down" },
        { "weapon", "bullet_seeker_diag_downright" },
        { "weapon", "bullet_seeker_hor" },
        { "weapon", "bullet_seeker_ver_up" },
        { "weapon", "bullet_seeker_diag_upright" },
        { "weapon", "bullet_seeker_upgraded_ver_down" },
        { "weapon", "bullet_seeker_upgraded_diag_downright" },
        { "weapon", "bullet_seeker_upgraded_hor" },
        { "weapon", "bullet_seeker_upgraded_ver_up" },
        { "weapon", "bullet_seeker_upgraded_diag_upright" },
        { "weapon", "bullet_rf_hor" },
        { "weapon", "bullet_rf_diag_downright" },
        { "weapon", "bullet_rf_upgraded_diag_downright" },
        { "pickup", "ammo_rf_upgraded" },
        { "pickup", "ammo_rf" },
        { "weapon", "bullet_rf_upgraded_hor" },
        { "weapon", "bullet_rf_upgraded_ver" },
        { "weapon", "bullet_rf_upgraded_diag_upright" },
        { "weapon", "bullet_rf_ver" },
        { "weapon", "bullet_rf_diag_upright" },
        { "weapon", "bullet_toaster" },
        { "pickup", "ammo_toaster_upgraded" },
        { "pickup", "ammo_toaster" },
        { "weapon", "bullet_toaster_upgraded" },
        { "weapon", "bullet_tnt" },
        { "weapon", "bullet_fireball_hor" },
        { "pickup", "ammo_pepper_upgraded" },
        { "pickup", "ammo_pepper" },
        { "weapon", "bullet_fireball_upgraded_hor" },
        { "weapon", "bullet_fireball_ver" },
        { "weapon", "bullet_fireball_upgraded_ver" },
        { "weapon", "bullet_bladegun" },
        { "pickup", "ammo_electro_upgraded" },
        { "pickup", "ammo_electro" },
        { "weapon", "bullet_bladegun_upgraded" },
        { "common", "explosion_tiny" },
        { "common", "explosion_freezer_maybe" },
        { "common", "explosion_tiny_black" },
        { "weapon", "bullet_fireball_upgraded_hor_2" },
        { "unknown", "flare_hor_2" },
        { "unknown", "green_explosion" },
        { "weapon", "bullet_bladegun_alt" },
        { "weapon", "bullet_tnt_explosion" },
        { "object", "container_ammo_shrapnel_1" },
        { "object", "container_ammo_shrapnel_2" },
        { "common", "explosion_upwards" },
        { "common", "explosion_bomb" },
        { "common", "smoke_circling_white" }
    },{ // 1
        { "bat", "idle" },
        { "bat", "resting" },
        { "bat", "takeoff_1" },
        { "bat", "takeoff_2" },
        { "bat", "roost" }
    }, { // 2
        { "bee", "swarm" }
    }, { // 3
        { "bee", "swarm_2" }
    }, { // 4
        { "object", "pushbox_crate" }
    }, { // 5
        { "object", "pushbox_rock" }
    }, { // 6
        { "unknown", "diamondus_tileset_tree" }
    }, { // 7
        { "bilsy", "throw_fireball" },
        { "bilsy", "appear" },
        { "bilsy", "vanish" },
        { "bilsy", "bullet_fireball" },
        { "bilsy", "idle" }
    }, { // 8
        { "birdy", "charge_diag_downright" },
        { "birdy", "charge_ver" },
        { "birdy", "charge_diag_upright" },
        { "birdy", "caged" },
        { "birdy", "cage_destroyed" },
        { "birdy", "die" },
        { "birdy", "feather_green" },
        { "birdy", "feather_red" },
        { "birdy", "feather_green_and_red" },
        { "birdy", "fly" },
        { "birdy", "hurt" },
        { "birdy", "idle_worm" },
        { "birdy", "idle_turn_head_left" },
        { "birdy", "idle_look_left" },
        { "birdy", "idle_turn_head_left_back" },
        { "birdy", "idle_turn_head_right" },
        { "birdy", "idle_look_right" },
        { "birdy", "idle_turn_head_right_back" },
        { "birdy", "idle" },
        { "birdy", "corpse" }
    }, { // 9
        { "unimplemented", "bonus_birdy" }
    }, { // 10
        { "platform", "ball" },
        { "platform", "ball_chain" }
    }, { // 11
        { "object", "bonus_active" },
        { "object", "bonus_inactive" }
    }, { // 12
        { "ui", "boss_health_bar" }
    }, { // 13
        { "bridge", "rope" },
        { "bridge", "stone" },
        { "bridge", "vine" },
        { "bridge", "stone_red" },
        { "bridge", "log" },
        { "bridge", "gem" },
        { "bridge", "lab" }
    }, { // 14
        { "bubba", "spew_fireball" },
        { "bubba", "corpse" },
        { "bubba", "jump" },
        { "bubba", "jump_fall" },
        { "bubba", "fireball" },
        { "bubba", "hop" },
        { "bubba", "tornado" },
        { "bubba", "tornado_start" },
        { "bubba", "tornado_end" }
    }, { // 15
        { "bee", "bee" },
        { "bee", "bee_turn" }
    }, { // 16
        { "unimplemented", "butterfly" }
    }, { // 17
        { "pole", "carrotus", CARROTUS_POLE_PALETTE }
    }, { // 18
        { "cheshire", "platform_appear" },
        { "cheshire", "platform_vanish" },
        { "cheshire", "platform_idle" },
        { "cheshire", "platform_invisible" }
    }, { // 19
        { "cheshire", "hook_appear" },
        { "cheshire", "hook_vanish" },
        { "cheshire", "hook_idle" },
        { "cheshire", "hook_invisible" }
    }, { // 20
        { "caterpillar", "exhale_start" },
        { "caterpillar", "exhale" },
        { "caterpillar", "disoriented" },
        { "caterpillar", "idle" },
        { "caterpillar", "inhale_start" },
        { "caterpillar", "inhale" },
        { "caterpillar", "smoke" }
    }, { // 21
        { "birdy_yellow", "charge_diag_downright_placeholder" },
        { "birdy_yellow", "charge_ver" },
        { "birdy_yellow", "charge_diag_upright" },
        { "birdy_yellow", "caged" },
        { "birdy_yellow", "cage_destroyed" },
        { "birdy_yellow", "die" },
        { "birdy_yellow", "feather_blue" },
        { "birdy_yellow", "feather_yellow" },
        { "birdy_yellow", "feather_blue_and_yellow" },
        { "birdy_yellow", "fly" },
        { "birdy_yellow", "hurt" },
        { "birdy_yellow", "idle_worm" },
        { "birdy_yellow", "idle_turn_head_left" },
        { "birdy_yellow", "idle_look_left" },
        { "birdy_yellow", "idle_turn_head_left_back" },
        { "birdy_yellow", "idle_turn_head_right" },
        { "birdy_yellow", "idle_look_right" },
        { "birdy_yellow", "idle_turn_head_right_back" },
        { "birdy_yellow", "idle" },
        { "birdy_yellow", "corpse" }
    }, { // 22
        { "common", "water_bubble_1" },
        { "common", "water_bubble_2" },
        { "common", "water_bubble_3" },
        { "common", "water_splash" }
    }, { // 23
        { "jazz", "gameover_continue" },
        { "jazz", "gameover_idle" },
        { "jazz", "gameover_end" },
        { "spaz", "gameover_continue" },
        { "spaz", "gameover_idle" },
        { "spaz", "gameover_end" }
    }, { // 24
        { "demon", "idle" },
        { "demon", "attack_start" },
        { "demon", "attack" },
        { "demon", "attack_end" }
    }, { // 25
        { "", "" },
        { "", "" },
        { "", "" },
        { "", "" },
        { "common", "ice_block" }
    }, { // 26
        { "devan", "bullet_small" },
        { "devan", "remote_idle" },
        { "devan", "remote_fall_warp_out" },
        { "devan", "remote_fall" },
        { "devan", "remote_fall_rotate" },
        { "devan", "remote_fall_warp_in" },
        { "devan", "remote_warp_out" }
    }, { // 27
        { "devan", "demon_spew_fireball" },
        { "devan", "disoriented" },
        { "devan", "freefall" },
        { "devan", "disoriented_start" },
        { "devan", "demon_fireball" },
        { "devan", "demon_fly" },
        { "devan", "demon_transform_start" },
        { "devan", "demon_transform_end" },
        { "devan", "disarmed_idle" },
        { "devan", "demon_turn" },
        { "devan", "disarmed_warp_in" },
        { "devan", "disoriented_warp_out" },
        { "devan", "disarmed" },
        { "devan", "crouch" },
        { "devan", "shoot" },
        { "devan", "disarmed_gun" },
        { "devan", "jump" },
        { "devan", "bullet" },
        { "devan", "run" },
        { "devan", "run_end" },
        { "devan", "jump_end" },
        { "devan", "idle" },
        { "devan", "warp_in" },
        { "devan", "warp_out" }
    }, { // 28
        { "pole", "diamondus", DIAMONDUS_POLE_PALETTE }
    }, { // 29
        { "doggy", "attack" },
        { "doggy", "walk" }
    }, { // 30
        { "unimplemented", "door" },
        { "unimplemented", "door_enter_jazz_spaz" }
    }, { // 31
        { "dragonfly", "idle" }
    }, { // 32
        { "dragon", "attack" },
        { "dragon", "idle" },
        { "dragon", "turn" }
    }, { // 33
    }, { // 34
    }, { // 35 + 1 TSFCC
    }, { // 36
    }, { // 37
        { "eva", "blink" },
        { "eva", "idle" },
        { "eva", "kiss_start" },
        { "eva", "kiss_end" }
    }, { // 38
        { "ui", "icon_birdy" },
        { "ui", "icon_birdy_yellow" },
        { "ui", "icon_frog" },
        { "ui", "icon_jazz" },
        // { Jazz2AnimVersion::TSF | Jazz2AnimVersion::CC, "ui", "icon_lori" },
        { "ui", "icon_spaz" }
    }, { // 39
    }, { // 40
        { "chick", "attack" },
        { "chick", "walk" }
    }, { // 41
        { "fencer", "attack" },
        { "fencer", "idle" }
    }, { // 42
        { "fish", "attack" },
        { "fish", "idle" }
    }, { // 43
        { "ctf", "arrow" },
        { "ctf", "base" },
        { "ctf", "lights" },
        { "ctf", "flag_blue" },
        { "ui", "ctf_flag_blue" },
        { "ctf", "base_eva" },
        { "ctf", "base_eva_cheer" },
        { "ctf", "flag_red" },
        { "ui", "ctf_flag_red" }
    }, { // 44
    }, { // 45
        { "ui", "font_medium" },
        { "ui", "font_small" },
        { "ui", "font_large" },
        { "ui", "logo" }
        // { Jazz2AnimVersion::CC, "ui", "cc_logo" },
    }, { // 46
        { "frog", "fall_land" },
        { "frog", "hurt" },
        { "frog", "idle" },
        { "jazz", "transform_frog" },
        { "frog", "fall" },
        { "frog", "jump_start" },
        { "frog", "crouch" },
        // { Jazz2AnimVersion::TSF | Jazz2AnimVersion::CC, "lori", "transform_frog" },
        { "frog", "tongue_diag_upright" },
        { "frog", "tongue_hor" },
        { "frog", "tongue_ver" },
        { "spaz", "transform_frog" },
        { "frog", "run" }
    }, { // 47
        { "platform", "carrotus_fruit" },
        { "platform", "carrotus_fruit_chain" }
    }, { // 48
        { "pickup", "gem_gemring", GEM_PALETTE }
    }, { // 49
        { "unimplemented", "boxing_glove_stiff" },
        { "unimplemented", "boxing_glove_stiff_idle" },
        { "unimplemented", "boxing_glove_normal" },
        { "unimplemented", "boxing_glove_normal_idle" },
        { "unimplemented", "boxing_glove_relaxed" },
        { "unimplemented", "boxing_glove_relaxed_idle" }
    }, { // 50
        { "platform", "carrotus_grass" },
        { "platform", "carrotus_grass_chain" }
    }, { // 51
        { "mad_hatter", "cup" },
        { "mad_hatter", "hat" },
        { "mad_hatter", "attack" },
        { "mad_hatter", "bullet_spit" },
        { "mad_hatter", "walk" }
    }, { // 52
        { "helmut", "idle" },
        { "helmut", "walk" }
    }, { // 53
    }, { // 54
        { "jazz", "unused_unknown_disoriented" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "airboard" },
        { "jazz", "airboard_turn" },
        { "jazz", "buttstomp_end" },
        { "jazz", "corpse" },
        { "jazz", "die" },
        { "jazz", "crouch_start" },
        { "jazz", "crouch" },
        { "jazz", "crouch_shoot" },
        { "jazz", "crouch_end" },
        { "jazz", "unused_door_enter" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "vine_walk" },
        { "jazz", "eol" },
        { "jazz", "fall" },
        { "jazz", "buttstomp" },
        { "jazz", "fall_end" },
        { "jazz", "shoot" },
        { "jazz", "shoot_ver" },
        { "jazz", "shoot_end" },
        { "jazz", "transform_frog_end" },
        { "jazz", "unused_ledge_climb" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "vine_shoot_start" },
        { "jazz", "vine_shoot_up_end" },
        { "jazz", "vine_shoot_up" },
        { "jazz", "vine_idle" },
        { "jazz", "vine_idle_flavor" },
        { "jazz", "vine_shoot_end" },
        { "jazz", "vine_shoot" },
        { "jazz", "copter" },
        { "jazz", "copter_shoot_start" },
        { "jazz", "copter_shoot" },
        { "jazz", "pole_h" },
        { "jazz", "hurt" },
        { "jazz", "idle_flavor_1" },
        { "jazz", "idle_flavor_2" },
        { "jazz", "idle_flavor_3" },
        { "jazz", "idle_flavor_4" },
        { "jazz", "idle_flavor_5" },
        { "jazz", "vine_shoot_up_start" },
        { "jazz", "fall_shoot" },
        { "jazz", "jump_unknown_1" },
        { "jazz", "jump_unknown_2" },
        { "jazz", "jump" },
        { "jazz", "ledge" },
        { "jazz", "lift" },
        { "jazz", "lift_jump_light" },
        { "jazz", "lift_jump_heavy" },
        { "jazz", "lookup_start" },
        { "jazz", "unused_run_diag_upright" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_run_ver_up" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_run_diag_upleft_reverse" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_run_reverse" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_run_diag_downleft_reverse" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_run_ver_down" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_run_diag_downright" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "dizzy_walk" },
        { "jazz", "push" },
        { "jazz", "shoot_start" },
        { "jazz", "revup_start" },
        { "jazz", "revup" },
        { "jazz", "revup_end" },
        { "jazz", "fall_diag" },
        { "jazz", "unused_unknown_mid_frame" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "jump_diag" },
        { "jazz", "unused_jump_shoot_end" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_jump_shoot_ver_start" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_jump_shoot_ver" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "ball" },
        { "jazz", "run" },
        { "jazz", "unused_run_aim_diag" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "dash_start" },
        { "jazz", "dash" },
        { "jazz", "dash_stop" },
        { "jazz", "walk_stop" },
        { "jazz", "run_stop" },
        { "jazz", "spring" },
        { "jazz", "idle" },
        { "jazz", "uppercut" },
        { "jazz", "uppercut_end" },
        { "jazz", "uppercut_start" },
        { "jazz", "dizzy" },
        { "jazz", "swim_diag_downright" },
        { "jazz", "swim_right" },
        { "jazz", "swim_diag_right_to_downright" },
        { "jazz", "swim_diag_right_to_upright" },
        { "jazz", "swim_diag_upright" },
        { "jazz", "swing" },
        { "jazz", "warp_in" },
        { "jazz", "warp_out_freefall" },
        { "jazz", "freefall" },
        { "jazz", "warp_in_freefall" },
        { "jazz", "warp_out" },
        { "jazz", "pole_v" },
        { "jazz", "unused_unarmed_crouch_start" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_unarmed_crouch_end" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_unarmed_fall" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_unarmed_hurt" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_unarmed_idle" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_unarmed_jump" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_unarmed_crouch_end_2" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_lookup_start" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_unarmed_run" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_unarmed_stare" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "jazz", "unused_lookup_start_2" } //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
    }, { // 55
        { "unimplemented", "bonus_jazz_idle_flavor_2" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "unimplemented", "bonus_jazz_jump_2" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "unimplemented", "bonus_jazz_dash_2" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "unimplemented", "bonus_jazz_rotate_2" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "unimplemented", "bonus_jazz_ball_2" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "unimplemented", "bonus_jazz_run_2" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "unimplemented", "bonus_jazz_idle_2" }, //Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH, 
        { "unimplemented", "bonus_jazz_idle_flavor" },
        { "unimplemented", "bonus_jazz_jump" },
        { "unimplemented", "bonus_jazz_ball" },
        { "unimplemented", "bonus_jazz_run" },
        { "unimplemented", "bonus_jazz_dash" },
        { "unimplemented", "bonus_jazz_rotate" },
        { "unimplemented", "bonus_jazz_idle" }
    }, { // 56
    }, { // 57
        { "pole", "jungle", JUNGLE_POLE_PALETTE }
    }, { // 58
        { "lab_rat", "attack" },
        { "lab_rat", "idle" },
        { "lab_rat", "walk" }
    }, { // 59
        { "lizard", "copter_attack" },
        { "lizard", "bomb" },
        { "lizard", "copter_idle" },
        { "lizard", "copter" },
        { "lizard", "walk" }
    }, { // 60 + 3 TSFCC (Lori)
        { "ui", "multiplayer_char", MENU_PALETTE },
        { "ui", "multiplayer_color", MENU_PALETTE },
        { "ui", "character_art_difficulty_jazz", MENU_PALETTE },
        //{ Jazz2AnimVersion::TSF | Jazz2AnimVersion::CC, "ui", "character_art_difficulty_lori", MENU_PALETTE },
        { "ui", "character_art_difficulty_spaz", MENU_PALETTE },
        { "unimplemented", "key", MENU_PALETTE },
        { "ui", "loading_bar", MENU_PALETTE },
        { "ui", "multiplayer_mode", MENU_PALETTE },
        { "ui", "character_name_jazz", MENU_PALETTE },
        //{ Jazz2AnimVersion::TSF | Jazz2AnimVersion::CC, "ui", "character_name_lori", MENU_PALETTE },
        { "ui", "character_name_spaz", MENU_PALETTE },
        { "ui", "character_art_jazz", MENU_PALETTE },
        //{ Jazz2AnimVersion::TSF | Jazz2AnimVersion::CC, "ui", "character_art_lori", MENU_PALETTE },
        { "ui", "character_art_spaz", MENU_PALETTE }
    }, { // 61
        { "ui", "font_medium_2", MENU_PALETTE },
        { "ui", "font_small_2", MENU_PALETTE },
        { "ui", "logo_large", MENU_PALETTE },
        //{ Jazz2AnimVersion::TSF | Jazz2AnimVersion::CC, "ui", "tsf_title", MENU_PALETTE },
        //{ Jazz2AnimVersion::CC, "ui", "menu_snow", MENU_PALETTE },
        { "ui", "logo_small", MENU_PALETTE },
        //{ Jazz2AnimVersion::CC, "ui", "cc_title", MENU_PALETTE },
        //{ Jazz2AnimVersion::CC, "ui", "cc_title_small", MENU_PALETTE }
    }, { // 62
    }, { // 63
        { "monkey", "banana" },
        { "monkey", "banana_splat" },
        { "monkey", "jump" },
        { "monkey", "walk_start" },
        { "monkey", "walk_end" },
        { "monkey", "attack" },
        { "monkey", "walk" }
    }, { // 64
        { "moth", "green" },
        { "moth", "gray" },
        { "moth", "purple" },
        { "moth", "pink" }
    }, { // 65
    }, { // 66
    }, { // 67
        { "pickup", "1up" },
        { "pickup", "food_apple" },
        { "pickup", "food_banana" },
        { "object", "container_barrel" },
        { "common", "poof_brown" },
        { "object", "container_box_crush" },
        { "object", "container_barrel_shrapnel_1" },
        { "object", "container_barrel_shrapnel_2" },
        { "object", "container_barrel_shrapnel_3" },
        { "object", "container_barrel_shrapnel_4" },
        { "object", "powerup_shield_bubble" },
        { "pickup", "food_burger" },
        { "pickup", "food_cake" },
        { "pickup", "food_candy" },
        { "object", "savepoint" },
        { "pickup", "food_cheese" },
        { "pickup", "food_cherry" },
        { "pickup", "food_chicken" },
        { "pickup", "food_chips" },
        { "pickup", "food_chocolate" },
        { "pickup", "food_cola" },
        { "pickup", "carrot" },
        { "pickup", "gem", GEM_PALETTE },
        { "pickup", "food_cucumber" },
        { "pickup", "food_cupcake" },
        { "pickup", "food_donut" },
        { "pickup", "food_eggplant" },
        { "unknown", "green_blast_thing" },
        { "object", "exit_sign" },
        { "pickup", "fast_fire_jazz" },
        { "pickup", "fast_fire_spaz" },
        { "object", "powerup_shield_fire" },
        { "pickup", "food_fries" },
        { "pickup", "fast_feet" },
        { "object", "gem_super", GEM_PALETTE },
        { "pickup", "gem_2", GEM_PALETTE },
        { "pickup", "airboard" },
        { "pickup", "coin_gold" },
        { "pickup", "food_grapes" },
        { "pickup", "food_ham" },
        { "pickup", "carrot_fly" },
        { "ui", "heart" },
        { "pickup", "freeze_enemies" },
        { "pickup", "food_ice_cream" },
        { "common", "ice_break_shrapnel_1" },
        { "common", "ice_break_shrapnel_2" },
        { "common", "ice_break_shrapnel_3" },
        { "common", "ice_break_shrapnel_4" },
        { "pickup", "food_lemon" },
        { "pickup", "food_lettuce" },
        { "pickup", "food_lime" },
        { "object", "powerup_shield_lightning" },
        { "object", "trigger_crate" },
        { "pickup", "food_milk" },
        { "object", "crate_ammo_bouncer" },
        { "object", "crate_ammo_freezer" },
        { "object", "crate_ammo_seeker" },
        { "object", "crate_ammo_rf" },
        { "object", "crate_ammo_toaster" },
        { "object", "crate_ammo_tnt" },
        { "object", "powerup_upgrade_blaster_jazz" },
        { "object", "powerup_upgrade_bouncer" },
        { "object", "powerup_upgrade_freezer" },
        { "object", "powerup_upgrade_seeker" },
        { "object", "powerup_upgrade_rf" },
        { "object", "powerup_upgrade_toaster" },
        { "object", "powerup_upgrade_pepper" },
        { "object", "powerup_upgrade_electro" },
        { "object", "powerup_transform_birdy" },
        { "object", "powerup_transform_birdy_yellow" },
        { "object", "powerup_swap_characters" },
        { "pickup", "food_orange" },
        { "pickup", "carrot_invincibility" },
        { "pickup", "food_peach" },
        { "pickup", "food_pear" },
        { "pickup", "food_soda" },
        { "pickup", "food_pie" },
        { "pickup", "food_pizza" },
        { "pickup", "potion" },
        { "pickup", "food_pretzel" },
        { "pickup", "food_sandwich" },
        { "pickup", "food_strawberry" },
        { "pickup", "carrot_full" },
        { "object", "powerup_upgrade_blaster_spaz" },
        { "pickup", "coin_silver" },
        { "unknown", "green_blast_thing_2" },
        { "common", "generator" },
        { "pickup", "stopwatch" },
        { "pickup", "food_taco" },
        { "pickup", "food_thing" },
        { "object", "tnt" },
        { "pickup", "food_hotdog" },
        { "pickup", "food_watermelon" },
        { "object", "container_crate_shrapnel_1" },
        { "object", "container_crate_shrapnel_2" }
    }, { // 68
        { "pinball", "bumper_500", PINBALL_PALETTE },
        { "pinball", "bumper_500_hit", PINBALL_PALETTE },
        { "pinball", "bumper_carrot", PINBALL_PALETTE },
        { "pinball", "bumper_carrot_hit", PINBALL_PALETTE },
        { "pinball", "paddle_left" },
        { "pinball", "paddle_right" }
    }, { // 69
        { "platform", "lab" },
        { "platform", "lab_chain" }
    }, { // 70
        { "pole", "psych", PSYCH_POLE_PALETTE }
    }, { // 71
        { "queen", "scream" },
        { "queen", "ledge" },
        { "queen", "ledge_recover" },
        { "queen", "idle" },
        { "queen", "brick" },
        { "queen", "fall" },
        { "queen", "stomp" },
        { "queen", "backstep" }
    }, { // 72
        { "rapier", "attack" },
        { "rapier", "attack_swing" },
        { "rapier", "idle" },
        { "rapier", "attack_start" },
        { "rapier", "attack_end" }
    }, { // 73
        { "raven", "attack" },
        { "raven", "idle" },
        { "raven", "turn" }
    }, { // 74
        { "robot", "spike_ball" },
        { "robot", "attack_start" },
        { "robot", "attack" },
        { "robot", "copter" },
        { "robot", "idle" },
        { "robot", "attack_end" },
        { "robot", "shrapnel_1" },
        { "robot", "shrapnel_2" },
        { "robot", "shrapnel_3" },
        { "robot", "shrapnel_4" },
        { "robot", "shrapnel_5" },
        { "robot", "shrapnel_6" },
        { "robot", "shrapnel_7" },
        { "robot", "shrapnel_8" },
        { "robot", "shrapnel_9" },
        { "robot", "run" },
        { "robot", "copter_start" },
        { "robot", "copter_end" }
    }, { // 75
        { "object", "rolling_rock" }
    }, { // 76
        { "rocket_turtle", "downright" },
        { "rocket_turtle", "upright" },
        { "rocket_turtle", "smoke" },
        { "rocket_turtle", "upright_to_downright" }
    }, { // 77
    }, { // 78
    }, { // 79
        { "skeleton", "bone" },
        { "skeleton", "skull" },
        { "skeleton", "walk" }
    }, { // 80
        { "pole", "diamondus_tree", DIAMONDUS_POLE_PALETTE }
    }, { // 81
        { "common", "snow", SNOW_PALETTE }
    }, { // 82
        { "spike", "rocket" },
        { "spike", "mace_chain" },
        { "spike", "bottom" },
        { "spike", "top" },
        { "spike", "puff" },
        { "spike", "mace" },
        { "spike", "turret" },
        { "spike", "crosshairs" }
    }, { // 83
        { "platform", "sonic" },
        { "platform", "sonic_chain" }
    }, { // 84
        { "sparks", "idle" }
    }, { // 85
        { "spaz", "unused_unknown_disoriented" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "airboard" },
        { "spaz", "airboard_turn" },
        { "spaz", "buttstomp_end" },
        { "spaz", "corpse" },
        { "spaz", "die" },
        { "spaz", "crouch_start" },
        { "spaz", "crouch_shoot_2" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        //{ Jazz2AnimVersion::TSF | Jazz2AnimVersion::CC, "spaz", "crouch" },
        { "spaz", "crouch_shoot" },
        { "spaz", "crouch_end" },
        { "spaz", "unused_door_enter" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "vine_walk" },
        { "spaz", "eol" },
        { "spaz", "fall" },
        { "spaz", "buttstomp" },
        { "spaz", "fall_end" },
        { "spaz", "shoot" },
        { "spaz", "shoot_ver" },
        { "spaz", "shoot_end" },
        { "spaz", "transform_frog_end" },
        { "spaz", "unused_ledge_climb" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "vine_shoot_start" },
        { "spaz", "vine_shoot_up_end" },
        { "spaz", "vine_shoot_up" },
        { "spaz", "vine_idle" },
        { "spaz", "vine_idle_flavor" },
        { "spaz", "vine_shoot_end" },
        { "spaz", "vine_shoot" },
        { "spaz", "copter" },
        { "spaz", "copter_shoot_start" },
        { "spaz", "copter_shoot" },
        { "spaz", "pole_h" },
        { "spaz", "hurt" },
        { "spaz", "idle_flavor_1" },
        { "spaz", "idle_flavor_2" },
        { "spaz", "idle_flavor_3_placeholder" },
        { "spaz", "idle_flavor_4" },
        { "spaz", "idle_flavor_5" },
        { "spaz", "vine_shoot_up_start" },
        { "spaz", "fall_shoot" },
        { "spaz", "jump_unknown_1" },
        { "spaz", "jump_unknown_2" },
        { "spaz", "jump" },
        { "spaz", "ledge" },
        { "spaz", "lift" },
        { "spaz", "lift_jump_light" },
        { "spaz", "lift_jump_heavy" },
        { "spaz", "lookup_start" },
        { "spaz", "unused_run_diag_upright" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_run_ver_up" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_run_diag_upleft_reverse" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_run_reverse" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_run_diag_downleft_reverse" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_run_ver_down" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_run_diag_downright" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "dizzy_walk" },
        { "spaz", "push" },
        { "spaz", "shoot_start" },
        { "spaz", "revup_start" },
        { "spaz", "revup" },
        { "spaz", "revup_end" },
        { "spaz", "fall_diag" },
        { "spaz", "unused_unknown_mid_frame" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "jump_diag" },
        { "spaz", "unused_jump_shoot_end" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_jump_shoot_ver_start" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_jump_shoot_ver" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "ball" },
        { "spaz", "run" },
        { "spaz", "unused_run_aim_diag" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "dash_start" },
        { "spaz", "dash" },
        { "spaz", "dash_stop" },
        { "spaz", "walk_stop" },
        { "spaz", "run_stop" },
        { "spaz", "spring" },
        { "spaz", "idle" },
        { "spaz", "sidekick" },
        { "spaz", "sidekick_end" },
        { "spaz", "sidekick_start" },
        { "spaz", "dizzy" },
        { "spaz", "swim_diag_downright" },
        { "spaz", "swim_right" },
        { "spaz", "swim_diag_right_to_downright" },
        { "spaz", "swim_diag_right_to_upright" },
        { "spaz", "swim_diag_upright" },
        { "spaz", "swing" },
        { "spaz", "warp_in" },
        { "spaz", "warp_out_freefall" },
        { "spaz", "freefall" },
        { "spaz", "warp_in_freefall" },
        { "spaz", "warp_out" },
        { "spaz", "pole_v" },
        { "spaz", "unused_unarmed_crouch_start" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_unarmed_crouch_end" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_unarmed_fall" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_unarmed_hurt" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_unarmed_idle" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_unarmed_jump" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_unarmed_crouch_end_2" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_lookup_start" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_unarmed_run" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_unarmed_stare" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "spaz", "unused_lookup_start_2" } // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
    }, { // 86
        { "spaz", "idle_flavor_3_start" },
        { "spaz", "idle_flavor_3" },
        { "spaz", "idle_flavor_3_bird" },
        { "spaz", "idle_flavor_5_spaceship" }
    }, { // 87
        { "unimplemented", "bonus_spaz_idle_flavor" },
        { "unimplemented", "bonus_spaz_jump" },
        { "unimplemented", "bonus_spaz_ball" },
        { "unimplemented", "bonus_spaz_run" },
        { "unimplemented", "bonus_spaz_dash" },
        { "unimplemented", "bonus_spaz_rotate" },
        { "unimplemented", "bonus_spaz_idle" }
    }, { // 88
    }, { // 89
        { "object", "3d_spike" },
        { "object", "3d_spike_chain" }
    }, { // 90
        { "object", "3d_spike_2" },
        { "object", "3d_spike_2_chain" }
    }, { // 91
        { "platform", "spike" },
        { "platform", "spike_chain" }
    }, { // 92
        { "object", "spring_blue_ver" },
        { "object", "spring_blue_hor" },
        { "object", "spring_blue_ver_reverse" },
        { "object", "spring_green_ver_reverse" },
        { "object", "spring_red_ver_reverse" },
        { "object", "spring_green_ver" },
        { "object", "spring_green_hor" },
        { "object", "spring_red_ver" },
        { "object", "spring_red_hor" }
    }, { // 93
        { "common", "steam_note" }
    }, { // 94
    }, { // 95
        { "sucker_tube", "walk_top" },
        { "sucker_tube", "inflated_deflate" },
        { "sucker_tube", "walk_ver_down" },
        { "sucker_tube", "fall" },
        { "sucker_tube", "inflated" },
        { "sucker_tube", "poof" },
        { "sucker_tube", "walk" },
        { "sucker_tube", "walk_ver_up" }
    }, { // 96
        { "tube_turtle", "idle" }
    }, { // 97
        { "tough_turtle_boss", "attack_start" },
        { "tough_turtle_boss", "attack_end" },
        { "tough_turtle_boss", "shell" },
        { "tough_turtle_boss", "mace" },
        { "tough_turtle_boss", "idle" },
        { "tough_turtle_boss", "walk" }
    }, { // 98
        { "tough_turtle", "walk" }
    }, { // 99
        { "turtle", "attack" },
        { "turtle", "idle_flavor" },
        { "turtle", "turn_start" },
        { "turtle", "turn_end" },
        { "turtle", "shell_reverse" },
        { "turtle", "shell" },
        { "turtle", "idle" },
        { "turtle", "walk" }
    }, { // 100
        { "tweedle", "magnet_start" },
        { "tweedle", "spin" },
        { "tweedle", "magnet_end" },
        { "tweedle", "shoot_jazz" },
        { "tweedle", "shoot_spaz" },
        { "tweedle", "hurt" },
        { "tweedle", "idle" },
        { "tweedle", "magnet" },
        { "tweedle", "walk" }
    }, { // 101
        { "uterus", "closed_start" },
        { "uterus", "crab_spawn" },
        { "uterus", "closed_idle" },
        { "uterus", "idle" },
        { "crab", "fall_end" },
        { "uterus", "closed_end" },
        { "uterus", "shield" },
        { "crab", "walk" }
    }, { // 102
        { "object", "vine_mask" },
        { "object", "vine" }
    }, { // 103
        { "object", "bonus_10" }
    }, { // 104
        { "object", "bonus_100" }
    }, { // 105
        { "object", "bonus_20" }
    }, { // 106
        { "object", "bonus_50" }
    }, { // 107
    }, { // 108
        { "witch", "attack" },
        { "witch", "die" },
        { "witch", "idle" },
        { "witch", "bullet_magic" }
    }
};

const SampleIDMap IDMapper::BASE_GAME_SAMPLE_MAP = {
    { // 0
        { "weapon", "bullet_shield_bubble_1" },
        { "weapon", "bullet_shield_bubble_2" },
        { "weapon", "unknown_bmp1" },
        { "weapon", "unknown_bmp2" },
        { "weapon", "unknown_bmp3" },
        { "weapon", "unknown_bmp4" },
        { "weapon", "unknown_bmp5" },
        { "weapon", "unknown_bmp6" },
        { "weapon", "tnt_explosion" },
        { "weapon", "ricochet_contact" },
        { "weapon", "ricochet_bullet_1" },
        { "weapon", "ricochet_bullet_2" },
        { "weapon", "ricochet_bullet_3" },
        { "weapon", "bullet_shield_fire_1" },
        { "weapon", "bullet_shield_fire_2" },
        { "weapon", "bullet_bouncer_1" },
        { "weapon", "bullet_blaster_jazz_1" },
        { "weapon", "bullet_blaster_jazz_2" },
        { "weapon", "bullet_blaster_jazz_3" },
        { "weapon", "bullet_bouncer_2" },
        { "weapon", "bullet_bouncer_3" },
        { "weapon", "bullet_bouncer_4" },
        { "weapon", "bullet_bouncer_5" },
        { "weapon", "bullet_bouncer_6" },
        { "weapon", "bullet_bouncer_7" },
        { "weapon", "bullet_blaster_jazz_4" },
        { "weapon", "bullet_pepper" },
        { "weapon", "bullet_freezer_1" },
        { "weapon", "bullet_freezer_2" },
        { "weapon", "bullet_freezer_upgraded_1" },
        { "weapon", "bullet_freezer_upgraded_2" },
        { "weapon", "bullet_freezer_upgraded_3" },
        { "weapon", "bullet_freezer_upgraded_4" },
        { "weapon", "bullet_freezer_upgraded_5" },
        { "weapon", "bullet_electro_1" },
        { "weapon", "bullet_electro_2" },
        { "weapon", "bullet_electro_3" },
        { "weapon", "bullet_rf" },
        { "weapon", "bullet_seeker" },
        { "weapon", "bullet_blaster_spaz_1" },
        { "weapon", "bullet_blaster_spaz_2" },
        { "weapon", "bullet_blaster_spaz_3" }
    },{ // 1
        { "bat", "noise" }
    },{ // 2
    },{ // 3
    },{ // 4
    },{ // 5
    },{ // 6
    },{ // 7
        { "bilsy", "appear_2" },
        { "bilsy", "snap" },
        { "bilsy", "throw_fireball" },
        { "bilsy", "fire_start" },
        { "bilsy", "scary" },
        { "bilsy", "thunder" },
        { "bilsy", "appear_1" }
    },{ // 8
    },{ // 9
    },{ // 10
    },{ // 11
        { "unknown", "unknown_bonus1" },
        { "unknown", "unknown_bonusblub" }
    },{ // 12
    },{ // 13
    },{ // 14
        { "bubba", "hop_1" },
        { "bubba", "hop_2" },
        { "bubba", "unknown_bubbaexplo" },
        { "bubba", "unknown_frog2" },
        { "bubba", "unknown_frog3" },
        { "bubba", "unknown_frog4" },
        { "bubba", "unknown_frog5" },
        { "bubba", "sneeze" },
        { "bubba", "tornado" }
    },{ // 15
        { "bee", "noise" }
    },{ // 16
    },{ // 17
    },{ // 18
    },{ // 19
    },{ // 20
        { "caterpillar", "dizzy" }
    },{ // 21
    },{ // 22
        { "common", "char_airboard" },
        { "common", "char_airboard_turn_1" },
        { "common", "char_airboard_turn_2" },
        { "common", "unknown_base" },
        { "common", "powerup_shield_damage_1" },
        { "common", "powerup_shield_damage_2" },
        { "common", "bomb" },
        { "birdy", "fly_1" },
        { "birdy", "fly_2" },
        { "weapon", "bouncer" },
        { "common", "blub1" },
        { "weapon", "shield_bubble_bullet" },
        { "weapon", "shield_fire_bullet" },
        { "common", "ambient_fire" },
        { "object", "container_barrel_break" },
        { "common", "powerup_shield_timer" },
        { "pickup", "coin" },
        { "common", "scenery_collapse" },
        { "common", "cup" },
        { "common", "scenery_destruct" },
        { "common", "down" },
        { "common", "downfl2" },
        { "pickup", "food_drink_1" },
        { "pickup", "food_drink_2" },
        { "pickup", "food_drink_3" },
        { "pickup", "food_drink_4" },
        { "pickup", "food_edible_1" },
        { "pickup", "food_edible_2" },
        { "pickup", "food_edible_3" },
        { "pickup", "food_edible_4" },
        { "pickup", "shield_lightning_bullet_1" },
        { "pickup", "shield_lightning_bullet_2" },
        { "pickup", "shield_lightning_bullet_3" },
        { "weapon", "tnt" },
        { "weapon", "wall_poof" },
        { "weapon", "toaster" },
        { "common", "flap" },
        { "common", "swish_9" },
        { "common", "swish_10" },
        { "common", "swish_11" },
        { "common", "swish_12" },
        { "common", "swish_13" },
        { "object", "gem_super_break" },
        { "object", "powerup_break" },
        { "common", "gunsm1" },
        { "pickup", "1up" },
        { "unknown", "common_head" },
        { "common", "copter_noise" },
        { "common", "hibell" },
        { "common", "holyflut" },
        { "ui", "weapon_change" },
        { "common", "ice_break" },
        { "object", "shell_noise_1" },
        { "object", "shell_noise_2" },
        { "object", "shell_noise_3" },
        { "object", "shell_noise_4" },
        { "object", "shell_noise_5" },
        { "object", "shell_noise_6" },
        { "object", "shell_noise_7" },
        { "object", "shell_noise_8" },
        { "object", "shell_noise_9" },
        { "unknown", "common_itemtre" },
        { "common", "char_jump" },
        { "common", "char_jump_alt" },
        { "common", "land1" },
        { "common", "land2" },
        { "common", "land3" },
        { "common", "land4" },
        { "common", "land5" },
        { "common", "char_land" },
        { "common", "loadjazz" },
        { "common", "loadspaz" },
        { "common", "metalhit" },
        { "unimplemented", "powerup_jazz1_style" },
        { "object", "bonus_not_enough_coins" },
        { "pickup", "gem" },
        { "pickup", "ammo" },
        { "common", "pistol1" },
        { "common", "plop_5" },
        { "common", "plop_1" },
        { "common", "plop_2" },
        { "common", "plop_3" },
        { "common", "plop_4" },
        { "common", "plop_6" },
        { "spaz", "idle_flavor_4_spaceship" },
        { "common", "copter_pre" },
        { "common", "char_revup" },
        { "common", "ringgun1" },
        { "common", "ringgun2" },
        { "weapon", "shield_fire_noise" },
        { "weapon", "shield_lightning_noise" },
        { "weapon", "shield_lightning_noise_2" },
        { "common", "shldof3" },
        { "common", "slip" },
        { "common", "splat_1" },
        { "common", "splat_2" },
        { "common", "splat_3" },
        { "common", "splat_4" },
        { "common", "splat_5" },
        { "common", "splat_6" },
        { "spring", "spring_2" },
        { "common", "steam_low" },
        { "common", "step" },
        { "common", "stretch" },
        { "common", "swish_1" },
        { "common", "swish_2" },
        { "common", "swish_3" },
        { "common", "swish_4" },
        { "common", "swish_5" },
        { "common", "swish_6" },
        { "common", "swish_7" },
        { "common", "swish_8" },
        { "common", "warp_in" },
        { "common", "warp_out" },
        { "common", "char_double_jump" },
        { "common", "water_splash" },
        { "object", "container_crate_break" }
    },{ // 23
    },{ // 24
        { "demon", "attack" }
    },{ // 25
    },{ // 26
    },{ // 27
        { "devan", "spit_fireball" },
        { "devan", "flap" },
        { "devan", "unknown_frog4" },
        { "devan", "jump_up" },
        { "devan", "laugh" },
        { "devan", "shoot" },
        { "devan", "transform_demon_stretch_2" },
        { "devan", "transform_demon_stretch_4" },
        { "devan", "transform_demon_stretch_1" },
        { "devan", "transform_demon_stretch_3" },
        { "devan", "unknown_vanish" },
        { "devan", "unknown_whistledescending2" },
        { "devan", "transform_demon_wings" }
    },{ // 28
    },{ // 29
        { "doggy", "attack" },
        { "doggy", "noise" },
        { "doggy", "woof_1" },
        { "doggy", "woof_2" },
        { "doggy", "woof_3" }
    },{ // 30
    },{ // 31
        { "dragonfly", "noise" }
    },{ // 32
    },{ // 33
        { "cinematic", "ending_eva_thankyou" }
    },{ // 34
        { "jazz", "level_complete" }
    },{ // 35 + 1 TSFCC
        { "spaz", "level_complete" }
    },{ // 36
        { "cinematic", "logo_epic_1" },
        { "cinematic", "logo_epic_2" }
    },{ // 37
        { "eva", "blink" },
        { "eva", "idle" },
        { "eva", "kiss_start" },
        { "eva", "kiss_end" }
    },{ // 38
    },{ // 39
        { "unknown", "unknown_fan" }
    },{ // 40
        { "chick", "attack_1" },
        { "chick", "attack_2" },
        { "chick", "attack_3" }
    },{ // 41
        { "fencer", "attack" }
    },{ // 42
    },{ // 43
    },{ // 44
    },{ // 45
    },{ // 46
        { "frog", "noise_1" },
        { "frog", "noise_2" },
        { "frog", "noise_3" },
        { "frog", "noise_4" },
        { "frog", "noise_5" },
        { "frog", "noise_6" },
        { "frog", "transform" },
        { "frog", "tongue" }
    },{ // 47
    },{ // 48
    },{ // 49
        { "unimplemented", "boxing_glove_hit" }
    },{ // 50
    },{ // 51
        { "mad_hatter", "cup" },
        { "mad_hatter", "hat" },
        { "mad_hatter", "spit" },
        { "mad_hatter", "splash_1" },
        { "mad_hatter", "splash_2" }
    },{ // 52
    },{ // 53
        { "cinematic", "opening_blow" },
        { "cinematic", "opening_boom_1" },
        { "cinematic", "opening_boom_2" },
        { "cinematic", "opening_brake" },
        { "cinematic", "opening_end_shoot" },
        { "cinematic", "opening_rope_grab" },
        { "cinematic", "opening_sweep_1" },
        { "cinematic", "opening_sweep_2" },
        { "cinematic", "opening_sweep_3" },
        { "cinematic", "opening_gun_noise_1" },
        { "cinematic", "opening_gun_noise_2" },
        { "cinematic", "opening_gun_noise_3" },
        { "cinematic", "opening_helicopter" },
        { "cinematic", "opening_hit_spaz" },
        { "cinematic", "opening_hit_turtle" },
        { "cinematic", "opening_vo_1" },
        { "cinematic", "opening_gun_blow" },
        { "cinematic", "opening_insect" },
        { "cinematic", "opening_trolley_push" },
        { "cinematic", "opening_land" },
        { "cinematic", "opening_turtle_growl" },
        { "cinematic", "opening_turtle_grunt" },
        { "cinematic", "opening_rock" },
        { "cinematic", "opening_rope_1" },
        { "cinematic", "opening_rope_2" },
        { "cinematic", "opening_run" },
        { "cinematic", "opening_shot" },
        { "cinematic", "opening_shot_grn" },
        { "cinematic", "opening_slide" },
        { "cinematic", "opening_end_sfx" },
        { "cinematic", "opening_swish_1" },
        { "cinematic", "opening_swish_2" },
        { "cinematic", "opening_swish_3" },
        { "cinematic", "opening_swish_4" },
        { "cinematic", "opening_turtle_ugh" },
        { "cinematic", "opening_up_1" },
        { "cinematic", "opening_up_2" },
        { "cinematic", "opening_wind" }
    },{ // 54
    },{ // 55
    },{ // 56
        { "jazz", "ledge" },
        { "jazz", "hurt_1" },
        { "jazz", "hurt_2" },
        { "jazz", "hurt_3" },
        { "jazz", "hurt_4" },
        { "jazz", "idle_flavor_3" },
        { "jazz", "hurt_5" },
        { "jazz", "hurt_6" },
        { "jazz", "hurt_7" },
        { "jazz", "hurt_8" },
        { "jazz", "carrot" },
        { "jazz", "idle_flavor_4" }
    },{ // 57
    },{ // 58
        { "lab_rat", "attack" },
        { "lab_rat", "noise_1" },
        { "lab_rat", "noise_2" },
        { "lab_rat", "noise_3" },
        { "lab_rat", "noise_4" },
        { "lab_rat", "noise_5" }
    },{ // 59
        { "lizard", "noise_1" },
        { "lizard", "noise_2" },
        { "lizard", "noise_3" },
        { "lizard", "noise_4" }
    },{ // 60 + 3 TSFCC (Lori)
    },{ // 61
    },{ // 62
        { "ui", "select_1" },
        { "ui", "select_2" },
        { "ui", "select_3" },
        { "ui", "select_4" },
        { "ui", "select_5" },
        { "ui", "select_6" },
        { "ui", "select_7" },
        { "ui", "type_char" },
        { "ui", "type_enter" }
    },{ // 63
        { "monkey", "banana_splat" },
        { "monkey", "banana_throw" }
    },{ // 64
        { "moth", "flap" }
    },{ // 65
        { "cinematic", "orangegames_1_boom_l" },
        { "cinematic", "orangegames_1_boom_r" },
        { "cinematic", "orangegames_7_bubble_l" },
        { "cinematic", "orangegames_7_bubble_r" },
        { "cinematic", "orangegames_2_glass_1_l" },
        { "cinematic", "orangegames_2_glass_1_r" },
        { "cinematic", "orangegames_5_glass_2_l" },
        { "cinematic", "orangegames_5_glass_2_r" },
        { "cinematic", "orangegames_6_merge" },
        { "cinematic", "orangegames_3_sweep_1_l" },
        { "cinematic", "orangegames_3_sweep_1_r" },
        { "cinematic", "orangegames_4_sweep_2_l" },
        { "cinematic", "orangegames_4_sweep_2_r" },
        { "cinematic", "orangegames_5_sweep_3_l" },
        { "cinematic", "orangegames_5_sweep_3_r" }
    },{ // 66
        { "cinematic", "project2_unused_crunch" },
        { "cinematic", "project2_10_fart" },
        { "cinematic", "project2_unused_foew1" },
        { "cinematic", "project2_unused_foew4" },
        { "cinematic", "project2_unused_foew5" },
        { "cinematic", "project2_unused_frog1" },
        { "cinematic", "project2_unused_frog2" },
        { "cinematic", "project2_unused_frog3" },
        { "cinematic", "project2_unused_frog4" },
        { "cinematic", "project2_unused_frog5" },
        { "cinematic", "project2_unused_kiss4" },
        { "cinematic", "project2_unused_open" },
        { "cinematic", "project2_unused_pinch1" },
        { "cinematic", "project2_unused_pinch2" },
        { "cinematic", "project2_3_plop_1" },
        { "cinematic", "project2_4_plop_2" },
        { "cinematic", "project2_5_plop_3" },
        { "cinematic", "project2_6_plop_4" },
        { "cinematic", "project2_7_plop_5" },
        { "cinematic", "project2_9_spit" },
        { "cinematic", "project2_unused_splout" },
        { "cinematic", "project2_2_splat" },
        { "cinematic", "project2_1_8_throw" },
        { "cinematic", "project2_unused_tong" }
    },{ // 67
        { "object", "savepoint_open" },
        { "object", "copter" },
        { "unknown", "unknown_pickup_stretch1a" }
    },{ // 68
        { "pinball", "bumper_hit" },
        { "pinball", "flipper_1" },
        { "pinball", "flipper_2" },
        { "pinball", "flipper_3" },
        { "pinball", "flipper_4" }
    },{ // 69
    },{ // 70
    },{ // 71
        { "queen", "spring" },
        { "queen", "scream" }
    },{ // 72
        { "rapier", "die" },
        { "rapier", "noise_1" },
        { "rapier", "noise_2" },
        { "rapier", "noise_3" },
        { "rapier", "clunk" }
    },{ // 73
    },{ // 74
        { "robot", "unknown_big1" },
        { "robot", "unknown_big2" },
        { "robot", "unknown_can1" },
        { "robot", "unknown_can2" },
        { "robot", "attack_start" },
        { "robot", "attack_end" },
        { "robot", "attack" },
        { "robot", "unknown_hydropuf" },
        { "robot", "unknown_idle1" },
        { "robot", "unknown_idle2" },
        { "robot", "unknown_jmpcan1" },
        { "robot", "unknown_jmpcan10" },
        { "robot", "unknown_jmpcan2" },
        { "robot", "unknown_jmpcan3" },
        { "robot", "unknown_jmpcan4" },
        { "robot", "unknown_jmpcan5" },
        { "robot", "unknown_jmpcan6" },
        { "robot", "unknown_jmpcan7" },
        { "robot", "unknown_jmpcan8" },
        { "robot", "unknown_jmpcan9" },
        { "robot", "shrapnel_1" },
        { "robot", "shrapnel_2" },
        { "robot", "shrapnel_3" },
        { "robot", "shrapnel_4" },
        { "robot", "shrapnel_5" },
        { "robot", "attack_start_shutter" },
        { "robot", "unknown_out" },
        { "robot", "unknown_poep" },
        { "robot", "unknown_pole" },
        { "robot", "unknown_shoot" },
        { "robot", "walk_1" },
        { "robot", "walk_2" },
        { "robot", "walk_3" }
    },{ // 75
        { "object", "rolling_rock" }
    },{ // 76
    },{ // 77
        { "unknown", "sugar_rush_heartbeat" }, // Jazz2AnimVersion::ORIGINAL | Jazz2AnimVersion::HH
        { "pickup", "food_sugar_rush" },
    },{ // 78
        { "common", "science_noise" }
    },{ // 79
        { "skeleton", "bone_1" },
        { "skeleton", "bone_2" },
        { "skeleton", "bone_3" },
        { "skeleton", "bone_4" },
        { "skeleton", "bone_5" },
        { "skeleton", "bone_6" }
    },{ // 80
        { "pole", "tree_fall_1" },
        { "pole", "tree_fall_2" },
        { "pole", "tree_fall_3" }
    },{ // 81
    },{ // 82
        { "spike", "missile_1" },
        { "spike", "missile_2" },
        { "spike", "missile_3" },
        { "spike", "noise" },
        { "spike", "lock_on" }
    },{ // 83
    },{ // 84
    },{ // 85
    },{ // 86
    },{ // 87
    },{ // 88
        { "spaz", "hurt_1" },
        { "spaz", "hurt_2" },
        { "spaz", "idle_flavor_3_bird_land" },
        { "spaz", "idle_flavor_4" },
        { "spaz", "idle_flavor_3_bird" },
        { "spaz", "idle_flavor_3_eat" },
        { "spaz", "jump_1" },
        { "spaz", "jump_2" },
        { "spaz", "idle_flavor_2" },
        { "spaz", "hihi" },
        { "spaz", "spring_1" },
        { "spaz", "double_jump" },
        { "spaz", "sidekick_1" },
        { "spaz", "sidekick_2" },
        { "spaz", "spring_2" },
        { "spaz", "oooh" },
        { "spaz", "ledge" },
        { "spaz", "jump_3" },
        { "spaz", "jump_4" }
    },{ // 89
    },{ // 90
    },{ // 91
    },{ // 92
        { "spring", "spring_ver_down" },
        { "spring", "spring" }
    },{ // 93
        { "common", "steam_note" }
    },{ // 94
        { "unimplemented", "dizzy" }
    },{ // 95
        { "sucker_tube", "deflate" },
        { "sucker_tube", "pinch_1" },
        { "sucker_tube", "pinch_2" },
        { "sucker_tube", "pinch_3" },
        { "sucker_tube", "plop_1" },
        { "sucker_tube", "plop_2" },
        { "sucker_tube", "plop_3" },
        { "sucker_tube", "plop_4" },
        { "sucker_tube", "up" }
    },{ // 96
    },{ // 97
        { "tough_turtle_boss", "attack_start" },
        { "tough_turtle_boss", "attack_end" },
        { "tough_turtle_boss", "mace" }
    },{ // 98
    },{ // 99
        { "turtle", "attack_bite" },
        { "turtle", "turn_start" },
        { "turtle", "shell_collide" },
        { "turtle", "idle_1" },
        { "turtle", "idle_2" },
        { "turtle", "attack_neck" },
        { "turtle", "noise_1" },
        { "turtle", "noise_2" },
        { "turtle", "noise_3" },
        { "turtle", "noise_4" },
        { "turtle", "turn_end" }
    },{ // 100
    },{ // 101
        { "uterus", "closed_start" },
        { "uterus", "closed_end" },
        { "crab", "noise_1" },
        { "crab", "noise_2" },
        { "crab", "noise_3" },
        { "crab", "noise_4" },
        { "crab", "noise_5" },
        { "crab", "noise_6" },
        { "crab", "noise_7" },
        { "crab", "noise_8" },
        { "uterus", "scream" },
        { "crab", "step_1" },
        { "crab", "step_2" }
    },{ // 102
    },{ // 103
    },{ // 104
    },{ // 105
    },{ // 106
    },{ // 107
        { "common", "wind" }
    },{ // 108
        { "witch", "laugh" },
        { "witch", "magic" }
    }
};
