<?php
/**
 * @file
 * sbl_ligaverwaltung.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function sbl_ligaverwaltung_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|mannschaft|full';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'mannschaft';
  $ds_fieldsetting->view_mode = 'full';
  $ds_fieldsetting->settings = array(
    'kader_team_current' => array(
      'weight' => '13',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'show_title' => 1,
        'title_wrapper' => '',
        'ctools' => 'a:3:{s:4:"conf";a:1:{s:9:"arguments";a:2:{s:16:"field_saison_tid";s:0:"";s:25:"field_fc_verein_target_id";s:0:"";}}s:4:"type";s:11:"views_panes";s:7:"subtype";s:20:"players-panel_pane_1";}',
        'load_terms' => 0,
      ),
    ),
    'title' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
    ),
  );
  $export['node|mannschaft|full'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_custom_fields_info().
 */
function sbl_ligaverwaltung_ds_custom_fields_info() {
  $export = array();

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'kader_team_current';
  $ds_field->label = 'Kader';
  $ds_field->field_type = 7;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = 'mannschaft|full';
  $ds_field->properties = array(
    'default' => array(),
    'settings' => array(
      'show_title' => array(
        'type' => 'checkbox',
      ),
      'title_wrapper' => array(
        'type' => 'textfield',
        'description' => 'Eg: h1, h2, p',
      ),
      'ctools' => array(
        'type' => 'ctools',
      ),
    ),
  );
  $export['kader_team_current'] = $ds_field;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function sbl_ligaverwaltung_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'field_collection_item|field_tm_game|default';
  $ds_layout->entity_type = 'field_collection_item';
  $ds_layout->bundle = 'field_tm_game';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'header' => array(
        0 => 'field_single_match_result',
      ),
      'left' => array(
        1 => 'field_mt_player_1',
        2 => 'field_mt_points_player_1',
        3 => 'field_mt_color_player_1',
      ),
      'right' => array(
        4 => 'field_mt_player_2',
        5 => 'field_mt_points_player_2',
        6 => 'field_mt_color_player_2',
      ),
    ),
    'fields' => array(
      'field_single_match_result' => 'header',
      'field_mt_player_1' => 'left',
      'field_mt_points_player_1' => 'left',
      'field_mt_color_player_1' => 'left',
      'field_mt_player_2' => 'right',
      'field_mt_points_player_2' => 'right',
      'field_mt_color_player_2' => 'right',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'left' => 'div',
      'right' => 'div',
      'footer' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'layout_disable_css' => 0,
  );
  $export['field_collection_item|field_tm_game|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'field_collection_item|field_tm_game|form';
  $ds_layout->entity_type = 'field_collection_item';
  $ds_layout->bundle = 'field_tm_game';
  $ds_layout->view_mode = 'form';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'header' => array(
        0 => 'field_single_match_result',
        1 => 'field_brettnummer',
      ),
      'left' => array(
        2 => 'field_mt_player_1',
        3 => 'field_mt_color_player_1',
        4 => 'field_mt_points_player_1',
      ),
      'right' => array(
        5 => 'field_mt_player_2',
        6 => 'field_mt_color_player_2',
        7 => 'field_mt_points_player_2',
      ),
      'hidden' => array(
        8 => '_add_existing_field',
      ),
    ),
    'fields' => array(
      'field_single_match_result' => 'header',
      'field_brettnummer' => 'header',
      'field_mt_player_1' => 'left',
      'field_mt_color_player_1' => 'left',
      'field_mt_points_player_1' => 'left',
      'field_mt_player_2' => 'right',
      'field_mt_color_player_2' => 'right',
      'field_mt_points_player_2' => 'right',
      '_add_existing_field' => 'hidden',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'left' => 'div',
      'right' => 'div',
      'footer' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'layout_disable_css' => 0,
  );
  $export['field_collection_item|field_tm_game|form'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|mannschaft|full';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'mannschaft';
  $ds_layout->view_mode = 'full';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'left' => array(
        0 => 'title',
        1 => 'field_logo_team',
        2 => 'field_facebook',
        3 => 'field_twitter_club',
        4 => 'field_der_bundesliga_seit',
        5 => 'field_groesste_erfolge',
      ),
      'right' => array(
        6 => 'field_venue_club',
        7 => 'field_website_club',
        8 => 'field_chairman_club',
        9 => 'field_captain_team',
        10 => 'field_web_admin_club',
        11 => 'field_travel_partner_club',
        12 => 'field_heimspiele',
      ),
      'footer' => array(
        13 => 'kader_team_current',
      ),
    ),
    'fields' => array(
      'title' => 'left',
      'field_logo_team' => 'left',
      'field_facebook' => 'left',
      'field_twitter_club' => 'left',
      'field_der_bundesliga_seit' => 'left',
      'field_groesste_erfolge' => 'left',
      'field_venue_club' => 'right',
      'field_website_club' => 'right',
      'field_chairman_club' => 'right',
      'field_captain_team' => 'right',
      'field_web_admin_club' => 'right',
      'field_travel_partner_club' => 'right',
      'field_heimspiele' => 'right',
      'kader_team_current' => 'footer',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'left' => 'div',
      'right' => 'div',
      'footer' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'layout_disable_css' => 0,
    'hide_page_title' => '1',
    'page_option_title' => '',
  );
  $export['node|mannschaft|full'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|team_match|form';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'team_match';
  $ds_layout->view_mode = 'form';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'header' => array(
        0 => 'title',
        1 => 'field_tm_spieltag_1',
        2 => 'field_tm_saison',
        3 => 'field_liga',
        4 => 'field_datum',
        5 => 'body',
        6 => 'field_ausrichter',
        8 => 'field_spiel_laeuft',
        9 => 'field_game_has_result',
        11 => 'field_livevideo',
        14 => 'field_video_zusammenfas',
        16 => 'field_match_report',
      ),
      'right' => array(
        7 => 'field_tm_team_2',
        10 => 'field_tm_team_points_tm_2',
        12 => 'field_tm_board_points_team_2',
        13 => 'field_tm_board_points_team_2_top',
        15 => 'field_tm_board_points_team_2_low',
        20 => 'group_team_away',
      ),
      'left' => array(
        17 => 'field_tm_team_1',
        18 => 'field_tm_team_points_tm_1',
        19 => 'group_team_home',
        21 => 'field_tm_board_points_team_1',
        23 => 'field_tm_board_points_team_1_top',
        24 => 'field_tm_board_points_team_1_low',
      ),
      'footer' => array(
        22 => 'field_tm_game',
        25 => 'path',
        26 => 'metatags',
      ),
      'hidden' => array(
        27 => 'rabbit_hole',
        28 => '_add_existing_field',
      ),
    ),
    'fields' => array(
      'title' => 'header',
      'field_tm_spieltag_1' => 'header',
      'field_tm_saison' => 'header',
      'field_liga' => 'header',
      'field_datum' => 'header',
      'body' => 'header',
      'field_ausrichter' => 'header',
      'field_tm_team_2' => 'right',
      'field_spiel_laeuft' => 'header',
      'field_game_has_result' => 'header',
      'field_tm_team_points_tm_2' => 'right',
      'field_livevideo' => 'header',
      'field_tm_board_points_team_2' => 'right',
      'field_tm_board_points_team_2_top' => 'right',
      'field_video_zusammenfas' => 'header',
      'field_tm_board_points_team_2_low' => 'right',
      'field_match_report' => 'header',
      'field_tm_team_1' => 'left',
      'field_tm_team_points_tm_1' => 'left',
      'group_team_home' => 'left',
      'group_team_away' => 'right',
      'field_tm_board_points_team_1' => 'left',
      'field_tm_game' => 'footer',
      'field_tm_board_points_team_1_top' => 'left',
      'field_tm_board_points_team_1_low' => 'left',
      'path' => 'footer',
      'metatags' => 'footer',
      'rabbit_hole' => 'hidden',
      '_add_existing_field' => 'hidden',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'left' => 'div',
      'right' => 'div',
      'footer' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'layout_disable_css' => 0,
  );
  $export['node|team_match|form'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|team_match|full';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'team_match';
  $ds_layout->view_mode = 'full';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'header' => array(
        0 => 'field_video_zusammenfas',
        3 => 'field_tm_saison',
        4 => 'field_tm_spieltag_1',
        5 => 'field_datum',
        6 => 'body',
      ),
      'footer' => array(
        1 => 'field_ausrichter',
        2 => 'field_spiel_laeuft',
        13 => 'field_tm_game',
      ),
      'left' => array(
        7 => 'field_tm_team_1',
        8 => 'field_tm_team_points_tm_1',
        9 => 'field_tm_board_points_team_1',
      ),
      'right' => array(
        10 => 'field_tm_team_2',
        11 => 'field_tm_team_points_tm_2',
        12 => 'field_tm_board_points_team_2',
      ),
    ),
    'fields' => array(
      'field_video_zusammenfas' => 'header',
      'field_ausrichter' => 'footer',
      'field_spiel_laeuft' => 'footer',
      'field_tm_saison' => 'header',
      'field_tm_spieltag_1' => 'header',
      'field_datum' => 'header',
      'body' => 'header',
      'field_tm_team_1' => 'left',
      'field_tm_team_points_tm_1' => 'left',
      'field_tm_board_points_team_1' => 'left',
      'field_tm_team_2' => 'right',
      'field_tm_team_points_tm_2' => 'right',
      'field_tm_board_points_team_2' => 'right',
      'field_tm_game' => 'footer',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'left' => 'div',
      'right' => 'div',
      'footer' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'layout_disable_css' => 0,
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|team_match|full'] = $ds_layout;

  return $export;
}
