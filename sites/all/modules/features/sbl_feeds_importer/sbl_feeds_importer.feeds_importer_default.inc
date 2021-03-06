<?php
/**
 * @file
 * sbl_feeds_importer.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function sbl_feeds_importer_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'import_players';
  $feeds_importer->config = array(
    'name' => 'Spieler importieren',
    'description' => 'Spielerdaten mit zugeordnetem Verein pro Saison importieren',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
        'directory' => 'private://feeds',
        'allowed_schemes' => array(
          0 => 'public',
          1 => 'private',
        ),
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => '46',
        'authorize' => 1,
        'mappings' => array(
          0 => array(
            'source' => 'Kombinierter Name',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'Akad. Titel',
            'target' => 'field_pl_acad_title',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'Nachname',
            'target' => 'field_pl_last_name',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'Vorname',
            'target' => 'field_pl_first_name',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'ID_NUMBER',
            'target' => 'field_fide_id',
            'unique' => 1,
          ),
        ),
        'update_existing' => '2',
        'update_non_existent' => 'skip',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'spieler',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['import_players'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'player_team_mapping';
  $feeds_importer->config = array(
    'name' => 'Spieler Saisondaten',
    'description' => 'Zuordnung Spieler zu Vereinen pro Saison',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
        'directory' => 'private://feeds',
        'allowed_schemes' => array(
          0 => 'public',
          1 => 'private',
        ),
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsFieldCollectionProcessor',
      'config' => array(
        'field_name' => 'field_sp_verein',
        'host_entity_type' => 'node',
        'is_field' => 1,
        'guid_field_name' => 'field_fide_id',
        'identifier_field_name' => 'field_year_id',
        'mappings' => array(
          0 => array(
            'source' => 'ID_NUMBER',
            'target' => 'host_entity_guid',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'Jahres-ID',
            'target' => 'identifier_field',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'Saison',
            'target' => 'field_saison',
            'term_search' => '0',
            'autocreate' => 0,
          ),
          3 => array(
            'source' => 'VER',
            'target' => 'field_fc_verein:label',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'COUNTRY',
            'target' => 'field_collection_item:field_sp_verein:field_pl_country:iso3',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'Jahres-ID',
            'target' => 'field_year_id',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'TITLE',
            'target' => 'field_pl_master_title',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'RO',
            'target' => 'field_pl_elo',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'BNR',
            'target' => 'field_pl_board_nr',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'LIGA',
            'target' => 'field_pl_league',
            'term_search' => '0',
            'autocreate' => 0,
          ),
        ),
        'update_existing' => '2',
        'update_non_existent' => 'skip',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'field_sp_verein',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['player_team_mapping'] = $feeds_importer;

  return $export;
}
